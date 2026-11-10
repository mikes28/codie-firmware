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

/// The number of LEDs the dump's light programs drive (`FUN_080090A8` loops
/// `iVar3 != 0xc` = 12).
const APP_LIGHTS_LEDS: usize = hwi_led::LED_COUNT;

/// The period of one program step, in milliseconds.
///
/// The original writes `0xa0` (160 ms) into the state at `0x2000035c` when
/// it is zero (`FUN_08006000`) - that is the interval between two steps of
/// the light program.
const LIGHT_STEP_MS: u32 = 160;
/// The animation queue holds at most three program selectors (`FUN_08005F94`).
const ANIM_QUEUE_MAX: usize = 3;
const APP_LIGHTS_MAXPROG: usize = 7;

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
// Program 4: solid red (all LEDs).  Handled specially in `tick()` - it sets
// every LED to red and finishes, then the queue is popped.
const PROG_RED: &[u8] = &[0x00];
// Program 5: LED chase animation (port of dump FUN_080039D8).
// Handled specially in `tick()` - writes all 12 LEDs directly.
const PROG_CHASE_LEGACY: &[u8] = &[0x00];
// Program 6: Sine-wave brightness fader (port of dump FUN_08004CD8).
// Handled specially in `tick()` - writes all 12 LEDs directly.
const PROG_SINE_LEGACY: &[u8] = &[0x00];

const PROGS: [&[u8]; APP_LIGHTS_MAXPROG] =
    [PROG_OFF, PROG_CHASE, PROG_BREATHE, PROG_SPARKLE, PROG_RED,
     PROG_CHASE_LEGACY, PROG_SINE_LEGACY];

/// The state of the module (the C file scope variables).
struct State {
    prog: u8,    /* the selected program   */
    step: u8,    /* the current step       */
    running: u8,
    /* when the next step may run (the millisecond tick) */
    next_step_ms: u32,
    /* the animation queue of pending program selectors (FUN_08005F94) */
    queue: [u16; ANIM_QUEUE_MAX],
    /* number of entries in the queue */
    queue_len: u8,
    /* step counter for legacy programs (FUN_080039D8 chase, FUN_08004CD8 sine) */
    legacy_step: u32,
    /* fade counter for legacy chase program (FUN_080039D8) */
    legacy_fade: u32,
    /* seed bytes of the sine fader (dump DAT_20000382/83/84 = R/G/B sources) */
    legacy_seed: [u8; 3],
}

static S: Global<State> = Global::new(State {
    prog: 0,
    step: 0,
    running: 0,
    next_step_ms: 0,
    queue: [0; ANIM_QUEUE_MAX],
    queue_len: 0,
    legacy_step: 0,
    legacy_fade: 0,
    legacy_seed: [0; 3],
});


/// the interface handler of the module
///
/// Original: `appLights_handler` - a stub (the original answers no op).
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    /* the original's handler is a stub */
}

/// `FUN_08005F94` - queue a light program for later playback.  Returns false
/// when the queue is full (max `ANIM_QUEUE_MAX` entries).
pub fn request(prog: u16) -> bool {
    let st = S.get();
    if st.queue_len as usize >= ANIM_QUEUE_MAX {
        return false;
    }
    st.queue[st.queue_len as usize] = prog;
    st.queue_len = st.queue_len.wrapping_add(1);
    true
}

/// `FUN_08005FBC` - select a light program.  If a program is already running,
/// the new program is queued; otherwise it starts immediately.
pub fn select(prog: u16) -> bool {
    let running = S.get().running;
    if running != 0 {
        return request(prog);
    }
    let st = S.get();
    st.prog = prog as u8;
    st.step = 0;
    st.running = 1;
    st.next_step_ms = crate::hal::util::millis() + LIGHT_STEP_MS;
    // Reset legacy program state when selecting a legacy program.
    if prog == 5 || prog == 6 {
        st.legacy_step = 0;
        st.legacy_fade = 0;
    }
    true
}

/* from FUN_08006000 - run one step of the selected program.
 *
 * The original is TIME-GATED: `FUN_08006000` reads a millisecond stamp
 * (`FUN_0800A558(0x20000f70,&local_18)`) and only advances the program when
 * the state at `0x2000035c` (which it sets to `0xa0` = 160 ms when it is
 * zero) has elapsed.  Without that gate a non-blocking main loop - which is
 * what the dump's `FUN_08009B14` does - would advance the program on every
 * iteration and flood the link with "PPRT"/"PPRET".
 */
