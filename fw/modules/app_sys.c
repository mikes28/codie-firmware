/* app_sys.c -- the "appSys" interface of the original firmware.
 *
 * Original handler: 0x080064F1 (the entry in the interface table at
 * 0x0800D164) - in the original this handler is only a stub ("movs r0,#0;
 * bx lr"), the version block of the firmware is published in the greeting
 * (the key/value array in .data at 0x20000A14, the copy of the string table
 * at 0x0800C65C).
 *
 * The rebuild answers the version block as console ops as well, using the
 * very same strings that the original sends in the greeting:
 *   get ver   -> "0.5.0hw2.1"
 *   get mat   -> "4"
 *   get date  -> "1602121419"
 *   get ttpms -> "1000"
 *   call hwInit -> the hardware init (the original implements the same call
 *                   op in drvBattery, FUN_08006F2C)
 */
#include "codie.h"
#include <string.h>

/* the version block of the original (0x0800C65C .. 0x0800C698) */
#define APP_SYS_VER   "0.5.0hw2.1"
#define APP_SYS_MAT   "4"
#define APP_SYS_DATE  "1602121419"
#define APP_SYS_TTPMS "1000"

static void appSys_handler(uint8_t op, const char *opname, const char *args)
{
    (void)args;

    if (op == OP_GET) {
        if (strcmp(opname, "ver") == 0) {
            resp_value(OP_GET, "appSys", opname, APP_SYS_VER);
        } else if (strcmp(opname, "mat") == 0) {
            resp_value(OP_GET, "appSys", opname, APP_SYS_MAT);
        } else if (strcmp(opname, "date") == 0) {
            resp_value(OP_GET, "appSys", opname, APP_SYS_DATE);
        } else if (strcmp(opname, "ttpms") == 0) {
            resp_value(OP_GET, "appSys", opname, APP_SYS_TTPMS);
        }
        return;
    }

    if (op == OP_CALL) {
        if (strcmp(opname, "hwInit") == 0) {
            hw_init();
        }
    }
}

/* the application level "hwInit" (codie.h): the original asks the battery
 * pack for its values here (FUN_08006E94) before the app starts. */
void hw_init(void)
{
    drvBattery_hwInit();
}

static const iface_t appSys_iface = { "appSys", appSys_handler };

void appSys_init(void)
{
    iface_register(&appSys_iface);
}
