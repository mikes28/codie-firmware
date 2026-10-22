//! USART helpers (`hwi_b_hal.c`).

use super::addr::*;
use crate::mmio;

/// The configuration the original's `FUN_0800B3FC` takes from its caller.
///
/// The values are the ones the two callers pass in the dump: the BLE link
/// (`FUN_08007F7C`) uses `cr2 = 0x2000` (two stop bits) and `cr3 = 0x300`, the
/// console link (`FUN_08009A04`) uses `cr2 = 0` and `cr3 = 0`.
#[derive(Clone, Copy)]
pub struct Cfg {
    /// the baud rate
    pub baud: u32,
    /// the value OR'd into `CR2` (bit 13 = two stop bits)
    pub cr2: u16,
    /// the value OR'd into `CR3`
    pub cr3: u16,
}

impl Cfg {
    /// A configuration with the given baud rate and no extra bits.
    pub const fn new(baud: u32) -> Self {
        Cfg { baud, cr2: 0, cr3: 0 }
    }
}

/// `FUN_0800B3FC` - configure a USART.
///
/// The function is generic: the caller passes a config struct and the
/// original computes
/// ```text
/// CR1 = (CR1 & 0xE9F3) | cfg[3] | cfg[1] | cfg[4]
/// CR2 = (CR2 & 0xCFFF) | cfg[2]
/// CR3 = (CR3 & 0xFCFF) | cfg[5]
/// ```
/// Both callers of the dump (the BLE link at `FUN_08007F7C`, the console link
/// at `FUN_08009A04`) pass `cfg[4] = 0x000C` for the CR1 value, so the
/// receiver and the transmitter are enabled (`CR1` bits 2 and 3: RE + TE).
/// The C reconstruction and the first version of this port wrote `0x0300`
/// here - that is `PEIE | PS` (the parity error interrupt and the odd parity
/// selection), which leaves RE and TE **off**: the link would have been
/// completely dead on the robot.  The mistake was invisible in the host
/// simulator because it wraps the USART primitives and never looks at the
/// control registers.
///
/// The baud divisor is computed like the original
/// (`0x800b483`: `quotient + (remainder >= baud/2 ? 1 : 0)`), and it is
/// truncated to 16 bits (`uxth` at `0x800b49f`).
pub fn init(usart: u32, cfg: &Cfg) {
    if cfg.baud == 0 {
        return;
    }
    let mut brr = APB1_HZ / cfg.baud;
    if (cfg.baud / 2) <= (APB1_HZ % cfg.baud) {
        brr += 1;
    }
    brr &= 0xFFFF;

    mmio::rmw16(usart + USART_CR1, |v| v & 0xFFFE);
    // the original: CR2 = (CR2 & 0xCFFF) | cfg.cr2
    mmio::rmw16(usart + USART_CR2, |v| (v & 0xCFFF) | cfg.cr2);
    // RE + TE (the value of both callers of the dump)
    mmio::rmw16(usart + USART_CR1, |v| (v & 0xE9F3) | 0x000C);
    // the value the caller passes for the CR3
    mmio::rmw16(usart + USART_CR3, |v| (v & 0xFCFF) | cfg.cr3);
    mmio::w16(usart + USART_BRR, brr as u16);
}

/// `FUN_0800B53C` - is the transmit register free? (ISR bit 7, TXFNF)
pub fn tx_ready(usart: u32) -> bool {
    mmio::r32(usart + USART_ISR) & 0x80 != 0
}

/// `FUN_0800B4DC` - write one byte to the transmit register.
pub fn putc(usart: u32, c: u8) {
    mmio::w16(usart + USART_TDR, c as u16);
}

/// `FUN_0800B54C` - is a byte waiting in the receive register? (ISR bit 5)
pub fn rx_ready(usart: u32) -> bool {
    mmio::r32(usart + USART_ISR) & 0x20 != 0
}

/// `FUN_0800B4E4` - read one byte from the receive register.
pub fn getc(usart: u32) -> u8 {
    (mmio::r16(usart + USART_RDR) & 0x1FF) as u8
}

/// The raw status register.
pub fn status(usart: u32) -> u32 {
    mmio::r32(usart + USART_ISR)
}

/// `FUN_0800B548` - clear the error flags (ICR).
pub fn clear(usart: u32, mask: u32) {
    mmio::w32(usart + USART_ICR, mask);
}

/// `FUN_0800B4C0` - CR1.UE
pub fn enable(usart: u32) {
    mmio::rmw16(usart + USART_CR1, |v| v | 1);
}

/// `FUN_0800B4C0` - CR1.UE
pub fn disable(usart: u32) {
    mmio::rmw16(usart + USART_CR1, |v| v & !1);
}
