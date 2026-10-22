//! hwiSonar - the ultrasonic distance sensor of the robot (C: `fw/modules/hwi_sonar.c`).
//!
//! Original handler : FUN_08009F88 (0x08009F89), registered as "hwiSonar"
//! Original init    : FUN_08008F14 (hardware) + FUN_08009060 (start)
//! Original measure : FUN_08009268 (arm a measurement)
//!                    FUN_08008E9C (TIM17 update, IRQ 22)
//!                    FUN_08008EE8 (DMA1 channel 1, IRQ 9)
//! Original read    : FUN_08009094 (returns the "measurement running" flag)
//!
//! Wire behaviour of the original:
//!   the entry of "hwiSonar" in the interface table of the original firmware
//!   is a stub (FUN_08009F88 is "mov r0,#0; bx lr"), the module answers no
//!   op.  The op name "echo" exists in the string table of the original
//!   (0x0800C58C) but is not referenced by any code - the feature was
//!   never finished.  The handler below reproduces the stub.
//!
//! Hardware (recovered from the original binary):
//!   TIM17 (0x40014800) with the compare channel 1 on GPIOB pin 9
//!   (alternate function 2), PSC = 0, ARR = 59, PWM mode 2.
//!   DMA1 channel 1 (0x40020008) copies the captured value (TIM17_CCR1 at
//!   0x40014834) into RAM, the transfer count is set to 318 when a
//!   measurement is armed.

use crate::hal::{addr, dma, gpio, nvic, rcc, timer};
use crate::proto::proxy;
use crate::state::Global;

/// the timer window of the original is 60 ticks (ARR = 59).  The original
/// firmware never turned the capture into a distance, so the length of one
/// tick is not recoverable from the binary - it depends on the clock tree
/// the system module sets up.  One tick is treated as one microsecond.
const SONAR_TICK_US: i32 = 1;

/// the size of the capture buffer
const SONAR_CAPTURES: usize = 16;
/// the transfer count of the original
const SONAR_TRIGGER_CNT: u16 = 318;

/// TIM17
const SONAR_IRQ_TIM: u8 = 22;
/// DMA1 channel 1
const SONAR_IRQ_DMA: u8 = 9;

/// The state of the module (the C file scope variables).
struct State {
    /// 0/1 = not initialised, 2 = hardware set up, +4 = running
    state: u8,
    /// 1 while a measurement is running (the original keeps this flag in
    /// _DAT_2000085C)
    busy: u8,
    /// the values the DMA copies out of the timer
    capture: [u16; SONAR_CAPTURES],
}

static S: Global<State> = Global::new(State { state: 0, busy: 0, capture: [0; SONAR_CAPTURES] });

/// `hwiSonar_init`
pub fn init() {
    let st = S.get();

    if st.state < 2 {
        sonar_hw_init(st.capture.as_ptr() as u32);
        st.state = 2;
    }
    if (st.state & 4) == 0 {
        sonar_hw_start();
        st.state |= 4;
    }

    proxy::register(proxy::Iface { name: "hwiSonar", handler: handler });
}

/// `sonar_busy` - from FUN_08009094: the original returns the "measurement
/// running" flag.
///
/// The C keeps it in the static `s_busy` (the original binary keeps it in
/// `_DAT_2000085C`); the Rust port keeps it in the module state.
pub fn busy() -> bool {
    S.get().busy != 0
}

/// `hwiSonar_echo_us`
pub fn echo_us() -> i32 {
    if S.get().busy == 0 {
        // no measurement running: start one and report "nothing yet"
        let _ = sonar_trigger();
        return 0;
    }

    let raw = capture(0);
    raw as i32 * SONAR_TICK_US
}

/// the interface handler of the module
///
/// The original entry of "hwiSonar" in the interface table (0x08009F89) is
/// the stub FUN_08009F88 ("mov r0,#0; bx lr"): it answers no op and sends
/// no reply.  Reproduced as it is.
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    // the stub does nothing at all
}

/// IRQ 22 - the TIM17 update interrupt.
#[no_mangle]
pub extern "C" fn TIM17_IRQHandler() {
    let sr = timer::get16(addr::TIM17, addr::TIM_SR);
    let dier = timer::get16(addr::TIM17, addr::TIM_DIER);

    if (sr & 1) != 0 && (dier & 1) != 0 {
        timer::enable(addr::TIM17, false); // FUN_0800AFA0
        dma::channel_enable(addr::DMA1_CH1, false); // FUN_0800A87C
        timer::dier(addr::TIM17, 1, false); // FUN_0800B38C
        S.get().busy = 0;
        timer::sr_clear(addr::TIM17, 1); // FUN_0800b3c0
    }
}

/// IRQ 9 - the DMA1 channel 1 interrupt.
#[no_mangle]
pub extern "C" fn DMA1_Channel1_IRQHandler() {
    if dma::flag(0x0000_0002) {
        // FUN_0800a8b8: TCIF1
        timer::sr_clear(addr::TIM17, 1); // FUN_0800b3c0
        timer::dier(addr::TIM17, 1, true); // FUN_0800B38C
        dma::clear_flags(0x0000_0002); // FUN_0800a8c8
    }
}

