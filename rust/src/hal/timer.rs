//! Timer helpers.
//!
//! The original firmware had two sets of timer helpers, and they differ:
//!
//! * the `hwi_a_hal` set ([`base_cfg`], [`channel_cfg`], [`pwm_set`, …]) is
//!   the one that reproduces the original's register writes bit for bit,
//!   including the 32-bit compare-register writes that also clear the next
//!   compare register;
//! * the `hwi_b_hal` set ([`pwm_init`], [`pwm_duty`], …) is the simpler PWM
//!   helper the `hwiLed` / `hwiBuzz` modules use.
//!
//! Every register access below matches the machine code of the original flash
//! dump (`stm32_flash.bin`).  Addresses, offsets, masks, widths and write
//! order are unchanged from the original.

use super::addr::*;
use crate::mmio;

// ---------------------------------------------------------------------------
// Named constants for timer register fields
// ---------------------------------------------------------------------------

/// CCMRx low-byte field mask: CCxS(1:0) | OCxPE(3) | OCxM(6:4).
///
/// The dump loads this as an immediate (`movs r6,#0x73` @`0x800afec` for ch1,
/// `movs r3,#0x73` @`0x800b104` for ch3) and clears it with `bics` before ORing
/// in the caller's CCMR value.
const CCMR_LOW_MASK: u16 = 0x0073;

/// CCMRx high-byte clear mask: clears bits 0..7 of the high channel byte.
///
/// Used for channels 2 and 4 (their config lives in the high byte of CCMR1/2).
const CCMR_HIGH_CLEAR: u16 = 0x8CFF;

/// CR1 bits 4..7 mask (CMS / DIR) for TIM1/TIM2/TIM3.
const CR1_CMS_DIR_MASK: u16 = 0xFF8F;

/// CR1 bits 8..9 mask (CKD) applied to all timers.
const CR1_CKD_MASK: u16 = 0xFCFF;

/// PWM mode 1 + preload enable: OCxM = 110, OCxPE = 1.
const PWM_MODE_1_OCX_PE: u32 = 0x68;

// ---------------------------------------------------------------------------
// Base configuration record of the original's `FUN_0800AEC4`.
// ---------------------------------------------------------------------------

/// Base timer configuration: prescaler, auto-reload, CR1 bits, repetition
/// counter.
///
/// Fields map to the struct the original builds on the stack and passes to
/// `FUN_0800AEC4`.
#[derive(Clone, Copy)]
pub struct BaseCfg {
    /// Prescaler value (written to PSC at +0x28).
    pub psc: u16,
    /// CR1 bits 4..7 (CMS / DIR), merged into CR1 for TIM1/TIM2/TIM3.
    pub cr1_hi: u16,
    /// Auto-reload value (written to ARR at +0x2C; on TIM6 this word also
    /// covers RCR).
    pub arr: u32,
    /// CR1 bits 8..9 (clock division), merged into CR1.
    pub cr1_ckd: u16,
    /// Repetition counter (advanced timers only, written to RCR at +0x30).
    pub rcr: u8,
}

impl BaseCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        BaseCfg { psc: 0, cr1_hi: 0, arr: 0, cr1_ckd: 0, rcr: 0 }
    }
}

/// Compare-channel configuration record of the original's
/// `FUN_0800AFDC` / `FUN_0800B054` / `FUN_0800B0F4` / `FUN_0800B174`.
#[derive(Clone, Copy)]
pub struct ChanCfg {
    /// CCMRx byte that selects the output mode (OR'd into CCMR1/CCMR2).
    pub ccmr: u16,
    /// CCER enable bit (CCxE) for this channel.
    pub ccer: u16,
    /// Advanced timers: extra CCER bits (OR'd after ccer).
    pub ccer2: u16,
    /// Advanced timers: extra CCER bits (OR'd after ccer2).
    pub ccer3: u16,
    /// Compare value (written as 32 bits to CCRx at +0x34..+0x40).
    pub ccr: u32,
    /// Advanced timers: extra CCER bits.
    pub ccer4: u16,
    /// Advanced timers: extra CCER bits.
    pub ccer5: u16,
    /// Advanced timers: CR2 bits.
    pub cr2: u16,
    /// Advanced timers: CR2 bits.
    pub cr2b: u16,
}

impl ChanCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        ChanCfg { ccmr: 0, ccer: 0, ccer2: 0, ccer3: 0, ccr: 0, ccer4: 0,
            ccer5: 0, cr2: 0, cr2b: 0 }
    }
}

