/* hwi_b_hal.c - low level helpers shared by the "agent B" modules.
 *
 * Re-implementations of the original firmware's low level routines.
 * The original function each helper comes from is named in the comment
 * above it.  Behaviour (including polarity and bit positions) follows
 * the decompiled original.
 */
#include "hwi_b_hal.h"
#include "codie.h"

/* ------------------------------------------------------------------ *
 * volatile register access
 * ------------------------------------------------------------------ */
#define REG32(a)      (*(volatile uint32_t *)(uintptr_t)(a))
#define REG16(a)      (*(volatile uint16_t *)(uintptr_t)(a))

/* RCC register offsets */
#define RCC_AHBENR   0x14u
#define RCC_APB2ENR  0x18u
#define RCC_APB1ENR  0x1Cu

/* GPIO register offsets */
#define GPIO_MODER   0x00u
#define GPIO_OTYPER  0x04u
#define GPIO_OSPEEDR 0x08u
#define GPIO_PUPDR   0x0Cu
#define GPIO_IDR     0x10u
#define GPIO_ODR     0x14u
#define GPIO_BSRR    0x18u
#define GPIO_BRR     0x1Cu
#define GPIO_AFR0    0x20u
#define GPIO_AFR1    0x24u

/* timer register offsets */
#define TIM_CR1      0x00u
#define TIM_CR2      0x04u
#define TIM_CCMR1    0x18u
#define TIM_CCMR2    0x1Cu
#define TIM_CCER     0x20u
#define TIM_CNT      0x24u
#define TIM_PSC      0x28u
#define TIM_ARR      0x2Cu
#define TIM_CCR1     0x34u
#define TIM_EGR      0x14u
#define TIM_BDTR     0x44u

/* USART register offsets */
#define USART_CR1    0x00u
#define USART_CR2    0x04u
#define USART_CR3    0x08u
#define USART_BRR    0x0Cu
#define USART_RQR    0x18u
#define USART_ISR    0x1Cu
#define USART_ICR    0x20u
#define USART_RDR    0x24u
#define USART_TDR    0x28u

/* ------------------------------------------------------------------ *
 * GPIO
 * ------------------------------------------------------------------ */
/* from FUN_0800A964 - configure a group of pins of one port.
 * cfg = { u32 mask; u8 mode; u8 speed; u8 otype; u8 pull; }
 * The original writes MODER for every pin, OTYPER + OSPEEDR only for
 * output / alternate function pins and PUPDR for every pin.
 */
void hwi_gpio_config(uint32_t port, const hwi_gpio_cfg_t *cfg)
{
    uint8_t pin;

    for (pin = 0; pin < 16u; pin++) {
        uint32_t bit = 1u << pin;
        uint32_t two;

        if ((cfg->mask & bit) == 0u) {
            continue;
        }
        two = 3u << (pin * 2u);

        if ((cfg->mode - 1u) < 2u) {          /* output or alternate function */
            REG32(port + GPIO_OSPEEDR) = (REG32(port + GPIO_OSPEEDR) & ~two) |
                                         ((uint32_t)cfg->speed << (pin * 2u));
            REG16(port + GPIO_OTYPER) = (uint16_t)((REG16(port + GPIO_OTYPER) &
                              (uint16_t)~((uint16_t)bit << 1u)) |
                              (uint16_t)((uint16_t)(cfg->otype & 1u) << pin));
        }
        REG32(port + GPIO_MODER) = (REG32(port + GPIO_MODER) & ~two) |
                                   ((uint32_t)cfg->mode << (pin * 2u));
        REG32(port + GPIO_PUPDR) = (REG32(port + GPIO_PUPDR) & ~two) |
                                  ((uint32_t)cfg->pull << (pin * 2u));
    }
}

