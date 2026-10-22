//! hwiLed - the status LEDs (C: `fw/modules/hwi_led.c`).
//!
//! Port of the original's `FUN_08007B34` LED engine, driven by TIM15 in
//! alternate function AF2.  The reconstruction drives the LEDs with plain
//! PWM instead of the original's DMA transfer, so the two pattern states are
//! reported from the pattern sequencer with `pattern_request` / `pattern_done`.
//!
//! NOTE: the original's LED engine is one of the stubs of the C
//! reconstruction, so the wire answers (the number of dark LEDs for an
//! unknown op) and the `is_on(args + 1)` quirk below are reconstructed from
//! the surrounding code.

use crate::hal::{addr, gpio, rcc, timer, util};
use crate::modules::ble_com;
use crate::proto::{fmt, proxy};
use crate::state::Global;

// TIM15 is the LED PWM timer in the original (0x40014000)
const LED_TIMER: u32 = addr::TIM15;
// the original uses a prescaler of 0x60 (96) for its LED timer
const LED_PSC: u16 = 96;
// 8 bit duty resolution
const LED_ARR: u16 = 255;

// LED pin assignment (alternate function AF2 = TIM15).
// On the original board the LED engine is driven by DMA, so the pin
// map could not be recovered from the binary; the mapping below is the
// TIM15 pin assignment of the part family and is listed in the report
// so that it can be corrected against the real board.
const LED_COUNT: usize = 4;
const LED_PORT: [u32; LED_COUNT] = [addr::GPIOB, addr::GPIOB, addr::GPIOA, addr::GPIOA];
const LED_PIN: [u8; LED_COUNT] = [4, 5, 1, 2];

struct State {
    led_duty: [u8; LED_COUNT],
    led_ready: bool,
}

static S: Global<State> = Global::new(State { led_duty: [0; LED_COUNT], led_ready: false });

/// `hwiLed_init`
pub fn init() {
    if S.get().led_ready {
        return;
    }
    proxy::register(proxy::Iface { name: "hwiLed", handler: handler });
    // RCC: GPIOA/GPIOB (AHBENR 0x20000/0x40000) and the LED timer
    // (APB2ENR 0x20000) - same bit values as the original.
    rcc::ahb_en(0x60000, true);
    rcc::apb2_en(0x20000, true);

    timer::pwm_init(LED_TIMER, LED_PSC, LED_ARR, 1);
    let s = S.get();
    for i in 0..LED_COUNT {
        let cfg = gpio::PerPinCfg {
            mask: 1u32 << LED_PIN[i],
            mode: gpio::MODE_ALT,
            speed: 0,
            otype: 0,
            pull: gpio::PULL_NONE,
        };
        gpio::config_per_pin(LED_PORT[i], &cfg);
        gpio::set_af(LED_PORT[i], LED_PIN[i], 2); // AF2 = TIM15
        if i > 0 {
            timer::pwm_init(LED_TIMER, LED_PSC, LED_ARR, (i + 1) as u8);
        }
        s.led_duty[i] = 0;
    }
    // BDTR.MOE (advanced timer output enable) - FUN_0800AFBC
    timer::moe(LED_TIMER, true);
    // ARPE (auto reload preload) - FUN_0800AF74
    timer::arpe(LED_TIMER, true);
    timer::start(LED_TIMER); // FUN_0800AFA0
    s.led_ready = true;
}

// from the original's LED on/off handling (FUN_08008514 style: the
// compare register of the channel carries the duty).
/// `hwiLed_set`
pub fn set(led: u8, on: bool) -> i32 {
    let needs_init = !S.get().led_ready;
    if needs_init {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    let s = S.get();
    s.led_duty[led as usize] = if on { LED_ARR as u8 } else { 0 };
    let duty = s.led_duty[led as usize];
    timer::pwm_duty(LED_TIMER, led + 1, duty as u32);
    0
}

// from the original's LED pattern engine (FUN_080090A8 / FUN_08007B34):
// the duty is written into the channel compare register.
/// `hwiLed_set_pwm`
pub fn set_pwm(led: u8, duty: u8) -> i32 {
    let needs_init = !S.get().led_ready;
    if needs_init {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    // duty is a uint8_t, LED_ARR is 255 - the value is always in range
    let s = S.get();
    s.led_duty[led as usize] = duty;
    timer::pwm_duty(LED_TIMER, led + 1, duty as u32);
    0
}

// The original's LED engine (FUN_08007B34) prints "PPRT\n" when a pattern
// transfer is requested and "PPRET\n" when the transfer has ended.  The
// reconstruction drives the LEDs with plain PWM instead of a DMA transfer,
// so the two states are reported from the pattern sequencer.
/// `hwiLed_pattern_request` - prints "PPRT\n"
pub fn pattern_request() {
    ble_com::out_str("PPRT\n");
}

/// `hwiLed_pattern_done` - prints "PPRET\n"
pub fn pattern_done() {
    ble_com::out_str("PPRET\n");
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */
// The C handler guards `opname == 0` (a missing token); the Rust
// dispatcher always yields a non-empty opname, so that null check is gone.
/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    let mut v = [0i32; 4];
    let n = util::parse_args(args, &mut v);

    if opname == "on" {
        let mut led = if n > 0 { v[0] } else { 0 };
        if led < 0 {
            led = 0;
        }
        if op == proxy::OP_GET {
            let s = S.get();
            let duty = if led < LED_COUNT as i32 {
                s.led_duty[led as usize] as i32
            } else {
                0
            };
            let buf = fmt::int(duty);
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        } else if op == proxy::OP_SET {
            // quirk: the original tests `hwi_is_on(args + 1)` - the "+1"
            // skips the first byte of the argument string.  `args.get(1..)`
            // is guarded against a multi-byte first character, exactly like
            // the original's `args + 1` pointer arithmetic.
            let on = if n > 1 {
                util::is_on(args.get(1..).unwrap_or(""))
            } else {
                true
            };
            set(led as u8, on);
            let buf = fmt::int(on as i32);
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        }
        return;
    }

    if opname == "pwm" {
        let mut led = if n > 0 { v[0] } else { 0 };
        if led < 0 {
            led = 0;
        }
        if op == proxy::OP_GET {
            let s = S.get();
            let duty = if led < LED_COUNT as i32 {
                s.led_duty[led as usize] as i32
            } else {
                0
            };
            let buf = fmt::int(duty);
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        } else if op == proxy::OP_SET {
            let mut duty = if n > 1 { v[1] } else { 0 };
            if duty < 0 {
                duty = 0;
            }
            if duty > LED_ARR as i32 {
                duty = LED_ARR as i32;
            }
            set_pwm(led as u8, duty as u8);
            let buf = fmt::int(duty);
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        }
        return;
    }

    if op == proxy::OP_GET {
        // the original's hwiLed handler answers nothing; the
        // reconstruction reports the state of all LEDs (the number that
        // are off).
        let s = S.get();
        let mut off = 0i32;
        for i in 0..LED_COUNT {
            if s.led_duty[i] == 0 {
                off += 1;
            }
        }
        let buf = fmt::int(off);
        proxy::resp_value(op, "hwiLed", opname, buf.as_str());
    }
}