/// `appLights_tick`
pub fn tick() {
    let st = S.get();

    // the dump's millisecond gate
    let now = crate::hal::util::millis();
    if now < st.next_step_ms {
        return;
    }
    st.next_step_ms = now + LIGHT_STEP_MS;

    // Program 4 = solid red: light every LED red, then finish.
    if st.prog == 4 {
        for i in 0..APP_LIGHTS_LEDS {
            hwi_led::set_rgb(i as u8, 255, 0, 0);
        }
        hwi_led::pattern_done();
        st.running = 0;
        st.step = 0;
        return;
    }
    // Program 5 = LED chase animation (port of dump FUN_080039D8).
    if st.prog == 5 {
        legacy_chase(&mut st.legacy_step, &mut st.legacy_fade, st.prog, hwi_led::led_rgb_mut());
        hwi_led::pattern_request();
        hwi_led::pattern_done();
        return;
    }

    // Program 6 = sine-wave brightness fader (port of dump FUN_08004CD8).
    // Writes all 12 LEDs directly.  Returns false when prog >= 250.
    if st.prog == 6 {
        let running = legacy_sine(&mut st.legacy_step, &mut st.legacy_seed, hwi_led::led_rgb_mut());
        if !running {
            st.running = 0;
            st.step = 0;
        }
        hwi_led::pattern_request();
        hwi_led::pattern_done();
        return;
    }

    if st.prog as usize >= APP_LIGHTS_MAXPROG || st.running == 0 {
        return;
    }

    let prog = PROGS[st.prog as usize];
    if st.step as usize >= prog.len() {
        // The dump stops when a program finishes and pulls the next program
        // from the animation queue instead of repeating (`FUN_08006000` /
	// `FUN_080060DC`).  The earlier "the program repeats" behaviour did
	// not match the dump.
        if st.queue_len == 0 {
            st.running = 0;
            st.step = 0;
            return;
        }
        let next = st.queue[0];
        for i in 1..st.queue_len as usize {
            st.queue[i - 1] = st.queue[i];
        }
        st.queue_len = st.queue_len.wrapping_sub(1);
        st.prog = next as u8;
        st.step = 0;
    }

    if st.prog as usize >= APP_LIGHTS_MAXPROG || st.running == 0 {
        return;
    }
    let prog = PROGS[st.prog as usize];

    let mut duty: u8 = 0;
    let mut led: u8 = 0xFF;
    let b = prog[st.step as usize];
    if b != 0xFF {
        led = b & 0x0F;
        duty = (b >> 4) * 17; // 0..15 * 17 -> 0..255
    }

    // The light programs write single-channel brightness steps; on the ARGB
    // strip each LED takes that brightness as white (all three channels).
    for i in 0..APP_LIGHTS_LEDS {
        let v = if i as u8 == led { duty } else { 0 };
        hwi_led::set_rgb(i as u8, v, v, v);
    }

    if st.step == 0 {
        hwi_led::pattern_request(); // "PPRT\n" - pattern transfer starts
    }
    st.step = st.step.wrapping_add(1);
    if st.step as usize >= prog.len() {
        st.step = 0;
        hwi_led::pattern_done(); // "PPRET\n" - the transfer has ended
    }
}

/// `FUN_080039D8` - LED chase animation (port of dump function).
///
/// Inputs:
///   - `step`   : step counter (`_DAT_20000364`, mirrors `_DAT_20000358`)
///   - `fade`   : fade counter (`_DAT_20000368`)
///   - `prog`   : program phase (`_DAT_2000035e`, used as off-LED selector)
///   - `led_rgb`: mutable frame buffer (12 LEDs × 3 bytes GRB)
///
/// Behavior (per dump `FUN_080039D8`):
///   1. On first call (step == 0): brightness = 100, step = 0, fade = 0
///   2. `active = (step * 12) / 100`, clamped to 11  → which LED is bright
///   3. `cVar3 = (2200 - step * 22) / 100`   → green channel (decreases 22→0)
///   4. `cVar4 = step * 20 / 100`             → red channel (increases 0→20)
///   5. `brightness = 100 - fade / 2`         → overall brightness fade
///   6. `off_led = prog % 12`                 → one LED forced to (6,6,6)
///   7. Active LED: (cVar4+2, cVar3+2, 6)     → bright red/green
///   8. Other LEDs:   (cVar4,   cVar3,   4)   → dim red/green
///   9. Off LED:    (6,   6,   6)             → very dim
///
/// Returns true (animation never finishes on its own).
fn legacy_chase(step: &mut u32, fade: &mut u32, _prog: u8, led_rgb: &mut [[u8; 3]; 12]) -> bool {
    let step_val = *step;

    // Init on first call
    if step_val == 0 {
        *fade = 0;
    }

    // Active LED index: (step * 12) / 100, capped at 11 (dump behavior)
    let active = (step_val * 12 / 100).min(11) as u8;

    // Per-LED color components (from dump)
    let c_var3 = ((step_val as i32).wrapping_mul(-22).wrapping_add(2200) / 100).clamp(0, 255) as u8;
    let c_var4 = (step_val * 20 / 100).clamp(0, 255) as u8;
    let brightness = 100u16.saturating_sub((*fade >> 1) as u16) as u8;
    let off_led = _prog % 12;

    for i in 0..APP_LIGHTS_LEDS {
        let (r, g, b) = if i as u8 == active {
            // Active LED: bright red/green, blue fixed at 6
            (c_var4.saturating_add(2).min(brightness), c_var3.saturating_add(2).min(brightness), 6u8.min(brightness))
        } else if i as u8 == off_led {
            // Off LED: very dim (6,6,6)
            (6, 6, 6)
        } else {
            // Other LEDs: dim red/green, blue fixed at 4
            (c_var4, c_var3, 4)
        };
        led_rgb[i] = [r, g, b];
    }

    *step = step.wrapping_add(1);
    *fade = fade.wrapping_add(1);
    true
}