/// `true` for the advanced timers (TIM1, TIM15, TIM16, TIM17) — they have
/// RCR, BDTR and extra CCER/CR2 bits that basic timers lack.
fn is_advanced(tim: u32) -> bool {
    tim == TIM1 || tim == TIM15 || tim == TIM16 || tim == TIM17
}

// ---------------------------------------------------------------------------
// hwi_a_hal flavour: base_cfg, channel_cfg, pwm_set, … (`FUN_0800AEC4` …)
// ---------------------------------------------------------------------------

/// `FUN_0800AEC4` - prescaler, auto-reload, CR1 bits and the update event.
///
/// Write order: CR1 → ARR → PSC.  For TIM6 there is a dedicated branch
/// (CR1 → ARR as a 32-bit word at +0x2C → PSC as a halfword at +0x28) and
/// then the shared EGR tail.  Dump evidence:
///   `0x800aef2: strh r3,[r0,#0]`    ; CR1
///   `0x800aef6: str  r3,[r0,#44]`   ; ARR (+0x2c)
///   `0x800aefa: strh r2,[r0,#40]`   ; PSC (+0x28)
pub fn base_cfg(tim: u32, cfg: &BaseCfg) {
    let mut cr1 = mmio::r16(tim + TIM_CR1);

    if tim == TIM1 || tim == TIM2 || tim == TIM3 {
        cr1 = (cr1 & CR1_CMS_DIR_MASK) | cfg.cr1_hi;
    }

    if tim == TIM6 {
        // TIM6: the 32-bit ARR/RCR pair at +0x2c is a single word there.
        mmio::w16(tim + TIM_CR1, (cr1 & CR1_CKD_MASK) | cfg.cr1_ckd);
        mmio::w32(tim + TIM_ARR, cfg.arr); // ARR and RCR at once
        mmio::w16(tim + TIM_PSC, cfg.psc);
    } else {
        mmio::w16(tim + TIM_CR1, (cr1 & CR1_CKD_MASK) | cfg.cr1_ckd);
        mmio::w32(tim + TIM_ARR, cfg.arr); // ARR and RCR at once
        mmio::w16(tim + TIM_PSC, cfg.psc);
        if is_advanced(tim) {
            mmio::w16(tim + TIM_RCR, cfg.rcr as u16);
        }
    }
    mmio::w16(tim + TIM_EGR, 1); // EGR: UG (shared tail)
}

/// `FUN_0800af4c` - prescaler plus event generation register.
pub fn prescaler_egr(tim: u32, psc: u16, egr: u16) {
    mmio::w16(tim + TIM_PSC, psc);
    mmio::w16(tim + TIM_EGR, egr);
}

/// `FUN_0800AF74` - CR1 bit 7, auto-reload preload enable.
pub fn arpe(tim: u32, on: bool) {
    mmio::rmw16(tim + TIM_CR1, |v| if on { v | 0x80 } else { v & !0x80 });
}

/// `FUN_0800b1fc` / `b208` / `b220` / `b22c` - compare preload enable bit of
/// channels 1..4.
pub fn ocpe(tim: u32, ch: u8, on: bool) {
    let reg = if ch <= 2 { tim + TIM_CCMR1 } else { tim + TIM_CCMR2 };
    let ch = ch.clamp(1, 4);
    let bit: u16 = if ch <= 2 {
        1u16 << ((ch as u32 - 1) * 8 + 3)
    } else {
        1u16 << ((ch as u32 - 3) * 8 + 3)
    };
    mmio::rmw16(reg, |v| if on { v | bit } else { v & !bit });
}

/// `FUN_0800AFDC` (ch1), `FUN_0800B054` (ch2), `FUN_0800B0F4` (ch3),
/// `FUN_0800B174` (ch4) - the compare-channel configuration of one channel.
///
/// The CCMR low-byte mask is `0x0073` (CCxS|OCxPE|OCxM) for channels 1 and 3,
/// and `0x8CFF` (high-byte clear) for channels 2 and 4.  The old port used
/// `0x8C` for ch1/ch3 which left CC1S/OC1M stale; the dump confirms `0x73`.
///
/// The CCR is written as 32 bits, which also clears the next compare register
/// (reproduced verbatim from the original).
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
            // CCMR1 low byte: (CCMR1 & ~0x0073) | ccmr
            mmio::rmw16(tim + TIM_CCMR1, |v| (v & !CCMR_LOW_MASK) | cfg.ccmr);
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
            // CCMR1 high byte: (CCMR1 & 0x8CFF) | (ccmr << 8)
            mmio::rmw16(tim + TIM_CCMR1, |v| (v & CCMR_HIGH_CLEAR) | (cfg.ccmr << 8));
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
            // CCMR2 low byte: (CCMR2 & ~0x0073) | ccmr
            mmio::rmw16(tim + TIM_CCMR2, |v| (v & !CCMR_LOW_MASK) | cfg.ccmr);
            mmio::w32(tim + TIM_CCR3, cfg.ccr);
        }
        _ => {
            // channel 4
            ccer &= !0x1000;
            if tim == TIM1 {
                cr2 = (cr2 & 0xBFFF) | ((cfg.cr2 as u32) << 6);
            }
            mmio::w16(tim + TIM_CR2, cr2 as u16);
            // CCMR2 high byte: (CCMR2 & 0x8CFF) | (ccmr << 8)
            mmio::rmw16(tim + TIM_CCMR2, |v| (v & CCMR_HIGH_CLEAR) | (cfg.ccmr << 8));
            mmio::w32(tim + TIM_CCR4, cfg.ccr);
            ccer = (ccer & 0xDFFF) | ((cfg.ccer4 as u32) << 12) | ((cfg.ccer as u32) << 12);
        }
    }

    mmio::w16(tim + TIM_CCER, ccer as u16);
}

