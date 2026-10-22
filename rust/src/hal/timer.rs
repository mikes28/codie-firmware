//! Timer helpers.
//!
//! Again the original had two sets of timer helpers and they differ:
//!
//! * the `hwi_a_hal` set ([`base_cfg`], [`channel_cfg`], [`pwm_set`, ...])
//!   is the one that reproduces the original's register writes bit for bit,
//!   including the 32 bit compare register writes that also clear the next
//!   compare register.
//! * the `hwi_b_hal` set ([`pwm_init`], [`pwm_duty`], ...) is the simpler
//!   PWM helper the `hwiLed` / `hwiBuzz` modules use.

use super::addr::*;
use crate::mmio;

/// Base configuration record of the original's `FUN_0800AEC4`.
#[derive(Clone, Copy)]
pub struct BaseCfg {
    /// prescaler
    pub psc: u16,
    /// CR1 bits 4..7 (CMS / DIR)
    pub cr1_hi: u16,
    /// auto reload (the high half lands in RCR)
    pub arr: u32,
    /// CR1 bits 8..9 (clock division)
    pub cr1_ckd: u16,
    /// repetition counter (advanced timers only)
    pub rcr: u8,
}

impl BaseCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        BaseCfg { psc: 0, cr1_hi: 0, arr: 0, cr1_ckd: 0, rcr: 0 }
    }
}

/// Compare channel configuration record of the original's `FUN_0800AFDC`,
/// `FUN_0800B054`, `FUN_0800B0F4` and `FUN_0800B174`.
#[derive(Clone, Copy)]
pub struct ChanCfg {
    /// CCMRx byte that selects the output mode
    pub ccmr: u16,
    /// CCER enable bit (CCxE)
    pub ccer: u16,
    /// advanced timers: extra CCER bits
    pub ccer2: u16,
    /// advanced timers: extra CCER bits
    pub ccer3: u16,
    /// compare value (written as 32 bits!)
    pub ccr: u32,
    /// advanced timers: extra CCER bits
    pub ccer4: u16,
    /// advanced timers: extra CCER bits
    pub ccer5: u16,
    /// advanced timers: CR2 bits
    pub cr2: u16,
    /// advanced timers: CR2 bits
    pub cr2b: u16,
}

impl ChanCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        ChanCfg {
            ccmr: 0,
            ccer: 0,
            ccer2: 0,
            ccer3: 0,
            ccr: 0,
            ccer4: 0,
            ccer5: 0,
            cr2: 0,
            cr2b: 0,
        }
    }
}

/// `TRUE` for the advanced timers (TIM1, TIM15, TIM16, TIM17).
fn is_advanced(tim: u32) -> bool {
    tim == TIM1 || tim == TIM15 || tim == TIM16 || tim == TIM17
}

/// `FUN_0800AEC4` - prescaler, auto reload, CR1 bits and the update event.
pub fn base_cfg(tim: u32, cfg: &BaseCfg) {
    let mut cr1 = mmio::r16(tim + TIM_CR1);

    if tim == TIM1 || tim == TIM2 || tim == TIM3 {
        cr1 = (cr1 & 0xFF8F) | cfg.cr1_hi;
    }

    if tim == TIM6 {
        mmio::w32(tim + TIM_ARR, cfg.arr);
        mmio::w16(tim + TIM_PSC, cfg.psc);
        mmio::w16(tim + TIM_CR1, cr1);
    } else {
        mmio::w16(tim + TIM_CR1, (cr1 & 0xFCFF) | cfg.cr1_ckd);
        mmio::w32(tim + TIM_ARR, cfg.arr); // ARR and RCR at once
        mmio::w16(tim + TIM_PSC, cfg.psc);
        if is_advanced(tim) {
            mmio::w16(tim + TIM_RCR, cfg.rcr as u16);
        }
    }
    mmio::w16(tim + TIM_EGR, 1); // EGR: UG
}

/// `FUN_0800af4c` - prescaler plus event generation register.
pub fn prescaler_egr(tim: u32, psc: u16, egr: u16) {
    mmio::w16(tim + TIM_PSC, psc);
    mmio::w16(tim + TIM_EGR, egr);
}

/// `FUN_0800AF74` - CR1 bit 7 (auto reload preload).
pub fn arpe(tim: u32, on: bool) {
    mmio::rmw16(tim + TIM_CR1, |v| if on { v | 0x80 } else { v & !0x80 });
}

