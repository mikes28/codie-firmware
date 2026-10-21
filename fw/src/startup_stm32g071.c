/* Reset / startup code for STM32G071 (Cortex-M0+).
 * Mirrors the original vendor startup: set SP, copy .data, zero .bss,
 * SystemInit(), __libc_init_array(), main(). */
#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

extern int main(void);
extern void SystemInit(void);
extern void __libc_init_array(void);

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata; )
        *dst++ = *src++;
    for (uint32_t *dst = &_sbss; dst < &_ebss; )
        *dst++ = 0;

    SystemInit();
    __libc_init_array();
    main();
    for (;;) { }
}

/* weak: startup_handlers.c provides the strong definition used by the IRQ
 * aliases (GCC requires an alias target to live in the same translation unit) */
__attribute__((weak)) void Default_Handler(void)
{
    for (;;) { }
}

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

#include "startup_vectors.h"
