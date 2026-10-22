//! hwiLightSense - the ambient light sensor (C: `fw/modules/hwi_lightsense.c`).
//!
//! The light level is averaged over 8 ADC samples of channel 3; the original
//! command line handler does not answer with a value, the reconstruction
//! reports the level.

use crate::hal::util;
use crate::modules::hwi_adc;
use crate::proto::{fmt, proxy};
use crate::state::Global;

// the original's light sensor group id is 4 (unused by the reconstruction)
#[allow(dead_code)]
const LIGHT_SENSOR_ID: u8 = 4;
// the reconstruction reads it from ADC1 channel 3
const LIGHT_ADC_CHANNEL: u8 = 3;
// number of samples the original averages (FUN_08009340 counts up to the
// value in DAT_20000868)
const LIGHT_AVG_COUNT: u16 = 8;

struct State {
    light_sum: u32,
    light_count: u16,
    light_level: u16,
    light_ready: bool,
}

static S: Global<State> = Global::new(State {
    light_sum: 0,
    light_count: 0,
    light_level: 0,
    light_ready: false,
});

/// `hwiLightSense_init`
pub fn init() {
    proxy::register(proxy::Iface { name: "hwiLightSense", handler: handler });
    // The ADC is brought up by hwiAdc_init(); the original's light
    // sensor group is configured through the shared sensor table.
    let s = S.get();
    s.light_sum = 0;
    s.light_count = 0;
    s.light_level = 0;
    s.light_ready = true;
}

/// `hwiLightSense_read`
pub fn read(sensor: u8) -> u16 {
    let needs_init = !S.get().light_ready;
    if needs_init {
        init();
    }
    if sensor != 0 {
        return 0;
    }
    let sample = hwi_adc::read(LIGHT_ADC_CHANNEL);
    let s = S.get();
    s.light_sum = s.light_sum.wrapping_add(sample as u32);
    s.light_count = s.light_count.wrapping_add(1);
    if s.light_count >= LIGHT_AVG_COUNT {
        s.light_level = (s.light_sum / s.light_count as u32) as u16;
        s.light_sum = 0;
        s.light_count = 0;
    }
    s.light_level
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */
// The C handler guards `opname == 0` (a missing token); the Rust
// dispatcher always yields a non-empty opname, so that null check is gone.
/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    let mut v = [0i32; 2];
    let n = util::parse_args(args, &mut v);

    if opname == "read" {
        let mut sensor = if n > 0 { v[0] } else { 0 };
        if sensor < 0 {
            sensor = 0;
        }
        if op == proxy::OP_GET {
            let buf = fmt::int(read(sensor as u8) as i32);
            proxy::resp_value(op, "hwiLightSense", opname, buf.as_str());
        }
        return;
    }
    if opname == "level" {
        if op == proxy::OP_GET {
            // note: "level" advances the 8 sample accumulator, like the C's
            // `hwiLightSense_read(0u)`.
            let buf = fmt::int(read(0) as i32);
            proxy::resp_value(op, "hwiLightSense", opname, buf.as_str());
        }
        return;
    }
    if op == proxy::OP_GET {
        let buf = fmt::int(read(0) as i32);
        proxy::resp_value(op, "hwiLightSense", opname, buf.as_str());
    }
}