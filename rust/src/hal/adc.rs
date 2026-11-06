//! ADC1 helpers (`hwi_a_hal.c`).

use crate::mmio;

// ADC register offsets used by the original
const ADC_ISR: u32 = 0x00;
const ADC_CR: u32 = 0x08;
const ADC_CFGR1: u32 = 0x0C;
const ADC_SMPR: u32 = 0x14;
const ADC_CHSELR: u32 = 0x28;
const ADC_DR: u32 = 0x40;

/// `FUN_0800A6A0` - the configuration register CFGR1.
pub fn cfg1(adc: u32, clear_mask: u32, set_mask: u32, exten: u8) {
    let cfgr1 = mmio::r32(adc + ADC_CFGR1);
    let cfgr1 = (cfgr1 & clear_mask) | set_mask | ((exten as u32) << 13);
    mmio::w32(adc + ADC_CFGR1, cfgr1);
}

/// `FUN_0800a6f8` - select a channel and set the sample time.
pub fn channel_add(adc: u32, mask: u8, smpr: u32) {
    mmio::rmw32(adc + ADC_CHSELR, |v| v | mask as u32);
    mmio::w32(adc + ADC_SMPR, smpr);
}

/// `FUN_0800A77C` - the DMA request generation bits of CFGR1.
pub fn dmngt(adc: u32, v: u32) {
    let cfgr1 = mmio::r32(adc + ADC_CFGR1);
    mmio::w32(adc + ADC_CFGR1, (cfgr1 & 0xFFFF_FFFD) | v);
}

/// `FUN_0800A760` - the DMA mode bit of CFGR1.
pub fn dma_mode(adc: u32, on: bool) {
    let cfgr1 = mmio::r32(adc + ADC_CFGR1);
    if on {
        mmio::w32(adc + ADC_CFGR1, cfgr1 | 1);
    } else {
        mmio::w32(adc + ADC_CFGR1, cfgr1 & 0xFFFF_FFFE);
    }
}

/// `FUN_0800A7AC` - clear a status flag by writing its mask to the ISR.
///
/// The dump's helper stores its second argument through the base pointer it is
/// given (`*base = mask`), and `FUN_080071C0` passes the **ADC base** with
/// mask `0x10` (OVR) - the status register of this part sits at offset 0x00,
/// so the two are the same address.
pub fn isr_clear(adc: u32, mask: u32) {
    mmio::w32(adc + ADC_ISR, mask);
}

/// `FUN_0800A6DC` - CR bit 0 enables the ADC, bit 1 starts the shutdown.
pub fn enable(adc: u32, on: bool) {
    let cr = mmio::r32(adc + ADC_CR);
    if on {
        mmio::w32(adc + ADC_CR, cr | 1);
    } else {
        mmio::w32(adc + ADC_CR, cr | 2);
    }
}

/// `FUN_0800a754` - CR bit 2.
pub fn stop(adc: u32) {
    mmio::rmw32(adc + ADC_CR, |v| v | 4);
}

/// `FUN_0800A704` - start a conversion, wait for it and return the result.
///
/// The data register of this part sits at offset 0x40.
///
/// The wait is **bounded** in the dump: `FUN_0800A704` @0x800a704 loads the
/// counter from the literal at 0x800a750 (`0x0000_EFFF`), decrements it by
/// two per iteration (`subs r3,#1` twice, at 0x800a73c and 0x800a746), and
/// when it runs out it takes the path that returns **0** (`movs r0,#0`
/// @0x800a730) instead of the data register.  An unbounded spin would hang
/// the firmware for good on a conversion that never completes, so the port
/// reproduces the bound.
pub fn start(adc: u32) -> u32 {
    let cr = mmio::r32(adc + ADC_CR);
    mmio::w32(adc + ADC_CR, cr | 0x8000_0000);
    let mut countdown: u32 = 0xEFFF;
    while (mmio::r32(adc + ADC_CR) as i32) < 0 {
        if countdown <= 2 {
            return 0; // the dump gives up here and returns 0
        }
        countdown -= 2;
        core::hint::spin_loop();
    }
    mmio::r32(adc + ADC_DR)
}

/// `FUN_0800A78C` - test a status flag.
///
/// The conversion start flag of CR is handled separately by the original.
pub fn ready(adc: u32, mask: u32) -> bool {
    let src = if ((mask << 7) as i32) < 0 {
        mmio::r32(adc + ADC_CR) & 0xFEFF_FFFF
    } else {
        mmio::r32(adc + ADC_ISR)
    };
    mask & src != 0
}
