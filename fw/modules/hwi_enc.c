/* hwi_enc.c - hwiEnc: the two wheel encoders of the robot.
 *
 * Original handler : FUN_08008DFC (0x08008DFD), registered as "hwiEnc"
 * Original init    : FUN_08008C48 (hardware) + FUN_08008DBC (start)
 * Original count   : FUN_08008E00
 * Original ISRs    : FUN_08008BF8 (TIM1 update, IRQ 13)
 *                    FUN_08008C20 (TIM2 update, IRQ 15)
 *
 * Wire behaviour of the original:
 *   the entry of "hwiEnc" in the interface table of the original firmware
 *   is a stub (FUN_08008DFC is "mov r0,#0; bx lr"), so the module never
 *   answers an op - a request for hwiEnc produces no reply at all.  The
 *   same is true for the timer/encoder combination the original used
 *   internally.  The handler below reproduces the stub.
 *
 * Hardware (recovered from the original binary):
 *   TIM2 (0x40000000) channel 1 captures on GPIOA pin 0  (encoder 1)
 *   TIM1 (0x40012C00) channel 1 captures on GPIOA pin 8  (encoder 2)
 *   both in input capture mode, 16 bit, inverted (CC1P), the update
 *   interrupt of the timer counts the overflows of the 16 bit counter.
 *   GPIOA pin 15 switches the encoder supply on (low while the hardware
 *   is set up, high when the encoders run).
 */
#include "codie.h"
#include "hwi_a_hal.h"

#define ENC_TIMER_1     TIM2_BASE     /* encoder 1 */
#define ENC_TIMER_2     TIM1_BASE     /* encoder 2 */
#define ENC_IRQ_1       15u           /* TIM2  */
#define ENC_IRQ_2       13u           /* TIM1  */

/* 0/1 = not initialised, 2 = hardware set up, +4 = running */
static uint8_t s_state;

/* the high part of the 16 bit counters, kept by the update interrupts */
static uint32_t s_high_1;
static uint32_t s_high_2;

/* forward */
static void hwiEnc_handler(uint8_t op, const char *opname, const char *args);

static const iface_t s_iface = { "hwiEnc", hwiEnc_handler };

/* ------------------------------------------------------------------ *
 * hardware
 * ------------------------------------------------------------------ */

/* from FUN_08008C48: the capture input of one encoder timer */
static void enc_timer_cfg(uint32_t tim)
{
    timer_base_t base;
    uint16_t ccmr1, ccer;

    base.psc     = 0u;
    base.cr1_hi  = 0u;
    base.arr     = 0xFFFFu;       /* the counter is 16 bit wide */
    base.cr1_ckd = 0u;
    base.rcr     = 0u;
    timer_base_cfg(tim, &base);
    timer_prescaler_egr(tim, 0u, 1u);

    /* the capture configuration of the original:
     *   CCMR1 = (CCMR1 & 0xFF0C) | 0x0021   (CC1S = 01, capture on TI1)
     *   CCER  = (CCER  & ~0x0001) | 0x000B  (CC1E, CC1P, CC1OF)
     *   CR2   = (CR2   & 0xFFF8) | 0x0007
     *   SMCR  = (SMCR  & 0xFF8F) | 0x0050
     *   DIER  |= 0x0001                       (update interrupt) */
    ccmr1 = timer_get16(tim, 0x18);
    timer_set16(tim, 0x18, (uint16_t)((ccmr1 & 0xFF0Cu) | 0x0021u));

    ccer = timer_get16(tim, 0x20);
    timer_set16(tim, 0x20, (uint16_t)((ccer & (uint16_t)~0x0001u) | 0x000Bu));

    timer_set16(tim, 0x04, (uint16_t)((timer_get16(tim, 0x04) & 0xFFF8u) | 0x0007u));
    timer_set16(tim, 0x08, (uint16_t)((timer_get16(tim, 0x08) & 0xFF8Fu) | 0x0050u));
    timer_dier(tim, 0x0001u, 1);
}

