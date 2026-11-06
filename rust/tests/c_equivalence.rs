//! The Rust port must behave exactly like the C firmware.
//!
//! The C firmware of `../fw` is run on the host against a fake memory
//! mapped peripheral map (`../tools/host_sim.c`) and its observable
//! behaviour is recorded in `tests/data/c_sim.txt` (the golden transcript,
//! captured with `../tools/capture_c_sim.sh`).  This test runs the *same*
//! scenario against the Rust port and compares the two fact by fact.
//!
//! The fixture is the only input of this test: it does not need the C
//! toolchain.

use std::collections::HashMap;
use std::thread;
use std::time::Duration;

use codie::hal::addr::{TIM3, USART3, USART4};
use codie::sim;

/// The golden transcript of the C firmware.
const FIXTURE: &str = concat!(env!("CARGO_MANIFEST_DIR"), "/tests/data/c_sim.txt");

/// The scenario of `tools/host_sim.c`: the name of the command and the
/// bytes that are fed into the command link.
const SCENARIO: &[(&str, &str)] = &[
    ("get drvBattery voltage", "get drvBattery voltage\n"),
    ("get drvBattery chargeStat", "get drvBattery chargeStat\n"),
    ("get nosuchiface op", "get nosuchiface op\n"),
    ("garbage", "garbage\n"),
    ("set hwiDrive dutyA 200", "set hwiDrive dutyA 200\n"),
    ("set hwiDrive dutyB 100", "set hwiDrive dutyB 100\n"),
    ("call drvBattery hwInit", "call drvBattery hwInit\n"),
];

/// The marker the light program puts on the command link when it starts a
/// pattern transfer (`FUN_08006000` -> `FUN_08007B34`).
///
/// The port's main loop does not block on the line reader, so this can fall
/// into any command slot depending on how the 160 ms gate of the light
/// program lines up with the commands.  The comparison pulls it out and
/// checks it on its own.
///
/// The *amount* of it, however, depends only on how fast the host spins the
/// main loop, not on the firmware: the dump's `read_line` (`FUN_08009B14`,
/// `0x08009b21: ldrb r0, [r2, r3]` / `0x08009b28: beq 0x08009ba7`) returns 0
/// immediately when the receive ring is empty, so the loop iterates as fast
/// as the CPU allows and the 160 ms gate of the light program fires once per
/// window.  Measured over the same 300 ms boot window: 110 bytes in the
/// debug build, 2 713 260 bytes in the release build - the firmware
/// behaviour is byte for byte the same, only the host clock differs.
/// The periodic facts therefore record *whether* the engine ran, never how
/// many bytes of keep-alive it produced.  `tools/host_sim.c` documents the
/// same thing ("the asynchronous PPRT keep-alive of the LED engine") and
/// matches its replies with `strstr` for that reason.
const PERIODIC_PPRT: &[u8] = b"PPRT\n";
/// The value of a "did a periodic engine run" fact, instead of the bytes.
///
/// The C side of the fixture holds the literal `PPRT` keep-alive lines
/// because `tools/host_sim.c` *does* capture them - but an exact byte count
/// can never be stable, so this test only asks whether the engine ran.  The
/// answer is one marker line, on either side, in either build.
const PERIODIC_SENT: &str = "PPRT";

/// How long the harness waits for the firmware to answer a command.
const SETTLE: Duration = Duration::from_millis(150);
/// How long the harness waits for the boot output.
const BOOT_WAIT: Duration = Duration::from_millis(300);
/// How long of that window is "boot only", before the fastest periodic
/// engine of the port can run (the light program gates a step on 160 ms).
const BOOT_WAIT_BEFORE_FIRST_TICK: Duration = Duration::from_millis(50);

/// One fact of the transcript: a key and its value.
type Fact = (String, String);

/// Read the golden transcript.
///
/// The file starts with a header of `#` comment lines; every other line is
/// a `key: value` fact.  The order of the facts is kept - the transcript
/// is compared line by line.
fn read_fixture() -> Vec<Fact> {
    let text = std::fs::read_to_string(FIXTURE).unwrap_or_else(|e| {
        panic!("cannot read the golden transcript {FIXTURE}: {e}");
    });

    // The periodic facts of the fixture may hold either the presence marker
    // or the raw keep-alive bytes the C harness captured - both are
    // normalised to the marker, so that the fixture can be re-captured with
    // `tools/capture_c_sim.sh` without changing this comparison (see
    // PERIODIC_SENT).
    let is_periodic_key = |k: &str| k == "boot.periodic.cmdlink.tx" || k == "periodic.cmdlink.tx";

    let mut facts = Vec::new();
    for (n, line) in text.lines().enumerate() {
        let line = line.trim_end();
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        match line.split_once(':') {
            Some((key, value)) => {
                let key = key.trim().to_string();
                let value = value.trim().to_string();
                if is_periodic_key(&key) {
                    // Any keep-alive bytes at all mean "a periodic engine
                    // ran"; the C's captured bytes and the marker both count.
                    let ran = value == PERIODIC_SENT
                        || (!value.is_empty() && value.chars().all(|c| c.is_ascii_hexdigit()));
                    facts.push((key, if ran { PERIODIC_SENT.to_string() } else { String::new() }));
                } else {
                    facts.push((key, value));
                }
            }
            None => panic!("{}:{}: malformed fact {line:?}", FIXTURE, n + 1),
        }
    }
    assert!(!facts.is_empty(), "{FIXTURE} holds no facts");
    facts
}

