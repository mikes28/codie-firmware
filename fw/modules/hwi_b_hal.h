/* hwi_b_hal.h - low level helpers shared by the "agent B" hardware modules
 * (hwi_led.c, hwi_buzz.c, hwi_mic.c, hwi_line.c, hwi_power.c,
 *  hwi_lightsense.c, bleCom.c).
 *
 * Every helper in here is a re-implementation of one of the original
 * firmware's low level routines (the original names are given in the
 * comment above each declaration).
 *
 * Register addresses are the ones observed in the original flash image
 * (MEMORY.md section 2).  They are NOT taken from a datasheet.
 */
#ifndef HWI_B_HAL_H
#define HWI_B_HAL_H

#include <stdint.h>

/* ------------------------------------------------------------------ *
 * peripheral bases (ground truth from the original binary)
 * ------------------------------------------------------------------ */
#define HWI_GPIOA   0x48000000u
#define HWI_GPIOB   0x48000400u
#define HWI_GPIOC   0x48000800u
#define HWI_GPIOF   0x48001400u

#define HWI_TIM1    0x40012C00u
#define HWI_TIM3    0x40000400u
#define HWI_TIM6    0x40001000u
#define HWI_TIM15   0x40014000u
#define HWI_TIM16   0x40014400u
#define HWI_TIM17   0x40014800u

#define HWI_DMA1    0x40020000u
#define HWI_DMA1_CH1 0x40020008u
#define HWI_DMA1_CH2 0x4002001Cu
#define HWI_DMA1_CH3 0x40020030u
#define HWI_DMA1_CH4 0x40020044u
#define HWI_DMA1_CH5 0x40020058u
#define HWI_DMA1_CH6 0x4002006Cu
#define HWI_DMA1_CH7 0x40020080u

#define HWI_USART3  0x40004800u   /* BLE module link (TX + RX)           */
#define HWI_USART4  0x40004C00u   /* console / command link (RX)         */
#define HWI_ADC1    0x40012400u

#define HWI_RCC     0x40021000u
#define HWI_SYSCFG  0x40010000u
#define HWI_EXTI    0x40010400u
#define HWI_IWDG    0x40003000u
#define HWI_FLASH   0x40022000u
#define HWI_SYSTICK 0xE000E010u

/* ------------------------------------------------------------------ *
 * GPIO
 * ------------------------------------------------------------------ */
/* mode values used by the original's FUN_0800A964 */
#define HWI_MODE_INPUT   0u
#define HWI_MODE_OUTPUT  1u
#define HWI_MODE_ALT     2u
#define HWI_MODE_ANALOG  3u

/* pull codes (PUPDR) */
#define HWI_PULL_NONE  0u
#define HWI_PULL_UP    1u
#define HWI_PULL_DOWN  2u

typedef struct {
    uint32_t mask;   /* bit n selects pin n                             */
    uint8_t  mode;   /* HWI_MODE_*                                     */
    uint8_t  speed;  /* OSPEEDR field, 0..3                            */
    uint8_t  otype;  /* 0 = push-pull, 1 = open-drain                  */
    uint8_t  pull;   /* HWI_PULL_*                                     */
} hwi_gpio_cfg_t;

void hwi_gpio_config(uint32_t port, const hwi_gpio_cfg_t *cfg); /* FUN_0800A964 */
void hwi_gpio_set_af(uint32_t port, uint8_t pin, uint8_t af);  /* FUN_0800AAC8 */
int  hwi_gpio_read(uint32_t port, uint16_t mask);              /* FUN_0800AABC */
void hwi_gpio_set(uint32_t port, uint16_t mask);               /* BSRR set    */
void hwi_gpio_reset(uint32_t port, uint16_t mask);             /* BRR  reset  */

/* ------------------------------------------------------------------ *
 * clock enables (FUN_0800AD5C / FUN_0800AD7C / FUN_0800AD9C)
 *   ahb  -> RCC AHBENR  (0x14)
 *   apb2 -> RCC APB2ENR (0x18)
 *   apb1 -> RCC APB1ENR (0x1C)
 * NOTE: in the original image the GPIO/enable bits are shifted by one
 *       compared to the published STM32 maps (0x20000 = GPIOA,
 *       0x40000 = GPIOB, 0x80000 = GPIOC, APB2 0x20000 = TIM15/16/17,
 *       APB1 0x40000 = USART3, 0x80000 = UART4).  The values used by
 *       the original are reproduced verbatim.
 * ------------------------------------------------------------------ */
