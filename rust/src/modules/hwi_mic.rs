//! hwiMic - the microphone level (C: `fw/modules/hwi_mic.c`).
//!
//! The original's `FUN_080094F8` runs a tracking filter over the
//! microphone ADC group once per main loop.  The command line handler of the
//! original does not answer with a value; the reconstruction reports the
//! level.

use crate::modules::hwi_adc;
use crate::proto::{fmt, proxy};
use crate::state::Global;

// the original's microphone sensor group id is 2 (kept for traceability)
#[allow(dead_code)]
const MIC_SENSOR_ID: u8 = 2;
// the reconstruction reads the microphone from ADC1 channel 2
const MIC_ADC_CHANNEL: u8 = 2;

struct State {
    mic_level: u16,
    mic_ready: bool,
}

static S: Global<State> = Global::new(State { mic_level: 0, mic_ready: false });

/// `hwiMic_init`
pub fn init() {
    proxy::register(proxy::Iface { name: "hwiMic", handler: handler });
    let s = S.get();
    // The ADC itself is brought up by hwiAdc_init() (module hwi_adc.c); the
    // original's microphone group is configured by the application layer
    // through the same sensor table.  Nothing else to do here.
    // Unlike the other modules this init has no ready guard: it always
    // resets the level (matches the C).
    s.mic_level = 0;
    s.mic_ready = true;
}

/// `hwiMic_level`
pub fn level() -> u16 {
    let needs_init = !S.get().mic_ready;
    if needs_init {
        init();
    }
    let sample = hwi_adc::read(MIC_ADC_CHANNEL);
    let s = S.get();
    let old = s.mic_level;

    // the original keeps the sample that is closest to the previous
    // value (FUN_080071DC with mode 1) - a small deviation is ignored
    // so that the level does not jump on a single noisy conversion.
    // The two `uint32_t` subtractions wrap on underflow; that is the
    // original's `|sample - level| > 64` test expressed in unsigned math.
    let diff_hi = (sample as u32).wrapping_sub(old as u32);
    let diff_lo = (old as u32).wrapping_sub(sample as u32);
    if old == 0 || (diff_hi > 64 && diff_lo > 64) {
        s.mic_level = sample;
    } else {
        s.mic_level = ((old as u32).wrapping_mul(3).wrapping_add(sample as u32) / 4) as u16;
    }
    s.mic_level
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */
// The C handler discards `args` (`(void)args;`) and guards `opname == 0`;
// the Rust dispatcher always yields a non-empty opname, so the null check
// is gone.
/// the interface handler of the module
pub fn handler(op: u8, opname: &str, _args: &str) {
    if opname == "level" {
        if op == proxy::OP_GET {
            let buf = fmt::int(level() as i32);
            proxy::resp_value(op, "hwiMic", opname, buf.as_str());
        }
        return;
    }
    if op == proxy::OP_GET {
        let buf = fmt::int(level() as i32);
        proxy::resp_value(op, "hwiMic", opname, buf.as_str());
    }
}