//! The vector table and the reset handler
//! (C: `fw/src/startup_stm32g071.c` + `fw/src/startup_handlers.c` +
//! `fw/src/startup_vectors.h`).

// --- the vector type -------------------------------------------------------
//
// On Cortex-M the first word of the vector table is the initial stack pointer
// (a data value, not a function pointer) and all other words are handler
// addresses.  A `union` mirrors the C `vector_t` (which is a function pointer
// type) that is cast to the SP value for the first entry.  Using a union
// avoids the const-evaluator's validity check on the integer->fn transmute.

/// One entry of the vector table: either the initial stack pointer or a
/// handler function pointer.
#[repr(C)]
pub union Vector {
    /// The initial stack pointer (first entry only).
    pub sp: usize,
    /// An exception or interrupt handler.
    pub handler: unsafe extern "C" fn(),
}

/// The initial stack pointer: top of the 16 KB RAM (0x20004000).
const SP_ADDR: usize = 0x2000_4000usize;

/// The initial stack pointer of the vector table: the top of the 16 KB RAM.
pub const INITIAL_SP: usize = SP_ADDR;

// The linker symbols of the C runtime.
//
// Only the target build has them - the host build of the tests has no
// `.data` to copy and no `.bss` to clear.
#[cfg(target_os = "none")]
extern "C" {
    static _sidata: u32;
    static _sdata: u32;
    static _edata: u32;
    static _sbss: u32;
    static _ebss: u32;
}

/// `Reset_Handler` - set up the C runtime and call `main()`.
#[no_mangle]
pub unsafe extern "C" fn Reset_Handler() {
    // Copy .data from flash (_sidata -> _sdata.._edata) and zero .bss
    // (_sbss.._ebss), exactly like the C startup.
    #[cfg(target_os = "none")]
    unsafe {
        let mut src = &_sidata as *const u32;
        let mut dst = &_sdata as *const u32 as *mut u32;
        let edata = &_edata as *const u32 as *mut u32;
        while dst < edata {
            *dst = *src;
            src = src.add(1);
            dst = dst.add(1);
        }

        let mut dst = &_sbss as *const u32 as *mut u32;
        let ebss = &_ebss as *const u32 as *mut u32;
        while dst < ebss {
            *dst = 0;
            dst = dst.add(1);
        }

        crate::system::system_init();
    }

    crate::boot::run();
}

/// The default handler of every interrupt the firmware does not use.
///
/// The original's `Default_Handler` (at 0x08002954) is an endless loop; every
/// unused vector points at it.
#[no_mangle]
pub unsafe extern "C" fn Default_Handler() {
    loop {
        core::hint::spin_loop();
    }
}

/// The vector table of the original firmware (`g_vectors` in
/// `startup_vectors.h`).
///
/// 48 words: the initial stack pointer, the reset vector, the 15 system
/// exceptions and the 31 external IRQs (0..30) plus one reserved word.
#[link_section = ".vector_table"]
#[used]
pub static VECTORS: [Vector; 48] = [
    Vector { sp: SP_ADDR },                          //  0: initial SP
    Vector { handler: Reset_Handler },               //  1: reset
    // --- system exceptions ---
    Vector { handler: Default_Handler },             //  2: NMI
    Vector { handler: Default_Handler },             //  3: HardFault
    Vector { handler: Default_Handler },             //  4: MemManage (reserved on M0+)
    Vector { handler: Default_Handler },             //  5: BusFault  (reserved on M0+)
    Vector { handler: Default_Handler },             //  6: UsageFault (reserved on M0+)
    Vector { sp: 0 },                                //  7: reserved
    Vector { sp: 0 },                                //  8: reserved
    Vector { sp: 0 },                                //  9: reserved
    Vector { sp: 0 },                                // 10: reserved
    Vector { handler: Default_Handler },             // 11: SVC
    Vector { handler: Default_Handler },             // 12: DebugMon
    Vector { sp: 0 },                                // 13: reserved
    Vector { handler: Default_Handler },             // 14: PendSV
    Vector { handler: crate::system::SysTick_Handler }, // 15: SysTick
    // --- external interrupts (IRQ 0..30) + reserved ---
    Vector { handler: Default_Handler },             // 16: WWDG            (IRQ  0)
    Vector { handler: Default_Handler },             // 17: PVD             (IRQ  1)
    Vector { handler: Default_Handler },             // 18: RTC_TAMP        (IRQ  2)
    Vector { handler: Default_Handler },             // 19: FLASH           (IRQ  3)
    Vector { handler: Default_Handler },             // 20: RCC             (IRQ  4)
    Vector { handler: Default_Handler },             // 21: EXTI0_1         (IRQ  5)
    Vector { handler: Default_Handler },             // 22: EXTI2_3         (IRQ  6)
    Vector { handler: Default_Handler },             // 23: EXTI4_15        (IRQ  7)
    Vector { handler: Default_Handler },             // 24: UCPD1_2         (IRQ  8)
    Vector { handler: crate::modules::hwi_sonar::DMA1_Channel1_IRQHandler }, // 25: DMA1_Channel1  (IRQ 9)
    Vector { handler: Default_Handler },             // 26: DMA1_Channel2_3 (IRQ 10)
    Vector { handler: Default_Handler },             // 27: DMA1_Ch4_7      (IRQ 11)
    Vector { handler: Default_Handler },             // 28: ADC1_COMP       (IRQ 12)
    Vector { handler: crate::modules::hwi_enc::TIM1_BRK_UP_TRG_COM_IRQHandler }, // 29: TIM1_BRK_UP_TRG_COM (IRQ 13)
    Vector { handler: Default_Handler },             // 30: TIM1_CC         (IRQ 14)
    Vector { handler: crate::modules::hwi_enc::TIM2_IRQHandler }, // 31: TIM2       (IRQ 15)
    Vector { handler: Default_Handler },             // 32: TIM3            (IRQ 16)
    Vector { handler: Default_Handler },             // 33: TIM6_DAC_LPTIM1 (IRQ 17)
    Vector { handler: Default_Handler },             // 34: TIM7_LPTIM2     (IRQ 18)
    Vector { handler: Default_Handler },             // 35: TIM14           (IRQ 19)
    Vector { handler: Default_Handler },             // 36: TIM15           (IRQ 20)
    Vector { handler: Default_Handler },             // 37: TIM16           (IRQ 21)
    Vector { handler: crate::modules::hwi_sonar::TIM17_IRQHandler }, // 38: TIM17   (IRQ 22)
    Vector { handler: Default_Handler },             // 39: I2C1            (IRQ 23)
    Vector { handler: Default_Handler },             // 40: I2C2            (IRQ 24)
    Vector { handler: Default_Handler },             // 41: SPI1            (IRQ 25)
    Vector { handler: Default_Handler },             // 42: SPI2            (IRQ 26)
    Vector { handler: Default_Handler },             // 43: USART1          (IRQ 27)
    Vector { handler: Default_Handler },             // 44: USART2          (IRQ 28)
    Vector { handler: Default_Handler },             // 45: USART3_4_LPUART1 (IRQ 29)
    Vector { handler: Default_Handler },             // 46: CEC             (IRQ 30)
    Vector { sp: 0 },                                // 47: reserved (IRQ 31)
];