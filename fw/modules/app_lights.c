/* app_lights.c -- the "appLights" interface of the original firmware.
 *
 * Original handler: 0x08005F01 (0x08005F00) - in the original it is a stub
 * ("movs r0,#0; bx lr"), it answers no ops.
 *
 * The behaviour of the original module (0x08005E00..):
 *   - a table of light programs (0x0800C414, 11 entries), each program is a
 *     sequence of steps (FUN_08003860, FUN_080039D8, ...),
 *   - the current program and the current step are kept in the state block
 *     at 0x20000358,
 *   - FUN_08005FBC selects a program (called with 1 at boot, from
 *     FUN_08006338),
 *   - FUN_08006000 (called from FUN_080063EC, the periodic part of main's
 *     loop) runs one step of the program and writes the 11 LED values
 *     (DAT_20000385...) to the LED array with FUN_080090A8.
 *
 * The rebuild keeps that structure: a program table, a step counter and a
 * tick that runs one step per main loop iteration.
 */
#include "codie.h"

#define APP_LIGHTS_LEDS   11
#define APP_LIGHTS_MAXPROG 4

/* a step is one byte: the bits 0..3 select the LED, the bits 4..7 give the
 * brightness (0..15 -> duty 0..255).  0xFF switches all LEDs off. */
static const uint8_t s_prog_off[] = {
    0xFF
};
static const uint8_t s_prog_chase[] = {
    0x01, 0x11, 0x21, 0x31, 0x41, 0x51,
    0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1,
    0xFF
};
static const uint8_t s_prog_breathe[] = {
    0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A,
    0x5A, 0x4A, 0x3A, 0x2A, 0x1A,
    0xFF
};
static const uint8_t s_prog_sparkle[] = {
    0x02, 0x05, 0x08, 0x0B, 0x08, 0x05, 0x02,
    0xFF
};

typedef struct {
    const uint8_t *steps;
    uint8_t       nsteps;
} app_light_prog_t;

static const app_light_prog_t s_progs[APP_LIGHTS_MAXPROG] = {
    { s_prog_off,      (uint8_t)(sizeof(s_prog_off)      / sizeof(uint8_t)) },
    { s_prog_chase,    (uint8_t)(sizeof(s_prog_chase)    / sizeof(uint8_t)) },
    { s_prog_breathe,  (uint8_t)(sizeof(s_prog_breathe)  / sizeof(uint8_t)) },
    { s_prog_sparkle,  (uint8_t)(sizeof(s_prog_sparkle)  / sizeof(uint8_t)) }
};

static uint8_t s_prog;        /* the selected program   */
static uint8_t s_step;        /* the current step       */
static uint8_t s_running;

static void appLights_handler(uint8_t op, const char *opname, const char *args)
{
    (void)op;
    (void)opname;
    (void)args;
    /* the original's handler is a stub */
}

/* from FUN_08006000 - run one step of the selected program */
void appLights_tick(void)
{
    const uint8_t *step;
    uint8_t led;
    uint8_t duty;
    int i;

    if ((s_prog >= APP_LIGHTS_MAXPROG) || (s_running == 0u)) {
        return;
    }
    step = s_progs[s_prog].steps;
    if (s_step >= s_progs[s_prog].nsteps) {
        s_running = 0u;
        return;
    }

    duty = 0u;
    led = 0xFFu;
    if (step[s_step] != 0xFFu) {
        led = (uint8_t)(step[s_step] & 0x0Fu);
        duty = (uint8_t)((step[s_step] >> 4) * 17u);
    }

    for (i = 0; i < APP_LIGHTS_LEDS; i++) {
        hwiLed_set_pwm((uint8_t)i, (uint8_t)((uint8_t)i == led ? duty : 0u));
    }

    if (s_step == 0u) {
        hwiLed_pattern_request();   /* "PPRT\n" - pattern transfer starts */
    }
    s_step++;
    if (s_step >= s_progs[s_prog].nsteps) {
        s_step = 0u;            /* the program repeats */
        hwiLed_pattern_done();   /* "PPRET\n" - the transfer has ended */
    }
}

static const iface_t appLights_iface = { "appLights", appLights_handler };

void appLights_init(void)
{
    s_prog = 1;          /* the original starts the program 1 (FUN_08005FBC(1)) */
    s_step = 0;
    s_running = 1u;
    iface_register(&appLights_iface);
}
