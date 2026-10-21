/* hwi_sonar.c - hwiSonar: the ultrasonic distance sensor of the robot.
 *
 * Original handler : FUN_08009F88 (0x08009F89), registered as "hwiSonar"
 * Original init    : FUN_08008F14 (hardware) + FUN_08009060 (start)
 * Original measure : FUN_08009268 (arm a measurement)
 *                    FUN_08008E9C (TIM17 update, IRQ 22)
 *                    FUN_08008EE8 (DMA1 channel 1, IRQ 9)
 * Original read    : FUN_08009094 (returns the "measurement running" flag)
 *
 * Wire behaviour of the original:
 *   the entry of "hwiSonar" in the interface table of the original firmware
 *   is a stub (FUN_08009F88 is "mov r0,#0; bx lr"), the module answers no
 *   op.  The op name "echo" exists in the string table of the original
 *   (0x0800C58C) but is not referenced by any code - the feature was
 *   never finished.  The handler below reproduces the stub.
 *
 * Hardware (recovered from the original binary):
 *   TIM17 (0x40014800) with the compare channel 1 on GPIOB pin 9
 *   (alternate function 2), PSC = 0, ARR = 59, PWM mode 2.
 *   DMA1 channel 1 (0x40020008) copies the captured value (TIM17_CCR1 at
 *   0x40014834) into RAM, the transfer count is set to 318 when a
 *   measurement is armed.
 */
#include "codie.h"
#include "hwi_a_hal.h"

/* the timer window of the original is 60 ticks (ARR = 59).  The original
 * firmware never turned the capture into a distance, so the length of one
 * tick is not recoverable from the binary - it depends on the clock tree
 * the system module sets up.  One tick is treated as one microsecond. */
#define SONAR_TICK_US          1

#define SONAR_CAPTURES      16u     /* the size of the capture buffer    */
#define SONAR_TRIGGER_CNT  318u     /* the transfer count of the original */

#define SONAR_IRQ_TIM      22u      /* TIM17                              */
#define SONAR_IRQ_DMA       9u      /* DMA1 channel 1                     */

/* 0/1 = not initialised, 2 = hardware set up, +4 = running */
static uint8_t s_state;

/* 1 while a measurement is running (the original keeps this flag in
 * _DAT_2000085C) */
static uint8_t s_busy;

/* the values the DMA copies out of the timer */
static volatile uint16_t s_capture[SONAR_CAPTURES];

/* forward */
static void hwiSonar_handler(uint8_t op, const char *opname, const char *args);

static const iface_t s_iface = { "hwiSonar", hwiSonar_handler };

/* ------------------------------------------------------------------ *
 * hardware
 * ------------------------------------------------------------------ */

/* from FUN_08008F14: the setup of the sensor */
static void sonar_hw_init(void)
{
    gpio_cfg_t gpio;
    timer_base_t base;
    timer_chan_t chan;

    rcc_ahb_enable(0x00040000u, 1);        /* FUN_0800AD5C */
    rcc_apb2_enable(0x00040000u, 1);       /* FUN_0800AD7C: TIM17 clock */

    /* GPIOB pin 9: the echo input of the sensor */
    gpio.pins   = 0x0200u;
    gpio.mode   = 2u;      /* alternate function */
    gpio.ospeed = 3u;
    gpio.otype  = 0u;
    gpio.pupd   = 0u;
    gpio_config(GPIOB_BASE, &gpio);
    gpio_set_afr(GPIOB_BASE, 9u, 2u);

    /* the timer (FUN_0800AEC4): PSC = 0, ARR = 59 */
    base.psc     = 0u;
    base.cr1_hi  = 0u;
    base.arr     = 0x003Bu;
    base.cr1_ckd = 0u;
    base.rcr     = 0u;
    timer_base_cfg(TIM17_BASE, &base);

    nvic_enable(SONAR_IRQ_TIM, 3u);       /* FUN_0800AB30: TIM17       */
    nvic_enable(SONAR_IRQ_DMA, 2u);       /* FUN_0800AB30: DMA1 ch1    */

    timer_ocpe(TIM17_BASE, 1u, 1);         /* FUN_0800b1fc */
    timer_arpe(TIM17_BASE, 0);            /* FUN_0800AF74 */

    /* the compare channel (FUN_0800AFDC) */
    chan.ccmr  = 0x0060u;    /* PWM mode 2 */
    chan.ccer  = 0x0001u;    /* CC1E */
    chan.ccer2 = 0u;
    chan.ccer3 = 0u;
    chan.ccr   = 0u;
    chan.ccer4 = 0u;
    chan.ccer5 = 0u;
    chan.cr2   = 0x0100u;
    chan.cr2b  = 0u;
    timer_channel_cfg(TIM17_BASE, 1u, &chan);

    rcc_ahb_enable(0x00000001u, 1);        /* FUN_0800AD5C: the DMA clock */

    /* DMA1 channel 1: the captured value of TIM17_CCR1 goes into RAM.
     * The configuration word is the one of the original. */
    dma_channel_reset(DMA1_CH1);          /* FUN_0800A7B0 */
    dma_channel_cfg(DMA1_CH1, TIM17_BASE + 0x34u,
                    (uint32_t)(uintptr_t)s_capture, 0u, 0x00002190u);
    timer_dier(TIM17_BASE, 0x0100u, 1);    /* FUN_0800B3C8: capture int */

    dma_set_dir(DMA1_CH1, 0x00000002u);    /* FUN_0800A8A0: periph -> mem */
}

