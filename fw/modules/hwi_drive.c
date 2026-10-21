/* hwi_drive.c - hwiDrive: the two motor drivers of the robot.
 *
 * Original handler : FUN_08008B3C (0x08008B3D), registered as "hwiDrive"
 * Original init    : FUN_080087D8 (hardware) + FUN_08008910 (start)
 * Original duty    : FUN_080089D0 (motor A) / FUN_08008A50 (motor B)
 * Original fault   : FUN_08008AD0 (fault flags), FUN_08008BD4 (max duty)
 * Original input   : GPIOC pin 14 = the fault input of the driver
 *
 * Wire behaviour of the original (reproduced verbatim):
 *
 *   "set hwiDrive dutyA <n>"  ->  the duty of motor A is set.  The
 *       argument is parsed as a HEXADECIMAL number (FUN_080069D4 with the
 *       base 16), the result is truncated to 16 bit and then clamped to
 *       +-1000.  No reply is sent.
 *   "set hwiDrive dutyB <n>"  ->  the same for motor B.
 *   "get hwiDrive nFault"     ->  the answer is always the constant "0".
 *       The original reads the fault input (GPIOC pin 14) but throws the
 *       result away and answers with the literal string "0".  The reply is
 *       sent with the op code 2 ("set"), not with "get" - that is what the
 *       original does.
 *   every other op            ->  ignored, no reply.
 *
 * Hardware (recovered from the original binary):
 *   TIM3 (0x40000400) in centre aligned mode, PSC = 0, ARR = 1000,
 *   four PWM channels (PWM mode 2) on GPIOC pins 6, 7, 8 and 9
 *   (alternate function 0):  CH1 = motor A forward, CH2 = motor A reverse,
 *   CH3 = motor B forward, CH4 = motor B reverse.
 *   GPIOC pin 2 = the enable line of the motor driver (high = enabled).
 *   GPIOC pin 1 = an input with pull up.
 */
#include "codie.h"
#include "hwi_a_hal.h"
#include <string.h>

#define DRIVE_MAX_DUTY   1000        /* the ARR of TIM3                    */
#define DRIVE_MAX_PERCENT 100u       /* the maximum duty in percent       */

/* 0/1 = not initialised, 2 = hardware set up, +4 = running */
static uint8_t s_state;

/* the maximum duty in percent (0..100) per motor, the original keeps
 * these two bytes in its .data section */
static uint8_t s_max_duty_b;
static uint8_t s_max_duty_a;

/* the fault flags of the original: 0xff = fault, 1 = ok */
static uint8_t s_fault_a;
static uint8_t s_fault_b;

/* the last duty that was commanded (the original does not keep it, it is
 * a reconstruction of hwiDrive_get_duty) */
static int32_t s_duty_a;
static int32_t s_duty_b;

/* forward */
static void hwiDrive_handler(uint8_t op, const char *opname, const char *args);

static const iface_t s_iface = { "hwiDrive", hwiDrive_handler };

/* ------------------------------------------------------------------ *
 * hardware
 * ------------------------------------------------------------------ */

