//! The clock tree and the millisecond tick
//! (`fw/src/system_stm32g0.c`).
//!
//! `system_init()` is the original's `FUN_08002958` (called from the startup
//! code at 0x08002904 before `main()`).
//!
//! Recovered behaviour of `FUN_08002958` (register writes, in order):
//!   RCC->CR    |= 0x00000001                (HSI on)
//!   RCC->CFGR &= 0xF8FFB80C
//!   RCC->CR    &= 0xFEF6FFFF
//!   RCC->CR    &= 0xFFFBFFFF
//!   RCC->CFGR &= 0xFFC0FFFF
//!   RCC[0x2C]  &= 0xFFFFFFF0                (HSIDIV / reset flags)
//!   RCC[0x30]  &= 0xFFFFFEAC                (CSR: reset flags)
//!   RCC[0x34]  &= 0xFFFFFFFE                (CCIPR)
//!   RCC->CIR   = 0
//!   RCC->CR    |= 0x00010000                (PLL on)
//!   wait up to 0x5000 times for RCC->CR & 0x00020000 (PLL ready)
//!   if (RCC->CR & 0x00010000) {
//!       FLASH->ACR = 0x11                    (latency 1 + prefetch)
//!       RCC->CFGR &= 0xFFC07FFF
//!       RCC->CFGR |= 0x00130000
//!       RCC->CR   |= 0x01000000
//!       wait for RCC->CR & 0x02000000
//!       RCC->CFGR = (RCC->CFGR & ~0x3) | 0x2 (SYSCLK <- PLL)
//!       wait until (RCC->CFGR & 0xC) == 0x8 (SWS = PLL)
//!   }
//!
//! The resulting clock tree (decoded with the original's own clock helper
//! `FUN_0800AC40`):
//!   source        : PLL, PLLSRC (CFGR bits 16..17) = 0b11, base 16 MHz
//!                   (16 000 000 / (HSI divider + 1))
//!   PLLMUL       : (CFGR bits 18..21) + 2 = 4 + 2 = 6  ->  16 MHz * 6
//!   SYSCLK       : 96 MHz
//!   bus dividers  : HPRE = 0 (/1), PPRE1 = 0 (/1), PPRE2 = 0 (/1)
//!                   (the reset values are kept, the original never touches
//!                    them)  ->  AHB = APB1 = APB2 = 96 MHz
//!   flash        : ACR = 0x11 (1 wait state, prefetch enabled)
//!   millisecond tick: SysTick, 96 MHz / 96000 = 1 kHz, reload 95999

use core::sync::atomic::{AtomicU32, Ordering};

use crate::hal::addr::{FLASH, RCC, SYSTICK};
use crate::mmio;

// RCC register offsets
const RCC_CR: u32 = 0x00;
const RCC_CFGR: u32 = 0x04;
const RCC_CIR: u32 = 0x08;
const RCC_AUX0: u32 = 0x2C;
const RCC_CSR: u32 = 0x30;
const RCC_CCIPR: u32 = 0x34;

const FLASH_ACR: u32 = 0x00;

const SYSTICK_CTRL: u32 = 0x00;
const SYSTICK_LOAD: u32 = 0x04;
const SYSTICK_VAL: u32 = 0x08;

/// The millisecond counter of the firmware.
///
/// The original keeps it in the TIM6 interrupt, the rebuild uses the
/// SysTick.
static TICK_MS: AtomicU32 = AtomicU32::new(0);

/// The shared millisecond tick.
pub fn tick_ms() -> u32 {
    TICK_MS.load(Ordering::Relaxed)
}

/// `FUN_08002958` - the clock tree of the original.
pub fn system_init() {
    mmio::rmw32(RCC + RCC_CR, |v| v | 0x0000_0001);
    mmio::rmw32(RCC + RCC_CFGR, |v| v & 0xF8FF_B80C);
    mmio::rmw32(RCC + RCC_CR, |v| v & 0xFEF6_FFFF);
    mmio::rmw32(RCC + RCC_CR, |v| v & 0xFFFB_FFFF);
    mmio::rmw32(RCC + RCC_CFGR, |v| v & 0xFFC0_FFFF);
    mmio::rmw32(RCC + RCC_AUX0, |v| v & 0xFFFF_FFF0);
    mmio::rmw32(RCC + RCC_CSR, |v| v & 0xFFFF_FEAC);
    mmio::rmw32(RCC + RCC_CCIPR, |v| v & 0xFFFF_FFFE);
    mmio::w32(RCC + RCC_CIR, 0);

    mmio::rmw32(RCC + RCC_CR, |v| v | 0x0001_0000); // PLL on

    let mut spin = 0u32;
    while mmio::r32(RCC + RCC_CR) & 0x0002_0000 == 0 {
        spin += 1;
        if spin >= 0x5000 {
            break;
        }
    }

    if mmio::r32(RCC + RCC_CR) & 0x0001_0000 != 0 {
        mmio::w32(FLASH + FLASH_ACR, 0x11); // latency 1 + prefetch
        mmio::rmw32(RCC + RCC_CFGR, |v| v & 0xFFC0_7FFF);
        mmio::rmw32(RCC + RCC_CFGR, |v| v | 0x0013_0000);
        mmio::rmw32(RCC + RCC_CR, |v| v | 0x0100_0000);
        while mmio::r32(RCC + RCC_CR) & 0x0400_0000 == 0 {
            // wait for the PLL/oscillator ready flag.  The dump tests bit 22
            // (register 0x08002a17: `lsls r2, r3, #18` -> 0x0400_0000, then
            // `tst r1, r2` / `beq`).  The first version of this wait tested
            // bit 25 - the same function clears that bit a dozen
            // instructions earlier - so the firmware would have hung here
            // and never brought up a single peripheral.
            core::hint::spin_loop();
        }
        mmio::rmw32(RCC + RCC_CFGR, |v| (v & !0x0000_0003) | 0x0000_0002); // SYSCLK = PLL
        while mmio::r32(RCC + RCC_CFGR) & 0x0000_000C != 0x0000_0008 {
            // wait until SWS reports the PLL
            core::hint::spin_loop();
        }
    }
}

/// Program the millisecond tick (the original's `main()` resets the SysTick
/// registers at the start of the "reset cause" path).
pub fn sys_tick_init(reload: u32) {
    mmio::w32(SYSTICK + SYSTICK_CTRL, 0);
    mmio::w32(SYSTICK + SYSTICK_LOAD, reload.wrapping_sub(1));
    mmio::w32(SYSTICK + SYSTICK_VAL, 0);
    mmio::w32(SYSTICK + SYSTICK_CTRL, 7); // enable, tick interrupt, processor clock
}

/// The SysTick interrupt of the rebuild.
///
/// NOTE: the Cortex-M0+ has no atomic compare-and-swap, so `fetch_add` is
/// not available for `thumbv6m-none-eabi` - the counter is incremented with
/// a load and a store, exactly like the original's TIM6 handler.
#[no_mangle]
pub extern "C" fn SysTick_Handler() {
    let now = TICK_MS.load(Ordering::Relaxed);
    TICK_MS.store(now.wrapping_add(1), Ordering::Relaxed);
}
