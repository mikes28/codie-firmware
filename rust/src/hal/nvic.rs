//! NVIC helper (`hwi_a_hal.c`).

use super::addr::{NVIC_IPR, NVIC_ISER};
use crate::mmio;

/// `FUN_0800AB30` - enable an interrupt in the NVIC and set its priority.
///
/// The original puts the two priority bits into the top of the byte.
pub fn enable(irq: u8, prio: u8) {
    let ipr = NVIC_IPR + (irq as u32 & 0x1F) * 4;
    mmio::rmw8(ipr, |v| (v & !0xC0) | ((prio & 3) << 6));
    mmio::w32(NVIC_ISER, 1u32 << (irq as u32 & 0x1F));
}