/* from FUN_0800AAC8 - set the 4 bit alternate function field of a pin */
void hwi_gpio_set_af(uint32_t port, uint8_t pin, uint8_t af)
{
    uint32_t reg = port + GPIO_AFR0 + (uint32_t)((pin >> 3) * 4u);
    uint32_t shift = (uint32_t)((pin & 7u) * 4u);

    REG32(reg) = (REG32(reg) & ~(0xFu << shift)) | ((uint32_t)(af & 0xFu) << shift);
}

/* from FUN_0800AABC - returns non zero when the selected IDR bits are SET
 * (the original callers invert the result where they need a low level) */
int hwi_gpio_read(uint32_t port, uint16_t mask)
{
    return (REG16(port + GPIO_IDR) & mask) != 0u;
}

/* from the original's direct BSRR / BRR writes */
void hwi_gpio_set(uint32_t port, uint16_t mask)
{
    REG32(port + GPIO_BSRR) = mask;
}

void hwi_gpio_reset(uint32_t port, uint16_t mask)
{
    REG32(port + GPIO_BRR) = mask;
}

/* ------------------------------------------------------------------ *
 * clock enables
 * ------------------------------------------------------------------ */
void hwi_rcc_ahb_en(uint32_t mask, int enable)
{
    if (enable) {
        REG32(HWI_RCC + RCC_AHBENR) |= mask;
    } else {
        REG32(HWI_RCC + RCC_AHBENR) &= ~mask;
    }
}

void hwi_rcc_apb2_en(uint32_t mask, int enable)
{
    if (enable) {
        REG32(HWI_RCC + RCC_APB2ENR) |= mask;
    } else {
        REG32(HWI_RCC + RCC_APB2ENR) &= ~mask;
    }
}

void hwi_rcc_apb1_en(uint32_t mask, int enable)
{
    if (enable) {
        REG32(HWI_RCC + RCC_APB1ENR) |= mask;
    } else {
        REG32(HWI_RCC + RCC_APB1ENR) &= ~mask;
    }
}

/* ------------------------------------------------------------------ *
 * timers
 * ------------------------------------------------------------------ */
/* from FUN_0800AEC4 (PSC/ARR/CR1/EGR) + FUN_0800AFDC (channel setup) */
void hwi_tim_pwm_init(uint32_t tim, uint16_t psc, uint16_t arr, uint8_t channel)
{
    uint32_t ccmr;

    REG16(tim + TIM_CR1) = (uint16_t)(REG16(tim + TIM_CR1) & (uint16_t)0xFF7F); /* ARPE off */
    REG16(tim + TIM_PSC) = psc;
    REG32(tim + TIM_ARR) = arr;
    REG16(tim + TIM_EGR) = 1u;                       /* UG: reload            */

    if (channel <= 2u) {
        ccmr = tim + TIM_CCMR1;
    } else {
        ccmr = tim + TIM_CCMR2;
    }
    /* OCxM = 110 (PWM mode 1), OCxPE = 1 */
    REG32(ccmr) = (REG32(ccmr) & ~(0xFFu << ((channel & 1u) * 8u))) |
                  (0x68u << ((channel & 1u) * 8u));
    REG16(tim + TIM_CCER) = (uint16_t)(REG16(tim + TIM_CCER) |
                  (uint16_t)(1u << (((channel - 1u) * 4u) + 0u)));
}

void hwi_tim_pwm_duty(uint32_t tim, uint8_t channel, uint32_t ccr)
{
    REG32(tim + TIM_CCR1 + ((uint32_t)(channel - 1u) * 4u)) = ccr;
}

uint32_t hwi_tim_ccr(uint32_t tim, uint8_t channel)
{
    return REG32(tim + TIM_CCR1 + ((uint32_t)(channel - 1u) * 4u));
}

void hwi_tim_set_arr(uint32_t tim, uint32_t arr)
{
    REG32(tim + TIM_ARR) = arr;
    REG16(tim + TIM_EGR) = 1u;
}

