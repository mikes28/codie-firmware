//! The control registers of the two serial links, against the dump.
//!
//! This test needs the peripheral model to itself: it resets it and brings
//! the link up from scratch, so it lives in its own file rather than next to
//! a test that runs the firmware in a thread (that race cost a day).
//!
//! The three tests below also share one global firmware model (the
//! `codie::sim` peripheral model and the `ble_com` module statics), so they
//! run one at a time under `SERIAL` - the same reasoning as
//! `tests/battery.rs`.  In particular the line-sensor dispatch keeps its flag
//! bits set across calls, so a parallel test can leave it armed and make the
//! "must not fire without the flag raised" check fail.

use codie::hal::addr::{USART3, USART4};
use std::sync::{Mutex, MutexGuard};

/// Serializes the tests of this file: one global firmware model, one driver.
static SERIAL: Mutex<()> = Mutex::new(());

/// Run `f` while holding the file's lock.
fn serial<F: FnOnce()>(f: F) {
    let guard: MutexGuard<'_, ()> = SERIAL.lock().unwrap_or_else(|e| e.into_inner());
    f();
    drop(guard);
}

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
    serial(|| {
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
    });
}

// Callback dispatch for the BLE link / line sensor services (FUN_080096EC /
// FUN_08009EA0).
//
// Verifies that `ble_com::link_service_tick` and
// `ble_com::line_sensor_service_tick` fire their registered callbacks after
// the dump's gates (800 ms and 55 ms).

use std::sync::atomic::{AtomicBool, AtomicU8, Ordering};

use codie::modules::ble_com;
use codie::system;

static LINK_FIRED: AtomicBool = AtomicBool::new(false);
static LINE_ARG: AtomicU8 = AtomicU8::new(0);

/// Advance the millisecond tick by `ms` (each SysTick_Handler is 1 ms).
fn advance_ms(ms: u32) {
    for _ in 0..ms {
        system::SysTick_Handler();
    }
}

#[test]
fn link_service_tick_fires_after_800ms() {
    serial(|| {
    LINK_FIRED.store(false, Ordering::Relaxed);
    ble_com::register_link_callback(|| {
        LINK_FIRED.store(true, Ordering::Relaxed);
    });

    // Without raising the flag, the callback must not fire even after
    // advancing time.
    advance_ms(2000);
    ble_com::link_service_tick();
    assert!(
        !LINK_FIRED.load(Ordering::Relaxed),
        "link callback must not fire without the flag raised"
    );

    // With the flag raised and 800 ms elapsed, the callback fires.
    ble_com::raise_link_flag();
    advance_ms(801);
    ble_com::link_service_tick();
    assert!(
        LINK_FIRED.load(Ordering::Relaxed),
        "link callback must fire after flag raised and 800 ms elapsed"
    );
    });
}

#[test]
fn line_sensor_service_tick_fires_start_then_done() {
    serial(|| {
    LINE_ARG.store(0, Ordering::Relaxed);
    ble_com::register_line_callback(|arg| {
        LINE_ARG.store(arg, Ordering::Relaxed);
    });

    // Without raising the flag, the callback must not fire.
    advance_ms(2000);
    ble_com::line_sensor_service_tick();
    assert_eq!(
        LINE_ARG.load(Ordering::Relaxed),
        0,
        "line callback must not fire without the flag raised"
    );

    // With the flag raised and 55 ms elapsed, cb(3) fires then cb(0).
    ble_com::raise_line_flag();
    advance_ms(56);
    ble_com::line_sensor_service_tick();
    assert_eq!(
        LINE_ARG.load(Ordering::Relaxed),
        3,
        "start callback must fire with arg 3 after 55 ms"
    );
    ble_com::line_sensor_service_tick();
    assert_eq!(
        LINE_ARG.load(Ordering::Relaxed),
        0,
        "done callback must fire with arg 0"
    );
    });
}
