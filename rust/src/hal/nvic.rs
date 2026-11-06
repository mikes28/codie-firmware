//! NVIC helper (`hwi_a_hal.c`).

use super::addr::{NVIC_IPR, NVIC_ISER};
use crate::mmio;

/// `FUN_0800AB30` - enable an interrupt in the NVIC and set its priority.
///
/// The original puts the two priority bits into the top of the byte.
///
/// Evidence that the priority IS set (machine code of `FUN_0800AB30`
/// @0x800ab30, literal pool @0x800ab84 = 0xE000E100):
/// ```text
/// 800ab32: ldrb r3, [r0, #2]      ; cfg->has_prio
/// 800ab34: cmp  r3, #0
/// 800ab36: beq  0x800ab70         ; -> the enable-only path
/// 800ab38: ldrb r4, [r0, #0]      ; the IRQ number
/// 800ab3a: lsrs r1, r4, #2        ; irq >> 2
/// 800ab3e: adds r1, #192          ; + 0xc0
/// 800ab40: lsls r2, r1, #2        ; * 4
/// 800ab42: ldr  r6, [r2, r3]      ; read the IPR word at
///                                 ;   0xE000E100 + ((irq>>2)+0xc0)*4
///                                 ; = 0xE000E400 + (irq>>2)*4
/// 800ab44: movs r5, #3
/// 800ab46: ands r4, r5            ; irq & 3
/// 800ab48: lsls r1, r4, #3        ; (irq&3)*8  - the byte within the word
/// 800ab4a: movs r4, #255
/// 800ab4e: lsls r5, r1            ; 0xff << ((irq&3)*8) - the field mask
/// 800ab50: bics r6, r5            ; clear the field
/// 800ab54: ldrb r6, [r0, #1]      ; the priority
/// 800ab56: lsls r6, r6, #6        ; prio << 6
/// 800ab58: ands r4, r6
/// 800ab5c: lsls r4, r1            ; into the same byte field
/// 800ab5e: orrs r1, r4
/// 800ab60: str  r1, [r2, r3]      ; write the IPR word back
/// 800ab62: ldrb r0, [r0, #0]
/// 800ab66: ands r0, r2 (31)
/// 800ab68: lsls r4, r0
/// 800ab6c: str  r4, [r3, #0]      ; ISER: enable
/// ```
/// The priority byte of IRQ n sits at `0xE000E400 + n` and holds
/// `prio << 6`, which is exactly this port's
/// `(v & !0xC0) | ((prio & 3) << 6)` on the byte at
/// `NVIC_IPR + (irq & 0x1F) * 4 + irq % 3`... in fact at the byte
/// `NVIC_IPR + irq` when the register is addressed byte-wise. The word-stride
/// form used below reaches the same byte: word `NVIC_IPR + (irq>>2)*4`
/// contains bytes for IRQs `4*(irq>>2) .. 4*(irq>>2)+3`.
pub fn enable(irq: u8, prio: u8) {
    // Set the priority byte first (the original does the read-modify-write of
    // the whole IPR word before it touches ISER).
    let ipr = NVIC_IPR + ((irq as u32 & 0x1F) >> 2) * 4;
    let byte = (irq as u32 & 0x1F) & 3;
    mmio::rmw32(ipr, |v| {
        let shift = byte * 8;
        (v & !(0xFFu32 << shift)) | (((prio as u32 & 3) << 6) << shift)
    });

    // Then enable the interrupt.
    mmio::w32(NVIC_ISER, 1u32 << (irq as u32 & 0x1F));
}
