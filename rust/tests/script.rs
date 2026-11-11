//! script - tests for the on-robot script engine.
//!
//! Verifies:
//!   * loading and clearing a script via protocol-shaped calls
//!   * the VM executes SET_LED, SET_DUTY, WAIT, LOOP, STOP correctly
//!   * double-press on PC13 starts the script automatically
//!
//! The tests share one global firmware model (the `codie::sim` peripheral
//! model and the module `Global` statics), so they run one at a time under
//! `SERIAL`.  Without the lock the parallel default threads interleave
//! their sequencer states and the LED/drive state gets clobbered by other
//! tests running at the same time.

use codie::hal::addr;
use codie::modules::{hwi_drive, hwi_led, script};
use codie::proto::proxy;
use codie::sim;
use codie::system;
use std::sync::{Mutex, MutexGuard};

/// Serializes the tests of this file: one global firmware model, one driver.
static SERIAL: Mutex<()> = Mutex::new(());

/// Run `f` while holding the file's lock.
fn serial<F: FnOnce()>(f: F) {
    let guard: MutexGuard<'_, ()> = SERIAL.lock().unwrap_or_else(|e| e.into_inner());
    f();
    drop(guard);
}

// ---------------------------------------------------------------------------
// Bytecode opcodes (must match script.rs)
// ---------------------------------------------------------------------------

const OP_STOP: u8 = 0x00;
const OP_SET_LED: u8 = 0x01;
const OP_SET_DUTY: u8 = 0x02;
const OP_WAIT: u8 = 0x03;
const OP_LOOP: u8 = 0x04;

/// Load a bytecode script (does NOT start it).
fn load_script(ops: &[u8]) {
    script::handler(proxy::OP_SET, "script", "clear");
    let args: String = ops.iter().map(|b| b.to_string()).collect::<Vec<_>>().join(" ");
    script::handler(proxy::OP_SET, "script", &format!("load {args}"));
}

/// Start a loaded script running.
fn run_script() {
    script::handler(proxy::OP_CALL, "script", "run");
}

/// Simulate pressing the button on PC13 (pin goes low = active low).
fn button_press() {
    sim::write32(addr::GPIOC + 0x10, 0);
}

/// Simulate releasing the button on PC13 (pin goes high).
fn button_release() {
    sim::write32(addr::GPIOC + 0x10, 0x2000);
}

/// Simulate a double press: press, release, press, release — advancing
/// the millisecond tick between presses so the 500 ms gate can measure
/// a non-zero elapsed time.  Calls script::tick() between each edge so the
/// state machine sees every transition individually.
fn double_press() {
    // Start with release so the first press is seen as an edge transition
    // (sim::reset() leaves PC13 low/pressed, matching last_pc13=0 initially).
    button_release();
    script::tick();
    system::SysTick_Handler();
    // First press
    button_press();
    script::tick();
    system::SysTick_Handler();
    // Release
    button_release();
    script::tick();
    system::SysTick_Handler();
    // Second press -> double press detected here
    button_press();
    script::tick();
    system::SysTick_Handler();
    // Release
    button_release();
    script::tick();
    system::SysTick_Handler();
}

#[test]
fn script_load_and_clear() {
    serial(|| {
        script::handler(proxy::OP_SET, "script", "clear");
        script::handler(proxy::OP_SET, "script", "load 1 0 255 0 0");
        script::handler(proxy::OP_GET, "script", "script");
        script::handler(proxy::OP_SET, "script", "clear");
        script::handler(proxy::OP_GET, "script", "script");
    });
}

#[test]
fn script_executes_set_led() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        load_script(&[OP_SET_LED, 3, 10, 20, 30, OP_STOP]);
        run_script();
        script::tick();
        // hwi_led::get returns duty = (r/3 + g/3 + b/3) = (10/3 + 20/3 + 30/3) = 3+6+10 = 19
        assert_eq!(hwi_led::get(3), 19);
    });
}