/* from FUN_08009060: enable the capture output */
static void sonar_hw_start(void)
{
    timer_ccer(TIM17_BASE, 1u, 1);         /* FUN_0800B244 */
    timer_moe(TIM17_BASE, 1);             /* FUN_0800AFBC */
}

void hwiSonar_init(void)
{
    if (s_state < 2u) {
        sonar_hw_init();
        s_state = 2u;
    }
    if ((s_state & 4u) == 0u) {
        sonar_hw_start();
        s_state |= 4u;
    }
    iface_register(&s_iface);
}

/* ------------------------------------------------------------------ *
 * measuring
 * ------------------------------------------------------------------ */

/* from FUN_08009268: arm a measurement, returns 1 when it was started */
static int sonar_trigger(void)
{
    if (s_busy != 0u)
        return 0;

    timer_sr_clear(TIM17_BASE, 2u);        /* FUN_0800b3c0 */
    dma_set_cntdr(DMA1_CH1, SONAR_TRIGGER_CNT);  /* FUN_0800a89c */
    timer_pwm_set(TIM17_BASE, 1u, 0u);     /* FUN_0800b1ec: CCR1 = 0 */
    dma_channel_enable(DMA1_CH1, 1);       /* FUN_0800A87C */
    timer_set32(TIM17_BASE, 0x24, 0u);    /* the counter */
    timer_enable(TIM17_BASE, 1);          /* FUN_0800AFA0 */
    timer_egr(TIM17_BASE, 1u);            /* FUN_0800b3a4: UG */
    s_busy = 1u;
    return 1;
}

/* from FUN_08009094: the original returns the "measurement running" flag */
static int sonar_busy(void)
{
    return (int)s_busy;
}

int32_t hwiSonar_echo_us(void)
{
    uint16_t raw;

    if (s_busy == 0u) {
        /* no measurement running: start one and report "nothing yet" */
        (void)sonar_trigger();
        return 0;
    }

    raw = s_capture[0];
    return (int32_t)raw * (int32_t)SONAR_TICK_US;
}

/* ------------------------------------------------------------------ *
 * the interrupts of the sensor
 * ------------------------------------------------------------------ */

/* from FUN_08008E9C (TIM17 update, IRQ 22) */
void TIM17_IRQHandler(void)
{
    uint16_t sr   = timer_get16(TIM17_BASE, 0x10);
    uint16_t dier = timer_get16(TIM17_BASE, 0x0C);

    if ((sr & 1u) != 0u && (dier & 1u) != 0u) {
        timer_enable(TIM17_BASE, 0);       /* FUN_0800AFA0 */
        dma_channel_enable(DMA1_CH1, 0);   /* FUN_0800A87C */
        timer_dier(TIM17_BASE, 1u, 0);     /* FUN_0800B38C */
        s_busy = 0u;
        timer_sr_clear(TIM17_BASE, 1u);    /* FUN_0800b3c0 */
    }
}

/* from FUN_08008EE8 (DMA1 channel 1, IRQ 9) */
void DMA1_Channel1_IRQHandler(void)
{
    if (dma_flag(0x00000002u)) {          /* FUN_0800a8b8: TCIF1 */
        timer_sr_clear(TIM17_BASE, 1u);    /* FUN_0800b3c0 */
        timer_dier(TIM17_BASE, 1u, 1);     /* FUN_0800B38C */
        dma_clear_flags(0x00000002u);     /* FUN_0800a8c8 */
    }
}

/* ------------------------------------------------------------------ *
 * wire interface
 * ------------------------------------------------------------------ */

/* The original entry of "hwiSonar" in the interface table (0x08009F89) is
 * the stub FUN_08009F88 ("mov r0,#0; bx lr"): it answers no op and sends
 * no reply.  Reproduced as it is. */
static void hwiSonar_handler(uint8_t op, const char *opname, const char *args)
{
    (void)op;
    (void)opname;
    (void)args;
}