/* from FUN_0800AFA0 - CR1.CEN */
void hwi_tim_start(uint32_t tim)
{
    REG16(tim + TIM_CR1) = (uint16_t)(REG16(tim + TIM_CR1) | 1u);
}

void hwi_tim_stop(uint32_t tim)
{
    REG16(tim + TIM_CR1) = (uint16_t)(REG16(tim + TIM_CR1) & (uint16_t)~1u);
}

/* ------------------------------------------------------------------ *
 * USART
 * ------------------------------------------------------------------ */
/* from FUN_0800B3FC - computes BRR = periph_clock / baud (rounded like
 * the original: quotient + (baud/2 <= remainder ? 1 : 0)) and sets
 * CR1/CR2/CR3.  stop_bits: 1 or 2.
 */
void hwi_usart_init(uint32_t usart, uint32_t baud, uint8_t stop_bits)
{
    uint32_t brr;

    if (baud == 0u) {
        return;
    }
    brr = HWI_APB1_HZ / baud;
    if ((baud / 2u) <= (HWI_APB1_HZ % baud)) {
        brr += 1u;
    }

    REG16(usart + USART_CR1) = (uint16_t)(REG16(usart + USART_CR1) & (uint16_t)0xFFFE);
    REG16(usart + USART_CR2) = (uint16_t)((REG16(usart + USART_CR2) & (uint16_t)0xCFFF) |
                                           (uint16_t)(((uint16_t)stop_bits & 1u) << 13));
    REG16(usart + USART_CR1) = (uint16_t)((REG16(usart + USART_CR1) & (uint16_t)0xE9F3) |
                                           (uint16_t)0x0300u);   /* TE | RE */
    REG16(usart + USART_CR3) = (uint16_t)((REG16(usart + USART_CR3) & (uint16_t)0xFCFF) |
                                           (uint16_t)0x000Cu);
    REG16(usart + USART_BRR) = (uint16_t)brr;
}

/* from FUN_0800B53C - ISR bit 7 (TXFNF) */
int hwi_usart_tx_ready(uint32_t usart)
{
    return (REG32(usart + USART_ISR) & 0x80u) != 0u;
}

/* from FUN_0800B4DC - TDR */
void hwi_usart_putc(uint32_t usart, uint8_t c)
{
    REG16(usart + USART_TDR) = c;
}

/* from FUN_0800B54C - ISR bit 5 (RXNE) */
int hwi_usart_rx_ready(uint32_t usart)
{
    return (REG32(usart + USART_ISR) & 0x20u) != 0u;
}

/* from FUN_0800B4E4 - RDR */
uint8_t hwi_usart_getc(uint32_t usart)
{
    return (uint8_t)(REG16(usart + USART_RDR) & 0x1FFu);
}

uint32_t hwi_usart_status(uint32_t usart)
{
    return REG32(usart + USART_ISR);
}

/* from FUN_0800B548 - ICR */
void hwi_usart_clear(uint32_t usart, uint32_t mask)
{
    REG32(usart + USART_ICR) = mask;
}

/* from FUN_0800B4C0 - CR1.UE */
void hwi_usart_enable(uint32_t usart)
{
    REG16(usart + USART_CR1) = (uint16_t)(REG16(usart + USART_CR1) | 1u);
}

void hwi_usart_disable(uint32_t usart)
{
    REG16(usart + USART_CR1) = (uint16_t)(REG16(usart + USART_CR1) & (uint16_t)~1u);
}

/* ------------------------------------------------------------------ *
 * ring buffer
 * ------------------------------------------------------------------ */
void hwi_ring_init(hwi_ring_t *r, uint8_t *buf, uint16_t size)
{
    r->buf   = buf;
    r->size  = size;
    r->head  = 0u;
    r->tail  = 0u;
    r->count = 0u;
}