/* from FUN_080087D8: the setup of the motor PWM */
static void drive_hw_init(void)
{
    gpio_cfg_t gpio;
    timer_base_t base;
    timer_chan_t chan;

    rcc_apb1_enable(0x00000002u, 1);         /* FUN_0800AD9C: TIM3 clock */

    /* GPIOC 6..9: the four PWM outputs, alternate function 0 */
    gpio.pins   = 0x03C0u;
    gpio.mode   = 2u;      /* alternate function */
    gpio.ospeed = 3u;
    gpio.otype  = 3u;      /* the original shifts the whole byte here ... */
    gpio.pupd   = 0u;
    gpio_config(GPIOC_BASE, &gpio);
    gpio_set_afr(GPIOC_BASE, 6u, 0u);
    gpio_set_afr(GPIOC_BASE, 7u, 0u);
    gpio_set_afr(GPIOC_BASE, 8u, 0u);
    gpio_set_afr(GPIOC_BASE, 9u, 0u);

    /* GPIOC 2: the enable line of the driver */
    gpio.pins   = 0x0004u;
    gpio.mode   = 1u;      /* output */
    gpio.ospeed = 0u;
    gpio.otype  = 0u;
    gpio.pupd   = 0u;
    gpio_config(GPIOC_BASE, &gpio);

    /* GPIOC 1: an input with pull up */
    gpio.pins   = 0x0002u;
    gpio.mode   = 0u;      /* input */
    gpio.ospeed = 1u;
    gpio.otype  = 0u;
    gpio.pupd   = 1u;
    gpio_config(GPIOC_BASE, &gpio);

    /* the compare preload enable of the four channels
     * (FUN_0800b1fc / b208 / b220 / b22c) */
    timer_ocpe(TIM3_BASE, 1u, 1);
    timer_ocpe(TIM3_BASE, 2u, 1);
    timer_ocpe(TIM3_BASE, 3u, 1);
    timer_ocpe(TIM3_BASE, 4u, 1);
    timer_arpe(TIM3_BASE, 0);               /* FUN_0800AF74 */

    /* the timer itself (FUN_0800AEC4): PSC = 0, ARR = 1000 and centre
     * aligned mode 1 (CR1 bits 4..7 = 0x20) */
    base.psc     = 0u;
    base.cr1_hi  = 0x0020u;
    base.arr     = DRIVE_MAX_DUTY;
    base.cr1_ckd = 0u;
    base.rcr     = 0u;
    timer_base_cfg(TIM3_BASE, &base);

    /* the four compare channels (FUN_0800AFDC / B054 / B0F4 / B174) */
    chan.ccmr  = 0x0060u;    /* PWM mode 2 */
    chan.ccer  = 0x0001u;    /* CCxE */
    chan.ccer2 = 0u;
    chan.ccer3 = 0u;
    chan.ccr   = 0u;
    chan.ccer4 = 0u;
    chan.ccer5 = 0x0008u;
    chan.cr2   = 0x0100u;
    chan.cr2b  = 0x0200u;

    timer_channel_cfg(TIM3_BASE, 1u, &chan);
    timer_channel_cfg(TIM3_BASE, 2u, &chan);
    timer_channel_cfg(TIM3_BASE, 3u, &chan);
    timer_channel_cfg(TIM3_BASE, 4u, &chan);
}

/* from FUN_08008910: enable the outputs and the driver */
static void drive_hw_start(void)
{
    gpio_write(GPIOC_BASE, 0x0004u, 1);         /* the enable line high */

    timer_ccer(TIM3_BASE, 1u, 1);            /* FUN_0800B244 */
    timer_ccer(TIM3_BASE, 2u, 1);
    timer_ccer(TIM3_BASE, 3u, 1);
    timer_ccer(TIM3_BASE, 4u, 1);

    timer_egr(TIM3_BASE, 1u);                /* FUN_0800b3a4: UG         */
    timer_enable(TIM3_BASE, 1);              /* FUN_0800AFA0: CR1.CEN     */
    timer_moe(TIM3_BASE, 1);                 /* FUN_0800AFBC: BDTR.MOE   */
}

/* from FUN_08008980: stop the motor (the enable line low) */
static void drive_hw_stop(void)
{
    timer_enable(TIM3_BASE, 0);
    timer_moe(TIM3_BASE, 0);
    gpio_write(GPIOC_BASE, 0x0004u, 0);
}

void hwiDrive_init(void)
{
    if (s_state < 2u) {
        drive_hw_init();
        s_state = 2u;
    }
    if ((s_state & 4u) == 0u) {
        drive_hw_start();
        s_state |= 4u;
    }
    iface_register(&s_iface);
}

/* ------------------------------------------------------------------ *
 * the motors
 * ------------------------------------------------------------------ */

/* from FUN_080089D0 (motor A) and FUN_08008A50 (motor B).
 *
 * motor 0 = A (TIM3 channel 1 forward, channel 2 reverse)
 * motor 1 = B (TIM3 channel 3 forward, channel 4 reverse)
 *
 * A duty of zero does not stop the motor, it drives both directions with
 * the maximum duty (a brake) - the original does exactly that. */