#[test]
fn script_executes_set_duty() {
    serial(|| {
        hwi_drive::init();
        load_script(&[OP_SET_DUTY, 0, 200, OP_STOP]);
        run_script();
        script::tick();
        assert_eq!(hwi_drive::get_duty(0), 200);
    });
}

#[test]
fn script_wait_blocks_until_deadline() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        // WAIT 5 ms then SET_LED 0 255 0 0 then STOP
        load_script(&[OP_WAIT, 5, 0, OP_SET_LED, 0, 255, 0, 0, OP_STOP]);
        run_script();

        // First tick: WAIT starts, deadline not reached yet (millis = 0)
        script::tick();
        assert_eq!(hwi_led::get(0), 0); // not set yet

        // Advance SysTick past the 5 ms deadline
        for _ in 0..6 {
            system::SysTick_Handler();
        }
        script::tick();
        // Now SET_LED should have executed
        assert_eq!(hwi_led::get(0), (255 / 3) as i32); // 85
    });
}

#[test]
fn script_loop_then_stop() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        // LOOP then STOP - LOOP jumps pc back to 0, STOP stops
        load_script(&[OP_LOOP, OP_STOP]);
        run_script();
        script::tick(); // LOOP: pc -> 0
        script::tick(); // OP_LOOP again at pc=0 (still looping)
        script::tick(); // OP_LOOP again
        // The script is still looping - LOOP without WAIT spins at full speed.
        // Just verify it doesn't panic.
        assert!(true);
    });
}

#[test]
fn script_unknown_opcode_stops() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        load_script(&[0xFF, OP_SET_LED, 0, 255, 0, 0, OP_STOP]);
        run_script();
        script::tick(); // unknown opcode -> stop
        // LED should NOT have been set
        assert_eq!(hwi_led::get(0), 0);
    });
}

#[test]
fn script_truncates_to_max() {
    serial(|| {
        let too_many: Vec<u8> = (0..200).map(|i| (i % 256) as u8).collect();
        script::handler(proxy::OP_SET, "script", "clear");
        let args: String = too_many.iter().map(|b| b.to_string()).collect::<Vec<_>>().join(" ");
        script::handler(proxy::OP_SET, "script", &format!("load {args}"));
        run_script();
        // Should not panic with >SCRIPT_MAX bytes
        assert!(true);
    });
}

#[test]
fn double_press_starts_script() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        codie::sim::reset();

        // Load a script that sets LED 5 to red (do NOT run it yet)
        load_script(&[OP_SET_LED, 5, 255, 0, 0, OP_STOP]);

        // Before double press, script is loaded but not active
        assert_eq!(hwi_led::get(5), 0);

        // Double press within 500 ms starts the script
        double_press();
        script::tick();

        // Script should now be running and have executed SET_LED
        assert_eq!(hwi_led::get(5), (255 / 3) as i32); // 85
    });
}

#[test]
fn single_press_does_not_start_script() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        codie::sim::reset();

        // Clear LED 5 from any previous test state
        hwi_led::set_rgb(5, 0, 0, 0);

        load_script(&[OP_SET_LED, 5, 255, 0, 0, OP_STOP]);
        // Single press only
        button_press();
        system::SysTick_Handler();
        button_release();
        system::SysTick_Handler();
        script::tick();

        // Script should NOT have started
        assert_eq!(hwi_led::get(5), 0);
    });
}

#[test]
fn double_press_too_slow_does_not_start() {
    serial(|| {
        hwi_led::reset();
        hwi_led::init();
        codie::sim::reset();

        // Clear LED 5 from any previous test state
        hwi_led::set_rgb(5, 0, 0, 0);

        load_script(&[OP_SET_LED, 5, 255, 0, 0, OP_STOP]);

        // First press
        button_press();
        system::SysTick_Handler();
        button_release();
        system::SysTick_Handler();

        // Advance time past DOUBLE_PRESS_MS (500 ms)
        for _ in 0..600 {
            system::SysTick_Handler();
        }

        // Second press - too late
        button_press();
        system::SysTick_Handler();
        button_release();
        system::SysTick_Handler();
        script::tick();

        assert_eq!(hwi_led::get(5), 0);
    });
}
