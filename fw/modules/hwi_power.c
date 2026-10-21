/* hwi_power.c - power / battery switch (interface "hwiPower").
 *
 * Original firmware:
 *   - FUN_08009654  GPIO setup: PC13 = input with pull down (the power
 *     good sense line, EXTI13 on GPIOC), PC12/PC14/PC15 = outputs
 *     (RCC AHBENR 0x80000 = GPIOC, APB2ENR 1 = SYSCFG, SYSCFG EXTICR3
 *     field 13 = 2 -> GPIOC).
 *   - FUN_080098C4  the "hwiPower" init: sets PC12 (the power hold) and
 *     checks PC13.  If PC13 is low the power is already gone and the
 *     module switches the robot off (FUN_080097E8), otherwise the
 *     falling edge of PC13 is armed (EXTI_RPR1 |= 0x2000,
 *     EXTI_RTS1 &= ~0x2000).
 *   - FUN_080097E8  switches the robot off: it clears PC12
 *     (GPIOC_BRR = 0x1000) and then spins forever (kicking the
 *     watchdog), so that the supply collapses.
 *   - FUN_08009874  the main loop tick: waits up to 401 ms for the
 *     power good line and switches off when it disappeared.
 *   - FUN_0800981C  the "hwiPower" command line handler:
 *       call hwiPower pwrOff        -> switch the robot off
 *       set  hwiPower Vsens <on|1|true> -> set PC15
 *       set  hwiPower Vsens <off>       -> reset PC15
 *     (the "on"/"1"/"true" test is the original's FUN_0800A084)
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include <string.h>

#define PWR_PORT        HWI_GPIOC
#define PWR_HOLD_PIN    12u    /* PC12 - keeps the supply alive       */
#define PWR_SENSE_PIN   13u    /* PC13 - power good sense (input)     */
#define PWR_VSENS_PIN   15u    /* PC15 - "Vsens" line                 */
#define PWR_AUX_PIN     14u    /* PC14 - auxiliary line               */

#define SYSCFG_EXTICR3  (HWI_SYSCFG + 0x0Cu)
#define EXTI_RPR1       (HWI_EXTI + 0x0Cu)
#define EXTI_RTS1       (HWI_EXTI + 0x08u)

static uint8_t power_ready;

static const iface_t iface_hwiPower; /* the interface record, defined below */

void hwiPower_init(void)
{
    iface_register(&iface_hwiPower);
    hwi_gpio_cfg_t cfg;

    if (power_ready) {
        return;
    }
    /* RCC: GPIOC (AHBENR 0x80000) and SYSCFG (APB2ENR 1) */
    hwi_rcc_ahb_en(0x80000u, 1);
    hwi_rcc_apb2_en(0x0001u, 1);

    /* PC13: input with pull down */
    cfg.mask  = 1u << PWR_SENSE_PIN;
    cfg.mode  = HWI_MODE_INPUT;
    cfg.speed = 1u;
    cfg.otype = 0u;
    cfg.pull  = HWI_PULL_DOWN;
    hwi_gpio_config(PWR_PORT, &cfg);

    /* PC12, PC14, PC15: outputs */
    cfg.mask  = (1u << PWR_HOLD_PIN) | (1u << PWR_AUX_PIN) | (1u << PWR_VSENS_PIN);
    cfg.mode  = HWI_MODE_OUTPUT;
    cfg.speed = 0u;
    cfg.otype = 0u;
    cfg.pull  = HWI_PULL_NONE;
    hwi_gpio_config(PWR_PORT, &cfg);

    /* SYSCFG: EXTI13 belongs to GPIOC (field value 2) */
    *(volatile uint32_t *)SYSCFG_EXTICR3 =
        (*(volatile uint32_t *)SYSCFG_EXTICR3 & ~(0xFu << 4u)) | (2u << 4u);

    /* keep the supply alive */
    hwi_gpio_set(PWR_PORT, (uint16_t)(1u << PWR_HOLD_PIN));

    /* arm the falling edge of the power good line */
    *(volatile uint32_t *)EXTI_RPR1 |= (1u << PWR_SENSE_PIN);
    *(volatile uint32_t *)EXTI_RTS1 &= ~(1u << PWR_SENSE_PIN);

    power_ready = 1u;
}

/* from FUN_080097E8 - cut the power.  The original clears the power
 * hold line and then spins forever (the watchdog is kicked, the supply
 * collapses and the board resets). */
void hwiPower_off(void)
{
    hwi_gpio_reset(PWR_PORT, (uint16_t)(1u << PWR_HOLD_PIN));
    for (;;) {
        hwi_iwdg_kick();
    }
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void hwiPower_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];

    if (opname == 0) {
        return;
    }

    /* call hwiPower pwrOff  ->  switch the robot off */
    if ((op == OP_CALL) && (strcmp(opname, "pwrOff") == 0)) {
        hwiPower_off();
        return;                 /* not reached */
    }

    /* set hwiPower Vsens <on|1|true|off> */
    if ((op == OP_SET) && (strcmp(opname, "Vsens") == 0)) {
        if (hwi_is_on(args) != 0) {
            hwi_gpio_set(PWR_PORT, (uint16_t)(1u << PWR_VSENS_PIN));
            fmt_int(val, (int)sizeof(val), 1);
        } else {
            hwi_gpio_reset(PWR_PORT, (uint16_t)(1u << PWR_VSENS_PIN));
            fmt_int(val, (int)sizeof(val), 0);
        }
        resp_value(op, "hwiPower", opname, val);
        return;
    }

    if (op == OP_GET) {
        /* report the state of the power good line */
        fmt_int(val, (int)sizeof(val),
                (int32_t)(hwi_gpio_read(PWR_PORT, (uint16_t)(1u << PWR_SENSE_PIN)) ? 1 : 0));
        resp_value(op, "hwiPower", opname, val);
    }
}

static const iface_t iface_hwiPower = { "hwiPower", hwiPower_handler };