int hwi_ring_put(hwi_ring_t *r, uint8_t b)
{
    if (r->count >= r->size) {
        return 0;                    /* buffer full */
    }
    r->buf[r->head] = b;
    r->head = (uint16_t)((r->head + 1u) % r->size);
    r->count++;
    return 1;
}

int hwi_ring_get(hwi_ring_t *r, uint8_t *b)
{
    if (r->count == 0u) {
        return 0;
    }
    *b = r->buf[r->tail];
    r->tail = (uint16_t)((r->tail + 1u) % r->size);
    r->count--;
    return 1;
}

uint16_t hwi_ring_read(hwi_ring_t *r, uint8_t *dst, uint16_t max)
{
    uint16_t n = 0u;

    while ((n < max) && (r->count != 0u)) {
        dst[n++] = r->buf[r->tail];
        r->tail = (uint16_t)((r->tail + 1u) % r->size);
        r->count--;
    }
    return n;
}

uint16_t hwi_ring_count(const hwi_ring_t *r)
{
    return r->count;
}

void hwi_ring_flush(hwi_ring_t *r)
{
    r->head  = 0u;
    r->tail  = 0u;
    r->count = 0u;
}

/* ------------------------------------------------------------------ *
 * misc
 * ------------------------------------------------------------------ */
uint32_t hwi_millis(void)
{
    return g_tick_ms;
}

/* Busy wait.  The original's "delay" helper (FUN_08002DD4) re-programs
 * the SysTick and then spins; the reconstruction waits on the shared
 * millisecond tick instead so that it does not disturb the system tick.
 * If the tick does not advance the wait is aborted (bounded spin). */
void hwi_delay_ms(uint32_t ms)
{
    uint32_t start = g_tick_ms;
    uint32_t spins = 0u;

    while ((g_tick_ms - start) < ms) {
        spins++;
        if (spins > 200000u) {       /* tick not running - give up */
            break;
        }
    }
}

/* from FUN_0800AB10 - independent watchdog kick */
void hwi_iwdg_kick(void)
{
    REG32(HWI_IWDG + 0x00u) = 0xAAAAu;
}

/* from FUN_0800A084 - the original accepts "on", "1" and "true" */
int hwi_is_on(const char *s)
{
    if (s == 0) {
        return 0;
    }
    if ((s[0] == 'o' || s[0] == 'O') && (s[1] == 'n' || s[1] == 'N') && (s[2] == '\0')) {
        return 1;
    }
    if ((s[0] == '1') && (s[1] == '\0')) {
        return 1;
    }
    if (((s[0] == 't' || s[0] == 'T') && (s[1] == 'r' || s[1] == 'R') &&
         (s[2] == 'u' || s[2] == 'U') && (s[3] == 'e' || s[3] == 'E')) && (s[4] == '\0')) {
        return 1;
    }
    return 0;
}

int hwi_atoi(const char *s)
{
    int v = 0;
    int neg = 0;

    if (s == 0) {
        return 0;
    }
    while ((*s == ' ') || (*s == '\t')) {
        s++;
    }
    if (*s == '-') {
        neg = 1;
        s++;
    } else if (*s == '+') {
        s++;
    }
    while ((*s >= '0') && (*s <= '9')) {
        v = (v * 10) + (int)(*s - '0');
        s++;
    }
    return neg ? -v : v;
}

/* Parse a whitespace separated list of decimal numbers.  Returns the
 * number of values found. */
int hwi_parse_args(const char *args, int *vals, int max)
{
    int n = 0;

    if ((args == 0) || (vals == 0) || (max <= 0)) {
        return 0;
    }
    while ((*args != '\0') && (n < max)) {
        while ((*args == ' ') || (*args == '\t') || (*args == ',')) {
            args++;
        }
        if (*args == '\0') {
            break;
        }
        vals[n++] = hwi_atoi(args);
        while ((*args != '\0') && (*args != ' ') && (*args != '\t') && (*args != ',')) {
            args++;
        }
    }
    return n;
}
