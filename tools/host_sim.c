/* host_sim.c - run the reconstructed firmware on the host against a fake
 * memory-mapped peripheral map, so its observable behaviour can be compared
 * with the original binary's behaviour.
 *
 * The firmware is compiled unmodified; only the USART primitives are
 * interposed (ld --wrap) so the harness can inject bytes and capture them.
 * Everything else (GPIO, timers, ADC, DMA, RCC) is plain MMIO that the
 * harness reads back to see the side effects.
 *
 *   gcc -std=gnu99 -I fw/include -I fw/modules tools/host_sim.c \
 *       fw/src/main.c fw/src/system_stm32g0.c fw/modules/*.c \
 *       -o /tmp/sim -lpthread \
 *       -Wl,--wrap=hwi_usart_init,--wrap=hwi_usart_enable,--wrap=hwi_usart_putc,\
 *       --wrap=hwi_usart_rx_ready,--wrap=hwi_usart_getc,--wrap=hwi_usart_status,\
 *       --wrap=hwi_usart_clear
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

/* ---- the addresses the firmware uses (from codie.h / hwi_b_hal.h) ------- */
#define USART3 0x40004800UL     /* AT channel to the BLE module  */
#define USART4 0x40004C00UL     /* command + reply link           */
#define TIM3   0x40000400UL
#define TIM15  0x40014000UL
#define TIM16  0x40014400UL
#define TIM17  0x40014800UL
#define TIM1   0x40012C00UL
#define TIM2   0x40000000UL
#define TIM6   0x40001000UL
#define ADC1   0x40012400UL
#define DMA1   0x40020000UL
#define GPIOA  0x48000000UL
#define GPIOB  0x48000400UL
#define GPIOC  0x48000800UL
#define GPIOF  0x48001400UL
#define RCC    0x40021000UL

#define REG32(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define REG16(a) (*(volatile uint16_t *)(uintptr_t)(a))
#define REG8(a)  (*(volatile uint8_t  *)(uintptr_t)(a))

/* USART register offsets (the G4-style layout the original uses) */
#define U_CR1 0x00
#define U_CR2 0x04
#define U_CR3 0x08
#define U_BRR 0x0C
#define U_ISR 0x1C
#define U_ICR 0x20
#define U_RDR 0x24
#define U_TDR 0x28
#define ISR_RXNE (1u << 5)
#define ISR_TXE  (1u << 7)

