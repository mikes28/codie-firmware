//! hwiBuzz - the buzzer (C: `fw/modules/hwi_buzz.c`).

use crate::hal::{addr, gpio, rcc, timer, util};
use crate::proto::{fmt, proxy};
use crate::state::Global;

const BUZZ_TIMER: u32 = addr::TIM16;
const BUZZ_PIN: u8 = 8;
const BUZZ_PORT: u32 = addr::GPIOB;
// the original uses 0x60 (96) for its buzzer timer
const BUZZ_PSC: u16 = 96;
// percent, default duty
const BUZZ_DUTY: u32 = 50;

#[derive(Clone, Copy)]
struct BuzzStep {
    freq: u16, // Hz
    ms: u16,   // duration in milliseconds
}

// The tone patterns of the original are queued as (freq, duration) pairs by
// the application bus; the reconstruction offers a small fixed set of
// patterns.  Copied verbatim from the C.
const BUZZ_PATTERN_0: [BuzzStep; 2] =
    [BuzzStep { freq: 2000, ms: 100 }, BuzzStep { freq: 0, ms: 0 }];
const BUZZ_PATTERN_1: [BuzzStep; 4] = [
    BuzzStep { freq: 2000, ms: 80 },
    BuzzStep { freq: 0, ms: 60 },
    BuzzStep { freq: 2000, ms: 80 },
    BuzzStep { freq: 0, ms: 0 },
];
const BUZZ_PATTERN_2: [BuzzStep; 2] =
    [BuzzStep { freq: 1200, ms: 500 }, BuzzStep { freq: 0, ms: 0 }];
const BUZZ_PATTERN_3: [BuzzStep; 6] = [
    BuzzStep { freq: 2500, ms: 60 },
    BuzzStep { freq: 0, ms: 60 },
    BuzzStep { freq: 2500, ms: 60 },
    BuzzStep { freq: 0, ms: 60 },
    BuzzStep { freq: 2500, ms: 60 },
    BuzzStep { freq: 0, ms: 0 },
];
const BUZZ_PATTERN_4: [BuzzStep; 4] = [
    BuzzStep { freq: 1000, ms: 100 },
    BuzzStep { freq: 2000, ms: 100 },
    BuzzStep { freq: 3000, ms: 100 },
    BuzzStep { freq: 0, ms: 0 },
];

const BUZZ_PATTERNS: [&[BuzzStep]; 5] = [
    &BUZZ_PATTERN_0,
    &BUZZ_PATTERN_1,
    &BUZZ_PATTERN_2,
    &BUZZ_PATTERN_3,
    &BUZZ_PATTERN_4,
];
const BUZZ_PATTERN_COUNT: usize = BUZZ_PATTERNS.len();

struct State {
    buzz_ready: bool,
    buzz_period: u32, // current ARR value
}

static S: Global<State> = Global::new(State { buzz_ready: false, buzz_period: 1000 });

/// `hwiBuzz_init`
pub fn init() {
    if S.get().buzz_ready {
        return;
    }
    proxy::register(proxy::Iface { name: "hwiBuzz", handler: handler });
    // RCC: GPIOB (AHBENR 0x40000) and the timer (APB2ENR 0x20000)
    rcc::ahb_en(0x40000, true);
    rcc::apb2_en(0x20000, true);

    let cfg = gpio::PerPinCfg {
        mask: 1u32 << BUZZ_PIN,
        mode: gpio::MODE_ALT,
        speed: 0,
        otype: 0,
        pull: gpio::PULL_NONE,
    };
    gpio::config_per_pin(BUZZ_PORT, &cfg);
    gpio::set_af(BUZZ_PORT, BUZZ_PIN, 2); // AF2 = TIM16_CH1

    timer::pwm_init(BUZZ_TIMER, BUZZ_PSC, 0, 1);
    let s = S.get();
    // (ARR = 0 above, then the running period is set next)
    s.buzz_period = 1000;
    timer::set_arr(BUZZ_TIMER, s.buzz_period);
    // BDTR.MOE - FUN_0800AFBC
    timer::moe(BUZZ_TIMER, true);
    // ARPE - FUN_0800AF74
    timer::arpe(BUZZ_TIMER, true);
    timer::start(BUZZ_TIMER); // FUN_0800AFA0
    s.buzz_ready = true;
}

// from FUN_0800849C - ARR = timerclock / freq - 1
fn set_frequency(freq_hz: u32) {
    if freq_hz == 0 {
        return;
    }
    let mut period = addr::TIMCLK_HZ / freq_hz;
    if period == 0 {
        period = 1;
    }
    period -= 1;
    S.get().buzz_period = period;
    timer::set_arr(BUZZ_TIMER, period);
}

// from FUN_08008514 - CCR = ARR * duty / 100
fn set_duty(duty_percent: u32) {
    let period = S.get().buzz_period;
    let mut dc = duty_percent;
    if dc > 100 {
        dc = 100;
    }
    // the original computes `(uint32_t)(buzz_period * duty_percent)` which
    // wraps on overflow; reproduce that so debug and release behave the same.
    let ccr = period.wrapping_mul(dc) / 100;
    timer::pwm_duty(BUZZ_TIMER, 1, ccr);
}

// Play a tone pattern.  The original queues the tones and plays them
// from the main loop tick (FUN_08008574); the reconstruction plays them
// synchronously so that the module works without an extra tick hook.
/// `hwiBuzz_play`
pub fn play(pattern: u8) -> i32 {
    let needs_init = !S.get().buzz_ready;
    if needs_init {
        init();
    }
    if pattern as usize >= BUZZ_PATTERN_COUNT {
        return -1;
    }
    let steps = BUZZ_PATTERNS[pattern as usize];
    for step in steps {
        if step.ms == 0 {
            break;
        }
        if step.freq != 0 {
            set_frequency(step.freq as u32);
            set_duty(BUZZ_DUTY);
        } else {
            set_duty(0); // silence
        }
        util::delay_ms(step.ms as u32);
    }
    set_duty(0);
    0
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */
/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    let mut v = [0i32; 2];
    let n = util::parse_args(args, &mut v);

    if opname == "play" {
        let mut pattern = if n > 0 { v[0] } else { 0 };
        if pattern < 0 {
            pattern = 0;
        }
        if pattern >= BUZZ_PATTERN_COUNT as i32 {
            pattern = 0;
        }
        play(pattern as u8);
        let buf = fmt::int(pattern);
        proxy::resp_value(op, "hwiBuzz", opname, buf.as_str());
        return;
    }
    if opname == "stop" {
        set_duty(0);
        let buf = fmt::int(0);
        proxy::resp_value(op, "hwiBuzz", opname, buf.as_str());
        return;
    }
    if op == proxy::OP_GET {
        let buf = fmt::int(0);
        proxy::resp_value(op, "hwiBuzz", opname, buf.as_str());
    }
}