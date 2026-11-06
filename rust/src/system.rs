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

use crate::hal::addr::{FLASH, IWDG, RCC, SYSTICK};
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
/// SysTick.  `dump_tick` mirrors the original's raw TIM6 counter
/// (`DAT_20001018`, incremented by `FUN_0800A43C`) for anyone who needs the
/// original's cadence.
static TICK_MS: AtomicU32 = AtomicU32::new(0);

/// The raw TIM6 update counter of the original (IRQ 17, `FUN_0800A43C`).
static DUMP_TICK: AtomicU32 = AtomicU32::new(0);

/// The shared millisecond tick.
pub fn tick_ms() -> u32 {
    TICK_MS.load(Ordering::Relaxed)
}

/// The original's TIM6 update count.
///
/// `FUN_0800A43C` (@0x800a43c) is the IRQ-17 handler: it tests the TIM6
/// status flag, increments `DAT_20001018` and clears the flag.  The port's
/// `tim6_tick` does the same, so the original's cadence stays observable even
/// though the firmware's own `millis()` is fed by the SysTick.
pub fn dump_tick() -> u32 {
    DUMP_TICK.load(Ordering::Relaxed)
}

/// `FUN_0800A43C` - the TIM6 update interrupt (IRQ 17).
#[no_mangle]
pub extern "C" fn tim6_tick() {
    let cr = crate::hal::addr::TIM6;
    let sr = mmio::r32(cr + 0x10); // TIM_SR
    if sr & 1 != 0 {
        // `fetch_add` is unavailable on the thumbv6m core without the atomics
        // feature, and this runs in interrupt context with interrupts masked,
        // so a plain load/store is both sufficient and race-free.
        let n = DUMP_TICK.load(Ordering::Relaxed);
        DUMP_TICK.store(n.wrapping_add(1), Ordering::Relaxed);
        mmio::w32(cr + 0x10, 1); // clear UIF
    }
}

/// `FUN_0800A464` - the millisecond tick of the original, on TIM6.
///
/// Machine code of the dump:
/// ```text
/// 0x800a468: movs r0,#16 ; movs r1,#1 ; bl 0x800ad9c  -> APB1ENR |= 0x10 (TIM6 clock)
/// 0x800a470: ldr r4,[pc,#68] @0x800a4b8 -> 0x40001000 (TIM6)
/// 0x800a474: movs r1,#0 ; bl 0x800af74             -> ARPE off
/// struct at sp+4: strh 0@[r1,#2] (cr1_hi); strh 48@[r1,#0] (psc=0x30);
///                str 0xFFFF@[r1,#4] (arr); strh 0@[r1,#8] (ckd)
/// 0x800a48c: bl 0x800aec4                         -> the base config
/// NVIC: strb 17@[sp,#0] (irq), strb 1@[sp,#2], strb 2@[sp,#1] (prio 2); bl 0x800ab30
/// 0x800a4a6: movs r1,#1 ; bl 0x800af58            -> DIER update IE on
/// 0x800a4b0: movs r1,#1 ; bl 0x800b3c0             -> clear the pending flag
/// ```
/// NOTE: PSC = 48 with ARR = 0xFFFF on a 48 MHz timer clock is a ~65.5 ms
/// update period, i.e. ~15 Hz - not the 1 kHz the greeting's
/// `"ttpms":"1000"` advertises.  The port therefore keeps the SysTick as the
/// source of `millis()` (which the periodic engines gate on) and runs this
/// TIM6 in addition so the original's counter is still there.  With the
/// `dump-tick` feature this becomes the only tick.
pub fn tim6_tick_init() {
    use crate::hal::{addr, rcc, timer};
    rcc::apb1_en(0x10, true); // TIM6 clock
    let base = timer::BaseCfg {
        psc: 48,
        cr1_hi: 0,
        arr: 0xFFFF,
        cr1_ckd: 0,
        rcr: 0,
    };
    timer::base_cfg(addr::TIM6, &base);
    crate::hal::nvic::enable(17, 2); // TIM6 update
    timer::dier(addr::TIM6, 1, true); // update interrupt
    timer::sr_clear(addr::TIM6, 1);
    timer::enable(addr::TIM6, true);
}