/* ---- MMIO map ---------------------------------------------------------- */
static void map_peripherals(void)
{
    void *p;
    /* 0x40000000 .. 0x50000000: RCC, USARTs, timers, ADC, DMA and the GPIO
     * bus at 0x48000000 (lazily committed, so the size is harmless) */
    p = mmap((void *)0x40000000, 0x10000000, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (p == MAP_FAILED) { perror("mmap peripherals"); exit(1); }
    /* RAM 0x20000000, 64 KB */
    p = mmap((void *)0x20000000, 0x00010000, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (p == MAP_FAILED) { perror("mmap ram"); exit(1); }
    /* Cortex-M system space: SysTick 0xE000E010, NVIC, SCB 0xE000ED00 */
    p = mmap((void *)0xE0000000, 0x00010000, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (p == MAP_FAILED) { perror("mmap cortex-m"); exit(1); }
    /* flash 0x08000000, 128 KB (the vector table lives here) */
    p = mmap((void *)0x08000000, 0x00020000, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    if (p == MAP_FAILED) { perror("mmap flash"); exit(1); }
}

/* ---- the interposed USARTs -------------------------------------------- */
typedef struct {
    uint32_t base;
    char     tx[4096];
    int      tx_len;
    uint8_t  rx[4096];
    int      rx_len;
    int      rx_head;
    int      rx_tail;
    uint32_t brr;
    uint32_t cr1, cr2, cr3;
    int      enabled;
} usart_t;

static usart_t s_u3 = { USART3, {0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0 };
static usart_t s_u4 = { USART4, {0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0 };

static usart_t *usart_of(uint32_t base)
{
    if (base == USART3) return &s_u3;
    if (base == USART4) return &s_u4;
    return NULL;
}

/* --- wrapped firmware entry points --- */
void __wrap_hwi_usart_init(uint32_t base, uint32_t baud, uint8_t stop_bits);
void __wrap_hwi_usart_enable(uint32_t base);
void __wrap_hwi_usart_putc(uint32_t base, uint8_t c);
int  __wrap_hwi_usart_rx_ready(uint32_t base);
uint8_t __wrap_hwi_usart_getc(uint32_t base);
uint32_t __wrap_hwi_usart_status(uint32_t base);
void __wrap_hwi_usart_clear(uint32_t base, uint32_t mask);
int  __wrap_hwi_usart_tx_ready(uint32_t base);

int __wrap_hwi_usart_tx_ready(uint32_t base)
{
    usart_t *u = usart_of(base);
    if (!u) return 0;
    return u->enabled;
}

void __wrap_hwi_usart_init(uint32_t base, uint32_t baud, uint8_t stop_bits)
{
    usart_t *u = usart_of(base);
    if (!u) return;
    /* mirror the real register writes so the firmware's own state is sane */
    REG16(base + U_CR1) = 0;
    REG16(base + U_CR2) = (uint16_t)(((uint16_t)stop_bits & 1u) << 13);
    REG16(base + U_CR3) = 0x000Cu;
    u->brr = baud ? (96000000u / baud) : 0u;   /* 96 MHz APB (see MEMORY.md 4.4) */
    REG16(base + U_BRR) = (uint16_t)u->brr;
    u->cr1 = 0; u->cr2 = REG16(base + U_CR2); u->cr3 = 0x000Cu;
}

void __wrap_hwi_usart_enable(uint32_t base)
{
    usart_t *u = usart_of(base);
    if (!u) return;
    u->enabled = 1;
    u->cr1 |= 0x0300u;                 /* TE | RE */
    REG16(base + U_CR1) = (uint16_t)u->cr1;
}

void __wrap_hwi_usart_putc(uint32_t base, uint8_t c)
{
    usart_t *u = usart_of(base);
    if (!u) return;
    if (u->tx_len < (int)sizeof(u->tx))
        u->tx[u->tx_len++] = (char)c;
    REG8(base + U_TDR) = c;
    REG32(base + U_ISR) |= ISR_TXE;     /* TX always ready in the model */
}

int __wrap_hwi_usart_rx_ready(uint32_t base)
{
    usart_t *u = usart_of(base);
    if (!u) return 0;
    return (u->rx_tail != u->rx_head);
}

uint8_t __wrap_hwi_usart_getc(uint32_t base)
{
    usart_t *u = usart_of(base);
    if (!u) return 0;
    if (u->rx_tail == u->rx_head) return 0;
    {
        uint8_t c = u->rx[u->rx_head];
        u->rx_head = (u->rx_head + 1) % (int)sizeof(u->rx);
        return c;
    }
}

uint32_t __wrap_hwi_usart_status(uint32_t base)
{
    usart_t *u = usart_of(base);
    uint32_t v = 0;
    if (u && u->enabled)
        v |= ISR_TXE;
    return v;
}

void __wrap_hwi_usart_clear(uint32_t base, uint32_t mask)
{
    (void)base; (void)mask;
}

/* ---- helpers ----------------------------------------------------------- */
static int g_fail;

static void check(const char *what, const char *want, const char *got, int got_len)
{
    char g[1024];
    int n = got_len;
    if (n > (int)sizeof(g) - 1)
        n = (int)sizeof(g) - 1;
    memcpy(g, got, (size_t)n);
    g[n] = '\0';
    if (strcmp(want, g) == 0) {
        printf("  ok   %-34s %s\n", what, g);
    } else {
        printf("  FAIL %-34s got [%s] want [%s]\n", what, g, want);
        g_fail++;
    }
}

/* the reply may be interleaved with the asynchronous "PPRT" keep-alive of
 * the LED engine (the original prints it on the same link) */
static void check_contains(const char *what, const char *want, const char *got, int got_len)
{
    char g[1024];
    int n = got_len;
    if (n > (int)sizeof(g) - 1)
        n = (int)sizeof(g) - 1;
    memcpy(g, got, (size_t)n);
    g[n] = '\0';
    if (strstr(g, want) != 0) {
        printf("  ok   %-34s %s\n", what, g);
    } else {
        printf("  FAIL %-34s got [%s] want to contain [%s]\n", what, g, want);
        g_fail++;
    }
}

static void checku(const char *what, uint32_t want, uint32_t got)
{
    if (want == got) {
        printf("  ok   %-34s 0x%08X\n", what, got);
    } else {
        printf("  FAIL %-34s got 0x%08X want 0x%08X\n", what, got, want);
        g_fail++;
    }
}

static void feed(usart_t *u, const char *s)
{
    while (*s) {
        if (u->rx_tail + 1 < (int)sizeof(u->rx)) {
            u->rx[u->rx_tail] = *s++;
            u->rx_tail = (u->rx_tail + 1) % (int)sizeof(u->rx);
        } else {
            break;
        }
    }
}

static void dump_tx(const char *label, usart_t *u)
{
    printf("%-28s %.*s\n", label, u->tx_len, u->tx);
    u->tx_len = 0;
}

/* the firmware's main() */
extern int fw_main(void);
extern void SysTick_Handler(void);
extern void SystemInit(void);

static void *tick_thread(void *arg)
{
    (void)arg;
    for (;;) {
        struct timespec ts = { 0, 1000000 };   /* 1 ms */
        nanosleep(&ts, NULL);
        SysTick_Handler();
    }
    return NULL;
}

/* models the bits the real hardware would set on its own */
static void *model_thread(void *arg)
{
    (void)arg;
    for (;;) {
        usleep(100);
        REG32(ADC1 + 0x40) &= ~0x00000004u;   /* ADC_CCR.CAL clears */
        REG32(ADC1 + 0x00) |= 0x00000003u;    /* ADC_ISR.ADRDY + EOC */
        REG32(ADC1 + 0x08) &= ~0x80000000u;  /* ADC_CR.ADSTART clears      */
        REG32(USART3 + U_ISR) |= ISR_TXE;    /* the transmit register is free */
        REG32(USART4 + U_ISR) |= ISR_TXE;
        REG32(TIM1  + 0x24)++;                /* CNT */
        REG32(TIM2  + 0x24)++;
        REG32(TIM3  + 0x24)++;
        REG32(TIM6  + 0x24)++;
        REG32(TIM15 + 0x24)++;
        REG32(TIM16 + 0x24)++;
        REG32(TIM17 + 0x24)++;
    }
    return NULL;
}

int main(void)
{
    pthread_t th;

    map_peripherals();

    /* the ADC: the firmware waits for EOC in ADC1->ISR and reads ADC1->DR */
    REG32(ADC1 + 0x00) = 0x00000003u;   /* ADRDY + EOC */
    REG32(ADC1 + 0x1C) = 0x00000CCCu;   /* DR = a mid-scale sample */

    /* the IWDG: some firmware paths wait for the PV flag */
    REG32(0x40003000 + 0x0C) = 0x00000003u;

    /* SystemInit() is NOT called: it waits for RCC PLLRDY/HSERDY bits that
     * only the real hardware sets, which would hang the model.  The clock
     * tree itself is verified separately (MEMORY.md 4.4). */
    pthread_create(&th, NULL, model_thread, NULL);
    pthread_create(&th, NULL, tick_thread, NULL);

    /* the firmware's main() never returns - run it in a thread */
    /* step through the module inits to see where the model gets stuck */
    {
        extern void hwiAdc_init(void);   extern void hwiLed_init(void);
        extern void hwiDrive_init(void); extern void hwiSonar_init(void);
        extern void bleCom_init(void);   extern void hwiBuzz_init(void);
        extern void hwiMic_init(void);   extern void hwiEnc_init(void);
        extern void hwiLine_init(void);  extern void hwiPower_init(void);
        extern void hwiLightSense_init(void);
        extern void appLights_init(void); extern void appDrive_init(void);
        extern void appSys_init(void);   extern void appLightSense_init(void);
        extern void drvBattery_init(void);
        struct { const char *n; void (*f)(void); } seq[] = {
            {"hwiAdc", hwiAdc_init}, {"hwiLed", hwiLed_init},
            {"hwiDrive", hwiDrive_init}, {"drvBattery", drvBattery_init},
            {"hwiSonar", hwiSonar_init}, {"bleCom", bleCom_init},
            {"hwiBuzz", hwiBuzz_init}, {"hwiMic", hwiMic_init},
            {"hwiEnc", hwiEnc_init}, {"hwiLine", hwiLine_init},
            {"hwiPower", hwiPower_init}, {"hwiLightSense", hwiLightSense_init},
            {"appLights", appLights_init}, {"appDrive", appDrive_init},
            {"appSys", appSys_init}, {"appLightSense", appLightSense_init},
        };
        unsigned i;
        {
            extern int adc_ready(uint32_t adc, uint32_t mask);
            printf("[model] ADC1 ISR=0x%08X  adc_ready(1)=%d\n",
                   REG32(ADC1 + 0x00), adc_ready(ADC1, 1u));
            fflush(stdout);
        }
        for (i = 0; i < sizeof(seq)/sizeof(seq[0]); i++) {
            printf("  init %-14s ... ", seq[i].n);
            fflush(stdout);
            seq[i].f();
            printf("ok\n");
            fflush(stdout);
        }
    }

    pthread_create(&th, NULL, (void *(*)(void *))fw_main, NULL);

    usleep(300000);   /* let it boot and send the greeting */

    printf("=== boot ===\n");
    {
        char want[512];
        snprintf(want, sizeof(want),
                 "BLE-boot\ncall :proxy greeting \"ver\":\"0.5.0hw2.1\" \"mat\":\"4\" "
                 "\"date\":\"1602121419\" \"ttpms\":\"1000\" \"+ack\":\"0\"\n");
        check("boot: BLE-boot + greeting", want, s_u4.tx, s_u4.tx_len);
        s_u4.tx_len = 0;
        check("boot: the AT link stays quiet", "", s_u3.tx, s_u3.tx_len);
        s_u3.tx_len = 0;
    }

    printf("\n=== commands on USART4 (the command link) ===\n");

    feed(&s_u4, "get drvBattery voltage\n");
    usleep(150000);
    check_contains("get drvBattery voltage", "set drvBattery voltage 0\n", s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;

    feed(&s_u4, "get drvBattery chargeStat\n");
    usleep(150000);
    check("get drvBattery chargeStat", "set drvBattery chargeStat 1\n", s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;

    feed(&s_u4, "get nosuchiface op\n");
    usleep(150000);
    check("unknown interface", "call :proxy message err No interface:nosuchiface\n",
          s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;

    feed(&s_u4, "garbage\n");
    usleep(150000);
    check("invalid command", "call :proxy message err Invalid command!\n",
          s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;

    feed(&s_u4, "set hwiDrive dutyA 200\n");
    usleep(150000);
    check("set hwiDrive dutyA (no reply)", "", s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;
    checku("set hwiDrive dutyA -> TIM3 CCR1", 0x200u, REG32(TIM3 + 0x34));

    feed(&s_u4, "set hwiDrive dutyB 100\n");
    usleep(150000);
    check("set hwiDrive dutyB (no reply)", "", s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;
    checku("set hwiDrive dutyB -> TIM3 CCR3", 0x100u, REG32(TIM3 + 0x3C));

    feed(&s_u4, "call drvBattery hwInit\n");
    usleep(150000);
    check("call drvBattery hwInit (no reply)", "", s_u4.tx, s_u4.tx_len);
    s_u4.tx_len = 0;

    printf("\n=== the AT channel (USART3) ===\n");
    printf("  USART3 BRR=%u (14400 @96MHz)  USART4 BRR=%u (460800 @96MHz)\n",
           s_u3.brr, s_u4.brr);
    checku("AT link BRR = 96e6/14400", 6666u, s_u3.brr);
    checku("command link BRR = 96e6/460800", 208u, s_u4.brr);

    printf("\n%s\n", g_fail ? "SOME CHECKS FAILED" : "all behavioural checks passed");
    fflush(stdout);
    return g_fail ? 1 : 0;
}
