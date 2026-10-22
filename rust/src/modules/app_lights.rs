//! appLights - the light programs of the LED array (C: `fw/modules/app_lights.c`).
//!
//! Original handler: 0x08005F01 (0x08005F00) - in the original it is a stub
//! ("movs r0,#0; bx lr"), it answers no ops.
//!
//! The behaviour of the original module (0x08005E00..):
//!   - a table of light programs (0x0800C414, 11 entries), each program is a
//!     sequence of steps,
//!   - the current program and the current step are kept in the state block
//!     at 0x20000358,
//!   - FUN_08005FBC selects a program (called with 1 at boot, from
//!     FUN_08006338),
//!   - FUN_08006000 (called from FUN_080063EC, the periodic part of main's
//!     loop) runs one step of the program and writes the 11 LED values
//!     (DAT_20000385...) to the LED array with FUN_080090A8.
//!
//! The rebuild keeps that structure: a program table, a step counter and a
//! tick that runs one step per main loop iteration.

use crate::modules::hwi_led;
use crate::proto::proxy;
use crate::state::Global;

const APP_LIGHTS_LEDS: usize = 11;
const APP_LIGHTS_MAXPROG: usize = 4;

// a step is one byte: the bits 0..3 select the LED, the bits 4..7 give the
// brightness (0..15 -> duty 0..255).  0xFF switches all LEDs off.
const PROG_OFF: &[u8] = &[0xFF];
const PROG_CHASE: &[u8] = &[
    0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xFF,
];
const PROG_BREATHE: &[u8] = &[
    0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x5A, 0x4A, 0x3A, 0x2A, 0x1A, 0xFF,
];
const PROG_SPARKLE: &[u8] = &[0x02, 0x05, 0x08, 0x0B, 0x08, 0x05, 0x02, 0xFF];

const PROGS: [&[u8]; APP_LIGHTS_MAXPROG] = [PROG_OFF, PROG_CHASE, PROG_BREATHE, PROG_SPARKLE];

/// The state of the module (the C file scope variables).
struct State {
    prog: u8,    /* the selected program   */
    step: u8,    /* the current step       */
    running: u8,
}

static S: Global<State> = Global::new(State {
    prog: 0,
    step: 0,
    running: 0,
});

/// the interface handler of the module
///
/// Original: `appLights_handler` - a stub (the original answers no op).
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    /* the original's handler is a stub */
}

/* from FUN_08006000 - run one step of the selected program */
/// `appLights_tick`
pub fn tick() {
    let st = S.get();

    if st.prog as usize >= APP_LIGHTS_MAXPROG || st.running == 0 {
        return;
    }
    let prog = PROGS[st.prog as usize];
    if st.step as usize >= prog.len() {
        st.running = 0;
        return;
    }

    let mut duty: u8 = 0;
    let mut led: u8 = 0xFF;
    let b = prog[st.step as usize];
    if b != 0xFF {
        led = b & 0x0F;
        duty = (b >> 4) * 17; // 0..15 * 17 -> 0..255
    }

    for i in 0..APP_LIGHTS_LEDS {
        hwi_led::set_pwm(i as u8, if i as u8 == led { duty } else { 0 });
    }

    if st.step == 0 {
        hwi_led::pattern_request(); // "PPRT\n" - pattern transfer starts
    }
    // the C increments a uint8_t; wrap on overflow to match the original.
    st.step = st.step.wrapping_add(1);
    if st.step as usize >= prog.len() {
        st.step = 0; // the program repeats
        hwi_led::pattern_done(); // "PPRET\n" - the transfer has ended
    }
}

/// `appLights_init`
pub fn init() {
    let st = S.get();
    // the original starts the program 1 (FUN_08005FBC(1))
    st.prog = 1;
    st.step = 0;
    st.running = 1;
    proxy::register(proxy::Iface { name: "appLights", handler: handler });
}