/// `FUN_0800A640` - the watchdog bring-up of the original's hardware prep.
///
/// The dump calls this from `main()` (`FUN_08009FA4`) before the module
/// inits, and it is the ONLY place the watchdog is ever started.  The
/// sequence, read from the decompilation and confirmed in the machine code:
///
/// ```text
/// FUN_0800A5F0();                      RCC AHBENR |= 0xE00000 (GPIOA|B|C),
///                                      then a GPIO bank setup
/// FUN_0800ABE8(1);
/// do { i = FUN_0800ADDC(0x41); } while (i == 0);
///                                      wait for a ready flag: 0x41 >> 5 == 2
///                                      -> [RCC + 0x24], bit 0x41 & 31 == 1
/// FUN_0800aaec(0x5555);               PR  (prescaler)
/// FUN_0800aaf8(2);                    RLR (reload)
/// FUN_0800ab04(0xffe);                WLR (window)
/// FUN_0800ab10();                     refresh (KR = 0xAAAA)
/// FUN_0800ab20();                     enable  (KR = 0xCCCC)
/// ```
///
/// The C reconstruction implements none of this, so its watchdog never runs
/// and the kick in `hwi_power::off` is a write into a dead peripheral there.
/// This port **does** start it: `watchdog_init` below reproduces the whole
/// sequence and `boot::run` refreshes it (`util::iwdg_kick`, i.e. the dump's
/// `FUN_0800AB10`) on every main-loop iteration, so a hang is recovered by a
/// reset even though the C is not.
///
/// NOTE on the wait: `FUN_0800ADDC` is a reusable flag test
/// (`arg >> 5` selects one of `[RCC+0]`, `[RCC+0x20]`, `[RCC+0x24]`,
/// `[RCC+0x34]`; `arg & 31` selects the bit).  The dump waits for bit 1 of
/// `RCC+0x24`.  That register is outside the map the C reconstruction uses,
/// so the port waits on the same address and bit.
pub fn watchdog_init() {
    // FUN_0800A5F0: GPIOA|GPIOB|GPIOC on, then the pin setup of the IWDG group
    crate::hal::rcc::ahb_en(0x000E_0000, true);

    // FUN_0800ABE8(1) - the reset-release of the same GPIO group; the C
    // reconstruction does not model it, so it is a no-op here

    // the ready flag of the oscillator (bit 1 of RCC+0x24)
    // Dump FUN_0800A640: do { i = FUN_0800ADDC(0x41); } while (i == 0);
    // unbounded wait - add bound to prevent permanent hang
    let mut countdown: u32 = 0x5000;
    while mmio::r32(RCC + 0x24) & (1 << 1) == 0 {
        if countdown == 0 {
            break;
        }
        countdown -= 1;
        core::hint::spin_loop();
    }

    // IWDG magic writes (FUN_0800A5F0; helpers FUN_0800AAEC..AB20).  KR=0x00,
    // PR=0x04, RLR=0x08 (G071RM §30.4); each value goes to its matching
    // offset - the port had the first three shifted up by one register:
    mmio::w32(IWDG + 0x00, 0x5555); // KR: unlock PR/RLR write access (FUN_0800AAEC)
    mmio::w32(IWDG + 0x04, 2);      // PR: prescaler                (FUN_0800AAF8)
    mmio::w32(IWDG + 0x08, 0x0FFE); // RLR: reload                  (FUN_0800AB04)
    mmio::w32(IWDG + 0x00, 0xAAAA); // KR: reload counter           (FUN_0800AB10)
    mmio::w32(IWDG + 0x00, 0xCCCC); // KR: start watchdog           (FUN_0800AB20)
}

