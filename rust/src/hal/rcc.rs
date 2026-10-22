//! RCC clock enables (`hwi_a_hal.c` / `hwi_b_hal.c`).
//!
//! NOTE: in the original image the GPIO/enable bits are shifted by one
//! compared to the published STM32 maps (0x20000 = GPIOA, 0x40000 = GPIOB,
//! 0x80000 = GPIOC, APB2 0x20000 = TIM15/16/17, APB1 0x40000 = USART3,
//! 0x80000 = USART4).  The values used by the original are reproduced
//! verbatim by the modules.

use super::addr::RCC;
use crate::mmio;

const RCC_AHBENR: u32 = 0x14;
const RCC_APB2ENR: u32 = 0x18;
const RCC_APB1ENR: u32 = 0x1C;

/// `FUN_0800AD5C` - the AHB peripheral clock enables.
pub fn ahb_en(mask: u32, enable: bool) {
    mmio::rmw32(RCC + RCC_AHBENR, |v| if enable { v | mask } else { v & !mask });
}

/// `FUN_0800AD7C` - the APB2 peripheral clock enables.
pub fn apb2_en(mask: u32, enable: bool) {
    mmio::rmw32(RCC + RCC_APB2ENR, |v| if enable { v | mask } else { v & !mask });
}

/// `FUN_0800AD9C` - the APB1 peripheral clock enables.
pub fn apb1_en(mask: u32, enable: bool) {
    mmio::rmw32(RCC + RCC_APB1ENR, |v| if enable { v | mask } else { v & !mask });
}
