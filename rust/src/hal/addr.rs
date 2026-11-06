//! Peripheral base addresses and register offsets.
//!
//! Ground truth from the original flash image (`../MEMORY.md` section 2),
//! NOT from a data sheet.  Keep these addresses: they are what the board is
//! wired to.

#![allow(dead_code)]

// --- GPIO (bus at 0x48000000) --------------------------------------------
pub const GPIOA: u32 = 0x4800_0000;
pub const GPIOB: u32 = 0x4800_0400;
pub const GPIOC: u32 = 0x4800_0800;
pub const GPIOF: u32 = 0x4800_1400;

// GPIO register offsets
pub const GPIO_MODER: u32 = 0x00;
pub const GPIO_OTYPER: u32 = 0x04;
pub const GPIO_OSPEEDR: u32 = 0x08;
pub const GPIO_PUPDR: u32 = 0x0C;
pub const GPIO_IDR: u32 = 0x10;
pub const GPIO_ODR: u32 = 0x14;
pub const GPIO_BSRR: u32 = 0x18;
pub const GPIO_BRR: u32 = 0x1C;
pub const GPIO_AFR0: u32 = 0x20;
pub const GPIO_AFR1: u32 = 0x24;

// --- timers ---------------------------------------------------------------
pub const TIM1: u32 = 0x4001_2C00;
pub const TIM2: u32 = 0x4000_0000;
pub const TIM3: u32 = 0x4000_0400;
/// TIM6 - the millisecond tick timer of the original.
///
/// The base is **0x40001000**, NOT 0x40010000 (which is SYSCFG - the port had
/// the two confused).  Dump evidence: `FUN_0800A464` @0x800a464 loads its
/// base from the literal pool at 0x800a4b8, which holds 0x40001000, and then
/// writes the tick prescaler/ARR through it; the same base appears in the
/// IRQ-17 handler `FUN_0800A43C` @0x800a43c (`FUN_0800B3A8(&DAT_40001000,1)`).
pub const TIM6: u32 = 0x4000_1000;
pub const TIM15: u32 = 0x4001_4000;
pub const TIM16: u32 = 0x4001_4400;
pub const TIM17: u32 = 0x4001_4800;

// timer register offsets
pub const TIM_CR1: u32 = 0x00;
pub const TIM_CR2: u32 = 0x04;
pub const TIM_SMCR: u32 = 0x08;
pub const TIM_DIER: u32 = 0x0C;
pub const TIM_SR: u32 = 0x10;
pub const TIM_EGR: u32 = 0x14;
pub const TIM_CCMR1: u32 = 0x18;
pub const TIM_CCMR2: u32 = 0x1C;
pub const TIM_CCER: u32 = 0x20;
pub const TIM_CNT: u32 = 0x24;
pub const TIM_PSC: u32 = 0x28;
pub const TIM_ARR: u32 = 0x2C;
pub const TIM_RCR: u32 = 0x30;
pub const TIM_CCR1: u32 = 0x34;
pub const TIM_CCR2: u32 = 0x38;
pub const TIM_CCR3: u32 = 0x3C;
pub const TIM_CCR4: u32 = 0x40;
pub const TIM_BDTR: u32 = 0x44;

// --- ADC1 / DMA1 ----------------------------------------------------------
pub const ADC1: u32 = 0x4001_2400;
pub const DMA1: u32 = 0x4002_0000;

// DMA1 channel register blocks: base + 0x08 + (n - 1) * 0x14
pub const DMA1_CH1: u32 = DMA1 + 0x08;
pub const DMA1_CH2: u32 = DMA1 + 0x1C;
pub const DMA1_CH3: u32 = DMA1 + 0x30;
pub const DMA1_CH4: u32 = DMA1 + 0x44;
pub const DMA1_CH5: u32 = DMA1 + 0x58;
pub const DMA1_CH6: u32 = DMA1 + 0x6C;
pub const DMA1_CH7: u32 = DMA1 + 0x80;

// --- serial links ---------------------------------------------------------
/// AT / control channel to the BLE module (14400 bit/s).
pub const USART3: u32 = 0x4000_4800;
/// Command + reply link of the robot (460800 bit/s).
pub const USART4: u32 = 0x4000_4C00;

// USART register offsets (the G4 style layout the original uses)
pub const USART_CR1: u32 = 0x00;
pub const USART_CR2: u32 = 0x04;
pub const USART_CR3: u32 = 0x08;
pub const USART_BRR: u32 = 0x0C;
pub const USART_RQR: u32 = 0x18;
pub const USART_ISR: u32 = 0x1C;
pub const USART_ICR: u32 = 0x20;
pub const USART_RDR: u32 = 0x24;
pub const USART_TDR: u32 = 0x28;

// --- core peripherals -----------------------------------------------------
pub const RCC: u32 = 0x4002_1000;
pub const FLASH: u32 = 0x4002_2000;
pub const SYSCFG: u32 = 0x4001_0000;
pub const EXTI: u32 = 0x4001_0400;

// EXTI register offsets (the dump's power module writes 0x0C and 0x08).
/// Rising trigger selection (`EXTI_RTSR1`).
pub const EXTI_RTSR: u32 = 0x08;
/// Falling trigger selection (`EXTI_FTSR1`).
pub const EXTI_FTSR: u32 = 0x0C;
/// Pending flag (`EXTI_PR1`).
pub const EXTI_PR: u32 = 0x14;
pub const IWDG: u32 = 0x4000_3000;
pub const NVIC_ISER: u32 = 0xE000_E100;
pub const NVIC_IPR: u32 = 0xE000_E400;
pub const SYSTICK: u32 = 0xE000_E010;

// --- clock ----------------------------------------------------------------
/// Peripheral clock the USART baud divisor is computed from.
///
/// The rebuild reproduces the original's clock tree: HSI16 -> PLL (x6) =
/// 96 MHz, AHB = APB1 = APB2 = 96 MHz.  See `../MEMORY.md` section 4.4.
pub const APB1_HZ: u32 = 96_000_000;
/// Timer clock of the buzzer / tone generator.
///
/// The dump settles this: `FUN_0800849C` (0x0800849C) computes
/// `ARR = (SystemCoreClock >> 1) / freq - 1` - the halving is the
/// instruction at `0x80084a4` (`lsrs r0, r0, #1`), and the store of the
/// result into `ARR` is at `0x80084ae`.  `FUN_08008514` (0x8008518) then
/// reads that `ARR` back out of `+0x2c` for the duty maths.  So the tone
/// timer's clock is SystemCoreClock/2 = 48 MHz, NOT the 2x APB value.
pub const TIMCLK_HZ: u32 = APB1_HZ / 2;
