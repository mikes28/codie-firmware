/* drv_battery.c -- the "drvBattery" interface of the original firmware.
 *
 * Original handler: 0x08006F2D (FUN_08006F2C in the decompilation).
 *
 * The original answers the ops
 *   get voltage           -> the pack voltage in mV
 *   get temp              -> the pack temperature in 1/10 degC
 *   get current           -> the pack current in mA
 *   get remainingCapacity -> the remaining capacity
 *   get chargeStat        -> the charge state read from a GPIO pin
 *   call hwInit           -> the battery pack initialisation
 *
 * The replies are built with FUN_080068D4 (unsigned) and FUN_0800693C
 * (signed), both with radix 16, so the numbers are printed in hexadecimal -
 * exactly like the original.  The reply word is the "set" word (op code 2):
 * the original hard codes 2 in every FUN_0800A16C call, so even a "get"
 * request is answered with "set <iface> <op> <value>".
 *
 * The measurement values themselves are produced by the battery pack (a BLE
 * BMS) in the original; the state block below mirrors the original's block
 * at 0x200004A0 (temp, voltage, current, remaining capacity).
 * The charge state is a real GPIO read (FUN_08007CFC): the charge status pin
 * of the pack is PF6 and is probed with a pull-up and afterwards with a
 * pull-down.
 */
#include "codie.h"
#include <string.h>

/* the charge status pin of the pack: PF6 on port F.
 * The register accesses are done here directly because the low level HAL
 * (hal.c) is not part of the shared contract. */
#define DRVB_REG32(a)  (*(volatile uint32_t *)(uintptr_t)(a))
#define DRVB_GPIO_F    0x48001400u
#define DRVB_PUPDR     (DRVB_GPIO_F + 0x0Cu)   /* pull up/down control */
#define DRVB_IDR       (DRVB_GPIO_F + 0x10u)   /* input data            */
#define DRVB_PF6       0x0040u

/* the original's battery state block (0x200004A0) */
static volatile int16_t  s_temp_c10;         /* +2 : temperature in 1/10 degC */
static volatile uint16_t s_voltage_mv;       /* +4 : pack voltage in mV     */
static volatile int16_t  s_current_ma;       /* +6 : pack current in mA     */
static volatile int16_t  s_remaining;        /* +8 : remaining capacity     */

/* the original waits with FUN_08002DD4 (delay in ms); the shared contract
 * has no delay helper, so the millisecond tick is used.  The spin count is
 * a fallback for the case that the tick does not run. */
static void drv_delay_ms(uint32_t ms)
{
    uint32_t start = g_tick_ms;
    uint32_t spins = 0u;

    while ((g_tick_ms - start) < ms) {
        if (++spins > 4000000u) {
            break;
        }
    }
}

/* from FUN_08007CFC - the charge state of the pack.
 * 0 = the pin is pulled low (charging), 1 = the pin is floating,
 * 2 = the pin is driven high. */
static int charge_state(void)
{
    volatile uint32_t *pupdr = &DRVB_REG32(DRVB_PUPDR);
    volatile uint32_t *idr   = &DRVB_REG32(DRVB_IDR);
    int state;

    *pupdr = (*pupdr & ~0x1000u) | 0x1000u;    /* PF6: pull-up */
    drv_delay_ms(10);
    if ((*idr & DRVB_PF6) != 0u) {
        state = 0;                              /* high with pull-up */
    } else {
        *pupdr = (*pupdr & ~0x1000u) | 0x2000u; /* PF6: pull-down */
        drv_delay_ms(10);
        state = ((*idr & DRVB_PF6) != 0u) ? 2 : 1;
    }
    *pupdr &= ~0x1000u;                        /* no pull */
    return state;
}

static void drvBattery_handler(uint8_t op, const char *opname, const char *args)
{
    char value[24];

    (void)args;

    if (op == OP_GET) {
        if (strcmp(opname, "voltage") == 0) {
            fmt_num(value, 16, (uint32_t)s_voltage_mv);
            resp_value(OP_SET, "drvBattery", opname, value);
        } else if (strcmp(opname, "temp") == 0) {
            fmt_num_s(value, 16, (int32_t)s_temp_c10);
            resp_value(OP_SET, "drvBattery", opname, value);
        } else if (strcmp(opname, "current") == 0) {
            fmt_num_s(value, 16, (int32_t)s_current_ma);
            resp_value(OP_SET, "drvBattery", opname, value);
        } else if (strcmp(opname, "remainingCapacity") == 0) {
            fmt_num_s(value, 16, (int32_t)s_remaining);
            resp_value(OP_SET, "drvBattery", opname, value);
        } else if (strcmp(opname, "chargeStat") == 0) {
            fmt_num(value, 16, (uint32_t)charge_state());
            resp_value(OP_SET, "drvBattery", opname, value);
        }
        return;
    }

    if (op == OP_CALL) {
        if (strcmp(opname, "hwInit") == 0) {
            drvBattery_hwInit();
        }
    }
}

static const iface_t drvBattery_iface = { "drvBattery", drvBattery_handler };

void drvBattery_init(void)
{
    s_temp_c10 = 0;
    s_voltage_mv = 0;
    s_current_ma = 0;
    s_remaining = 0;
    iface_register(&drvBattery_iface);
}

/* from FUN_08006F2C / FUN_08006E94 - the original's "hwInit": it waits for the
 * battery pack to become available and then requests the battery
 * information (0x73A).  The pack of the original is a BLE device; without the
 * pack the request is simply dropped and the state stays empty. */
void drvBattery_hwInit(void)
{
    s_voltage_mv = 0;
    s_current_ma = 0;
    s_temp_c10 = 0;
    s_remaining = 0;
}

int32_t drvBattery_voltage_mv(void)
{
    return (int32_t)s_voltage_mv;
}

int32_t drvBattery_current_ma(void)
{
    return (int32_t)s_current_ma;
}

int32_t drvBattery_remaining_pct(void)
{
    return (int32_t)s_remaining;
}

int drvBattery_charge_state(void)
{
    return charge_state();
}

int32_t drvBattery_temp_c10(void)
{
    return (int32_t)s_temp_c10;
}