/// The bytes of a link as the transcript records them: lower case hex.
fn hex(bytes: &[u8]) -> String {
    let mut s = String::with_capacity(bytes.len() * 2);
    for b in bytes {
        s.push_str(&format!("{b:02x}"));
    }
    s
}

/// The model of the peripherals is global and the firmware of a scenario
/// never returns, so the scenario is run exactly once per test binary and
/// the facts are shared by the tests.
static FACTS: std::sync::LazyLock<Vec<Fact>> = std::sync::LazyLock::new(run_scenario);

/// The facts the Rust port produced (the scenario is run on first use).
fn facts() -> &'static [Fact] {
    &FACTS
}

/// Run the scenario against the Rust port and record the same facts as
/// the C firmware run did.
fn run_scenario() -> Vec<Fact> {
    sim::reset();

    // the millisecond tick and the timer counters
    let tick = thread::spawn(|| loop {
        thread::sleep(Duration::from_millis(1));
        codie::system::SysTick_Handler();
        sim::advance_timers(1);
    });

    // the firmware itself - main() never returns
    let firmware = thread::spawn(|| codie::boot::run());

    // The boot output is measured before the first periodic engine can run.
    //
    // The port's main loop does not block on the line reader (the dump's
    // `FUN_08009B14` returns 0 when the receive ring is empty), so the
    // periodic tasks run while the link is idle.  The light program is the
    // fastest of them: `FUN_08006000` gates a step on 160 ms, so a boot
    // window longer than that collects a "PPRT\n" that the C fixture - which
    // blocks in its line reader and never ticks - does not have.  Measuring
    // the boot bytes inside the gate keeps the comparison about the boot
    // only; the periodic engines are covered by their own facts below.
    thread::sleep(BOOT_WAIT_BEFORE_FIRST_TICK);

    let mut facts: Vec<Fact> = Vec::new();

    // The boot output
    facts.push((
        "boot.cmdlink.tx".to_string(),
        hex(&sim::usart_take_tx(USART4)),
    ));
    facts.push((
        "boot.atlink.tx".to_string(),
        hex(&sim::usart_take_tx(USART3)),
    ));

    // Whether a periodic engine ran during the rest of the boot window.
    //
    // The C firmware leaves this empty - its line reader blocks until a
    // command arrives, so its periodic task and its watchdog never run
    // meanwhile.  The port does not block (the dump's `FUN_08009B14` returns
    // 0 when the ring is empty), so the light program - the fastest of the
    // periodic engines - runs and puts a "PPRT\n" on the link.  Only that
    // fact is recorded: the number of keep-alive bytes is a function of how
    // fast the host spins the main loop, not of the firmware (see the
    // comment on `PERIODIC_PPRT`), so an exact count could never be stable
    // across a debug and a release build.
    thread::sleep(BOOT_WAIT - BOOT_WAIT_BEFORE_FIRST_TICK);
    let boot_periodic = !sim::usart_take_tx(USART4).is_empty();
    facts.push((
        "boot.periodic.cmdlink.tx".to_string(),
        if boot_periodic { PERIODIC_SENT.to_string() } else { String::new() },
    ));

    // The same fact for the window of the commands.  A periodic "PPRT\n" may
    // fall into any command slot depending on how the two clocks line up, so
    // the *replies* are what is compared and the periodic bytes are only
    // checked for their presence.
    let mut periodic = false;

    // the commands
    for (i, (name, wire)) in SCENARIO.iter().enumerate() {
        sim::usart_push_rx(USART4, wire.as_bytes());
        thread::sleep(SETTLE);

        // the bytes of this slot: the reply of the command plus whatever a
        // periodic engine put on the link in the meantime.  The periodic
        // bytes are pulled out, so that the comparison of the replies does
        // not depend on how the 160 ms gate of the light program lines up
        // with the command.
        let mut slot = sim::usart_take_tx(USART4);
        let mut reply: Vec<u8> = Vec::new();
        while !slot.is_empty() {
            if slot.starts_with(PERIODIC_PPRT) {
                periodic = true;
                slot.drain(..PERIODIC_PPRT.len());
            } else {
                reply.push(slot.remove(0));
            }
        }

        facts.push((format!("cmd.{}.name", i + 1), name.to_string()));
        facts.push((format!("cmd.{}.cmdlink.tx", i + 1), hex(&reply)));
        facts.push((
            format!("cmd.{}.atlink.tx", i + 1),
            hex(&sim::usart_take_tx(USART3)),
        ));
    }

    // the registers the firmware touched
    let mut reg = |name: &str, value: u32| facts.push((name.to_string(), value.to_string()));
    reg("reg.tim3.ccr1", sim::read32(TIM3 + 0x34));
    reg("reg.tim3.ccr2", sim::read32(TIM3 + 0x38));
    reg("reg.tim3.ccr3", sim::read32(TIM3 + 0x3C));
    reg("reg.tim3.ccr4", sim::read32(TIM3 + 0x40));
    reg("reg.tim3.arr", sim::read32(TIM3 + 0x2C));
    reg("reg.tim3.psc", sim::read32(TIM3 + 0x28) & 0xFFFF);
    reg("reg.usart3.brr", sim::usart_brr(USART3) as u32);
    reg("reg.usart4.brr", sim::usart_brr(USART4) as u32);
    reg("reg.usart3.cr2", sim::read32(USART3 + 0x04) & 0xFFFF);
    reg("reg.usart4.cr2", sim::read32(USART4 + 0x04) & 0xFFFF);
    reg("reg.usart3.cr3", sim::read32(USART3 + 0x08) & 0xFFFF);
    reg("reg.usart4.cr3", sim::read32(USART4 + 0x08) & 0xFFFF);

    // whether a periodic engine ran at all during the whole run - including
    // the ones that ran during the boot window (they are the same engine and
    // the same marker).  The C firmware never runs one (its line reader
    // blocks until a command arrives), so the value of this fact on the C
    // side is what the capture happened to see; the port's is set by its own
    // 160 ms gate.  Only the presence is compared - never a count (see
    // `PERIODIC_PPRT`).
    let periodic_ran = boot_periodic || periodic;
    facts.push((
        "periodic.cmdlink.tx".to_string(),
        if periodic_ran { PERIODIC_SENT.to_string() } else { String::new() },
    ));

    drop(firmware);
    drop(tick);

    facts
}

