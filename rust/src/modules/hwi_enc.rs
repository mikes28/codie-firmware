//! hwiEnc - the two wheel encoders of the robot (C: `fw/modules/hwi_enc.c`).
//!
//! Original handler : FUN_08008DFC (0x08008DFD), registered as "hwiEnc"
//! Original init    : FUN_08008C48 (hardware) + FUN_08008DBC (start)
//! Original count   : FUN_08008E00
//! Original ISRs    : FUN_08008BF8 (TIM1 update, IRQ 13)
//!                    FUN_08008C20 (TIM2 update, IRQ 15)
//!
//! Wire behaviour of the original:
//!   the entry of "hwiEnc" in the interface table of the original firmware
//!   is a stub (FUN_08008DFC is "mov r0,#0; bx lr"), so the module never
//!   answers an op - a request for hwiEnc produces no reply at all.  The
//!   same is true for the timer/encoder combination the original used
//!   internally.  The handler below reproduces the stub.
//!
//! Hardware (recovered from the original binary):
//!   TIM2 (0x40000000) channel 1 captures on GPIOA pin 0  (encoder 1)
//!   TIM1 (0x40012C00) channel 1 captures on GPIOA pin 8  (encoder 2)
//!   both in input capture mode, 16 bit, inverted (CC1P), the update
//!   interrupt of the timer counts the overflows of the 16 bit counter.
//!   GPIOA pin 15 switches the encoder supply on (low while the hardware
//!   is set up, high when the encoders run).

use crate::hal::{addr, gpio, nvic, rcc, timer};
use crate::proto::proxy;
use crate::state::Global;

/// encoder 1
const ENC_TIMER_1: u32 = addr::TIM2;
/// encoder 2
const ENC_TIMER_2: u32 = addr::TIM1;
/// TIM2
const ENC_IRQ_1: u8 = 15;
/// TIM1
const ENC_IRQ_2: u8 = 13;

/// The state of the module (the C file scope variables).
struct State {
    /// 0/1 = not initialised, 2 = hardware set up, +4 = running
    state: u8,
    /// the high part of the 16 bit counters, kept by the update interrupts
    high_1: u32,
    high_2: u32,
}

static S: Global<State> = Global::new(State { state: 0, high_1: 0, high_2: 0 });

/// `hwiEnc_init`
pub fn init() {
    let st = S.get();

    if st.state < 2 {
        enc_hw_init();
        st.state = 2;
    }
    if (st.state & 4) == 0 {
        enc_hw_start();
        st.state |= 4;
    }

    proxy::register(proxy::Iface { name: "hwiEnc", handler: handler });
}

/// `hwiEnc_count`
///
/// from FUN_08008E00
pub fn count(encoder: u8) -> i32 {
    let st = S.get();

    if encoder == 1 {
        let cnt = timer::get16(ENC_TIMER_1, addr::TIM_CNT) as u32 & 0xFFFF;
        return cnt.wrapping_add(st.high_1 << 16) as i32;
    }
    if encoder == 2 {
        let cnt = timer::get16(ENC_TIMER_2, addr::TIM_CNT) as u32 & 0xFFFF;
        return cnt.wrapping_add(st.high_2 << 16) as i32;
    }
    if encoder == 0x20 {
        // the original combines both encoders here
        let cnt = timer::get16(ENC_TIMER_1, addr::TIM_CNT) as u32 & 0xFFFF;
        let cnt2 = timer::get16(ENC_TIMER_2, addr::TIM_CNT) as u32 & 0xFFFF;
        return cnt
            .wrapping_add(st.high_1 << 16)
            .wrapping_add(st.high_2 << 16)
            .wrapping_add(cnt2 >> 1) as i32;
    }
    0
}

/// `hwiEnc_reset`
///
/// The original firmware has no way to reset an encoder, this is the
/// reconstruction of hwiEnc_reset: the 16 bit counter and the overflow
/// counter of the selected encoder are cleared.
pub fn reset(encoder: u8) {
    if encoder == 1 {
        timer::set16(ENC_TIMER_1, addr::TIM_CNT, 0);
        S.get().high_1 = 0;
    } else if encoder == 2 {
        timer::set16(ENC_TIMER_2, addr::TIM_CNT, 0);
        S.get().high_2 = 0;
    }
}

/// the interface handler of the module
///
/// The original entry of "hwiEnc" in the interface table (0x08008DFD) is
/// the stub FUN_08008DFC ("mov r0,#0; bx lr"): it answers no op and sends
/// no reply.  Reproduced as it is.
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    // the stub does nothing at all
}