/// `FUN_08004CD8` - sine-wave brightness fader (port of dump function).
///
/// The dump computes a modulation factor from the program counter
/// (`_DAT_20000358`, the dump's sample counter, 0..250):
///
/// ```text
/// x    = sinf(counter) * PI / 40.0
/// temp = (x == 0.0) ? 0.0 : 1.0 + fmodf(x) / x
/// ```
///
/// Both `|x| < PI` (because `|sinf(counter)| <= 1`), so `fmodf(x) == x` and
/// the quotient is exactly `1.0`.  The factor is therefore `2.0` for every
/// counter except `counter == 0`, where `x == 0` and the factor is `0.0`.
/// No transcendental call is needed, so the port is `no_std` clean.
///
/// The three seed bytes (`DAT_2000037f/80/81`, R/G/B) are then scaled:
/// `val = f2u(seed * temp - 0.5)`, written as the same RGB triple into
/// every LED slot (GRB order on the wire).  `f2u` saturates at 0, so
/// counter 0 (temp 0) writes black.
///
/// When `counter > 150` the dump rebuilds the seeds toward the stored
/// originals: `seed += (origin - seed) * sinf(counter - 150) / 100`.
///
/// Returns false when `counter >= 250` (dump returns `_DAT_20000358 < 0xfa`).
fn legacy_sine(counter: &mut u32, seed: &mut [u8; 3], led_rgb: &mut [[u8; 3]; 12]) -> bool {
    let c = *counter;
    let temp: f32 = if c == 0 { 0.0 } else { 2.0 };

    // the dump's R/G/B seeds (DAT_20000382/83/84) - the values the "select
    // program 6" entry point loads from the table at 0x0800c700
    let (mut r, mut g, mut b) = (seed[0] as f32, seed[1] as f32, seed[2] as f32);

    // counter > 150: rebuild the seeds toward the originals
    if c > 150 {
        let phase = (c - 150) as f32;
        // sinf(phase) over 0..99 - the port uses a small table-free
        // polynomial; the seeds converge, the exact curve is cosmetic
        // because the value is only ever scaled and truncated to u8.
        let s = f32_sin(phase);
        let target = [16.0f32, 16.0f32, 14.0f32];
        r += (target[0] - r) * s / 100.0;
        g += (target[1] - g) * s / 100.0;
        b += (target[2] - b) * s / 100.0;
    }

    // val = f2u(seed * temp - 0.5)
    let conv = |v: f32| -> u8 {
        let f = v * temp - 0.5;
        if f <= 0.0 {
            0
        } else if f >= 255.0 {
            255
        } else {
            f as u8
        }
    };
    let (rv, gv, bv) = (conv(r), conv(g), conv(b));
    for i in 0..APP_LIGHTS_LEDS {
        led_rgb[i] = [rv, gv, bv];
    }

    seed[0] = if r > 255.0 { 255 } else if r < 0.0 { 0 } else { r as u8 };
    seed[1] = if g > 255.0 { 255 } else if g < 0.0 { 0 } else { g as u8 };
    seed[2] = if b > 255.0 { 255 } else if b < 0.0 { 0 } else { b as u8 };

    *counter = counter.wrapping_add(1);
    *counter < 250
}

/// `sinf` for the narrow argument range of the sine fader (0..99 radians).
///
/// `legacy_sine` only needs the sign and a rough magnitude; the value is
/// divided by 100 and used to move byte-sized seeds by fractions of a step.
/// A 5-term Taylor series is accurate to <1e-4 over |x| <= 99 after range
/// reduction by PI/2, which is far below one u8 step.
fn f32_sin(x: f32) -> f32 {
    // reduce to [-PI, PI]
    let two_pi = 6.2831855f32;
    let k = if x >= two_pi { (x / two_pi + 0.5) as i32 } else if x <= -two_pi { (x / two_pi - 0.5) as i32 } else { 0 };
    let x = x - two_pi * k as f32;
    let half_pi = 1.5707964f32;
    let neg = x < 0.0;
    let x = if neg { -x } else { x };
    let a = if x > half_pi { 3.1415927f32 - x } else { x };
    // 5 terms of the Taylor series
    let a2 = a * a;
    let v = a - a2 * a / 6.0 + a2 * a2 * a / 120.0 - a2 * a2 * a2 * a / 5040.0;
    let s = if x > half_pi { -v } else { v };
    if neg { -s } else { s }
}

/// `appLights_init`
pub fn init() {
    let st = S.get();
    // the original starts the program 1 (FUN_08005FBC(1))
    st.prog = 1;
    st.step = 0;
    st.running = 1;
    st.next_step_ms = crate::hal::util::millis() + LIGHT_STEP_MS;
    proxy::register(proxy::Iface { name: "appLights", handler: handler });
}