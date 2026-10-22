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
pub fn start(adc: u32) -> u32 {
    let cr = mmio::r32(adc + ADC_CR);
    mmio::w32(adc + ADC_CR, cr | 0x8000_0000);
    while (mmio::r32(adc + ADC_CR) as i32) < 0 {
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
