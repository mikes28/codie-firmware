/* hwi_led.c - LED driver (interface "hwiLed").
 *
 * Original firmware:
 *   - FUN_080090A8  builds the 12 x 24 LED pattern table (one byte per
 *                   LED column, 0x0e = off / 0x2e = on) out of a 36 bit
 *                   RGB bitmap.
 *   - FUN_08007B34  plays that table with TIM15 (0x40014000) + DMA1
 *                   channel 5 (0x40020058) and prints "PPRT\n" / "PPRET\n"
 *                   when a transfer completes.
 *   - FUN_08009268  second LED engine on TIM17 (0x40014800) + DMA1
 *                   channel 1 (0x40020008).
 *   - FUN_08008380/FUN_08008440  the buzzer engine on TIM16 (0x40014400).
 *   The command line handler of "hwiLed" in the original is a 4 byte
 *   stub (0x080090A4: "movs r0,#0; bx lr") - it accepts every request
 *   and does nothing.  The reconstruction exposes the LED engine as
 *   real operations.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

/* TIM15 is the LED PWM timer in the original (0x40014000) */
#define LED_TIMER   HWI_TIM15
/* the original uses a prescaler of 0x60 (96) for its LED timer */
#define LED_PSC     96u
/* 8 bit duty resolution */
#define LED_ARR     255u

/* LED pin assignment (alternate function AF2 = TIM15).
 * On the original board the LED engine is driven by DMA, so the pin
 * map could not be recovered from the binary; the mapping below is the
 * TIM15 pin assignment of the part family and is listed in the report
 * so that it can be corrected against the real board. */
#define LED_COUNT   4u
static const uint32_t led_port[LED_COUNT] = {
    HWI_GPIOB, HWI_GPIOB, HWI_GPIOA, HWI_GPIOA
};
static const uint8_t led_pin[LED_COUNT] = { 4u, 5u, 1u, 2u };

static uint8_t led_duty[LED_COUNT];
static uint8_t led_ready;

static const iface_t iface_hwiLed;   /* the interface record, defined below */

void hwiLed_init(void)
{
    uint8_t i;
    hwi_gpio_cfg_t cfg;

    if (led_ready) {
        return;
    }
    iface_register(&iface_hwiLed);
    /* RCC: GPIOA/GPIOB (AHBENR 0x20000/0x40000) and the LED timer
     * (APB2ENR 0x20000) - same bit values as the original. */
    hwi_rcc_ahb_en(0x60000u, 1);
    hwi_rcc_apb2_en(0x20000u, 1);

    hwi_tim_pwm_init(LED_TIMER, (uint16_t)LED_PSC, (uint16_t)LED_ARR, 1u);
    for (i = 0u; i < LED_COUNT; i++) {
        cfg.mask  = 1u << led_pin[i];
        cfg.mode  = HWI_MODE_ALT;
        cfg.speed = 0u;
        cfg.otype = 0u;
        cfg.pull  = HWI_PULL_NONE;
        hwi_gpio_config(led_port[i], &cfg);
        hwi_gpio_set_af(led_port[i], led_pin[i], 2u);   /* AF2 = TIM15 */
        if (i > 0u) {
            hwi_tim_pwm_init(LED_TIMER, (uint16_t)LED_PSC, (uint16_t)LED_ARR,
                             (uint8_t)(i + 1u));
        }
        led_duty[i] = 0u;
    }
    /* BDTR.MOE (advanced timer output enable) - FUN_0800AFBC */
    *(volatile uint16_t *)(LED_TIMER + 0x44u) =
        (uint16_t)(*(volatile uint16_t *)(LED_TIMER + 0x44u) | 0x8000u);
    /* ARPE (auto reload preload) - FUN_0800AF74 */
    *(volatile uint16_t *)(LED_TIMER + 0x00u) =
        (uint16_t)(*(volatile uint16_t *)(LED_TIMER + 0x00u) | 0x0080u);
    hwi_tim_start(LED_TIMER);            /* FUN_0800AFA0 */
    led_ready = 1u;
}

/* from the original's LED on/off handling (FUN_08008514 style: the
 * compare register of the channel carries the duty). */
int hwiLed_set(uint8_t led, int on)
{
    if (!led_ready) {
        hwiLed_init();
    }
    if (led >= LED_COUNT) {
        return -1;
    }
    led_duty[led] = on ? (uint8_t)LED_ARR : 0u;
    hwi_tim_pwm_duty(LED_TIMER, (uint8_t)(led + 1u), led_duty[led]);
    return 0;
}

/* from the original's LED pattern engine (FUN_080090A8 / FUN_08007B34):
 * the duty is written into the channel compare register. */
int hwiLed_set_pwm(uint8_t led, uint8_t duty)
{
    if (!led_ready) {
        hwiLed_init();
    }
    if (led >= LED_COUNT) {
        return -1;
    }
    /* duty is a uint8_t, LED_ARR is 255 - the value is always in range */
    led_duty[led] = duty;
    hwi_tim_pwm_duty(LED_TIMER, (uint8_t)(led + 1u), duty);
    return 0;
}

/* The original's LED engine (FUN_08007B34) prints "PPRT\n" when a pattern
 * transfer is requested and "PPRET\n" when the transfer has ended.  The
 * reconstruction drives the LEDs with plain PWM instead of a DMA transfer,
 * so the two states are reported from the pattern sequencer. */
void hwiLed_pattern_request(void)
{
    out_str("PPRT\n");
}

void hwiLed_pattern_done(void)
{
    out_str("PPRET\n");
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiLed_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];
    int  v[4];
    int  n;
    int  led;

    if (opname == 0) {
        return;
    }
    n = hwi_parse_args(args, v, 4);

    if (strcmp(opname, "on") == 0) {
        led = (n > 0) ? v[0] : 0;
        if (led < 0) {
            led = 0;
        }
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val),
                    (led < (int)LED_COUNT) ? (int32_t)led_duty[led] : 0);
            resp_value(op, "hwiLed", opname, val);
        } else if (op == OP_SET) {
            int on = (n > 1) ? hwi_is_on(args + 1) : 1;
            hwiLed_set((uint8_t)led, on);
            fmt_int(val, (int)sizeof(val), on);
            resp_value(op, "hwiLed", opname, val);
        }
        return;
    }

    if (strcmp(opname, "pwm") == 0) {
        led = (n > 0) ? v[0] : 0;
        if (led < 0) {
            led = 0;
        }
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val),
                    (led < (int)LED_COUNT) ? (int32_t)led_duty[led] : 0);
            resp_value(op, "hwiLed", opname, val);
        } else if (op == OP_SET) {
            int duty = (n > 1) ? v[1] : 0;
            if (duty < 0) {
                duty = 0;
            }
            if (duty > (int)LED_ARR) {
                duty = (int)LED_ARR;
            }
            hwiLed_set_pwm((uint8_t)led, (uint8_t)duty);
            fmt_int(val, (int)sizeof(val), duty);
            resp_value(op, "hwiLed", opname, val);
        }
        return;
    }

    if (op == OP_GET) {
        /* the original's hwiLed handler answers nothing; the
         * reconstruction reports the state of all LEDs. */
        int i;
        int off = 0;
        for (i = 0; i < (int)LED_COUNT; i++) {
            if (led_duty[i] == 0u) {
                off++;
            }
        }
        fmt_int(val, (int)sizeof(val), (int32_t)off);
        resp_value(op, "hwiLed", opname, val);
    }
}

static const iface_t iface_hwiLed = { "hwiLed", hwiLed_handler };
