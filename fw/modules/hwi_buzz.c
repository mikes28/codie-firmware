/* hwi_buzz.c - buzzer (interface "hwiBuzz").
 *
 * Original firmware:
 *   - FUN_08008380 / FUN_08008440  init of the buzzer engine:
 *     TIM16 (0x40014400) in PWM mode, output on PB8 with alternate
 *     function AF2, RCC: AHBENR 0x40000 (GPIOB) + APB2ENR 0x20000.
 *   - FUN_0800849C  sets the period: ARR = (SystemCoreClock/2)/freq - 1
 *   - FUN_08008514  sets the duty:   CCR = ARR * duty / 100
 *   - FUN_080084C0  queues a tone (two frequencies + duty + duration)
 *   - FUN_08008574  the main loop tick that plays the queued tones
 *   - FUN_08008668  the application bus entry that queues a tone
 *   The command line handler of "hwiBuzz" in the original is
 *   FUN_08008490 which only stamps the time base and returns 0.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

#define BUZZ_TIMER   HWI_TIM16
#define BUZZ_PIN     8u
#define BUZZ_PORT    HWI_GPIOB
#define BUZZ_PSC     96u          /* the original uses 0x60          */
#define BUZZ_DUTY    50u          /* percent, default               */

static uint8_t buzz_ready;
static uint32_t buzz_period;      /* current ARR value              */

static const iface_t iface_hwiBuzz;  /* the interface record, defined below */

void hwiBuzz_init(void)
{
    hwi_gpio_cfg_t cfg;

    if (buzz_ready) {
        return;
    }
    iface_register(&iface_hwiBuzz);
    /* RCC: GPIOB (AHBENR 0x40000) and the timer (APB2ENR 0x20000) */
    hwi_rcc_ahb_en(0x40000u, 1);
    hwi_rcc_apb2_en(0x20000u, 1);

    cfg.mask  = 1u << BUZZ_PIN;
    cfg.mode  = HWI_MODE_ALT;
    cfg.speed = 0u;
    cfg.otype = 0u;
    cfg.pull  = HWI_PULL_NONE;
    hwi_gpio_config(BUZZ_PORT, &cfg);
    hwi_gpio_set_af(BUZZ_PORT, BUZZ_PIN, 2u);   /* AF2 = TIM16_CH1 */

    hwi_tim_pwm_init(BUZZ_TIMER, (uint16_t)BUZZ_PSC, 0u, 1u);
    buzz_period = 1000u;
    hwi_tim_set_arr(BUZZ_TIMER, buzz_period);
    /* BDTR.MOE - FUN_0800AFBC */
    *(volatile uint16_t *)(BUZZ_TIMER + 0x44u) =
        (uint16_t)(*(volatile uint16_t *)(BUZZ_TIMER + 0x44u) | 0x8000u);
    /* ARPE - FUN_0800AF74 */
    *(volatile uint16_t *)(BUZZ_TIMER + 0x00u) =
        (uint16_t)(*(volatile uint16_t *)(BUZZ_TIMER + 0x00u) | 0x0080u);
    hwi_tim_start(BUZZ_TIMER);            /* FUN_0800AFA0 */
    buzz_ready = 1u;
}

/* from FUN_0800849C - ARR = timerclock / freq - 1 */
static void buzz_set_frequency(uint32_t freq_hz)
{
    if (freq_hz == 0u) {
        return;
    }
    buzz_period = (HWI_TIMCLK_HZ / freq_hz);
    if (buzz_period == 0u) {
        buzz_period = 1u;
    }
    buzz_period -= 1u;
    hwi_tim_set_arr(BUZZ_TIMER, buzz_period);
}

/* from FUN_08008514 - CCR = ARR * duty / 100 */
static void buzz_set_duty(uint32_t duty_percent)
{
    uint32_t ccr;

    if (duty_percent > 100u) {
        duty_percent = 100u;
    }
    ccr = (buzz_period * duty_percent) / 100u;
    hwi_tim_pwm_duty(BUZZ_TIMER, 1u, ccr);
}

typedef struct {
    uint16_t freq;      /* Hz                        */
    uint16_t ms;        /* duration in milliseconds  */
} buzz_step_t;

/* The tone patterns of the original are queued as (freq, duration,
 * duty) triples by the application bus; the reconstruction offers a
 * small fixed set of patterns. */
static const buzz_step_t buzz_pattern_0[] = { { 2000u, 100u }, { 0u, 0u } };
static const buzz_step_t buzz_pattern_1[] = { { 2000u, 80u }, { 0u, 60u },
                                             { 2000u, 80u }, { 0u, 0u } };
static const buzz_step_t buzz_pattern_2[] = { { 1200u, 500u }, { 0u, 0u } };
static const buzz_step_t buzz_pattern_3[] = { { 2500u, 60u }, { 0u, 60u },
                                             { 2500u, 60u }, { 0u, 60u },
                                             { 2500u, 60u }, { 0u, 0u } };
static const buzz_step_t buzz_pattern_4[] = { { 1000u, 100u }, { 2000u, 100u },
                                             { 3000u, 100u }, { 0u, 0u } };

static const buzz_step_t *const buzz_patterns[] = {
    buzz_pattern_0, buzz_pattern_1, buzz_pattern_2,
    buzz_pattern_3, buzz_pattern_4
};
#define BUZZ_PATTERN_COUNT ((int)(sizeof(buzz_patterns) / sizeof(buzz_patterns[0])))

/* Play a tone pattern.  The original queues the tones and plays them
 * from the main loop tick (FUN_08008574); the reconstruction plays them
 * synchronously so that the module works without an extra tick hook. */
int hwiBuzz_play(uint8_t pattern)
{
    const buzz_step_t *steps;
    int i;

    if (!buzz_ready) {
        hwiBuzz_init();
    }
    if (pattern >= BUZZ_PATTERN_COUNT) {
        return -1;
    }
    steps = buzz_patterns[pattern];

    for (i = 0; steps[i].ms != 0u; i++) {
        if (steps[i].freq != 0u) {
            buzz_set_frequency(steps[i].freq);
            buzz_set_duty(BUZZ_DUTY);
        } else {
            buzz_set_duty(0u);        /* silence */
        }
        hwi_delay_ms(steps[i].ms);
    }
    buzz_set_duty(0u);
    return 0;
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiBuzz_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];
    int  v[2];
    int  n;

    if (opname == 0) {
        return;
    }
    n = hwi_parse_args(args, v, 2);

    if (strcmp(opname, "play") == 0) {
        int pattern = (n > 0) ? v[0] : 0;
        if (pattern < 0) {
            pattern = 0;
        }
        if (pattern >= BUZZ_PATTERN_COUNT) {
            pattern = 0;
        }
        hwiBuzz_play((uint8_t)pattern);
        fmt_int(val, (int)sizeof(val), (int32_t)pattern);
        resp_value(op, "hwiBuzz", opname, val);
        return;
    }
    if (strcmp(opname, "stop") == 0) {
        buzz_set_duty(0u);
        fmt_int(val, (int)sizeof(val), 0);
        resp_value(op, "hwiBuzz", opname, val);
        return;
    }
    if (op == OP_GET) {
        fmt_int(val, (int)sizeof(val), 0);
        resp_value(op, "hwiBuzz", opname, val);
    }
}

static const iface_t iface_hwiBuzz = { "hwiBuzz", hwiBuzz_handler };
