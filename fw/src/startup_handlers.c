/* startup_handlers.c - weak default handlers for the external IRQs plus the
 * minimal crt glue the linker needs.
 *
 * Every IRQ the original firmware does not use points at its default handler
 * (an endless loop), exactly like the dump.  A module that needs a real
 * handler simply defines a strong symbol with the same name - it overrides
 * the weak alias below.
 */
#define CODIE_VECTORS_DECL_ONLY
#include "startup_vectors.h"

/* strong definition of the default handler: every unused vector spins here,
 * exactly like the original firmware's Default_Handler at 0x08002954 */
void Default_Handler(void)
{
    for (;;) { }
}

#define IRQ_ALIAS(name) void name(void) __attribute__((weak, alias("Default_Handler")))

IRQ_ALIAS(WWDG_IRQHandler);
IRQ_ALIAS(PVD_IRQHandler);
IRQ_ALIAS(RTC_TAMP_IRQHandler);
IRQ_ALIAS(FLASH_IRQHandler);
IRQ_ALIAS(RCC_IRQHandler);
IRQ_ALIAS(EXTI0_1_IRQHandler);
IRQ_ALIAS(EXTI2_3_IRQHandler);
IRQ_ALIAS(EXTI4_15_IRQHandler);
IRQ_ALIAS(UCPD1_2_IRQHandler);
IRQ_ALIAS(DMA1_Channel1_IRQHandler);
IRQ_ALIAS(DMA1_Channel2_3_IRQHandler);
IRQ_ALIAS(DMA1_Ch4_7_IRQHandler);
IRQ_ALIAS(ADC1_COMP_IRQHandler);
IRQ_ALIAS(TIM1_BRK_UP_TRG_COM_IRQHandler);
IRQ_ALIAS(TIM1_CC_IRQHandler);
IRQ_ALIAS(TIM2_IRQHandler);
IRQ_ALIAS(TIM3_IRQHandler);
IRQ_ALIAS(TIM6_DAC_LPTIM1_IRQHandler);
IRQ_ALIAS(TIM7_LPTIM2_IRQHandler);
IRQ_ALIAS(TIM14_IRQHandler);
IRQ_ALIAS(TIM15_IRQHandler);
IRQ_ALIAS(TIM16_IRQHandler);
IRQ_ALIAS(TIM17_IRQHandler);
IRQ_ALIAS(I2C1_IRQHandler);
IRQ_ALIAS(I2C2_IRQHandler);
IRQ_ALIAS(SPI1_IRQHandler);
IRQ_ALIAS(SPI2_IRQHandler);
IRQ_ALIAS(USART1_IRQHandler);
IRQ_ALIAS(USART2_IRQHandler);
IRQ_ALIAS(USART3_4_LPUART1_IRQHandler);
IRQ_ALIAS(CEC_IRQHandler);

/* crt glue (newlib's __libc_init_array references _init/_fini) */
void _init(void) { }
void _fini(void) { }
