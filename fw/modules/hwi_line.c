/* hwi_line.c - line follower sensor array (interface "hwiLine").
 *
 * Original firmware:
 *   - FUN_080093C8 / FUN_0800940C / FUN_0800942C  PA4 is an output that
 *     enables the sensor bar (RCC AHBENR 0x20000 = GPIOA).
 *   - FUN_080071DC(id, 0, 0) returns the average of the eight ADC
 *     samples of a sensor group.  The line sensors are the groups with
 *     the ids 0x20 and 0x40 (FUN_08009444 / FUN_08009484).
 *   - FUN_08009484  the "hwiLine" application bus entry: it sends the
 *     two 16 bit deltas (sample - calibration) to the application.
 *   The command line handler of the original (FUN_08009478) only stamps
 *   the time base and returns 0.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

/* the original's line sensor groups carry the ids 0x20 and 0x40 */
#define LINE_SENSOR_A_ID 0x20u
#define LINE_SENSOR_B_ID 0x40u
/* the reconstruction reads them from ADC1 channel 5 and channel 6 */
#define LINE_ADC_A       5u
#define LINE_ADC_B       6u

/* PA4 drives the sensor bar */
#define LINE_EN_PORT     HWI_GPIOA
#define LINE_EN_PIN      4u

/* number of sensors that are reported in the mask */
#define LINE_SENSOR_COUNT 2u

static uint16_t line_base_a;        /* calibration values  */
static uint16_t line_base_b;
static uint8_t  line_ready;

static const iface_t iface_hwiLine;  /* the interface record, defined below */

void hwiLine_init(void)
{
    iface_register(&iface_hwiLine);
    hwi_gpio_cfg_t cfg;

    if (line_ready) {
        return;
    }
    /* RCC AHBENR 0x20000 (GPIOA) */
    hwi_rcc_ahb_en(0x20000u, 1);

    cfg.mask  = 1u << LINE_EN_PIN;
    cfg.mode  = HWI_MODE_OUTPUT;
    cfg.speed = 0u;
    cfg.otype = 0u;
    cfg.pull  = HWI_PULL_NONE;
    hwi_gpio_config(LINE_EN_PORT, &cfg);
    hwi_gpio_set(LINE_EN_PORT, (uint16_t)(1u << LINE_EN_PIN));   /* sensor bar on */

    line_base_a = 0u;
    line_base_b = 0u;
    line_ready  = 1u;
}

uint16_t hwiLine_read(uint8_t sensor)
{
    uint16_t value;

    if (!line_ready) {
        hwiLine_init();
    }
    switch (sensor) {
    case 0u:
        value = hwiAdc_read(LINE_ADC_A);
        if (line_base_a == 0u) {
            line_base_a = value;
        }
        break;
    case 1u:
        value = hwiAdc_read(LINE_ADC_B);
        if (line_base_b == 0u) {
            line_base_b = value;
        }
        break;
    default:
        return 0u;
    }
    return value;
}

/* The original compares the two line sensors against their calibrated
 * values (FUN_08009444) and derives one line position from them.  The
 * reconstruction reports a bit per sensor in the mask. */
uint16_t hwiLine_mask(void)
{
    uint16_t mask = 0u;
    uint16_t a;
    uint16_t b;

    if (!line_ready) {
        hwiLine_init();
    }
    a = hwiLine_read(0u);
    b = hwiLine_read(1u);

    if (a > line_base_a) {
        mask |= 0x0001u;
    }
    if (b > line_base_b) {
        mask |= 0x0002u;
    }
    return mask;
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiLine_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];
    int  v[2];
    int  n;

    if (opname == 0) {
        return;
    }
    n = hwi_parse_args(args, v, 2);

    if (strcmp(opname, "read") == 0) {
        int sensor = (n > 0) ? v[0] : 0;
        if (sensor < 0) {
            sensor = 0;
        }
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val), (int32_t)hwiLine_read((uint8_t)sensor));
            resp_value(op, "hwiLine", opname, val);
        }
        return;
    }
    if (strcmp(opname, "mask") == 0) {
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val), (int32_t)hwiLine_mask());
            resp_value(op, "hwiLine", opname, val);
        }
        return;
    }
    if (op == OP_GET) {
        fmt_int(val, (int)sizeof(val), (int32_t)hwiLine_mask());
        resp_value(op, "hwiLine", opname, val);
    }
}

static const iface_t iface_hwiLine = { "hwiLine", hwiLine_handler };
