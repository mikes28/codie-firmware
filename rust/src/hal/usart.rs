//! USART helpers (`hwi_b_hal.c`).

use super::addr::*;
use crate::mmio;

/// `FUN_0800B3FC` - configure a USART.
///
/// The baud divisor is computed like the original:
/// `BRR = periph_clock / baud`, rounded up when the remainder is at least
/// half of the baud rate.
pub fn init(usart: u32, baud: u32, stop_bits: u8) {
    if baud == 0 {
        return;
    }
    let mut brr = APB1_HZ / baud;
    if (baud / 2) <= (APB1_HZ % baud) {
        brr += 1;
    }

    mmio::rmw16(usart + USART_CR1, |v| v & 0xFFFE);
    mmio::rmw16(usart + USART_CR2, |v| (v & 0xCFFF) | (((stop_bits as u16) & 1) << 13));
    mmio::rmw16(usart + USART_CR1, |v| (v & 0xE9F3) | 0x0300); // TE | RE
    mmio::rmw16(usart + USART_CR3, |v| (v & 0xFCFF) | 0x000C);
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
