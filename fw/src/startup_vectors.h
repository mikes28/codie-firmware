/* startup_vectors.h - exception vector table for the Codie robot firmware.
 *
 * The original firmware's vector table is 48 words at 0x08000000:
 *   SP, Reset, 15 system exceptions, 31 external IRQs (0..30).
 * The IRQs the original actually enables are 7, 9, 11, 13, 15, 17, 20, 22, 29;
 * all others point at the default handler, exactly like the dump.
 *
 * IRQ names follow the STM32G0x1/C0x1 numbering (the part is not fully pinned,
 * see MEMORY.md, but the vector count and the used IRQ numbers come from the
 * original binary).
 */
#ifndef STARTUP_VECTORS_H
#define STARTUP_VECTORS_H

#include <stdint.h>

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void WWDG_IRQHandler(void);
void PVD_IRQHandler(void);
void RTC_TAMP_IRQHandler(void);
void FLASH_IRQHandler(void);
void RCC_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void EXTI2_3_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
void UCPD1_2_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);
void DMA1_Ch4_7_IRQHandler(void);
void ADC1_COMP_IRQHandler(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM6_DAC_LPTIM1_IRQHandler(void);
void TIM7_LPTIM2_IRQHandler(void);
void TIM14_IRQHandler(void);
void TIM15_IRQHandler(void);
void TIM16_IRQHandler(void);
void TIM17_IRQHandler(void);
void I2C1_IRQHandler(void);
void I2C2_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_4_LPUART1_IRQHandler(void);
void CEC_IRQHandler(void);

typedef void (*vector_t)(void);

#ifndef CODIE_VECTORS_DECL_ONLY
__attribute__((section(".isr_vector"), used))
const vector_t g_vectors[] = {
    (vector_t)0x20004000,            /* initial SP = top of 16 KB RAM */
    Reset_Handler,                   /* reset                */
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    /* external interrupts 0..30 */
    WWDG_IRQHandler,                 /*  0 */
    PVD_IRQHandler,                  /*  1 */
    RTC_TAMP_IRQHandler,             /*  2 */
    FLASH_IRQHandler,                /*  3 */
    RCC_IRQHandler,                  /*  4 */
    EXTI0_1_IRQHandler,              /*  5 */
    EXTI2_3_IRQHandler,              /*  6 */
    EXTI4_15_IRQHandler,             /*  7  */
    UCPD1_2_IRQHandler,              /*  8 */
    DMA1_Channel1_IRQHandler,        /*  9  */
    DMA1_Channel2_3_IRQHandler,      /* 10 */
    DMA1_Ch4_7_IRQHandler,           /* 11 */
    ADC1_COMP_IRQHandler,            /* 12 */
    TIM1_BRK_UP_TRG_COM_IRQHandler,  /* 13 */
    TIM1_CC_IRQHandler,              /* 14 */
    TIM2_IRQHandler,                 /* 15 */
    TIM3_IRQHandler,                 /* 16 */
    TIM6_DAC_LPTIM1_IRQHandler,      /* 17 */
    TIM7_LPTIM2_IRQHandler,          /* 18 */
    TIM14_IRQHandler,                /* 19 */
    TIM15_IRQHandler,                /* 20 */
    TIM16_IRQHandler,                /* 21 */
    TIM17_IRQHandler,                /* 22 */
    I2C1_IRQHandler,                 /* 23 */
    I2C2_IRQHandler,                 /* 24 */
    SPI1_IRQHandler,                 /* 25 */
    SPI2_IRQHandler,                 /* 26 */
    USART1_IRQHandler,               /* 27 */
    USART2_IRQHandler,               /* 28 */
    USART3_4_LPUART1_IRQHandler,     /* 29 */
    CEC_IRQHandler,                  /* 30 */
    0,                               /* 31 (reserved on this part) */
};
#endif /* CODIE_VECTORS_DECL_ONLY */

#endif /* STARTUP_VECTORS_H */
