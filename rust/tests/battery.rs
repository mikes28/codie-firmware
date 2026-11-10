//! Complete BMS battery pack flow (drvBattery over the hwiLed single-outline line).
//!
//! The pack query is `drvBattery hwInit` -> `hwi_led::send_raw` on the LED
//! line, and the replies come back bit-banged on PB14 by the TIM15 update
//! interrupt (`tim15_update_irq`), which parks the sequencer in phase 16 so
//! that [`reply_take`] returns `Some`.  That interrupt never fires in host
//! tests, so `hwi_led::simulate_update_irq` injects it - this is the test
//! injection path that makes the flow testable.
//
//! The reply machine (dump `FUN_08006D34`) walks states 1..=7, sending
//! {cc,0xb4}/{cc,0xb8,x}/{cc,0xbe,x} and parsing replies at states 4 and 7.
//! Each `send_raw` arms the sequencer (state 2), so the test must complete
//! the transfer (back to idle, state 0) between ticks, otherwise `link_busy`
//! stalling the machine.  States 4 and 7 only read `reply_take` (they never
//! send), so they are intentionally not gated by `link_busy` - phase 16
//! holding a captured reply is itself "busy", which would otherwise make
//! `reply_take` unreachable.
//!
//! The tests share one global model (the `codie::sim` peripheral model and
//! the module `Global` statics), so they run one at a time under `SERIAL`.
//! Without the lock the parallel default threads interleave their sequencer
//! states and the reply machine sees a reply another test injected.

use codie::modules::{drv_battery, hwi_led};
use std::sync::{Mutex, MutexGuard};

/// Serializes the tests of this file: one global firmware model, one driver.
static SERIAL: Mutex<()> = Mutex::new(());

/// Run `f` while holding the file's lock.
fn serial<F: FnOnce()>(f: F) {
    let guard: MutexGuard<'_, ()> = SERIAL.lock().unwrap_or_else(|e| e.into_inner());
    f();
    drop(guard);
}

/// Reply parsed at reply-machine state 4: temp + voltage.
const REPLY_1: [u8; 8] = [0x00, 0x00, 25, 10, 0x00, 0x01, 0x00, 0x00];
/// Reply parsed at reply-machine state 7: current + remaining (overwrites).
const REPLY_2: [u8; 8] = [0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01];

/// Start each test from a clean module state.
fn setup() {
    codie::sim::reset();
    hwi_led::reset();
    hwi_led::init();
    drv_battery::init();
}

/// Run one main-loop tick of the battery module.
fn tick() {
    drv_battery::tick();
}

/// Complete the armed single-outline transfer with no reply, as the TIM15
/// update interrupt would, returning the sequencer to idle (state 0).
fn complete_no_reply() {
    assert!(hwi_led::simulate_update_irq(None));
}

/// Simulate the update interrupt having bit-banged `reply` back on PB14,
/// leaving the sequencer in phase 16 so `reply_take` returns `Some`.
fn inject_reply(reply: &[u8]) {
    assert!(hwi_led::simulate_update_irq(Some(reply)));
}

#[test]
fn the_complete_bms_pack_flow() {
    serial(|| {
    setup();

    // --- hw_init: arm the first transfer, register the callback -----------
    drv_battery::hw_init();
    complete_no_reply(); // transfer {cc,0x4e,..} completes, sequencer idle
    assert!(!hwi_led::link_busy());
    // --- hw_init_cb runs over two ticks -----------------------------------
    tick(); // hw_ab 1 -> 2, sends {cc,0x48,1}
    complete_no_reply();
    tick(); // hw_ab 2 -> 1, clears callback
    complete_no_reply();
    // hw_cb cleared, hw_ab == 1: reply machine takes over on the next tick
    // --- reply machine: states 1, 2, 3 send commands, no replies ---------
    tick(); // state 1: {cc,0xb4}
    complete_no_reply();
    tick(); // state 2: {cc,0xb8,0}
    complete_no_reply();
    tick(); // state 3: {cc,0xbe,0}
    complete_no_reply();

    // --- state 4: inject reply 1 -> temp + voltage parsed -----------------
    // reply_take needs phase 16; inject it, then the machine parses.
    inject_reply(&REPLY_1); // sequencer 0 -> 16 holding REPLY_1
    tick(); // state 4: reply_take() Some -> temp + voltage, hw_ab 4 -> 6
    assert_eq!(drv_battery::temp_c10(), 25);
    assert_eq!(drv_battery::voltage_mv(), 522);

    // phase 16 is terminal ("busy"); return to idle so state 6 can send.
    complete_no_reply(); // sequencer 16 -> 0

    // --- state 6: send {cc,0xb8,1} ---------------------------------------
    tick(); // hw_ab 6 -> 7
    complete_no_reply();

    // --- state 7: inject reply 2 -> current + remaining parsed ------------
    inject_reply(&REPLY_2); // sequencer 0 -> 16 holding REPLY_2
    tick(); // state 7: reply_take() Some -> current + remaining, hw_ab 7 -> 0
    assert_eq!(drv_battery::current_ma(), 5000);
    assert_eq!(drv_battery::remaining_pct(), 4000);

    // --- flow complete: machine back to idle ------------------------------
    assert_eq!(drv_battery::temp_c10(), 25);
    assert_eq!(drv_battery::voltage_mv(), 522);
    assert_eq!(drv_battery::current_ma(), 5000);
    assert_eq!(drv_battery::remaining_pct(), 4000);
    complete_no_reply(); // sequencer 16 -> 0 after reply_take consumed reply 2
    assert!(!hwi_led::link_busy());
    });
}

