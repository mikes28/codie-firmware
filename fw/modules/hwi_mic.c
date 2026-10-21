/* hwi_mic.c - microphone level (interface "hwiMic").
 *
 * Original firmware:
 *   - The microphone is one of the four analogue "sensor groups" that
 *     the ADC module samples into the RAM block at 0x200004B6 (eight
 *     16 bit samples per group, four groups).
 *   - FUN_080071DC(id, 0, 0)  returns the average of the eight samples
 *     of a group;  FUN_080071DC(id, 1, prev) returns the sample that is
 *     closest to "prev" (a tracking filter).
 *   - FUN_080094F8  runs the tracking filter for the microphone group
 *     (group id 2) once per main loop and stores the result.
 *   - FUN_08009638  the "hwiMic" command line handler: it stamps the
 *     time base (FUN_08003040) and sends the two byte level over the
 *     application bus (FUN_08006848).
 *   The command line handler of the original does not answer with a
 *   value; the reconstruction reports the level.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

/* the original's microphone sensor group id is 2 */
#define MIC_SENSOR_ID   2u
/* the reconstruction reads the microphone from ADC1 channel 2 */
#define MIC_ADC_CHANNEL 2u

static uint16_t mic_level;
static uint8_t  mic_ready;

static const iface_t iface_hwiMic;   /* the interface record, defined below */

void hwiMic_init(void)
{
    iface_register(&iface_hwiMic);
    /* The ADC itself is brought up by hwiAdc_init() (module hwi_adc.c);
     * the original's microphone group is configured by the application
     * layer through the same sensor table.  Nothing else to do here. */
    mic_level  = 0u;
    mic_ready  = 1u;
}

uint16_t hwiMic_level(void)
{
    uint16_t sample;

    if (!mic_ready) {
        hwiMic_init();
    }
    sample = hwiAdc_read(MIC_ADC_CHANNEL);

    /* the original keeps the sample that is closest to the previous
     * value (FUN_080071DC with mode 1) - a small deviation is ignored
     * so that the level does not jump on a single noisy conversion. */
    if ((mic_level == 0u) ||
        (((uint32_t)sample - mic_level > 64u) &&
         ((uint32_t)mic_level - sample > 64u))) {
        mic_level = sample;
    } else {
        mic_level = (uint16_t)(((uint32_t)mic_level * 3u + sample) / 4u);
    }
    return mic_level;
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiMic_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];

    (void)args;
    if (opname == 0) {
        return;
    }
    if (strcmp(opname, "level") == 0) {
        if (op == OP_GET) {
            fmt_int(val, (int)sizeof(val), (int32_t)hwiMic_level());
            resp_value(op, "hwiMic", opname, val);
        }
        return;
    }
    if (op == OP_GET) {
        fmt_int(val, (int)sizeof(val), (int32_t)hwiMic_level());
        resp_value(op, "hwiMic", opname, val);
    }
}

static const iface_t iface_hwiMic = { "hwiMic", hwiMic_handler };