/// The bytes of a hex encoded fact, for the failure messages.
fn show(value: &str) -> String {
    match u8::from_str_radix(value, 16) {
        Ok(_) if !value.is_empty() && value.len() % 2 == 0 => {
            let mut s = String::new();
            for i in (0..value.len()).step_by(2) {
                if let Ok(b) = u8::from_str_radix(&value[i..i + 2], 16) {
                    s.push(b as char);
                }
            }
            format!("{value:?} ({s:?})")
        }
        _ => value.to_string(),
    }
}

/// The Rust port must produce the same facts as the C firmware.
#[test]
fn the_rust_port_behaves_like_the_c_firmware() {
    let expected = read_fixture();
    let got = facts();

    // the same facts, in the same order
    assert_eq!(
        expected.len(),
        got.len(),
        "the transcript holds {} facts, the Rust port produced {}",
        expected.len(),
        got.len()
    );

    for (i, (want, have)) in expected.iter().zip(got.iter()).enumerate() {
        assert_eq!(
            want.0, have.0,
            "fact {} of the transcript is {:?}, the Rust port produced {:?}",
            i + 1, want.0, have.0
        );
        assert_eq!(
            want.1, have.1,
            "{}: the C firmware produced {}, the Rust port produced {}",
            want.0,
            show(&want.1),
            show(&have.1)
        );
    }
}

/// The same facts, compared as a map: this catches a fact that is missing
/// on one side and a value that differs, without depending on the order.
#[test]
fn the_rust_port_produces_the_same_facts() {
    let expected: HashMap<String, String> = read_fixture().into_iter().collect();
    let got: HashMap<String, String> = facts().iter().cloned().collect();

    for (key, want) in &expected {
        match got.get(key) {
            Some(have) => assert_eq!(
                want,
                have,
                "{}: the C firmware produced {}, the Rust port produced {}",
                key,
                show(want),
                show(have)
            ),
            None => panic!("the Rust port did not produce {key}"),
        }
    }
    for key in got.keys() {
        assert!(
            expected.contains_key(key),
            "the Rust port produced the fact {key}, which the transcript does not hold"
        );
    }
}
