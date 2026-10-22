//! The whole firmware running against a model of the peripherals.
//!
//! This is the Rust counterpart of `tools/host_sim.c`: the same checks, the
//! same expected behaviour.  The firmware runs in a thread, the test feeds
//! the commands into the command link (USART4) and reads the answers back.

use std::thread;
use std::time::Duration;

use codie::hal::addr::{TIM3, USART3, USART4};
use codie::sim;

/// Feed a line into the command link.
fn feed(line: &str) {
    sim::usart_push_rx(USART4, line.as_bytes());
}

/// The bytes the firmware has sent on the command link since the last call.
fn take() -> String {
    String::from_utf8_lossy(&sim::usart_take_tx(USART4)).into_owned()
}

/// Let the firmware work for a moment.
fn settle() {
    thread::sleep(Duration::from_millis(150));
}

#[test]
fn the_firmware_behaves_like_the_original() {
    sim::reset();

    // the millisecond tick and the timer counters
    let tick = thread::spawn(|| loop {
        thread::sleep(Duration::from_millis(1));
        codie::system::SysTick_Handler();
        sim::advance_timers(1);
    });

    // the firmware itself - main() never returns
    let firmware = thread::spawn(|| codie::boot::run());

    thread::sleep(Duration::from_millis(300));

    // --- the boot output -------------------------------------------------
    let boot = take();
    assert_eq!(
        boot,
        "BLE-boot\ncall :proxy greeting \"ver\":\"0.5.0hw2.1\" \"mat\":\"4\" \
         \"date\":\"1602121419\" \"ttpms\":\"1000\" \"+ack\":\"0\"\n",
        "the boot output of the command link differs"
    );
    assert!(
        sim::usart_take_tx(USART3).is_empty(),
        "the AT link must stay quiet at boot"
    );

    // --- the commands ----------------------------------------------------
    feed("get drvBattery voltage\n");
    settle();
    assert!(
        take().contains("set drvBattery voltage 0\n"),
        "get drvBattery voltage"
    );

    feed("get drvBattery chargeStat\n");
    settle();
    assert!(
        take().contains("set drvBattery chargeStat 1\n"),
        "get drvBattery chargeStat"
    );

    feed("get nosuchiface op\n");
    settle();
    assert!(
        take().contains("call :proxy message err No interface:nosuchiface\n"),
        "unknown interface"
    );

    feed("garbage\n");
    settle();
    assert!(
        take().contains("call :proxy message err Invalid command!\n"),
        "invalid command"
    );

    feed("set hwiDrive dutyA 200\n");
    settle();
    assert!(
        !take().contains("hwiDrive"),
        "set hwiDrive dutyA must not be answered"
    );
    assert_eq!(
        sim::read32(TIM3 + 0x34),
        0x200,
        "set hwiDrive dutyA 200 -> TIM3 CCR1"
    );

    feed("set hwiDrive dutyB 100\n");
    settle();
    assert!(
        !take().contains("hwiDrive"),
        "set hwiDrive dutyB must not be answered"
    );
    assert_eq!(
        sim::read32(TIM3 + 0x3C),
        0x100,
        "set hwiDrive dutyB 100 -> TIM3 CCR3"
    );

    feed("call drvBattery hwInit\n");
    settle();
    assert!(
        !take().contains("drvBattery"),
        "call drvBattery hwInit must not be answered"
    );

    // --- the two links ---------------------------------------------------
    // 96 MHz / 14400 = 6666.67 -> 6667 (the firmware rounds the divisor up,
    // the C simulator used a simplified model that reported 6666)
    assert_eq!(sim::usart_brr(USART3), 6667, "the BRR of the AT link");
    // 96 MHz / 460800 = 208.33 -> 208
    assert_eq!(sim::usart_brr(USART4), 208, "the BRR of the command link");

    drop(firmware);
    drop(tick);
}
