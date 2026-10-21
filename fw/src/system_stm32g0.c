/* system_stm32g0.c -- the clock tree and the millisecond tick.
 *
 * SystemInit() is the original's FUN_08002958 (called from the startup code
 * at 0x08002904 before main()).
 *
 * Recovered behaviour of FUN_08002958 (register writes, in order):
 *   RCC->CR    |= 0x00000001                (HSI on)
 *   RCC->CFGR &= 0xF8FFB80C
 *   RCC->CR    &= 0xFEF6FFFF
 *   RCC->CR    &= 0xFFFBFFFF
 *   RCC->CFGR &= 0xFFC0FFFF
 *   RCC[0x2C]  &= 0xFFFFFFF0                (HSI divider / reset flags)
 *   RCC[0x30]  &= 0xFFFFFEAC                (CSR: reset flags)
 *   RCC[0x34]  &= 0xFFFFFFFE                (CCIPR)
 *   RCC->CIR   = 0
 *   RCC->CR    |= 0x00010000                (PLL on)
 *   wait up to 0x5000 times for RCC->CR & 0x00020000 (PLL ready)
 *   if (RCC->CR & 0x00010000) {
 *       FLASH->ACR = 0x11                    (latency 1 + prefetch)
 *       RCC->CFGR &= 0xFFC07FFF
 *       RCC->CFGR |= 0x00130000
 *       RCC->CR   |= 0x01000000
 *       wait for RCC->CR & 0x02000000
 *       RCC->CFGR = (RCC->CFGR & ~0x3) | 0x2 (SYSCLK <- PLL)
 *       wait until (RCC->CFGR & 0xC) == 0x8 (SWS = PLL)
 *   }
 *
 * The resulting clock tree (decoded with the original's own clock helper
 * FUN_0800AC40):
 *   source        : PLL, PLLSRC (CFGR bits 16..17) = 0b11, base 16 MHz
 *                   (16 000 000 / (HSI divider + 1))
 *   PLLMUL       : (CFGR bits 18..21) + 2 = 4 + 2 = 6  ->  16 MHz * 6
 *   SYSCLK       : 96 MHz
 *   bus dividers  : HPRE = 0 (/1), PPRE1 = 0 (/1), PPRE2 = 0 (/1)
 *                   (the reset values are kept, the original never touches
 *                    them)  ->  AHB = APB1 = APB2 = 96 MHz
 *   flash        : ACR = 0x11 (1 wait state, prefetch enabled)
 *   millisecond tick: SysTick, 96 MHz / 96000 = 1 kHz, reload 95999
 */
#include "codie.h"

#define REG32(a)      (*(volatile uint32_t *)(uintptr_t)(a))

#define SYSC_RCC_BASE   0x40021000u
#define SYSC_FLASH_BASE 0x40022000u
#define RCC_CR        REG32(SYSC_RCC_BASE + 0x00u)
#define RCC_CFGR      REG32(SYSC_RCC_BASE + 0x04u)
#define RCC_CIR       REG32(SYSC_RCC_BASE + 0x08u)
#define RCC_AUX0      REG32(SYSC_RCC_BASE + 0x2Cu)   /* HSIDIV / reset flags   */
#define RCC_CSR       REG32(SYSC_RCC_BASE + 0x30u)   /* reset flags            */
#define RCC_CCIPR     REG32(SYSC_RCC_BASE + 0x34u)   /* peripheral clock select*/

#define FLASH_ACR     REG32(SYSC_FLASH_BASE + 0x00u)

#define SYSTICK_CTRL  REG32(0xE000E010u)
#define SYSTICK_LOAD  REG32(0xE000E014u)
#define SYSTICK_VAL   REG32(0xE000E018u)

/* the millisecond counter of the firmware (the original keeps it in the
 * TIM6 interrupt, the rebuild uses SysTick) */
volatile uint32_t g_tick_ms;

void SystemInit(void)
{
    uint32_t spin;

    /* from FUN_08002958 */
    RCC_CR    |= 0x00000001u;
    RCC_CFGR  &= 0xF8FFB80Cu;
    RCC_CR    &= 0xFEF6FFFFu;
    RCC_CR    &= 0xFFFBFFFFu;
    RCC_CFGR  &= 0xFFC0FFFFu;
    RCC_AUX0  &= 0xFFFFFFF0u;
    RCC_CSR   &= 0xFFFFFEACu;
    RCC_CCIPR &= 0xFFFFFFFEu;
    RCC_CIR    = 0u;

    RCC_CR    |= 0x00010000u;                  /* PLL on */

    spin = 0u;
    while ((RCC_CR & 0x00020000u) == 0u) {
        if (++spin >= 0x5000u) {
            break;
        }
    }

    if ((RCC_CR & 0x00010000u) != 0u) {
        FLASH_ACR   = 0x11u;                   /* latency 1 + prefetch */
        RCC_CFGR   &= 0xFFC07FFFu;
        RCC_CFGR   |= 0x00130000u;
        RCC_CR     |= 0x01000000u;
        while ((RCC_CR & 0x02000000u) == 0u) {
            /* wait for the PLL/oscillator ready flag */
        }
        RCC_CFGR    = (RCC_CFGR & ~0x00000003u) | 0x00000002u;  /* SYSCLK = PLL */
        while ((RCC_CFGR & 0x0000000Cu) != 0x00000008u) {
            /* wait until SWS reports the PLL */
        }
    }
}

/* from main() (the original resets the SysTick registers at the start of
 * the "reset cause" path) */
void sys_tick_init(uint32_t reload)
{
    SYSTICK_CTRL = 0u;
    SYSTICK_LOAD = reload - 1u;
    SYSTICK_VAL  = 0u;
    SYSTICK_CTRL = 7u;      /* enable, tick interrupt, processor clock */
}

void SysTick_Handler(void)
{
    g_tick_ms++;
}
