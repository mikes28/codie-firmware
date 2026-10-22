//! hwiDrive - the two motor drivers of the robot (C: `fw/modules/hwi_drive.c`).
//!
//! Original handler : FUN_08008B3C (0x08008B3D), registered as "hwiDrive"
//! Original init    : FUN_080087D8 (hardware) + FUN_08008910 (start)
//! Original duty    : FUN_080089D0 (motor A) / FUN_08008A50 (motor B)
//! Original fault   : FUN_08008AD0 (fault flags), FUN_08008BD4 (max duty)
//! Original input   : GPIOC pin 14 = the fault input of the driver
//!
//! Wire behaviour of the original (reproduced verbatim):
//!
//!   "set hwiDrive dutyA <n>"  ->  the duty of motor A is set.  The
//!       argument is parsed as a HEXADECIMAL number (FUN_080069D4 with the
//!       base 16), the result is truncated to 16 bit and then clamped to
//!       +-1000.  No reply is sent.
//!   "set hwiDrive dutyB <n>"  ->  the same for motor B.
//!   "get hwiDrive nFault"     ->  the answer is always the constant "0".
//!       The original reads the fault input (GPIOC pin 14) but throws the
//!       result away and answers with the literal string "0".  The reply is
//!       sent with the op code 2 ("set"), not with "get" - that is what the
//!       original does.
//!   every other op            ->  ignored, no reply.
//!
//! Hardware (recovered from the original binary):
//!   TIM3 (0x40000400) in centre aligned mode, PSC = 0, ARR = 1000,
//!   four PWM channels (PWM mode 2) on GPIOC pins 6, 7, 8 and 9
//!   (alternate function 0):  CH1 = motor A forward, CH2 = motor A reverse,
//!   CH3 = motor B forward, CH4 = motor B reverse.
//!   GPIOC pin 2 = the enable line of the motor driver (high = enabled).
//!   GPIOC pin 1 = an input with pull up.

use crate::hal::{addr, gpio, rcc, timer};
use crate::proto::proxy;
use crate::state::Global;

/// the ARR of TIM3
const DRIVE_MAX_DUTY: i32 = 1000;
/// the maximum duty in percent
const DRIVE_MAX_PERCENT: u8 = 100;

/// The state of the module (the C file scope variables).
struct State {
    /// 0/1 = not initialised, 2 = hardware set up, +4 = running
    state: u8,
    /// the maximum duty in percent (0..100) per motor, the original keeps
    /// these two bytes in its .data section
    max_duty_b: u8,
    max_duty_a: u8,
    /// the fault flags of the original: 0xff = fault, 1 = ok
    fault_a: u8,
    fault_b: u8,
    /// the last duty that was commanded (the original does not keep it, it is
    /// a reconstruction of hwiDrive_get_duty)
    duty_a: i32,
    duty_b: i32,
}

static S: Global<State> = Global::new(State {
    state: 0,
    max_duty_b: 0,
    max_duty_a: 0,
    fault_a: 0,
    fault_b: 0,
    duty_a: 0,
    duty_b: 0,
});

/// `hwiDrive_init`
pub fn init() {
    let st = S.get();

    if st.state < 2 {
        drive_hw_init();
        st.state = 2;
    }
    if (st.state & 4) == 0 {
        drive_hw_start();
        st.state |= 4;
    }

    proxy::register(proxy::Iface { name: "hwiDrive", handler: handler });
}

/// `hwiDrive_set_duty`
pub fn set_duty(motor: u8, duty: i32) -> i32 {
    if motor > 1 {
        return -1;
    }
    drive_set_duty(motor, duty);
    0
}

/// `hwiDrive_get_fault`
pub fn get_fault(motor: u8) -> i32 {
    let st = S.get();
    if motor == 0 {
        st.fault_a as i32
    } else {
        st.fault_b as i32
    }
}

/// `hwiDrive_get_duty`
pub fn get_duty(motor: u8) -> i32 {
    let st = S.get();
    if motor == 0 {
        st.duty_a
    } else {
        st.duty_b
    }
}

/// `hwiDrive_set_max_duty`
///
/// from FUN_08008BD4: the maximum duty in percent (0..100) of a motor.
/// Not part of the shared contract, the application layer uses it to limit
/// the speed when the battery is low.
pub fn set_max_duty(motor: u8, percent: u8) {
    let mut percent = percent;
    if percent > DRIVE_MAX_PERCENT {
        percent = DRIVE_MAX_PERCENT;
    }
    let st = S.get();
    if motor == 0 {
        st.max_duty_a = percent;
    } else {
        st.max_duty_b = percent;
    }
}

