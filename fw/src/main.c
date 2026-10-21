/* main.c -- the original's main() (FUN_08009FA4).
 *
 * The original boot sequence:
 *   1. reset cause / power magic: two words left in RAM by the reset path
 *      (0x200025A8 = 0xD7E5F7A8 and 0x200017A8 = 0x19C3FE40) are tested;
 *      if both match the firmware switches into the "power fail" mode:
 *      the clock tree is reset to HSI, the SysTick is stopped, PC13/PC14
 *      are driven and the main stack is selected.
 *   2. the hardware init (GPIO analog, RCC, flash unlock - FUN_0800A640),
 *      the TIM6 millisecond timer (FUN_0800A464) and its start
 *      (FUN_0800A4C0).
 *   3. all the module inits (FUN_08006338).
 *   4. the interface registry is published (FUN_0800A150).
 *   5. the greeting is sent (FUN_0800A1C8 with the key/value block of the
 *      version information).
 *   6. the main loop: read a line into a 0x28 (40) byte buffer, dispatch
 *      it, run the periodic tasks and kick the watchdog.
 *
 * The rebuild uses SysTick for the millisecond tick (the original uses
 * TIM6), so the SysTick is programmed here instead of the TIM6.
 */
#include "codie.h"

/* the dispatcher of the protocol layer; codie.h does not declare it (the
 * contract only fixes the reply helpers and the registry), so it is
 * declared here. */
extern int proxy_dispatch(char *cmd);

/* the greeting: the key/value block of the original lives in .data at
 * 0x20000A14 and is a copy of the string table at 0x0800C65C */
static const char *const s_greeting[] = {
    "ver",   "0.5.0hw2.1",
    "mat",   "4",
    "date",  "1602121419",
    "ttpms", "1000",
    "+ack",  "0",
    0
};

/* the command buffer of the original: 0x28 = 40 bytes (0x20000FE8) */
static char s_cmdbuf[CMD_BUF_SIZE];

/* 96 MHz / 96000 = 1 kHz millisecond tick */
#define SYSTICK_RELOAD 96000u

/* the reset cause words of the original (0x200025A8 / 0x200017A8) */
#define RESET_MAGIC0 0x19C3FE40u
#define RESET_MAGIC1 0xD7E5F7A8u
static volatile uint32_t s_reset_magic0;
static volatile uint32_t s_reset_magic1;

/* The power fail path of the original: hwiPower (or any other module that
 * triggers a reset) can call this before the reset to leave the magic
 * behind, the next boot then enters the power fail mode. */
void main_mark_reset_cause(void)
{
    s_reset_magic0 = RESET_MAGIC0;
    s_reset_magic1 = RESET_MAGIC1;
}

/* --- registers of the power fail path (from FUN_0800AB88) ------------ */
#define REG32(a)          (*(volatile uint32_t *)(uintptr_t)(a))
#define MAIN_RCC_BASE     0x40021000u
#define MAIN_GPIOC_BASE   0x48000800u
#define RCC_CR            REG32(MAIN_RCC_BASE + 0x00u)
#define RCC_CFGR          REG32(MAIN_RCC_BASE + 0x04u)
#define RCC_CIR           REG32(MAIN_RCC_BASE + 0x08u)
#define RCC_AHBENR        REG32(MAIN_RCC_BASE + 0x14u)
#define RCC_AUX0          REG32(MAIN_RCC_BASE + 0x2Cu)
#define RCC_CSR           REG32(MAIN_RCC_BASE + 0x30u)
#define RCC_CCIPR         REG32(MAIN_RCC_BASE + 0x34u)
#define GPIOC_MODER       REG32(MAIN_GPIOC_BASE + 0x00u)
#define GPIOC_BSRR        REG32(MAIN_GPIOC_BASE + 0x18u)
#define SYSTICK_CTRL      REG32(0xE000E010u)
#define SYSTICK_LOAD      REG32(0xE000E014u)
#define SYSTICK_VAL       REG32(0xE000E018u)

/* from FUN_0800AB88 (the clock tree back to HSI) and the GPIO setup of
 * main's reset cause path (FUN_0800A964 with the pin mask 0x5000 -> PC13
 * and PC14 as outputs, then GPIOC BSRR = 0x4000 -> PC14 high). */
static void power_fail_mode(void)
{
    RCC_CR     = (RCC_CR & 0xFEF2FFFFu) | 0x00000001u;
    RCC_CFGR  &= 0x8C0B80Cu;
    RCC_AUX0  &= 0xFFFFFFF0u;
    RCC_CSR   &= 0xFFFFFEACu;
    RCC_CCIPR &= 0xFFFFFFFEu;
    RCC_CIR    = 0u;

    RCC_AHBENR |= 0x00080000u;

    /* PC13/PC14 as outputs (the original's FUN_0800A964) */
    GPIOC_MODER = (GPIOC_MODER & ~0x14000000u) | 0x14000000u;
    GPIOC_BSRR  = 0x4000u;          /* PC14 high */
}

/* from FUN_08009B14 - read one line out of the link into the 40 byte
 * command buffer.  Returns the number of characters, 0 if no line was
 * received; on an overflow the buffer is emptied and the buffer size is
 * returned (the caller then reports "possible cmdBuf overflow"). */
static int read_line(char *buf, int size)
{
    int n = 0;

    for (;;) {
        char c;

        if (n >= size) {
            buf[0] = '\0';
            return size;              /* overflow */
        }
        if (bleCom_read(&c, 1) <= 0) {
            bleCom_poll();
            continue;                  /* nothing received yet */
        }
        if (c == '\n') {
            buf[n] = '\0';
            return n;
        }
        buf[n++] = c;
    }
}

int main(void)
{
    /* 1. reset cause / power magic */
    if ((s_reset_magic0 == RESET_MAGIC0) && (s_reset_magic1 == RESET_MAGIC1)) {
        s_reset_magic0 = 0u;
        s_reset_magic1 = 0u;
        power_fail_mode();
        /* the original stops the SysTick here (its tick comes from TIM6);
         * the rebuild re-programs it for the HSI clock so that the
         * millisecond tick keeps running. */
        sys_tick_init(16000u);
    } else {
        /* 2. the millisecond tick (the original uses TIM6, FUN_0800A464) */
        sys_tick_init(SYSTICK_RELOAD);
    }

    /* 3. the module inits, in the order of the original's interface table
     *    (0x0800D164) */
    hwiAdc_init();
    hwiLed_init();
    hwiDrive_init();
    drvBattery_init();
    hwiSonar_init();
    bleCom_init();
    hwiBuzz_init();
    hwiMic_init();
    hwiEnc_init();
    hwiLine_init();
    hwiPower_init();
    hwiLightSense_init();
    appLights_init();
    appDrive_init();
    appSys_init();
    appLightSense_init();

    /* 4. the battery pack init (the original's "hwInit") */
    hw_init();

    /* 5. the greeting */
    resp_greeting(s_greeting, 0);

    /* 6. the main loop */
    for (;;) {
        int n = read_line(s_cmdbuf, CMD_BUF_SIZE);

        if (n != 0) {
            if (s_cmdbuf[0] == '\0') {
                resp_message(LOG_ERR, "possible cmdBuf overflow", 0);
            }
            proxy_dispatch(s_cmdbuf);
        }

        /* the periodic tasks (the original: FUN_080063EC) */
        appLights_tick();
        appDrive_tick();
        appLightSense_tick();
    }
}