/// `FUN_0800b1fc` / `b208` / `b220` / `b22c` - the compare preload enable
/// bit of the channels 1..4.
pub fn ocpe(tim: u32, ch: u8, on: bool) {
    let reg = if ch <= 2 { tim + TIM_CCMR1 } else { tim + TIM_CCMR2 };
    let bit: u16 = if ch <= 2 {
        1u16 << ((ch as u32 - 1) * 8 + 3)
    } else {
        1u16 << ((ch as u32 - 3) * 8 + 3)
    };
    mmio::rmw16(reg, |v| if on { v | bit } else { v & !bit });
}

/// `FUN_0800AFDC` (ch1), `FUN_0800B054` (ch2), `FUN_0800B0F4` (ch3) and
/// `FUN_0800B174` (ch4) - the compare channel configuration of one channel.
pub fn channel_cfg(tim: u32, ch: u8, cfg: &ChanCfg) {
    let mut ccer = mmio::r16(tim + TIM_CCER) as u32;
    let mut cr2 = mmio::r16(tim + TIM_CR2) as u32;

    match ch {
        1 => {
            ccer &= !0x0001;
            ccer = (ccer & 0xFFFD) | cfg.ccer4 as u32 | cfg.ccer as u32;
            if is_advanced(tim) {
                ccer = ((ccer & 0xFFF7) | cfg.ccer5 as u32) & 0xFFFB;
                ccer |= cfg.ccer2 as u32;
                cr2 = (cr2 & 0xFCFF) | cfg.cr2 as u32 | cfg.cr2b as u32;
            }
            mmio::w16(tim + TIM_CR2, cr2 as u16);
            mmio::rmw16(tim + TIM_CCMR1, |v| (v & 0xFF8C) | cfg.ccmr);
            mmio::w32(tim + TIM_CCR1, cfg.ccr);
        }
        2 => {
            ccer &= !0x0010;
            ccer = (ccer & 0xFFDF) | ((cfg.ccer4 as u32) << 4) | ((cfg.ccer as u32) << 4);
            if tim == TIM1 {
                ccer = ((ccer & 0xFF7F) | ((cfg.ccer5 as u32) << 4)) & 0xFFBF;
                ccer |= (cfg.ccer2 as u32) << 4;
                cr2 = (cr2 & 0xFBFF) | ((cfg.cr2 as u32) << 2);
                cr2 = (cr2 & 0xF7FF) | ((cfg.cr2b as u32) << 2);
            } else if tim == TIM15 {
                cr2 = (cr2 & 0xFBFF) | ((cfg.cr2 as u32) << 2);
            }
            mmio::w16(tim + TIM_CR2, cr2 as u16);
            mmio::rmw16(tim + TIM_CCMR1, |v| (v & 0x8CFF) | (cfg.ccmr << 8));
            mmio::w32(tim + TIM_CCR2, cfg.ccr);
        }
        3 => {
            ccer &= !0x0100;
            ccer = (ccer & 0xFDFF) | ((cfg.ccer4 as u32) << 8) | ((cfg.ccer as u32) << 8);
            if tim == TIM1 {
                ccer = ((ccer & 0xF7FF) | ((cfg.ccer5 as u32) << 8)) & 0xFBFF;
                ccer |= (cfg.ccer2 as u32) << 8;
                cr2 = (cr2 & 0xCFFF) | ((cfg.cr2 as u32) << 4) | ((cfg.cr2b as u32) << 4);
            }
            mmio::w16(tim + TIM_CR2, cr2 as u16);
            mmio::rmw16(tim + TIM_CCMR2, |v| (v & 0xFF8C) | cfg.ccmr);
            mmio::w32(tim + TIM_CCR3, cfg.ccr);
        }
        _ => {
            // channel 4
            ccer &= !0x1000;
            if tim == TIM1 {
                cr2 = (cr2 & 0xBFFF) | ((cfg.cr2 as u32) << 6);
            }
            mmio::w16(tim + TIM_CR2, cr2 as u16);
            mmio::rmw16(tim + TIM_CCMR2, |v| (v & 0x8CFF) | (cfg.ccmr << 8));
            mmio::w32(tim + TIM_CCR4, cfg.ccr);
            ccer = (ccer & 0xDFFF) | ((cfg.ccer4 as u32) << 12) | ((cfg.ccer as u32) << 12);
        }
    }

    mmio::w16(tim + TIM_CCER, ccer as u16);
}

