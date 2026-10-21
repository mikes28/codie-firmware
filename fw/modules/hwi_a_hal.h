/* hwi_a_hal.h - low level register helpers shared by the four hardware
 * interface modules of this slice:
 *
 *     hwi_adc.c    (hwiAdc   - original handler FUN_080073DC @ 0x080073DD)
 *     hwi_drive.c  (hwiDrive - original handler FUN_08008B3C @ 0x08008B3D)
 *     hwi_enc.c   (hwiEnc   - original handler FUN_08008DFC @ 0x08008DFD)
 *     hwi_sonar.c  (hwiSonar - original handler FUN_08009F88 @ 0x08009F89)
 *
 * Every routine below is a re-implementation of one of the original
 * firmware's low level helpers; the original function is named in the
 * comment in front of it.  All peripheral addresses and register offsets
 * are the ones the ORIGINAL binary uses (MEMORY.md section 2), not the ones
 * a data sheet would suggest.
 *
 * No dynamic allocation, no libc, Cortex-M0+ (Thumb-1) only.
 */
#ifndef HWI_A_HAL_H
#define HWI_A_HAL_H

#include <stdint.h>

/* ------------------------------------------------------------------ *
 * peripheral base addresses (from the original binary)
 * ------------------------------------------------------------------ */
#define GPIOA_BASE    0x48000000u
#define GPIOB_BASE    0x48000400u
#define GPIOC_BASE    0x48000800u
#define GPIOF_BASE    0x48001400u

#define TIM1_BASE     0x40012C00u   /* advanced timer, wheel encoder 2   */
#define TIM2_BASE     0x40000000u   /* wheel encoder 1                   */
#define TIM3_BASE     0x40000400u   /* motor PWM (4 channels)            */
#define TIM6_BASE     0x40001000u   /* millisecond tick                 */
#define TIM15_BASE    0x40014000u
#define TIM16_BASE    0x40014400u
#define TIM17_BASE    0x40014800u   /* ultrasonic sensor                 */

#ifndef ADC1_BASE
#define ADC1_BASE 0x40012400u
#endif
#ifndef DMA1_BASE
#define DMA1_BASE 0x40020000u
#endif
#ifndef RCC_BASE
#define RCC_BASE 0x40021000u
#endif

/* DMA1 channel register blocks: base + 0x08 + (n-1) * 0x14 */
#define DMA1_CH1      (DMA1_BASE + 0x08u)
#define DMA1_CH2      (DMA1_BASE + 0x1Cu)
#define DMA1_CH3      (DMA1_BASE + 0x30u)
#define DMA1_CH4      (DMA1_BASE + 0x44u)
#define DMA1_CH5      (DMA1_BASE + 0x58u)
#define DMA1_CH6      (DMA1_BASE + 0x6Cu)
#define DMA1_CH7      (DMA1_BASE + 0x80u)

/* Cortex-M NVIC */
#define NVIC_ISER     0xE000E100u
#define NVIC_IPR      0xE000E400u

/* ------------------------------------------------------------------ *
 * GPIO
 * ------------------------------------------------------------------ */

/* Configuration record of the original FUN_0800A964.  The field order
 * matches the initialiser the other modules of the rebuild use; the
 * register effects of the original (which is what matters) are produced
 * by gpio_config() below. */
typedef struct {
    uint16_t pins;    /* pin bit mask                                     */
    uint8_t  mode;    /* GPIO_MODE_INPUT / _OUTPUT / _ALT / _ANALOG       */
    uint8_t  otype;   /* output type (the original shifts the whole byte  */
                       /* into the register, see the comment in            */
                       /* gpio_config())                                   */
    uint8_t  pupd;    /* 2 bit pull up/down                              */
    uint8_t  ospeed;  /* 2 bit output speed                              */
} gpio_cfg_t;

void gpio_config(uint32_t port, const gpio_cfg_t *cfg);   /* FUN_0800A964 */
void gpio_set_afr(uint32_t port, uint8_t pin, uint8_t afr); /* FUN_0800AAC8 */
int  gpio_read(uint32_t port, uint16_t mask);             /* FUN_0800AABC */
void gpio_write(uint32_t port, uint16_t mask, int level); /* BSRR / BRR   */

/* ------------------------------------------------------------------ *
 * timers
 * ------------------------------------------------------------------ */

/* Base configuration record of the original FUN_0800AEC4. */
typedef struct {
    uint16_t psc;      /* +0  prescaler                                  */
    uint16_t cr1_hi;   /* +2  CR1 bits 4..7 (CMS / DIR)                  */
    uint32_t arr;      /* +4  auto reload (the high half lands in RCR)   */
    uint16_t cr1_ckd;  /* +8  CR1 bits 8..9 (clock division)              */
    uint8_t  rcr;      /* +10 repetition counter (advanced timers only)  */
} timer_base_t;

