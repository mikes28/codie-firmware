//! hwiBuzz - the buzzer (C: `fw/modules/hwi_buzz.c`).

use crate::hal::{addr, gpio, rcc, timer, util};
use crate::proto::{fmt, proxy};
use crate::state::Global;

const BUZZ_TIMER: u32 = addr::TIM16;
const BUZZ_PIN: u8 = 8;
const BUZZ_PORT: u32 = addr::GPIOB;
// The prescaler of the tone timer, from the dump: FUND_08008380 @0x8008380
// builds its base-config struct at sp+20 and passes it to FUN_0800AEC4:
//   0x80083de: strh r7,[r1,#0]  ; psc = 2   (r7 = 2, set at 0x80083aa)
//   0x80083dc: strh r4,[r1,#2]  ; cr1_hi = 0
//   0x80083e2: str  0x00005DC0,[r1,#4] ; arr = 24000  (pool @0x8008438)
//   0x80083e6: strh r4,[r1,#8]  ; cr1_ckd = 0
//   0x80083e4: strb r6,[r1,#10] ; rcr = 0
// (r4 = 0, r6 = 0.)  With PSC = 2 the timer clock is TIMCLK_HZ/2 = 24 MHz and
// ARR = 24000 gives a 1 kHz base rate, which matches the tone table below.
const BUZZ_PSC: u16 = 2;
// The duty of a queued tone comes from its duration, not a global default:
// the dump's sequencer passes `duration >> 1` to `FUN_08008514`, which
// computes `CCR = ARR * x / 100`.

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

/// The depth of the original's tone queue (`0x2000064c`, 6-byte entries).
const BUZZ_QUEUE_MAX: usize = 8;

/// One queued tone: the frequency and the duration.
///
/// The original stores it as a 6-byte record: the frequency at +0 and the
/// duration at +4 (both 16 bit), which is what these two fields are.
#[derive(Clone, Copy)]
struct BuzzQueueEntry {
    freq: u16,
    ms: u16,
}

struct State {
    buzz_ready: bool,
    buzz_period: u32, // current ARR value
    /// the tone queue of the original (`0x20000650`)
    queue: [BuzzQueueEntry; BUZZ_QUEUE_MAX],
    /// the index of the tone currently playing, and the queue length
    /// (`_DAT_200006e0` / `_DAT_200006e2`)
    queue_head: u8,
    queue_len: u8,
    /// whether a tone is playing (`_DAT_200006ec`)
    playing: bool,
    /// when the current tone ends (the millisecond tick, `0x20000fb0`)
    tone_end_ms: u32,
}

static S: Global<State> = Global::new(State {
    buzz_ready: false,
    buzz_period: 1000,
    queue: [BuzzQueueEntry { freq: 0, ms: 0 }; BUZZ_QUEUE_MAX],
    queue_head: 0,
    queue_len: 0,
    playing: false,
    tone_end_ms: 0,
});

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
    gpio::set_afr(BUZZ_PORT, BUZZ_PIN, 2); // AF2 = TIM16_CH1

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

// Play a tone pattern.
/// `hwiBuzz_play`
///
/// The original queues the tones and plays them from the main loop tick
/// (`FUN_08008574`): `play` only fills the tone table at `0x20000650` and
/// returns, so the caller is never blocked.  The port now does the same -
/// `tick()` drives the queue.
pub fn play(pattern: u8) -> i32 {
    let needs_init = !S.get().buzz_ready;
    if needs_init {
        init();
    }
    if pattern as usize >= BUZZ_PATTERN_COUNT {
        return -1;
    }
    let steps = BUZZ_PATTERNS[pattern as usize];
    let st = S.get();
    // FUN_08008440: the queue is cleared before a new pattern is appended
    st.queue_head = 0;
    st.queue_len = 0;
    st.playing = false;
    for step in steps {
        if step.ms == 0 {
            break;
        }
        if (st.queue_len as usize) < BUZZ_QUEUE_MAX {
            st.queue[st.queue_len as usize] = BuzzQueueEntry { freq: step.freq, ms: step.ms };
            st.queue_len += 1;
        }
    }
    0
}

/// `FUN_08008574` - the tone sequencer of the periodic task.
///
/// Called from the dump's `FUN_080063EC` (@0x80063ec).  Two halves:
///
/// 1. if a tone is playing, read the start time of the tone
///    (`FUN_0800A558(0x20000fb0, ...)`); when the current tone's duration has
///    elapsed, stop it (`FUN_08008514(0)` = silence), advance the head and
///    reset the "playing" flag and the queue when the last tone is done;
/// 2. if nothing is playing and the queue is non-empty, start the next tone
///    (`FUN_0800849C(freq)`, `FUN_08008514(duty)`), stamp the start time and
///    set the "playing" flag.
pub fn tick() {
    let needs_init = !S.get().buzz_ready;
    if needs_init {
        init();
    }
    let st = S.get();
    let now = util::millis();

    if st.playing {
        // the current tone's duration has elapsed?
        if now.wrapping_sub(st.tone_end_ms) < 0x8000_0000 {
            set_duty(0); // FUN_08008514(0)
            st.queue_head += 1;
            if st.queue_head >= st.queue_len {
                st.queue_head = 0;
                st.queue_len = 0;
            }
            st.playing = false;
        } else {
            return; // still playing
        }
    }

    // start the next queued tone
    let head = st.queue_head as usize;
    if head < st.queue_len as usize {
        let entry = st.queue[head];
        set_frequency(entry.freq as u32);
        let duty = (entry.ms as u32) >> 1; // FUN_08008514(duration >> 1)
        set_duty(duty);
        st.tone_end_ms = now.wrapping_add(entry.ms as u32);
        st.playing = true;
    }
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