/// `hwiDrive_set_fault`
///
/// from FUN_08008AD0: the original reaction to a measured motor current.
/// A positive value sets the fault flag of the motor, a negative one
/// clears it; the motor is driven with the measured value afterwards.
/// Not part of the shared contract.
pub fn set_fault(motor: u8, value: i32) {
    // the C: `if ((int16_t)(-(int16_t)value) >= 1) fault = 1u;`
    // the truncation to 16 bit and the negation happen in the same order.
    let t = value as i16;
    let fault = if (-(t as i32)) as i16 >= 1 { 1u8 } else { 0xFFu8 };

    if motor & 1 != 0 {
        S.get().fault_b = fault;
        drive_set_duty(1, value);
    }
    if motor & 2 != 0 {
        S.get().fault_a = fault;
        drive_set_duty(0, value);
    }
}

/// the interface handler of the module
///
/// from FUN_08008B3C
pub fn handler(op: u8, opname: &str, args: &str) {
    if op == proxy::OP_SET {
        if opname == "dutyA" {
            set_duty(0, parse_hex16(args) as i32);
        } else if opname == "dutyB" {
            set_duty(1, parse_hex16(args) as i32);
        }
        return;
    }

    if op == proxy::OP_GET && opname == "nFault" {
        // the original reads the fault input of the driver (GPIOC pin 14)
        // and then answers with the constant "0" - the result of the read
        // is dropped.  The reply carries the op code 2 ("set").
        let _ = gpio::read(addr::GPIOC, 0x4000);
        proxy::resp_value(proxy::OP_SET, "hwiDrive", opname, "0");
    }
}

// ------------------------------------------------------------------
// hardware
// ------------------------------------------------------------------

/// from FUN_080087D8: the setup of the motor PWM
fn drive_hw_init() {
    let mut gpio = gpio::Cfg {
        // GPIOC 6..9: the four PWM outputs, alternate function 0
        pins: 0x03C0,
        mode: gpio::MODE_ALT,
        ospeed: 3,
        otype: 3, // the original shifts the whole byte here ...
        pupd: 0,
    };

    rcc::apb1_en(0x0000_0002, true); // FUN_0800AD9C: TIM3 clock

    gpio::config(addr::GPIOC, &gpio);
    gpio::set_afr(addr::GPIOC, 6, 0);
    gpio::set_afr(addr::GPIOC, 7, 0);
    gpio::set_afr(addr::GPIOC, 8, 0);
    gpio::set_afr(addr::GPIOC, 9, 0);

    // GPIOC 2: the enable line of the driver
    gpio.pins = 0x0004;
    gpio.mode = gpio::MODE_OUTPUT;
    gpio.ospeed = 0;
    gpio.otype = 0;
    gpio.pupd = 0;
    gpio::config(addr::GPIOC, &gpio);

    // GPIOC 1: an input with pull up
    gpio.pins = 0x0002;
    gpio.mode = gpio::MODE_INPUT;
    gpio.ospeed = 1;
    gpio.otype = 0;
    gpio.pupd = gpio::PULL_UP;
    gpio::config(addr::GPIOC, &gpio);

    // the compare preload enable of the four channels
    // (FUN_0800b1fc / b208 / b220 / b22c)
    timer::ocpe(addr::TIM3, 1, true);
    timer::ocpe(addr::TIM3, 2, true);
    timer::ocpe(addr::TIM3, 3, true);
    timer::ocpe(addr::TIM3, 4, true);
    timer::arpe(addr::TIM3, false); // FUN_0800AF74

    // the timer itself (FUN_0800AEC4): PSC = 0, ARR = 1000 and centre
    // aligned mode 1 (CR1 bits 4..7 = 0x20)
    let base = timer::BaseCfg {
        psc: 0,
        cr1_hi: 0x0020,
        arr: DRIVE_MAX_DUTY as u32,
        cr1_ckd: 0,
        rcr: 0,
    };
    timer::base_cfg(addr::TIM3, &base);

    // the four compare channels (FUN_0800AFDC / B054 / B0F4 / B174)
    let chan = timer::ChanCfg {
        ccmr: 0x0060, // PWM mode 2
        ccer: 0x0001, // CCxE
        ccer2: 0,
        ccer3: 0,
        ccr: 0,
        ccer4: 0,
        ccer5: 0x0008,
        cr2: 0x0100,
        cr2b: 0x0200,
    };
    timer::channel_cfg(addr::TIM3, 1, &chan);
    timer::channel_cfg(addr::TIM3, 2, &chan);
    timer::channel_cfg(addr::TIM3, 3, &chan);
    timer::channel_cfg(addr::TIM3, 4, &chan);
}