/// `FUN_0800b1ec` / `b1f0` / `b1f4` / `b1f8` - the compare value.
///
/// The original writes 32 bits at `TIMx_CCR1` (0x34) .. `TIMx_CCR4` (0x40),
/// so a write also clears the next compare register.  Reproduced verbatim.
pub fn pwm_set(tim: u32, ch: u8, ccr: u32) {
    let ch = ch.clamp(1, 4);
    mmio::w32(tim + TIM_CCR1 + 4 * (ch as u32 - 1), ccr);
}

/// `FUN_0800B244` - the channel enable bits of `CCER`.
pub fn ccer(tim: u32, ch: u8, on: bool) {
    let bit: u16 = 1u16 << ((ch.clamp(1, 4) as u32 - 1) * 4);
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

/// Read a 16-bit timer register.
pub fn get16(tim: u32, off: u32) -> u16 {
    mmio::r16(tim + off)
}

/// Write a 16-bit timer register.
pub fn set16(tim: u32, off: u32, v: u16) {
    mmio::w16(tim + off, v)
}

/// Read a 32-bit timer register.
pub fn get32(tim: u32, off: u32) -> u32 {
    mmio::r32(tim + off)
}

/// Write a 32-bit timer register.
pub fn set32(tim: u32, off: u32, v: u32) {
    mmio::w32(tim + off, v)
}

// ---------------------------------------------------------------------------
// hwi_b_hal flavour: pwm_init, pwm_duty, … (`FUN_0800AEC4` + `FUN_0800AFDC`)
// ---------------------------------------------------------------------------

/// `FUN_0800AEC4` + `FUN_0800AFDC` - set a channel up for PWM mode 1.
///
/// ARPE off, prescaler, auto-reload, update event, CCMR = 0x68
/// (OCxM = 110 PWM mode 1, OCxPE = 1), CCER channel enable.
pub fn pwm_init(tim: u32, psc: u16, arr: u16, channel: u8) {
    mmio::rmw16(tim + TIM_CR1, |v| v & 0xFF7F); // ARPE off
    mmio::w16(tim + TIM_PSC, psc);
    mmio::w32(tim + TIM_ARR, arr as u32);
    mmio::w16(tim + TIM_EGR, 1); // UG: reload

    let ccmr = if channel <= 2 { tim + TIM_CCMR1 } else { tim + TIM_CCMR2 };
    // OCxM = 110 (PWM mode 1), OCxPE = 1
    let shift = (channel & 1) as u32 * 8;
    mmio::rmw32(ccmr, |v| (v & !(0xFFu32 << shift)) | (PWM_MODE_1_OCX_PE << shift));
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

/// Set the auto-reload value and reload the counter.
pub fn set_arr(tim: u32, arr: u32) {
    mmio::w32(tim + TIM_ARR, arr);
    mmio::w16(tim + TIM_EGR, 1);
}
/// `FUN_0800af90` - CR1 bit 3, one-pulse mode.
///
/// The original computes `CR1 = (CR1 & 0xfff7) | param`, i.e. it sets or
/// clears OPM while leaving the other bits untouched.
pub fn one_pulse(tim: u32, on: bool) {
    mmio::rmw16(tim + TIM_CR1, |v| if on { v | 0x08 } else { v & !0x08 });
}

/// `FUN_0800AFA0` - start the counter.
pub fn start(tim: u32) {
    mmio::rmw16(tim + TIM_CR1, |v| v | 1);
}

/// `FUN_0800AFA0` - stop the counter.
pub fn stop(tim: u32) {
    mmio::rmw16(tim + TIM_CR1, |v| v & !1);
}