// ------------------------------------------------------------------
// hardware
// ------------------------------------------------------------------

/// from FUN_08008F14: the setup of the sensor
///
/// NOTE: the C reads the address of the capture buffer itself
/// (`(uint32_t)(uintptr_t)s_capture`); the Rust port gets the address
/// from the caller, because the buffer lives inside the global state.
fn sonar_hw_init(capture_buf: u32) {
    let gpio = gpio::Cfg {
        // GPIOB pin 9: the echo input of the sensor
        pins: 0x0200,
        mode: gpio::MODE_ALT,
        ospeed: 3,
        otype: 0,
        pupd: 0,
    };

    rcc::ahb_en(0x0004_0000, true); // FUN_0800AD5C
    rcc::apb2_en(0x0004_0000, true); // FUN_0800AD7C: TIM17 clock

    gpio::config(addr::GPIOB, &gpio);
    gpio::set_afr(addr::GPIOB, 9, 2);

    // the timer (FUN_0800AEC4): PSC = 0, ARR = 59
    let base = timer::BaseCfg {
        psc: 0,
        cr1_hi: 0,
        arr: 0x003B,
        cr1_ckd: 0,
        rcr: 0,
    };
    timer::base_cfg(addr::TIM17, &base);

    nvic::enable(SONAR_IRQ_TIM, 3); // FUN_0800AB30: TIM17
    nvic::enable(SONAR_IRQ_DMA, 2); // FUN_0800AB30: DMA1 ch1

    timer::ocpe(addr::TIM17, 1, true); // FUN_0800b1fc
    timer::arpe(addr::TIM17, false); // FUN_0800AF74

    // the compare channel (FUN_0800AFDC)
    let chan = timer::ChanCfg {
        ccmr: 0x0060, // PWM mode 2
        ccer: 0x0001, // CC1E
        ccer2: 0,
        ccer3: 0,
        ccr: 0,
        ccer4: 0,
        // NOTE: the C reconstruction uses 0u here and so does the original
        // binary (FUN_08008F14 leaves the whole `timer_chan_t` zeroed by
        // FUN_0800b1d8 and only sets ccmr / ccer / ccr / cr2).  The value
        // must stay 0: `timer::channel_cfg` ORs it into CCER, and 0x0008
        // would enable channel 2 of TIM17 (CC2E), which the original
        // never does.
        ccer5: 0,
        cr2: 0x0100,
        cr2b: 0,
    };
    timer::channel_cfg(addr::TIM17, 1, &chan);

    rcc::ahb_en(0x0000_0001, true); // FUN_0800AD5C: the DMA clock

    // DMA1 channel 1: the captured value of TIM17_CCR1 goes into RAM.
    // The configuration word is the one of the original.
    dma::channel_reset(addr::DMA1_CH1); // FUN_0800A7B0
    dma::channel_cfg(
        addr::DMA1_CH1,
        addr::TIM17 + addr::TIM_CCR1,
        capture_buf,
        0,
        0x0000_2190,
    );
    timer::dier(addr::TIM17, 0x0100, true); // FUN_0800B3C8: capture int

    dma::set_dir(addr::DMA1_CH1, 0x0000_0002); // FUN_0800A8A0: periph -> mem
}

/// from FUN_08009060: enable the capture output
fn sonar_hw_start() {
    timer::ccer(addr::TIM17, 1, true); // FUN_0800B244
    timer::moe(addr::TIM17, true); // FUN_0800AFBC
}

// ------------------------------------------------------------------
// measuring
// ------------------------------------------------------------------

/// from FUN_08009268: arm a measurement, returns true when it was started
fn sonar_trigger() -> bool {
    if S.get().busy != 0 {
        return false;
    }

    timer::sr_clear(addr::TIM17, 2); // FUN_0800b3c0
    dma::set_cntdr(addr::DMA1_CH1, SONAR_TRIGGER_CNT); // FUN_0800a89c
    timer::pwm_set(addr::TIM17, 1, 0); // FUN_0800b1ec: CCR1 = 0
    dma::channel_enable(addr::DMA1_CH1, true); // FUN_0800A87C
    timer::set32(addr::TIM17, addr::TIM_CNT, 0); // the counter
    timer::enable(addr::TIM17, true); // FUN_0800AFA0
    timer::egr(addr::TIM17, 1); // FUN_0800b3a4: UG
    S.get().busy = 1;
    true
}

/// read one value of the capture buffer
///
/// The C buffer is `volatile` (the DMA writes it in the background), the
/// Rust port reads it the same way.
#[inline]
fn capture(i: usize) -> u16 {
    // SAFETY: the buffer is a static, the index is below SONAR_CAPTURES.
    unsafe { core::ptr::read_volatile(S.get().capture.as_ptr().add(i)) }
}
