/* app_lightsense.c -- the "appLightSense" interface of the original
 * firmware.
 *
 * Original handler: 0x08003855 (FUN_08003854) - like the appDrive handler it
 * only stamps the shared application time (FUN_08003040) and returns 0, it
 * answers no ops.
 *
 * The module itself belongs to the application block around 0x08003000; the
 * ambient light sensors are read through the hwiLightSense module
 * (FUN_080093A9) and the result drives the brightness of the LED array.
 * The rebuild runs that in the periodic tick (the original calls the
 * application tick functions from FUN_080063EC in main's loop).
 */
#include "codie.h"

#define APP_LS_SENSORS 2
#define APP_LS_LEDS    11

static uint16_t s_level[APP_LS_SENSORS];
static uint8_t  s_brightness;

static void appLightSense_handler(uint8_t op, const char *opname, const char *args)
{
    (void)op;
    (void)opname;
    (void)args;
    /* the original's handler only refreshes the application timestamp */
}

/* from the application tick of the original: read the ambient light and
 * dim the LED array accordingly */
void appLightSense_tick(void)
{
    uint32_t sum = 0u;
    uint8_t brightness;
    int i;

    for (i = 0; i < APP_LS_SENSORS; i++) {
        s_level[i] = hwiLightSense_read((uint8_t)i);
        sum += s_level[i];
    }
    /* the average of the sensors, 0..255 */
    brightness = (uint8_t)((sum / (uint32_t)APP_LS_SENSORS) >> 2);
    s_brightness = brightness;

    for (i = 0; i < APP_LS_LEDS; i++) {
        hwiLed_set_pwm((uint8_t)i, brightness);
    }
}

static const iface_t appLightSense_iface = { "appLightSense", appLightSense_handler };

void appLightSense_init(void)
{
    int i;

    for (i = 0; i < APP_LS_SENSORS; i++) {
        s_level[i] = 0u;
    }
    s_brightness = 0u;
    iface_register(&appLightSense_iface);
}