/* from FUN_08008C48: the setup of both encoders */
static void enc_hw_init(void)
{
    gpio_cfg_t gpio;

    rcc_apb2_enable(0x00000800u, 1);      /* FUN_0800AD7C */
    rcc_apb1_enable(0x00000001u, 1);      /* FUN_0800AD9C */
    rcc_ahb_enable(0x00020000u, 1);       /* FUN_0800AD5C */

    /* GPIOA pin 0 (TIM2_CH1) and pin 8 (TIM1_CH1) as capture inputs */
    gpio.pins   = 0x0101u;
    gpio.mode   = 2u;      /* alternate function */
    gpio.ospeed = 3u;
    gpio.otype  = 0u;
    gpio.pupd   = 0u;
    gpio_config(GPIOA_BASE, &gpio);
    gpio_set_afr(GPIOA_BASE, 0u, 2u);
    gpio_set_afr(GPIOA_BASE, 8u, 2u);

    /* GPIOA pin 15: the enable line of the encoder supply */
    gpio.pins   = 0x8000u;
    gpio.mode   = 1u;      /* output */
    gpio.ospeed = 0u;
    gpio.otype  = 0u;
    gpio.pupd   = 0u;
    gpio_config(GPIOA_BASE, &gpio);
    gpio_write(GPIOA_BASE, 0x8000u, 0);      /* the original leaves it low */

    nvic_enable(ENC_IRQ_2, 2u);          /* TIM1 update, FUN_0800AB30 */
    nvic_enable(ENC_IRQ_1, 2u);          /* TIM2 update, FUN_0800AB30 */

    enc_timer_cfg(ENC_TIMER_2);          /* TIM1 */
    enc_timer_cfg(ENC_TIMER_1);          /* TIM2 */
}

/* from FUN_08008DBC: switch the encoders on */
static void enc_hw_start(void)
{
    gpio_write(GPIOA_BASE, 0x8000u, 1);     /* the enable line high */
    timer_enable(ENC_TIMER_2, 1);       /* TIM1 */
    timer_enable(ENC_TIMER_1, 1);       /* TIM2 */
}

void hwiEnc_init(void)
{
    if (s_state < 2u) {
        enc_hw_init();
        s_state = 2u;
    }
    if ((s_state & 4u) == 0u) {
        enc_hw_start();
        s_state |= 4u;
    }
    iface_register(&s_iface);
}

/* ------------------------------------------------------------------ *
 * counting
 * ------------------------------------------------------------------ */

/* from FUN_08008E00 */
int32_t hwiEnc_count(uint8_t encoder)
{
    uint32_t cnt;

    if (encoder == 1u) {
        cnt = timer_get16(ENC_TIMER_1, 0x24) & 0xFFFFu;
        return (int32_t)(cnt + (s_high_1 << 16));
    }
    if (encoder == 2u) {
        cnt = timer_get16(ENC_TIMER_2, 0x24) & 0xFFFFu;
        return (int32_t)(cnt + (s_high_2 << 16));
    }
    if (encoder == 0x20u) {
        /* the original combines both encoders here */
        cnt = timer_get16(ENC_TIMER_1, 0x24) & 0xFFFFu;
        return (int32_t)(cnt + (s_high_1 << 16) + (s_high_2 << 16) +
                         ((timer_get16(ENC_TIMER_2, 0x24) & 0xFFFFu) >> 1));
    }
    return 0;
}

/* The original firmware has no way to reset an encoder, this is the
 * reconstruction of hwiEnc_reset: the 16 bit counter and the overflow
 * counter of the selected encoder are cleared. */
void hwiEnc_reset(uint8_t encoder)
{
    if (encoder == 1u) {
        timer_set16(ENC_TIMER_1, 0x24, 0u);
        s_high_1 = 0u;
    } else if (encoder == 2u) {
        timer_set16(ENC_TIMER_2, 0x24, 0u);
        s_high_2 = 0u;
    }
}

/* ------------------------------------------------------------------ *
 * the update interrupts of the two capture timers
 * ------------------------------------------------------------------ */

/* from FUN_08008BF8 (TIM1) and FUN_08008C20 (TIM2) */
static void enc_update_isr(uint32_t tim, uint32_t *high)
{
    uint16_t sr   = timer_get16(tim, 0x10);
    uint16_t dier = timer_get16(tim, 0x0C);

    if ((sr & 1u) != 0u && (dier & 1u) != 0u) {
        *high = *high + 1u;
        timer_sr_clear(tim, 1u);
    }
}

/* IRQ 13 - TIM1 update, the high part of encoder 2 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    enc_update_isr(ENC_TIMER_2, &s_high_2);
}

/* IRQ 15 - TIM2 update, the high part of encoder 1 */
void TIM2_IRQHandler(void)
{
    enc_update_isr(ENC_TIMER_1, &s_high_1);
}

/* ------------------------------------------------------------------ *
 * wire interface
 * ------------------------------------------------------------------ */

/* The original entry of "hwiEnc" in the interface table (0x08008DFD) is
 * the stub FUN_08008DFC ("mov r0,#0; bx lr"): it answers no op and sends
 * no reply.  Reproduced as it is. */
static void hwiEnc_handler(uint8_t op, const char *opname, const char *args)
{
    (void)op;
    (void)opname;
    (void)args;
}
