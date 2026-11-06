//! The control registers of the two serial links, against the dump.
//!
//! This test needs the peripheral model to itself: it resets it and brings
//! the link up from scratch, so it lives in its own file rather than next to
//! a test that runs the firmware in a thread (that race cost a day).

use codie::hal::addr::{USART3, USART4};

/// The control registers the port writes into the two links.
///
/// The original (`FUN_0800B3FC`, called from `FUN_08007F7C` and
/// `FUN_08009A04`) enables the transmitter and the receiver: `CR1 |= 0x000C`
/// (bits 2 and 3: RE + TE).  The C reconstruction and the first version of
/// the port wrote `0x0300` there instead (PEIE | PS), which would have left
/// the link dead on the robot - the mistake was invisible in the C harness
/// because it wraps the USART primitives with `ld --wrap` and never looks at
/// the control registers.
///
/// The values of the two links (both taken from the dump):
///   USART3 (the AT link)  : CR2 = 0x2000 (two stop bits), CR3 = 0x300
///   USART4 (the command)  : CR2 = 0, CR3 = 0
#[test]
fn the_control_registers_enable_the_transmitter_and_the_receiver() {
    codie::sim::reset();
    codie::modules::ble_com::init();

    // CR1 of both links: RE (bit 2) + TE (bit 3) + UE (bit 0) set; the USART
    // interrupt-enable bits (CR1 0x50100) stay clear.  The original wires the
    // IRQ via `CR1 |= 0x50105` (FUN_000B4EC) + NVIC (FUN_000AB30,
    // 0x0800ab30) in FUN_0800806C:8960 (BLE) / FUN_0009A04:10963 (console);
    // this polling port drains ISR.RXNE/TC instead, so 0x50100 must stay clear.
    for usart in [USART3, USART4] {
        let cr1 = codie::sim::read32(usart + 0x00);
        assert_eq!(cr1 & 0x000C, 0x000C, "CR1 of 0x{:08X}: RE+TE not set", usart);
        assert_eq!(cr1 & 0x0001, 0x0001, "CR1 of 0x{:08X}: UE not set", usart);
        assert_eq!(cr1 & 0x50100, 0, "CR1 of 0x{:08X}: USART IRQ-enable bits set; port polls ISR", usart);
    }

    // the values the dump's callers pass
    assert_eq!(codie::sim::read32(USART3 + 0x04) & 0xFFFF, 0x2000, "CR2 of the AT link");
    assert_eq!(codie::sim::read32(USART3 + 0x08) & 0xFFFF, 0x0300, "CR3 of the AT link");
    assert_eq!(codie::sim::read32(USART4 + 0x04) & 0xFFFF, 0x0000, "CR2 of the command link");
    assert_eq!(codie::sim::read32(USART4 + 0x08) & 0xFFFF, 0x0000, "CR3 of the command link");
}