/// `FUN_0800b1ec` / `b1f0` / `b1f4` / `b1f8` - the compare value.
///
/// The original writes 32 bits at `TIMx_CCR1` (0x34) .. `TIMx_CCR4`
/// (0x40), so a write also clears the next compare register.  Reproduced
/// verbatim.
pub fn pwm_set(tim: u32, ch: u8, ccr: u32) {
    mmio::w32(tim + TIM_CCR1 + 4 * (ch as u32 - 1), ccr);
}

/// `FUN_0800B244` - the channel enable bits of `CCER`.
pub fn ccer(tim: u32, ch: u8, on: bool) {
    let bit: u16 = 1u16 << ((ch as u32 - 1) * 4);
    mmio::rmw16(tim + TIM_CCER, |v| if on { v | bit } else { v & !bit });
}

/// `FUN_0800AFA0` - CR1 bit 0, the counter enable.
pub fn enable(tim: u32, on: bool) {
    mmio::rmw16(tim + TIM_CR1, |v| if on { v | 1 } else { v & !1 });
}

/// `FUN_0800AFBC` - BDTR bit 15, the main output enable.
pub fn moe(tim: u32, on: bool) {
    mmio::rmw16(tim + TIM_BDTR, |v| if on { v | 0x8000 } else { v & !0x8000 });
}

/// `FUN_0800B38C` / `FUN_0800B3C8` - the interrupt enable register.
pub fn dier(tim: u32, mask: u16, on: bool) {
    mmio::rmw16(tim + TIM_DIER, |v| if on { v | mask } else { v & !mask });
}

/// `FUN_0800b3c0` - the status register is written with the inverted mask,
/// which clears the flags.
pub fn sr_clear(tim: u32, mask: u16) {
    mmio::w16(tim + TIM_SR, !mask);
}

/// `FUN_0800b3a4` - the event generation register.
pub fn egr(tim: u32, val: u16) {
    mmio::w16(tim + TIM_EGR, val);
}

/// Read a 16 bit timer register.
pub fn get16(tim: u32, off: u32) -> u16 {
    mmio::r16(tim + off)
}

/// Write a 16 bit timer register.
pub fn set16(tim: u32, off: u32, v: u16) {
    mmio::w16(tim + off, v)
}

/// Read a 32 bit timer register.
pub fn get32(tim: u32, off: u32) -> u32 {
    mmio::r32(tim + off)
}

/// Write a 32 bit timer register.
pub fn set32(tim: u32, off: u32, v: u32) {
    mmio::w32(tim + off, v)
}

// ---------------------------------------------------------------------------
// the hwi_b_hal flavour
// ---------------------------------------------------------------------------

/// `FUN_0800AEC4` + `FUN_0800AFDC` - set a channel up for PWM mode 1.
pub fn pwm_init(tim: u32, psc: u16, arr: u16, channel: u8) {
    mmio::rmw16(tim + TIM_CR1, |v| v & 0xFF7F); // ARPE off
    mmio::w16(tim + TIM_PSC, psc);
    mmio::w32(tim + TIM_ARR, arr as u32);
    mmio::w16(tim + TIM_EGR, 1); // UG: reload

    let ccmr = if channel <= 2 { tim + TIM_CCMR1 } else { tim + TIM_CCMR2 };
    // OCxM = 110 (PWM mode 1), OCxPE = 1
    let shift = (channel & 1) as u32 * 8;
    mmio::rmw32(ccmr, |v| (v & !(0xFFu32 << shift)) | (0x68u32 << shift));
    mmio::rmw16(tim + TIM_CCER, |v| v | (1u16 << ((channel as u32 - 1) * 4)));
}

/// Set the compare value of a PWM channel.
pub fn pwm_duty(tim: u32, channel: u8, ccr: u32) {
    mmio::w32(tim + TIM_CCR1 + (channel as u32 - 1) * 4, ccr);
}

/// Read the compare value of a PWM channel.
pub fn ccr(tim: u32, channel: u8) -> u32 {
    mmio::r32(tim + TIM_CCR1 + (channel as u32 - 1) * 4)
}

/// Set the auto reload value and reload the counter.
pub fn set_arr(tim: u32, arr: u32) {
    mmio::w32(tim + TIM_ARR, arr);
    mmio::w16(tim + TIM_EGR, 1);
}

/// `FUN_0800AFA0` - start the counter.
pub fn start(tim: u32) {
    mmio::rmw16(tim + TIM_CR1, |v| v | 1);
}

/// `FUN_0800AFA0` - stop the counter.
pub fn stop(tim: u32) {
    mmio::rmw16(tim + TIM_CR1, |v| v & !1);
}
