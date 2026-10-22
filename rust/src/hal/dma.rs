//! DMA1 helpers (`hwi_a_hal.c`).

use super::addr::*;
use crate::mmio;

/// `FUN_0800A840` - the four registers of a DMA channel.
///
/// The original keeps bits 4..14 and 31 of the configuration register and
/// replaces the rest with the value given by the caller.
pub fn channel_cfg(ch: u32, cpar: u32, cmar: u32, cntdr: u16, ccr: u32) {
    mmio::rmw32(ch + 0x00, |v| (v & 0xFFFF_800F) | ccr);
    mmio::w32(ch + 0x04, cntdr as u32);
    mmio::w32(ch + 0x08, cpar);
    mmio::w32(ch + 0x0C, cmar);
}

/// `FUN_0800A87C` - the channel enable bit.
pub fn channel_enable(ch: u32, on: bool) {
    mmio::rmw32(ch + 0x00, |v| if on { v | 1 } else { v & 0xFFFF_FFFE });
}

/// `FUN_0800A7B0` - reset a channel and clear its interrupt flags.
pub fn channel_reset(ch: u32) {
    let mut flags = 0u32;

    mmio::rmw32(ch + 0x00, |v| v & 0xFFFF_FFFE);
    mmio::w32(ch + 0x00, 0);
    mmio::w32(ch + 0x04, 0);
    mmio::w32(ch + 0x08, 0);
    mmio::w32(ch + 0x0C, 0);

    if ch == DMA1_CH1 {
        flags = 0x0000_000F;
    } else if ch == DMA1_CH2 {
        flags = 0x0000_00F0;
    } else if ch == DMA1_CH3 {
        flags = 0x0000_0F00;
    } else if ch == DMA1_CH4 {
        flags = 0x0000_F000;
    } else if ch == DMA1_CH5 {
        flags = 0x000F_0000;
    }

    mmio::rmw32(DMA1 + 0x04, |v| v | flags);
}

/// `FUN_0800a89c` - the number of transfers.
pub fn set_cntdr(ch: u32, n: u16) {
    mmio::w32(ch + 0x04, n as u32);
}

/// `FUN_0800A8A0` - the direction bit of the configuration register.
pub fn set_dir(ch: u32, mask: u32) {
    mmio::rmw32(ch + 0x00, |v| v | mask);
}

/// `FUN_0800a8b8` - the interrupt status register of the DMA.
pub fn flag(mask: u32) -> bool {
    mmio::r32(DMA1 + 0x00) & mask != 0
}

/// `FUN_0800a8c8` - the interrupt clear register of the DMA.
pub fn clear_flags(mask: u32) {
    mmio::w32(DMA1 + 0x04, mask);
}
