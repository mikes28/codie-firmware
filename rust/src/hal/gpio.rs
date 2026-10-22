//! GPIO helpers.
//!
//! The original firmware had *two* independent GPIO helpers and they are
//! **not** equivalent, so both are ported:
//!
//! * [`config`] - `FUN_0800A964` as it is used by the `hwi_a_hal` modules
//!   (hwiAdc, hwiDrive, hwiEnc, hwiSonar).  It collects the register values
//!   and writes them once at the end, and it writes the output type as a
//!   whole byte shifted by the pin number - a value > 1 therefore also sets
//!   the bit of the next pin.  The original does exactly that (the motor
//!   driver pins use `otype = 3`), so it is kept.
//! * [`config_per_pin`] - `FUN_0800A964` as it is used by the `hwi_b_hal`
//!   modules: every pin is written on its own and the output type is
//!   reduced to one bit.

use super::addr::*;
use crate::mmio;

/// Pin modes (the original's `FUN_0800A964` uses `mode - 1 < 2` for
/// "output or alternate function").
pub const MODE_INPUT: u8 = 0;
pub const MODE_OUTPUT: u8 = 1;
pub const MODE_ALT: u8 = 2;
pub const MODE_ANALOG: u8 = 3;

/// Pull up/down codes (`PUPDR`).
pub const PULL_NONE: u8 = 0;
pub const PULL_UP: u8 = 1;
pub const PULL_DOWN: u8 = 2;

/// Configuration record of the original's `FUN_0800A964` as it is used by
/// the `hwi_a_hal` modules.
#[derive(Clone, Copy)]
pub struct Cfg {
    /// bit n selects pin n
    pub pins: u16,
    /// `MODE_*`
    pub mode: u8,
    /// output type (shifted into the register as a whole byte, see above)
    pub otype: u8,
    /// 2 bit pull up/down
    pub pupd: u8,
    /// 2 bit output speed
    pub ospeed: u8,
}

impl Cfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        Cfg { pins: 0, mode: 0, otype: 0, pupd: 0, ospeed: 0 }
    }
}

/// Configuration record of the original's `FUN_0800A964` as it is used by
/// the `hwi_b_hal` modules.
#[derive(Clone, Copy)]
pub struct PerPinCfg {
    /// bit n selects pin n
    pub mask: u32,
    /// `MODE_*`
    pub mode: u8,
    /// output speed (`OSPEEDR` field, 0..3)
    pub speed: u8,
    /// 0 = push-pull, 1 = open-drain
    pub otype: u8,
    /// `PULL_*`
    pub pull: u8,
}

impl PerPinCfg {
    /// A configuration with every field zeroed.
    pub const fn zeroed() -> Self {
        PerPinCfg { mask: 0, mode: 0, speed: 0, otype: 0, pull: 0 }
    }
}

/// `FUN_0800A964` - configure a group of pins (the `hwi_a_hal` flavour).
///
/// The registers are written once, after the loop over the 16 pins.
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
            // NOTE: the original shifts the whole type byte by the pin
            // number, so a type > 1 also touches the next pin.
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

/// `FUN_0800A964` - configure a group of pins (the `hwi_b_hal` flavour).
///
/// Every pin is written on its own, the output type is reduced to one bit.
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
                (v & !((bit as u16) << 1)) | (((cfg.otype & 1) as u16) << pin)
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

/// `FUN_0800AAC8` - set the 4 bit alternate function field of one pin.
pub fn set_afr(port: u32, pin: u8, afr: u8) {
    let shift = (pin & 7) as u32 * 4;
    let reg = port + GPIO_AFR0 + (pin as u32 >> 3) * 4;
    mmio::rmw32(reg, |v| (v & !(0xFu32 << shift)) | ((afr as u32 & 0xF) << shift));
}

/// `FUN_0800AABC` - read the input data register.
///
/// Returns true when any of the masked bits is set.
pub fn read(port: u32, mask: u16) -> bool {
    mmio::r16(port + GPIO_IDR) & mask != 0
}

/// Set the bits of the port's `BSRR` (`0x18`).
pub fn set(port: u32, mask: u16) {
    mmio::w32(port + GPIO_BSRR, mask as u32);
}

/// Clear the bits of the port's `BRR` (`0x1C`).
pub fn reset(port: u32, mask: u16) {
    mmio::w32(port + GPIO_BRR, mask as u32);
}

/// `gpio_write` of `hwi_a_hal.c` - set or clear a group of pins.
///
/// NOTE: the C helper of the `hwi_a_hal` slice clears the bits in the
/// register at offset `0x28` (not `0x1C` like [`reset`] does).  The
/// behaviour is reproduced verbatim so that the port stays bit compatible
/// with the C firmware; see `ARCHITECTURE.md`.
pub fn write(port: u32, mask: u16, level: bool) {
    if level {
        mmio::w32(port + GPIO_BSRR, mask as u32);
    } else {
        mmio::w32(port + 0x28, mask as u32);
    }
}
