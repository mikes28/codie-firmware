//! GPIO helpers.
//!
//! The original firmware has **two** GPIO configuration helpers and they are
//! **not** equivalent, so both are ported:
//!
//! * [`config`] - `FUN_0800A964` as used by the `hwi_a_hal` modules
//!   (hwiAdc, hwiDrive, hwiEnc, hwiSonar).  It collects the register values
//!   and writes them once at the end, and it writes the output type as a
//!   whole byte shifted by the pin number - a value > 1 therefore also sets
//!   the bit of the next pin.  The original does exactly that, so it is kept.
//! * [`config_per_pin`] - `FUN_0800A964` as used by the `hwi_b_hal`
//!   modules: every pin is written on its own and the output type is reduced
//!   to one bit.
//!
//! Register offsets match the dump (GPIO bus at 0x48000000):
//!   MODER 0x00, OTYPER 0x04, OSPEEDR 0x08, PUPDR 0x0C, IDR 0x10, ODR 0x14,
//!   BSRR 0x18, LCKR 0x1C, AFR0 0x20, AFR1 0x24.
//!
//! **Important quirk:** the original clears pins at offset `0x28` (a halfword
//! store, `strh r1,[r2,#40]` @`0x8009e20`), NOT at `0x1C` (BRR).  On this part
//! `0x28` is past AFRH (0x24), but the access width and offset are reproduced
//! verbatim so the port stays bit-compatible with the original.

use super::addr::*;
use crate::mmio;

// ---------------------------------------------------------------------------
// Pin mode and pull constants (the original's `FUN_0800A964` uses
// `mode - 1 < 2` for "output or alternate function")
// ---------------------------------------------------------------------------

/// Pin modes.
pub const MODE_INPUT: u8 = 0;
pub const MODE_OUTPUT: u8 = 1;
pub const MODE_ALT: u8 = 2;
pub const MODE_ANALOG: u8 = 3;

/// Pull up/down codes (PUPDR).
pub const PULL_NONE: u8 = 0;
pub const PULL_UP: u8 = 1;
pub const PULL_DOWN: u8 = 2;

// ---------------------------------------------------------------------------
// hwi_a_hal flavour: Cfg + config (writes all registers once)
// ---------------------------------------------------------------------------

/// Configuration record of the original's `FUN_0800A964` as used by the
/// `hwi_a_hal` modules.  `pins` is a bitmask (bit n = pin n).
#[derive(Clone, Copy)]
pub struct Cfg {
    /// bit n selects pin n.
    pub pins: u16,
    /// `MODE_*`.
    pub mode: u8,
    /// Output type (shifted into the register as a whole byte by the pin
    /// number; a type > 1 also sets the bit of the next pin - the original
    /// does this deliberately).
    pub otype: u8,
    /// 2-bit pull up/down.
    pub pupd: u8,
    /// 2-bit output speed.
    pub ospeed: u8,
}

impl Cfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        Cfg { pins: 0, mode: 0, otype: 0, pupd: 0, ospeed: 0 }
    }
}

/// `FUN_0800A964` - configure a group of pins (the `hwi_a_hal` flavour).
///
/// The registers are read once, updated in a loop over the 16 pins, and
/// written back once at the end.
pub fn config(port: u32, cfg: &Cfg) {
    let mut moder = mmio::r32(port + GPIO_MODER);
    let mut otyper = mmio::r16(port + GPIO_OTYPER);
    let mut ospeedr = mmio::r32(port + GPIO_OSPEEDR);
    let mut pupdr = mmio::r32(port + GPIO_PUPDR);

    for pin in 0..16u8 {
        if cfg.pins & (1u16 << pin) == 0 {
            continue;
        }
        let two = 3u32 << (2 * pin as u32);

        if cfg.mode.wrapping_sub(1) < 2 {
            // output or alternate function
            ospeedr = (ospeedr & !two) | ((cfg.ospeed as u32) << (2 * pin as u32));
            // NOTE: the original shifts the whole type byte by the pin number,
            // so a type > 1 also touches the next pin.
            otyper = ((otyper & !(1u16 << pin)) | ((cfg.otype as u16) << pin)) as u16;
        }
        moder = (moder & !two) | ((cfg.mode as u32) << (2 * pin as u32));
        pupdr = (pupdr & !two) | ((cfg.pupd as u32) << (2 * pin as u32));
    }

    mmio::w32(port + GPIO_MODER, moder);
    mmio::w16(port + GPIO_OTYPER, otyper);
    mmio::w32(port + GPIO_OSPEEDR, ospeedr);
    mmio::w32(port + GPIO_PUPDR, pupdr);
}

// ---------------------------------------------------------------------------
// hwi_b_hal flavour: PerPinCfg + config_per_pin (each pin written on its own)
// ---------------------------------------------------------------------------