void hwi_rcc_ahb_en(uint32_t mask, int enable);
void hwi_rcc_apb2_en(uint32_t mask, int enable);
void hwi_rcc_apb1_en(uint32_t mask, int enable);

/* ------------------------------------------------------------------ *
 * timers (FUN_0800AEC4 / FUN_0800AFDC / FUN_0800AF74 / FUN_0800AFA0 /
 *         FUN_0800AFBC / FUN_0800B244 / FUN_0800B1EC)
 * ------------------------------------------------------------------ */
void hwi_tim_pwm_init(uint32_t tim, uint16_t psc, uint16_t arr, uint8_t channel);
void hwi_tim_pwm_duty(uint32_t tim, uint8_t channel, uint32_t ccr);
uint32_t hwi_tim_ccr(uint32_t tim, uint8_t channel);
void hwi_tim_start(uint32_t tim);      /* CR1.CEN  */
void hwi_tim_stop(uint32_t tim);       /* CR1.CEN  */
void hwi_tim_set_arr(uint32_t tim, uint32_t arr);

/* ------------------------------------------------------------------ *
 * USART (FUN_0800B3FC / FUN_0800B53C / FUN_0800B54C / FUN_0800B4DC /
 *         FUN_0800B4E4 / FUN_0800B548 / FUN_0800B4EC / FUN_0800B4C0)
 * ------------------------------------------------------------------ */
void hwi_usart_init(uint32_t usart, uint32_t baud, uint8_t stop_bits);
int  hwi_usart_tx_ready(uint32_t usart);   /* ISR.TXFNF (bit 7) */
void hwi_usart_putc(uint32_t usart, uint8_t c);
int  hwi_usart_rx_ready(uint32_t usart);   /* ISR.RXNE  (bit 5) */
uint8_t hwi_usart_getc(uint32_t usart);
uint32_t hwi_usart_status(uint32_t usart);
void hwi_usart_clear(uint32_t usart, uint32_t mask); /* ICR */
void hwi_usart_enable(uint32_t usart);
void hwi_usart_disable(uint32_t usart);

/* ------------------------------------------------------------------ *
 * ring buffer (FUN_08002E1C / FUN_08002E44 / FUN_08002E78 / FUN_08002F38)
 * ------------------------------------------------------------------ */
typedef struct {
    uint8_t *buf;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} hwi_ring_t;

void hwi_ring_init(hwi_ring_t *r, uint8_t *buf, uint16_t size);
int  hwi_ring_put(hwi_ring_t *r, uint8_t b);
int  hwi_ring_get(hwi_ring_t *r, uint8_t *b);
uint16_t hwi_ring_read(hwi_ring_t *r, uint8_t *dst, uint16_t max);
uint16_t hwi_ring_count(const hwi_ring_t *r);
void hwi_ring_flush(hwi_ring_t *r);

/* ------------------------------------------------------------------ *
 * misc
 * ------------------------------------------------------------------ */
void hwi_delay_ms(uint32_t ms);      /* busy wait, uses g_tick_ms        */
uint32_t hwi_millis(void);           /* g_tick_ms                        */
void hwi_iwdg_kick(void);            /* FUN_0800AB10, IWDG_KR = 0xAAAA   */
int  hwi_is_on(const char *s);       /* FUN_0800A084: "on" / "1" / "true" */
int  hwi_atoi(const char *s);        /* small decimal parser             */
int  hwi_parse_args(const char *args, int *vals, int max);

/* Peripheral clock used for the USART baud divisor.  The original
 * computes it from the RCC registers at run time (FUN_0800AC40 inside
 * FUN_0800B3FC); the reconstruction uses the configured value below.
 * APB1 = HCLK / 2 in the original (its buzzer uses SystemCoreClock/2). */
#ifndef HWI_APB1_HZ
#define HWI_APB1_HZ 96000000u   /* the PLL of system_stm32g0.c: 96 MHz */
#endif
#ifndef HWI_TIMCLK_HZ
#define HWI_TIMCLK_HZ (HWI_APB1_HZ * 2u) /* APB prescaler 1 -> timer clock = 2 x APB */
#endif

#endif /* HWI_B_HAL_H */
