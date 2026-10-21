/* hwi_lightsense.c - ambient light sensor (interface "hwiLightSense").
 *
 * Original firmware:
 *   - The light sensor is the analogue sensor group with the id 4.
 *   - FUN_08009340  the main loop tick of the module: it accumulates
 *     FUN_080071DC(4, 0, 0) (the average of the eight ADC samples of
 *     the group) and, once the sample counter wraps, computes the mean
 *     with FUN_08000128 (a plain division) into the variable at
 *     0x2000086E.
 *   - FUN_080093AC  the "hwiLightSense" command line handler: it stamps
 *     the time base (FUN_08003040) and sends the two byte light level
 *     over the application bus (FUN_08006848).
 *   The command line handler of the original does not answer with a
 *   value; the reconstruction reports the level.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

/* the original's light sensor group id is 4 */
#define LIGHT_SENSOR_ID  4u
/* the reconstruction reads it from ADC1 channel 3 */
#define LIGHT_ADC_CHANNEL 3u

/* number of samples the original averages (FUN_08009340 counts up to
 * the value in DAT_20000868) */
#define LIGHT_AVG_COUNT  8u

static uint32_t light_sum;
static uint16_t light_count;
static uint16_t light_level;
static uint8_t  light_ready;

static const iface_t iface_hwiLightSense;   /* the interface record, defined below */

void hwiLightSense_init(void)
{
    iface_register(&iface_hwiLightSense);
    /* The ADC is brought up by hwiAdc_init(); the original's light
     * sensor group is configured through the shared sensor table. */
    light_sum   = 0u;
    light_count = 0u;
    light_level = 0u;
    light_ready = 1u;
}

uint16_t hwiLightSense_read(uint8_t sensor)
{
    uint16_t sample;

    if (!light_ready) {
        hwiLightSense_init();
    }
    if (sensor != 0u) {
        return 0u;
    }
    sample = hwiAdc_read(LIGHT_ADC_CHANNEL);

    light_sum += sample;
    light_count++;
    if (light_count >= LIGHT_AVG_COUNT) {
        light_level = (uint16_t)(light_sum / light_count);
        light_sum   = 0u;
        light_count = 0u;
    }
    return light_level;
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiLightSense_handler(uint8_t op, const char *opname, const char *args)
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
            fmt_int(val, (int)sizeof(val),
                    (int32_t)hwiLightSense_read((uint8_t)sensor));
            resp_value(op, "hwiLightSense", opname, val);
        }
        return;
    }
    if (strcmp(opname, "level") == 0) {
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val), (int32_t)hwiLightSense_read(0u));
            resp_value(op, "hwiLightSense", opname, val);
        }
        return;
    }
    if (op == OP_GET) {
        fmt_int(val, (int)sizeof(val), (int32_t)hwiLightSense_read(0u));
        resp_value(op, "hwiLightSense", opname, val);
    }
}

static const iface_t iface_hwiLightSense = { "hwiLightSense", hwiLightSense_handler };