static void drive_set_duty(uint8_t motor, int32_t duty)
{
    int32_t  v = (int16_t)duty;      /* the original truncates to 16 bit */
    uint32_t fwd, rev, max;

    if (v > DRIVE_MAX_DUTY)
        v = DRIVE_MAX_DUTY;
    if (v < -DRIVE_MAX_DUTY)
        v = -DRIVE_MAX_DUTY;

    max = (motor == 0u) ? (uint32_t)s_max_duty_a : (uint32_t)s_max_duty_b;

    if (v > 0) {
        fwd = (uint32_t)v;
        rev = 0u;
    } else if (v < 0) {
        fwd = 0u;
        rev = (uint32_t)(-v);
    } else {
        fwd = max * 10u;
        rev = max * 10u;
    }

    if (motor == 0u) {
        timer_pwm_set(TIM3_BASE, 1u, fwd);   /* FUN_0800b1ec */
        timer_pwm_set(TIM3_BASE, 2u, rev);   /* FUN_0800b1f0 */
        s_duty_a = v;
    } else {
        timer_pwm_set(TIM3_BASE, 3u, fwd);   /* FUN_0800b1f4 */
        timer_pwm_set(TIM3_BASE, 4u, rev);   /* FUN_0800b1f8 */
        s_duty_b = v;
    }
}

int hwiDrive_set_duty(uint8_t motor, int32_t duty)
{
    if (motor > 1u)
        return -1;
    drive_set_duty(motor, duty);
    return 0;
}

int32_t hwiDrive_get_duty(uint8_t motor)
{
    return (motor == 0u) ? s_duty_a : s_duty_b;
}

int32_t hwiDrive_get_fault(uint8_t motor)
{
    return (motor == 0u) ? (int32_t)s_fault_a : (int32_t)s_fault_b;
}

/* from FUN_08008BD4: the maximum duty in percent (0..100) of a motor.
 * Not part of the shared contract, the application layer uses it to limit
 * the speed when the battery is low. */
void hwiDrive_set_max_duty(uint8_t motor, uint8_t percent)
{
    if (percent > DRIVE_MAX_PERCENT)
        percent = DRIVE_MAX_PERCENT;
    if (motor == 0u)
        s_max_duty_a = percent;
    else
        s_max_duty_b = percent;
}

/* from FUN_08008AD0: the original reaction to a measured motor current.
 * A positive value sets the fault flag of the motor, a negative one
 * clears it; the motor is driven with the measured value afterwards.
 * Not part of the shared contract. */
void hwiDrive_set_fault(uint8_t motor, int32_t value)
{
    uint8_t fault = 0xFFu;

    if ((int16_t)(-(int16_t)value) >= 1)
        fault = 1u;

    if (motor & 1u) {
        s_fault_b = fault;
        drive_set_duty(1u, value);
    }
    if (motor & 2u) {
        s_fault_a = fault;
        drive_set_duty(0u, value);
    }
}

/* ------------------------------------------------------------------ *
 * wire interface
 * ------------------------------------------------------------------ */

/* from FUN_080069D4 with the base 16: the original parses the argument of
 * the duty ops as a hexadecimal number.  Digits 0..9 have their usual
 * value, 'a'..'f' are 10..15, everything else is taken modulo 2^64 just
 * like the original (which uses the character value - 0x57). */
static int16_t parse_hex16(const char *s)
{
    uint64_t acc = 0;
    int      neg = 0;
    int32_t  d;

    if (s == 0)
        return 0;
    if (*s == '-') {
        neg = 1;
        s++;
    }
    if (*s == '\0')
        return 0;

    for (; *s != '\0'; s++) {
        uint8_t c = (uint8_t)*s;

        if (c < 0x3Au)
            d = (int32_t)c - 0x30;       /* '0'..'9' (and ':' .. '?') */
        else
            d = (int32_t)c - 0x57;       /* 'a'..'f'                  */
        acc = acc * 16u + (uint64_t)(int64_t)d;
    }

    if (neg)
        acc = (uint64_t)0 - acc;

    return (int16_t)(uint16_t)(uint32_t)acc;
}

/* from FUN_08008B3C */
static void hwiDrive_handler(uint8_t op, const char *opname, const char *args)
{
    if (op == OP_SET) {
        if (strcmp(opname, "dutyA") == 0) {
            hwiDrive_set_duty(0u, parse_hex16(args));
        } else if (strcmp(opname, "dutyB") == 0) {
            hwiDrive_set_duty(1u, parse_hex16(args));
        }
        return;
    }

    if (op == OP_GET && strcmp(opname, "nFault") == 0) {
        /* the original reads the fault input of the driver (GPIOC pin 14)
         * and then answers with the constant "0" - the result of the read
         * is dropped.  The reply carries the op code 2 ("set"). */
        (void)gpio_read(GPIOC_BASE, 0x4000u);
        resp_value(OP_SET, "hwiDrive", opname, "0");
    }
}
