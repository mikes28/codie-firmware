//! hwiLine - the line follower sensor array (C: `fw/modules/hwi_line.c`).
//!
//! Two line sensors are read from ADC1 and compared against a per-sensor
//! calibration value; the reconstruction reports one bit per sensor in the
//! mask (the original derives a single line position).

use crate::hal::{addr, gpio, rcc, util};
use crate::modules::hwi_adc;
use crate::proto::{fmt, proxy};
use crate::state::Global;

// the original's line sensor groups carry the ids 0x20 and 0x40 (unused by
// the reconstruction, kept for traceability)
#[allow(dead_code)]
const LINE_SENSOR_A_ID: u32 = 0x20;
#[allow(dead_code)]
const LINE_SENSOR_B_ID: u32 = 0x40;
// the reconstruction reads them from ADC1 channel 5 and channel 6
const LINE_ADC_A: u8 = 5;
const LINE_ADC_B: u8 = 6;

// PA4 drives the sensor bar
const LINE_EN_PORT: u32 = addr::GPIOA;
const LINE_EN_PIN: u8 = 4;

// number of sensors that are reported in the mask
#[allow(dead_code)]
const LINE_SENSOR_COUNT: usize = 2;

struct State {
    line_base_a: u16, // calibration values
    line_base_b: u16,
    line_ready: bool,
}

static S: Global<State> =
    Global::new(State { line_base_a: 0, line_base_b: 0, line_ready: false });

/// `hwiLine_init`
pub fn init() {
    proxy::register(proxy::Iface { name: "hwiLine", handler: handler });
    if S.get().line_ready {
        return;
    }
    // RCC AHBENR 0x20000 (GPIOA)
    rcc::ahb_en(0x20000, true);

    let s = S.get();
    let cfg = gpio::PerPinCfg {
        mask: 1u32 << LINE_EN_PIN,
        mode: gpio::MODE_OUTPUT,
        speed: 0,
        otype: 0,
        pull: gpio::PULL_NONE,
    };
    gpio::config_per_pin(LINE_EN_PORT, &cfg);
    gpio::set(LINE_EN_PORT, 1u16 << LINE_EN_PIN); // sensor bar on

    s.line_base_a = 0;
    s.line_base_b = 0;
    s.line_ready = true;
}

/// `hwiLine_read`
pub fn read(sensor: u8) -> u16 {
    let needs_init = !S.get().line_ready;
    if needs_init {
        init();
    }
    match sensor {
        0 => {
            let value = hwi_adc::read(LINE_ADC_A);
            let s = S.get();
            if s.line_base_a == 0 {
                s.line_base_a = value;
            }
            value
        }
        1 => {
            let value = hwi_adc::read(LINE_ADC_B);
            let s = S.get();
            if s.line_base_b == 0 {
                s.line_base_b = value;
            }
            value
        }
        _ => 0,
    }
}

// The original compares the two line sensors against their calibrated
// values (FUN_08009444) and derives one line position from them.  The
// reconstruction reports a bit per sensor in the mask.
/// `hwiLine_mask`
pub fn mask() -> u16 {
    let needs_init = !S.get().line_ready;
    if needs_init {
        init();
    }
    let a = read(0);
    let b = read(1);
    let s = S.get();
    let mut mask = 0u16;
    if a > s.line_base_a {
        mask |= 0x0001;
    }
    if b > s.line_base_b {
        mask |= 0x0002;
    }
    mask
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
            proxy::resp_value(op, "hwiLine", opname, buf.as_str());
        }
        return;
    }
    if opname == "mask" {
        if op == proxy::OP_GET {
            let buf = fmt::int(mask() as i32);
            proxy::resp_value(op, "hwiLine", opname, buf.as_str());
        }
        return;
    }
    if op == proxy::OP_GET {
        let buf = fmt::int(mask() as i32);
        proxy::resp_value(op, "hwiLine", opname, buf.as_str());
    }
}