/// from FUN_08008910: enable the outputs and the driver
fn drive_hw_start() {
    gpio::write(addr::GPIOC, 0x0004, true); // the enable line high

    timer::ccer(addr::TIM3, 1, true); // FUN_0800B244
    timer::ccer(addr::TIM3, 2, true);
    timer::ccer(addr::TIM3, 3, true);
    timer::ccer(addr::TIM3, 4, true);

    timer::egr(addr::TIM3, 1); // FUN_0800b3a4: UG
    timer::enable(addr::TIM3, true); // FUN_0800AFA0: CR1.CEN
    timer::moe(addr::TIM3, true); // FUN_0800AFBC: BDTR.MOE
}

/// from FUN_08008980: stop the motor (the enable line low)
///
/// The C has this as an unused static function, it is kept for the sake of
/// the traceability to the original.
#[allow(dead_code)]
fn drive_hw_stop() {
    timer::enable(addr::TIM3, false);
    timer::moe(addr::TIM3, false);
    gpio::write(addr::GPIOC, 0x0004, false);
}

// ------------------------------------------------------------------
// the motors
// ------------------------------------------------------------------

/// from FUN_080089D0 (motor A) and FUN_08008A50 (motor B).
///
/// motor 0 = A (TIM3 channel 1 forward, channel 2 reverse)
/// motor 1 = B (TIM3 channel 3 forward, channel 4 reverse)
///
/// A duty of zero does not stop the motor, it drives both directions with
/// the maximum duty (a brake) - the original does exactly that.
fn drive_set_duty(motor: u8, duty: i32) {
    let mut v = duty as i16 as i32; // the original truncates to 16 bit

    if v > DRIVE_MAX_DUTY {
        v = DRIVE_MAX_DUTY;
    }
    if v < -DRIVE_MAX_DUTY {
        v = -DRIVE_MAX_DUTY;
    }

    let max = {
        let st = S.get();
        if motor == 0 {
            st.max_duty_a as u32
        } else {
            st.max_duty_b as u32
        }
    };

    let (fwd, rev) = if v > 0 {
        (v as u32, 0)
    } else if v < 0 {
        (0, (-v) as u32)
    } else {
        (max * 10, max * 10)
    };

    let st = S.get();
    if motor == 0 {
        timer::pwm_set(addr::TIM3, 1, fwd); // FUN_0800b1ec
        timer::pwm_set(addr::TIM3, 2, rev); // FUN_0800b1f0
        st.duty_a = v;
    } else {
        timer::pwm_set(addr::TIM3, 3, fwd); // FUN_0800b1f4
        timer::pwm_set(addr::TIM3, 4, rev); // FUN_0800b1f8
        st.duty_b = v;
    }
}

// ------------------------------------------------------------------
// wire interface
// ------------------------------------------------------------------

/// from FUN_080069D4 with the base 16: the original parses the argument of
/// the duty ops as a hexadecimal number.  Digits 0..9 have their usual
/// value, 'a'..'f' are 10..15, everything else is taken modulo 2^64 just
/// like the original (which uses the character value - 0x57).
fn parse_hex16(s: &str) -> i16 {
    let b = s.as_bytes();
    let mut i = 0usize;
    let mut neg = false;

    if b.is_empty() {
        return 0;
    }
    if b[0] == b'-' {
        neg = true;
        i = 1;
    }
    if i >= b.len() {
        return 0;
    }

    let mut acc: u64 = 0;
    while i < b.len() {
        let c = b[i];
        let d: i32 = if c < 0x3A {
            c as i32 - 0x30 // '0'..'9' (and ':' .. '?')
        } else {
            c as i32 - 0x57 // 'a'..'f'
        };
        // the original computes this in unsigned 64 bit arithmetic
        acc = acc.wrapping_mul(16).wrapping_add((d as i64) as u64);
        i += 1;
    }

    if neg {
        acc = acc.wrapping_neg();
    }

    // (int16_t)(uint16_t)(uint32_t)acc
    ((acc as u32) as u16) as i16
}
