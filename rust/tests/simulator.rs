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

    // The boot output is measured before the fastest periodic engine of the
    // port can run.  The main loop does not block on the line reader (the
    // dump's `FUN_08009B14` returns 0 when the ring is empty), so the light
    // program steps after its 160 ms gate (`FUN_08006000`) and would
    // otherwise put a "PPRT\n" into this exact comparison.
    thread::sleep(Duration::from_millis(50));

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

    // let the rest of the boot window run, then drop the periodic bytes
    thread::sleep(Duration::from_millis(250));
    let _ = take();

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

    // The command-buffer overflow above leaves an empty line pending, which
    // the firmware answers with "Invalid command!" - drain it before the
    // LED checks so the captures below are not polluted.
    feed("get hwiBuzz stop\n");
    settle();
    let _ = take();

    // --- the ARGB strip (hwiLed) -----------------------------------------
    //
    // The dump drives a single-wire ARGB strip: TIM15 + DMA1 channel 5
    // streams the per-bit CCR values into TIM15_CCR1 (0x40014034).  The
    // wire encoding is `FUN_080090A8`: every source byte becomes 8 CCR
    // bytes, MSB first, 0 -> 0x0e and 1 -> 0x2e.
    let tim15 = codie::hal::addr::TIM15;
    let ccr1 = tim15 + 0x34;

    feed("set hwiLed rgb 0 255 0 0\n");
    settle();
    assert!(
        take().contains("set hwiLed rgb ff0000\n"),
        "set hwiLed rgb answers with the packed colour"
    );

    // the timer must be armed for the 1.25 us bit window and the DMA must
    // be pointed at CCR1 with the full stream length
    assert_eq!(sim::read32(tim15 + 0x28), 29, "the LED timer prescaler is 29");
    assert_eq!(sim::read32(tim15 + 0x2C), 0x307, "the LED bit period is ARR");
    assert_eq!(
        sim::read32(tim15 + 0x18) & 0xFF,
        0x60,
        "CCMR1: OC1M = PWM mode 1, OC1PE clear"
    );
    assert_ne!(
        sim::read32(tim15 + 0x20) & 1,
        0,
        "the channel 1 output must be enabled"
    );
    assert_ne!(
        sim::read32(tim15 + 0x44) & 0x8000,
        0,
        "BDTR.MOE must be set"
    );

    let dma = codie::hal::addr::DMA1_CH5;
    assert_eq!(
        sim::read32(dma + 0x08),
        ccr1,
        "the DMA must push into TIM15_CCR1"
    );
    // the stream is 12 LEDs * 3 bytes * 8 bits = 288 entries
    assert_eq!(sim::read32(dma + 0x04), 288, "the DMA transfer count");

    // `pwm` and `on` answer in decimal (the C `fmt_int`), `rgb` in hex (the
    // firmware's value-reply radix)
    feed("set hwiLed pwm 1 255\n");
    settle();
    assert!(
        take().contains("set hwiLed pwm 255\n"),
        "set hwiLed pwm answers in decimal"
    );

    feed("set hwiLed on 2 off\n");
    settle();
    assert!(
        take().contains("set hwiLed on 0\n"),
        "set hwiLed on with an off flag answers 0 in decimal"
    );

    // `get` reports the stored duty of the addressed LED.  The light
    // programs rewrite the whole strip every 160 ms, so only the reply
    // shape is asserted - a fixed number here would race the engine.
    feed("get hwiLed pwm 1\n");
    settle();
    assert!(
        take().contains("get hwiLed pwm "),
        "get hwiLed pwm answers with the stored duty"
    );

    drop(firmware);
    drop(tick);
}


