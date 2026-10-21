/* app_drive.c -- the "appDrive" interface of the original firmware.
 *
 * Original handler: 0x08003849 (FUN_08003848) - it only stamps the shared
 * application time (FUN_08003040) and returns 0, it answers no ops.
 *
 * The real behaviour of the original lives in the periodic function
 * FUN_08003330, which is called from FUN_080063EC (the periodic part of
 * main's loop): it closes the speed loop of the two motors, the encoders
 * (FUN_08008E00) give the speed, the controller (FUN_08008AD0) sets the
 * motor duty.
 *
 * The rebuild keeps the same structure: appDrive_tick() runs the speed loop
 * once per main loop iteration.
 */
#include "codie.h"

#define APP_DRIVE_MOTORS 2

static int32_t s_target[APP_DRIVE_MOTORS];   /* target speed, counts/tick */
static int32_t s_speed[APP_DRIVE_MOTORS];    /* measured speed            */
static int32_t s_duty[APP_DRIVE_MOTORS];     /* last duty that was set    */
static int32_t s_last_count[APP_DRIVE_MOTORS];

static void appDrive_handler(uint8_t op, const char *opname, const char *args)
{
    (void)op;
    (void)opname;
    (void)args;
    /* the original's handler only refreshes the application timestamp */
}

/* from FUN_08003330 (speed loop of the two motors) */
void appDrive_tick(void)
{
    int i;

    for (i = 0; i < APP_DRIVE_MOTORS; i++) {
        int32_t count = hwiEnc_count((uint8_t)i);
        int32_t delta = count - s_last_count[i];

        s_last_count[i] = count;
        s_speed[i] = delta;

        if (s_target[i] != 0) {
            int32_t err = s_target[i] - delta;
            int32_t duty = s_duty[i] + (err / 4);

            if (duty > 100) {
                duty = 100;
            }
            if (duty < -100) {
                duty = -100;
            }
            s_duty[i] = duty;
            hwiDrive_set_duty((uint8_t)i, duty);
        } else if (s_duty[i] != 0) {
            s_duty[i] = 0;
            hwiDrive_set_duty((uint8_t)i, 0);
        }
    }
}

static const iface_t appDrive_iface = { "appDrive", appDrive_handler };

void appDrive_init(void)
{
    int i;

    for (i = 0; i < APP_DRIVE_MOTORS; i++) {
        s_target[i] = 0;
        s_speed[i] = 0;
        s_duty[i] = 0;
        s_last_count[i] = hwiEnc_count((uint8_t)i);
    }
    iface_register(&appDrive_iface);
}