/* Compare channel configuration record of the original
 * FUN_0800AFDC / FUN_0800B054 / FUN_0800B0F4 / FUN_0800B174. */
typedef struct {
    uint16_t ccmr;     /* +0  CCMRx byte that selects the output mode    */
    uint16_t ccer;     /* +2  CCER enable bit (CCxE)                     */
    uint16_t ccer2;    /* +4  advanced timers: extra CCER bits           */
    uint16_t ccer3;    /* +6                                             */
    uint32_t ccr;      /* +8  compare value (written as 32 bits!)        */
    uint16_t ccer4;    /* +12 advanced timers: extra CCER bits           */
    uint16_t ccer5;    /* +14 advanced timers: extra CCER bits           */
    uint16_t cr2;      /* +16 advanced timers: CR2 bits                   */
    uint16_t cr2b;     /* +18 advanced timers: CR2 bits                   */
} timer_chan_t;

void timer_base_cfg(uint32_t tim, const timer_base_t *cfg);   /* FUN_0800AEC4 */
void timer_prescaler_egr(uint32_t tim, uint16_t psc, uint16_t egr); /* FUN_0800af4c */
void timer_arpe(uint32_t tim, int on);                        /* FUN_0800AF74 */
void timer_ocpe(uint32_t tim, uint8_t ch, int on);             /* FUN_0800b1fc/b208/b220/b22c */
void timer_channel_cfg(uint32_t tim, uint8_t ch, const timer_chan_t *cfg); /* FUN_0800AFDC..B174 */
void timer_pwm_set(uint32_t tim, uint8_t ch, uint32_t ccr);    /* FUN_0800b1ec/b1f0/b1f4/b1f8 */
void timer_ccer(uint32_t tim, uint8_t ch, int on);             /* FUN_0800B244 */
void timer_enable(uint32_t tim, int on);                       /* FUN_0800AFA0 */
void timer_moe(uint32_t tim, int on);                          /* FUN_0800AFBC */
void timer_dier(uint32_t tim, uint16_t mask, int on);          /* FUN_0800B38C / FUN_0800B3C8 */
void timer_sr_clear(uint32_t tim, uint16_t mask);             /* FUN_0800b3c0 */
void timer_egr(uint32_t tim, uint16_t val);                   /* FUN_0800b3a4 */
uint16_t timer_get16(uint32_t tim, uint32_t off);
void     timer_set16(uint32_t tim, uint32_t off, uint16_t v);
uint32_t timer_get32(uint32_t tim, uint32_t off);
void     timer_set32(uint32_t tim, uint32_t off, uint32_t v);

/* ------------------------------------------------------------------ *
 * ADC1
 * ------------------------------------------------------------------ */
void adc_cfg1(uint32_t adc, uint32_t clear_mask, uint32_t set_mask,
              uint8_t exten);                    /* FUN_0800A6A0 */
void adc_channel_add(uint32_t adc, uint8_t mask, uint32_t smpr); /* FUN_0800a6f8 */
void adc_dmngt(uint32_t adc, uint32_t v);       /* FUN_0800A77C */
void adc_dma_mode(uint32_t adc, int on);         /* FUN_0800A760 */
void adc_enable(uint32_t adc, int on);           /* FUN_0800A6DC */
void adc_stop(uint32_t adc);                     /* FUN_0800a754 */
uint32_t adc_start(uint32_t adc);                /* FUN_0800A704 */
int  adc_ready(uint32_t adc, uint32_t mask);     /* FUN_0800A78C */

/* ------------------------------------------------------------------ *
 * DMA1
 * ------------------------------------------------------------------ */
void dma_channel_cfg(uint32_t ch, uint32_t cpar, uint32_t cmar,
                     uint16_t cntdr, uint32_t ccr); /* FUN_0800A840 */
void dma_channel_enable(uint32_t ch, int on);       /* FUN_0800A87C */
void dma_channel_reset(uint32_t ch);                /* FUN_0800A7B0 */
void dma_set_cntdr(uint32_t ch, uint16_t n);        /* FUN_0800a89c */
void dma_set_dir(uint32_t ch, uint32_t mask);       /* FUN_0800A8A0 */
int  dma_flag(uint32_t mask);                       /* FUN_0800a8b8 */
void dma_clear_flags(uint32_t mask);                /* FUN_0800a8c8 */

/* ------------------------------------------------------------------ *
 * RCC clock enables and NVIC
 * ------------------------------------------------------------------ */
void rcc_ahb_enable(uint32_t mask, int on);   /* FUN_0800AD5C */
void rcc_apb2_enable(uint32_t mask, int on);  /* FUN_0800AD7C */
void rcc_apb1_enable(uint32_t mask, int on);  /* FUN_0800AD9C */
void nvic_enable(uint8_t irq, uint8_t prio);  /* FUN_0800AB30 */

#endif /* HWI_A_HAL_H */