#[test]
fn reply_take_returns_none_until_injected() {
    serial(|| {
    setup();

    // Arm a transfer with no reply expected.  Without the injection hook the
    // sequencer never reaches phase 16, so reply_take() is None - exactly the
    // testing problem this hook fixes.
    drv_battery::hw_init();
    complete_no_reply();
    assert_eq!(hwi_led::reply_take(), None);

    // Injecting a reply puts the sequencer in phase 16.
    inject_reply(&REPLY_1);
    assert_eq!(hwi_led::reply_take(), Some(REPLY_1));
    });
}

#[test]
fn simulate_update_irq_protects_held_reply() {
    serial(|| {
    setup();

    // Idle: injecting a reply parks the sequencer in phase 16.
    assert!(hwi_led::simulate_update_irq(Some(&REPLY_1)));
    assert_eq!(hwi_led::reply_take(), Some(REPLY_1));

    // Phase 16 holds the captured reply: injecting a new reply must not clobber it.
    assert!(!hwi_led::simulate_update_irq(Some(&REPLY_2)));
    assert_eq!(hwi_led::reply_take(), Some(REPLY_1));

    // Completing with no reply returns to idle (after reply_take consumed it).
    assert!(hwi_led::simulate_update_irq(None));
    assert!(hwi_led::sequencer_idle());
    });
}

#[test]
fn machine_stalls_without_reply_injection() {
    serial(|| {
    setup();

    drv_battery::hw_init();
    complete_no_reply();
    // hw_init_cb: two ticks, then the reply machine takes over at state 1.
    tick();
    complete_no_reply();
    tick();
    complete_no_reply();

    // States 1..3 send their frames; the transfer completes with no reply
    // each time.  With no injection, state 4 never sees phase 16, so
    // `reply_take` returns None and the machine parks at state 4 forever -
    // holding no line, sending nothing, parsing nothing.
    for _ in 0..3 {
        tick();
        complete_no_reply();
    }
    assert_eq!(drv_battery::temp_c10(), 0);
    assert_eq!(drv_battery::voltage_mv(), 0);

    // Many more ticks must not change anything: the stall is bounded, not a
    // spin or a freeze.
    for _ in 0..16 {
        tick();
    }
    assert_eq!(drv_battery::temp_c10(), 0);
    assert_eq!(drv_battery::voltage_mv(), 0);
    assert_eq!(drv_battery::current_ma(), 0);
    assert_eq!(drv_battery::remaining_pct(), 0);
    });
}

#[test]
fn state_7_parse_uses_shift_after_add() {
    serial(|| {
    setup();

    drv_battery::hw_init();
    complete_no_reply();
    tick();
    complete_no_reply();
    tick();
    complete_no_reply();
    tick();
    complete_no_reply();
    tick();
    complete_no_reply();
    tick();
    complete_no_reply();

    // State 4 needs a reply to advance to 6; use REPLY_1 which leaves
    // temp/voltage set (state 6/7 overwrite current/remaining).
    inject_reply(&REPLY_1);
    tick();
    complete_no_reply();
    tick(); // state 6 sends {cc,0xb8,1}
    complete_no_reply();

    // The machine computes `reply[5] + (reply[4] << 8)` * 0x271 >> 6.  With
    // reply = {0,0,0,0,0,2,0,1} that is (2 + 0) * 625 >> 6 = 5000, not the
    // little-endian (0 + 0x200) * 625 >> 6 = 19 the byte order alone would
    // give.  Lock the parse so the precedence stays right.
    inject_reply(&REPLY_2);
    tick();
    assert_eq!(drv_battery::current_ma(), 5000);

    // `reply[7] + (reply[6] << 8)` = 1, 1 * 625 >> 5 = 19, minus 1000 = -981
    // as i16 (wrapping at the cast).  Lock it too.
    assert_eq!(drv_battery::remaining_pct(), 4000);
    });
}