/// `FUN_08002958` - the clock tree of the original.
///
/// Machine code of the dump:
/// ```text
/// RCC[0x2C]  &= 0xFFFFFFF0                (HSIDIV / reset flags)
/// RCC[0x30]  &= 0xFFFFFEAC                (CSR: reset flags)
/// RCC[0x34]  &= 0xFFFFFFFE                (CCIPR)
/// RCC->CIR   = 0
/// RCC->CR    = (RCC->CR & 0xFEF2FFFF) | 0x10001   (HSI on, preserve HSERDY bit)
/// wait up to 0x5000 for RCC->CR & 0x00020000 (HSERDY, bit 17)
/// RCC->CFGR  &= 0xF8C0B80C
/// if (HSERDY was set in original CR) {
///     FLASH->ACR = 0x11                    (latency 1 + prefetch)
///     wait for RCC->CR & 0x02000000 (PLL ready, bit 25) - unbounded
///     do {
///         RCC->CR   = (RCC->CR & 0xFEF2FFFF) | 0x1010001  (HSE on)
///         RCC->CFGR = (RCC->CFGR & 0xF8C0380C) | 0x130002  (PLL as SYSCLK)
///     } while ((RCC->CFGR & 0xC) != 8)  (SWS = PLL)
/// }
/// RCC->CFGR = final CFGR value
/// RCC->CR   = final CR value
/// ```
pub fn system_init() {
    // Machine code of the dump (FUN_08002958 @0x8002958):
    //   RCC[0x2C]  &= 0xFFFFFFF0
    //   RCC[0x30]  &= 0xFFFFFEAC
    //   RCC[0x34]  &= 0xFFFFFFFE
    //   RCC->CIR   = 0
    //   RCC->CR    = (RCC->CR & 0xFEF2FFFF) | 0x10001   (HSI on, preserve HSERDY)
    //   wait up to 0x5000 for RCC->CR & 0x00020000 (HSERDY, bit 17)
    //   RCC->CFGR  &= 0xF8C0B80C
    //   if (HSERDY was set in the original CR value) {
    //       FLASH->ACR = 0x11
    //       wait for RCC->CR & 0x02000000 (PLL ready, bit 25) - unbounded
    //       do {
    //           RCC->CR   = (RCC->CR & 0xFEF2FFFF) | 0x1010001
    //           RCC->CFGR = (RCC->CFGR & 0xF8C0380C) | 0x130002
    //       } while ((RCC->CFGR & 0xC) != 8)
    //   }
    //   RCC->CFGR = final value
    //   RCC->CR   = final value

    // Steps 1-4: write RCC[0x2C], [0x30], [0x34], CIR first
    mmio::rmw32(RCC + RCC_AUX0, |v| v & 0xFFFF_FFF0);
    mmio::rmw32(RCC + RCC_CSR,  |v| v & 0xFFFF_FEAC);
    mmio::rmw32(RCC + RCC_CCIPR,|v| v & 0xFFFF_FFFE);
    mmio::w32(RCC + RCC_CIR, 0);

    // Step 5: CR = (CR & 0xFEF2FFFF) | 0x10001  (HSI on, preserve HSERDY bit)
    mmio::rmw32(RCC + RCC_CR, |v| (v & 0xFEF2_FFFF) | 0x0001_0001);

    // Step 6: wait for HSERDY (bit 17), bounded at 0x5000 spins
    let mut spin = 0u32;
    while mmio::r32(RCC + RCC_CR) & 0x0002_0000 == 0 {
        spin += 1;
        if spin >= 0x5000 {
            break;
        }
    }

    // Step 7: CFGR &= 0xF8C0B80C
    mmio::rmw32(RCC + RCC_CFGR, |v| v & 0xF8C0_B80C);

    // Step 8: if HSERDY was set in the CR value from step 5, proceed with PLL
    let cr_after_step5 = mmio::r32(RCC + RCC_CR);
    if cr_after_step5 & 0x0002_0000 != 0 {
        mmio::w32(FLASH + FLASH_ACR, 0x11);

        // wait for PLLRDY (bit 25) - unbounded
        while mmio::r32(RCC + RCC_CR) & 0x0200_0000 == 0 {
            core::hint::spin_loop();
        }

        // loop: CR = (CR & 0xFEF2FFFF) | 0x1010001, CFGR = (CFGR & 0xF8C0380C) | 0x130002
        // until SWS reports PLL
        loop {
            mmio::rmw32(RCC + RCC_CR, |v| (v & 0xFEF2_FFFF) | 0x0101_0001);
            mmio::rmw32(RCC + RCC_CFGR, |v| (v & 0xF8C0_380C) | 0x0130_0002);
            if mmio::r32(RCC + RCC_CFGR) & 0x0000_000C == 0x0000_0008 {
                break;
            }
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
