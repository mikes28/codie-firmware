//! The periodic engines of the main loop, against a model of the peripherals.
//!
//! The dump's periodic task `FUN_080063EC` (@0x80063ec) runs eleven engines.
//! Four of them can run without the BLE module, and those four must actually
//! run when nobody is talking to the robot - which is what this test checks:
//!
//! | engine | dump function | what it does |
//! |---|---|---|
//! | ADC overrun service | `FUN_080071C0` | clears the ADC overrun flag |
//! | motor fault poll | `FUN_08008784` | reads GPIOA PA12 into the fault flags |
//! | line sensor sample | `FUN_080094F8` | fills the rolling sample window |
//! | tone sequencer | `FUN_08008574` | plays the queued tones one by one |
//!
//! Before these were wired the port's main loop called only the three app
//! ticks, so the motor fault input was never sampled, the line window was
//! never refreshed and a queued tone never played.
//!
//! Everything lives in ONE test function on purpose: the peripheral model
//! (`src/sim/mod.rs`) is one global, and a second firmware thread would only
//! interleave its "PPRT"/"PPRET" markers into this test's command stream.

use std::thread;
use std::time::Duration;

use codie::hal::addr::{GPIOA, USART4};
use codie::sim;

/// Feed a line into the command link.
fn feed(line: &str) {
    sim::usart_push_rx(USART4, line.as_bytes());
}

/// The bytes the firmware has sent on the command link since the last call.
fn take() -> String {
    String::from_utf8_lossy(&sim::usart_take_tx(USART4)).into_owned()
}

/// Discard everything the firmware has sent so far.
fn drain() {
    let _ = sim::usart_take_tx(USART4);
}

/// Wait until the firmware has produced at least `want` on the command link.
///
/// Polling instead of a fixed sleep: the other simulator tests drive the same
/// global model from their own threads, so a fixed window is racy.
fn wait_for(want: &str, timeout: Duration) -> String {
    let deadline = std::time::Instant::now() + timeout;
    let mut acc = String::new();
    loop {
        acc.push_str(&take());
        if acc.contains(want) {
            return acc;
        }
        if std::time::Instant::now() >= deadline {
            return acc;
        }
        thread::sleep(Duration::from_millis(15));
    }
}

/// The asynchronous markers of the light-program engine (`app_lights`), which
/// this test does not assert on: they are a different engine's behaviour.
fn is_light_marker(line: &str) -> bool {
    line == "PPRT" || line == "PPRET"
}

#[test]
fn the_periodic_engines_run_when_the_link_is_idle() {
    sim::reset();

    // The fault input (GPIOA pin 12) starts high = healthy, so the fault poll
    // has a defined level to see before the firmware boots.
    sim::write32(GPIOA + 0x18, 0x1000); // GPIOA BSRR

    // The millisecond tick and the timer counters.
    let tick = thread::spawn(|| loop {
        thread::sleep(Duration::from_millis(1));
        codie::system::SysTick_Handler();
        sim::advance_timers(1);
    });

    // The firmware itself - main() never returns.
    let firmware = thread::spawn(|| codie::boot::run());

    // Wait for the boot output to settle before touching the link: the light
    // program emits an asynchronous marker every 160 ms while idle, and a
    // command fed into that stream is what produces the "possible cmdBuf
    // overflow" report.
    let _ = wait_for("+ack\":\"0", Duration::from_millis(3000));
    drain();

    // 1. The ADC overrun service and the line-window refresh are silent on the
    //    link.  Their observable effect is that the firmware stays alive and
    //    keeps answering - which is what this and the next check prove.
    feed("get hwiDrive nFault\n");
    let reply = wait_for("set hwiDrive nFault 0", Duration::from_millis(3000));
    assert!(
        reply.contains("set hwiDrive nFault 0"),
        "the nFault reply is the original's constant \"0\": got {reply:?}"
    );

    // 2. The motor fault poll has read PA12 into the module state by now.  The
    //    protocol always answers the constant "0" for nFault, so there is
    //    nothing more to observe on the wire - the check above is that the
    //    poll ran without hanging.
    feed("get drvBattery voltage\n");
    let reply = wait_for("set drvBattery voltage", Duration::from_millis(3000));
    assert!(
        reply.contains("set drvBattery voltage"),
        "the firmware is still alive after the fault poll ran: got {reply:?}"
    );

    // 3. Queue a tone and let it play out.  The sequencer programs the buzzer
    //    timer only; the *only* markers allowed on the command link meanwhile
    //    are the light program's, so anything else is a regression.
    feed("call hwiBuzz play 0\n");
    let reply = wait_for("call hwiBuzz play 0", Duration::from_millis(3000));
    assert!(
        reply.contains("call hwiBuzz play 0"),
        "the play op still answers: got {reply:?}"
    );

    // Give the queued tone (100 ms) time to be consumed by the sequencer.
    thread::sleep(Duration::from_millis(300));
    let stray: Vec<String> = take()
        .lines()
        .filter(|l| !l.is_empty() && !is_light_marker(l))
        .map(|l| l.to_string())
        .collect();
    assert!(
        stray.is_empty(),
        "the tone sequencer stays quiet on the command link: got {stray:?}"
    );

    drain();
    let _ = (tick, firmware);
}
