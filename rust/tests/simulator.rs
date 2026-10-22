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

    // --- the error paths of the two links --------------------------------
    //
    // The C reports the USART errors of the link they happen on
    // (fw/modules/bleCom.c): "BLE framing error", "BLE USART noise" and
    // "BLE USART overrun" on the AT link, "USART framing error",
    // "USART noise" and "USART overrun" on the command link.
    sim::usart_set_error(USART3, 0x02); // a framing error on the AT link
    settle();
    assert!(
        take().contains("call :proxy message err BLE framing error\n"),
        "a framing error on the AT link has to be reported"
    );

    sim::usart_set_error(USART3, 0x08); // an overrun on the AT link
    settle();
    assert!(
        take().contains("call :proxy message err BLE USART overrun\n"),
        "an overrun on the AT link has to be reported"
    );

    sim::usart_set_error(USART4, 0x04); // a noise error on the command link
    settle();
    assert!(
        take().contains("call :proxy message err USART noise\n"),
        "a noise error on the command link has to be reported"
    );

    sim::usart_set_error(USART4, 0x02); // a framing error on the command link
    settle();
    assert!(
        take().contains("call :proxy message err USART framing error\n"),
        "a framing error on the command link has to be reported"
    );

    // --- the answers of the BLE module -----------------------------------
    //
    // The C recognises the answers of the module ("BLE-RSP HELLO" and
    // "BLE-RSP GAP SET MODE: ") and repeats them on the command link
    // (fw/modules/bleCom.c, FUN_0800663C and FUN_080067F4).
    sim::usart_push_rx(USART3, b"BLE-RSP HELLO\n");
    settle();
    assert!(
        take().contains("BLE-RSP HELLO\n"),
        "the answer of the BLE module has to be repeated"
    );

    sim::usart_push_rx(USART3, b"BLE-RSP GAP SET MODE: 1\n");
    settle();
    assert!(
        take().contains("BLE-RSP GAP SET MODE: 1\n"),
        "the GAP answer of the BLE module has to be repeated"
    );

    // --- the overflow of the receive ring --------------------------------
    //
    // The C prints "BUFOVF!!!!!" when the receive ring of a link is full
    // (the ring of the command link holds 200 bytes).
    sim::usart_push_rx(USART4, &vec![b'x'; 400]);
    settle();
    assert!(
        take().contains("BUFOVF!!!!!"),
        "the overflow of the receive ring has to be reported"
    );

    // --- the overflow of the command buffer ------------------------------
    //
    // The C reads at most 40 bytes into the command buffer (0x28); a longer
    // line empties the buffer, the firmware reports "possible cmdBuf
    // overflow" and then dispatches the empty line, which answers
    // "Invalid command!" (fw/src/main.c).
    feed(&"x".repeat(60));
    settle();
    let out = take();
    assert!(
        out.contains("call :proxy message err possible cmdBuf overflow\n"),
        "the overflow of the command buffer has to be reported"
    );
    assert!(
        out.contains("call :proxy message err Invalid command!\n"),
        "the empty line after an overflow has to be answered"
    );

    drop(firmware);
    drop(tick);
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
    codie::sim::reset();
    codie::modules::ble_com::init();

    // CR1 of both links: RE (bit 2) and TE (bit 3) set, and UE (bit 0)
    for usart in [USART3, USART4] {
        let cr1 = codie::sim::read32(usart + 0x00) & 0xFFFF;
        assert_eq!(cr1 & 0x000C, 0x000C, "CR1 of 0x{:08X}: RE+TE not set", usart);
        assert_eq!(cr1 & 0x0001, 0x0001, "CR1 of 0x{:08X}: UE not set", usart);
    }

    // the values the dump's callers pass
    assert_eq!(codie::sim::read32(USART3 + 0x04) & 0xFFFF, 0x2000, "CR2 of the AT link");
    assert_eq!(codie::sim::read32(USART3 + 0x08) & 0xFFFF, 0x0300, "CR3 of the AT link");
    assert_eq!(codie::sim::read32(USART4 + 0x04) & 0xFFFF, 0x0000, "CR2 of the command link");
    assert_eq!(codie::sim::read32(USART4 + 0x08) & 0xFFFF, 0x0000, "CR3 of the command link");
}

