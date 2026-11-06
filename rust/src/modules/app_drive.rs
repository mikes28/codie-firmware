//! appDrive - the speed loop of the two motors (C: `fw/modules/app_drive.c`).
//!
//! Original handler: 0x08003849 (FUN_08003848) - it only stamps the shared
//! application time (FUN_08003040) and returns 0, it answers no ops.
//!
//! The real behaviour of the original lives in the periodic function
//! FUN_08003330, which is called from FUN_080063EC (the periodic part of
//! main's loop): it closes the speed loop of the two motors, the encoders
//! (FUN_08008E00) give the speed, the controller (FUN_08008AD0) sets the
//! motor duty.
//!
//! The rebuild keeps the same structure: appDrive_tick() runs the speed loop
//! once per main loop iteration.

use crate::modules::{hwi_drive, hwi_enc};
use crate::proto::proxy;
use crate::state::Global;

const APP_DRIVE_MOTORS: usize = 2;

/// The interval of the speed loop, in milliseconds (the dump's `200`).
const DRIVE_STEP_MS: u32 = 200;

/// The state of the module (the C file scope variables).
struct State {
    target: [i32; APP_DRIVE_MOTORS],   /* target speed, counts/tick */
    speed: [i32; APP_DRIVE_MOTORS],    /* measured speed            */
    duty: [i32; APP_DRIVE_MOTORS],     /* last duty that was set    */
    last_count: [i32; APP_DRIVE_MOTORS],
    /* when the speed loop may run next (the millisecond tick) */
    next_tick_ms: u32,
}

static S: Global<State> = Global::new(State {
    target: [0; APP_DRIVE_MOTORS],
    speed: [0; APP_DRIVE_MOTORS],
    duty: [0; APP_DRIVE_MOTORS],
    last_count: [0; APP_DRIVE_MOTORS],
    next_tick_ms: 0,
});

/// the interface handler of the module
///
/// Original: `appDrive_handler` (FUN_08003848) - a stub that only refreshes
/// the application timestamp and answers no op.
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    /* the original's handler only refreshes the application timestamp */
}

/* from FUN_08003330 (speed loop of the two motors).
 *
 * The original rate-limits the loop: it stamps the time with
 * `FUN_0800A558(0x20000f58,&...)` and only runs the controller when more
 * than 200 ms have passed (`... 200 < local_28`).  A main loop that does not
 * block on the line reader - which is what the dump's `FUN_08009B14` does -
 * would otherwise run the speed loop on every iteration.
 */
/// `appDrive_tick`
pub fn tick() {
    let st = S.get();

    // the dump's millisecond gate (200 ms)
    let now = crate::hal::util::millis();
    if now < st.next_tick_ms {
        return;
    }
    st.next_tick_ms = now + DRIVE_STEP_MS;

    for i in 0..APP_DRIVE_MOTORS {
        let count = hwi_enc::count(i as u8);
        // the C subtracts two int32_t values; wrap on overflow to match the
        // original's two's complement behaviour in debug and release builds.
        let delta = count.wrapping_sub(st.last_count[i]);

        st.last_count[i] = count;
        st.speed[i] = delta;

        if st.target[i] != 0 {
            let err = st.target[i].wrapping_sub(delta);
            let duty = st.duty[i].wrapping_add(err / 4);

            let duty = if duty > 100 {
                100
            } else if duty < -100 {
                -100
            } else {
                duty
            };
            st.duty[i] = duty;
            hwi_drive::set_duty(i as u8, duty);
        } else if st.duty[i] != 0 {
            st.duty[i] = 0;
            hwi_drive::set_duty(i as u8, 0);
        }
    }
}

/// `appDrive_init`
pub fn init() {
    let st = S.get();

    for i in 0..APP_DRIVE_MOTORS {
        st.target[i] = 0;
        st.speed[i] = 0;
        st.duty[i] = 0;
        st.last_count[i] = hwi_enc::count(i as u8);
    }
    st.next_tick_ms = crate::hal::util::millis() + DRIVE_STEP_MS;
    proxy::register(proxy::Iface { name: "appDrive", handler: handler });
}