/// Configuration record of the original's `FUN_0800A964` as used by the
/// `hwi_b_hal` modules.
#[derive(Clone, Copy)]
pub struct PerPinCfg {
    /// bit n selects pin n.
    pub mask: u32,
    /// `MODE_*`.
    pub mode: u8,
    /// Output speed (`OSPEEDR` field, 0..3).
    pub speed: u8,
    /// 0 = push-pull, 1 = open-drain (reduced to one bit).
    pub otype: u8,
    /// `PULL_*`.
    pub pull: u8,
}

impl PerPinCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        PerPinCfg { mask: 0, mode: 0, speed: 0, otype: 0, pull: 0 }
    }
}

/// `FUN_0800A964` - configure a group of pins (the `hwi_b_hal` flavour).
///
/// Every pin is written on its own (read-modify-write per register) and the
/// output type is reduced to one bit.  Dump evidence:
/// `FUN_0800A964 @ 0x800a9aa lsls r7, r4` uses `1 << pin` (r4 == pin),
/// NOT `1 << (pin + 1)` - the `<< 1` was a sign-error that cleared the wrong
/// OTYPER bit.
pub fn config_per_pin(port: u32, cfg: &PerPinCfg) {
    for pin in 0..16u8 {
        let bit = 1u32 << pin;
        if cfg.mask & bit == 0 {
            continue;
        }
        let two = 3u32 << (pin as u32 * 2);

        if cfg.mode.wrapping_sub(1) < 2 {
            // output or alternate function
            mmio::rmw32(port + GPIO_OSPEEDR, |v| {
                (v & !two) | ((cfg.speed as u32) << (pin as u32 * 2))
            });
            mmio::rmw16(port + GPIO_OTYPER, |v| {
                (v & !(bit as u16)) | (((cfg.otype & 1) as u16) << pin)
            });
        }
        mmio::rmw32(port + GPIO_MODER, |v| {
            (v & !two) | ((cfg.mode as u32) << (pin as u32 * 2))
        });
        mmio::rmw32(port + GPIO_PUPDR, |v| {
            (v & !two) | ((cfg.pull as u32) << (pin as u32 * 2))
        });
    }
}

// ---------------------------------------------------------------------------
// Alternate function, read, set, reset, write
// ---------------------------------------------------------------------------

/// `FUN_0800AAC8` - set the 4-bit alternate-function field of one pin.
///
/// Machine code of the dump (`FUN_0800AAC8`):
///   shift = (pin & 7) * 4
///   offset = (8 + (pin >> 3)) * 4   -> AFRL (0x20) for pins 0-7, AFRH (0x24) for 8-15
/// The 4-bit field in the register is masked; the value is **not** masked -
/// it is shifted in as the caller passed it.  Every call site passes 0..=4.
pub fn set_afr(port: u32, pin: u8, afr: u8) {
    let shift = (pin & 7) as u32 * 4;
    let reg = port + GPIO_AFR0 + (pin as u32 >> 3) * 4;
    mmio::rmw32(reg, |v| (v & !(0xFu32 << shift)) | ((afr as u32) << shift));
}

/// `FUN_0800AABC` - read the input data register.
///
/// Returns true when any of the masked bits is set.  Dump evidence:
/// `0x80043708: ldrh r0,[r0,#16]` confirms the IDR offset is 0x10 (halfword).
pub fn read(port: u32, mask: u16) -> bool {
    mmio::r16(port + GPIO_IDR) & mask != 0
}

/// Set the bits of the port's `BSRR` (offset `0x18`, word store).
pub fn set(port: u32, mask: u16) {
    mmio::w32(port + GPIO_BSRR, mask as u32);
}

/// Clear the bits of the port at offset `0x28` (halfword store).
///
/// **NOT** BRR (0x1C).  Dump evidence: `FUN_080097E8 @0x800980e`
/// stores `strh` to `[r3, #40]` (offset 0x28).  On STM32G0 this is past AFRH,
/// but the original firmware writes a 16-bit mask there and the port matches
/// it byte-for-byte.
pub fn reset(port: u32, mask: u16) {
    mmio::w16(port + 0x28, mask);
}

/// `gpio_write` of `hwi_a_hal.c` - set or clear a group of pins.
///
/// The `hwi_a_hal` flavour clears at offset `0x28` (not `0x1C` like [`reset`]
/// does).  Width: the dump's clear is a **halfword** store -
/// `FUN_08009E0C @0x8009e20` does `strh r1, [r2, #40]` - so the port writes
/// 16 bits, not 32.
pub fn write(port: u32, mask: u16, level: bool) {
    if level {
        mmio::w32(port + GPIO_BSRR, mask as u32);
    } else {
        mmio::w16(port + 0x28, mask);
    }
}