/// IRQ 13 - the TIM1 update interrupt, the high part of encoder 2.
#[no_mangle]
pub extern "C" fn TIM1_BRK_UP_TRG_COM_IRQHandler() {
    let st = S.get();
    enc_update_isr(ENC_TIMER_2, &mut st.high_2);
}

/// IRQ 15 - the TIM2 update interrupt, the high part of encoder 1.
#[no_mangle]
pub extern "C" fn TIM2_IRQHandler() {
    let st = S.get();
    enc_update_isr(ENC_TIMER_1, &mut st.high_1);
}

// ------------------------------------------------------------------
// hardware
// ------------------------------------------------------------------

/// from FUN_08008C48: the capture input of one encoder timer
fn enc_timer_cfg(tim: u32) {
    let base = timer::BaseCfg {
        psc: 0,
        cr1_hi: 0,
        arr: 0xFFFF, // the counter is 16 bit wide
        cr1_ckd: 0,
        rcr: 0,
    };
    timer::base_cfg(tim, &base);
    timer::prescaler_egr(tim, 0, 1);

    // the capture configuration of the original:
    //   CCMR1 = (CCMR1 & 0xFF0C) | 0x0021   (CC1S = 01, capture on TI1)
    //   CCER  = (CCER  & ~0x0001) | 0x000B  (CC1E, CC1P, CC1OF)
    //   CR2   = (CR2   & 0xFFF8) | 0x0007
    //   SMCR  = (SMCR  & 0xFF8F) | 0x0050
    //   DIER  |= 0x0001                       (update interrupt)
    let ccmr1 = timer::get16(tim, addr::TIM_CCMR1);
    timer::set16(tim, addr::TIM_CCMR1, (ccmr1 & 0xFF0C) | 0x0021);

    let ccer = timer::get16(tim, addr::TIM_CCER);
    timer::set16(tim, addr::TIM_CCER, (ccer & !0x0001) | 0x000B);

    timer::set16(tim, addr::TIM_CR2, (timer::get16(tim, addr::TIM_CR2) & 0xFFF8) | 0x0007);
    timer::set16(tim, addr::TIM_SMCR, (timer::get16(tim, addr::TIM_SMCR) & 0xFF8F) | 0x0050);
    timer::dier(tim, 0x0001, true);
}

/// from FUN_08008C48: the setup of both encoders
fn enc_hw_init() {
    let mut gpio = gpio::Cfg {
        // GPIOA pin 0 (TIM2_CH1) and pin 8 (TIM1_CH1) as capture inputs
        pins: 0x0101,
        mode: gpio::MODE_ALT,
        ospeed: 3,
        otype: 0,
        pupd: 0,
    };

    rcc::apb2_en(0x0000_0800, true); // FUN_0800AD7C
    rcc::apb1_en(0x0000_0001, true); // FUN_0800AD9C
    rcc::ahb_en(0x0002_0000, true); // FUN_0800AD5C

    gpio::config(addr::GPIOA, &gpio);
    gpio::set_afr(addr::GPIOA, 0, 2);
    gpio::set_afr(addr::GPIOA, 8, 2);

    // GPIOA pin 15: the enable line of the encoder supply
    gpio.pins = 0x8000;
    gpio.mode = gpio::MODE_OUTPUT;
    gpio.ospeed = 0;
    gpio.otype = 0;
    gpio.pupd = 0;
    gpio::config(addr::GPIOA, &gpio);
    gpio::write(addr::GPIOA, 0x8000, false); // the original leaves it low

    nvic::enable(ENC_IRQ_2, 2); // TIM1 update, FUN_0800AB30
    nvic::enable(ENC_IRQ_1, 2); // TIM2 update, FUN_0800AB30

    enc_timer_cfg(ENC_TIMER_2); // TIM1
    enc_timer_cfg(ENC_TIMER_1); // TIM2
}

/// from FUN_08008DBC: switch the encoders on
fn enc_hw_start() {
    gpio::write(addr::GPIOA, 0x8000, true); // the enable line high
    timer::enable(ENC_TIMER_2, true); // TIM1
    timer::enable(ENC_TIMER_1, true); // TIM2
}

// ------------------------------------------------------------------
// the update interrupts of the two capture timers
// ------------------------------------------------------------------

/// from FUN_08008BF8 (TIM1) and FUN_08008C20 (TIM2)
fn enc_update_isr(tim: u32, high: &mut u32) {
    let sr = timer::get16(tim, addr::TIM_SR);
    let dier = timer::get16(tim, addr::TIM_DIER);

    if (sr & 1) != 0 && (dier & 1) != 0 {
        *high = high.wrapping_add(1);
        timer::sr_clear(tim, 1);
    }
}
