/* hwi_a_hal.c - low level register helpers shared by hwiAdc / hwiDrive /
 * hwiEnc / hwiSonar.  See hwi_a_hal.h for the contract.
 *
 * Every function is a re-implementation of the original firmware's low
 * level helper of the same purpose; the original function name is given
 * in the comment above it.  Where the original does something odd (e.g. the
 * 32 bit compare register writes that also clear the next channel, or the
 * output type byte that is shifted into the register as a whole) the odd
 * behaviour is reproduced on purpose and flagged.
 */
#include "hwi_a_hal.h"

#define REG32(a) (*(volatile uint32_t *)(a))
#define REG16(a) (*(volatile uint16_t *)(a))

/* ---------------- GPIO ------------------------------------------------ */

/* from FUN_0800A964
 *
 * The original walks the 16 pins of the port two at a time and, for every
 * selected pin, writes
 *   MODER  = mode  << (2*pin)
 *   OSPEEDR= speed << (2*pin)     (only for mode 1 and 2)
 *   OTYPER = type  << pin        (only for mode 1 and 2)
 *   PUPDR  = pull  << (2*pin)
 * The output type is written as a whole byte shifted by the pin number,
 * so a value > 1 also sets the bit of the next pin - that is what the
 * original does (the motor driver pins use otype = 3) and it is kept.
 */
void gpio_config(uint32_t port, const gpio_cfg_t *cfg)
{
    uint32_t moder   = REG32(port + 0x00);
    uint16_t otyper  = REG16(port + 0x04);
    uint32_t ospeedr = REG32(port + 0x08);
    uint32_t pupdr   = REG32(port + 0x0C);
    uint8_t pin;

    for (pin = 0; pin < 16; pin++) {
        if ((cfg->pins & (uint16_t)(1u << pin)) == 0)
            continue;

        if ((uint8_t)(cfg->mode - 1u) < 2u) {          /* output or alt func */
            ospeedr = (ospeedr & ~(3u << (2u * pin)))
                    | ((uint32_t)cfg->ospeed << (2u * pin));
            otyper  = (uint16_t)((otyper & (uint16_t)~(1u << pin))
                    | (uint16_t)((uint16_t)cfg->otype << pin));
        }
        moder = (moder & ~(3u << (2u * pin)))
              | ((uint32_t)cfg->mode << (2u * pin));
        pupdr = (pupdr & ~(3u << (2u * pin)))
              | ((uint32_t)cfg->pupd << (2u * pin));
    }

    REG32(port + 0x00) = moder;
    REG16(port + 0x04) = otyper;
    REG32(port + 0x08) = ospeedr;
    REG32(port + 0x0C) = pupdr;
}

/* from FUN_0800AAC8: set the 4 bit alternate function field of one pin */
void gpio_set_afr(uint32_t port, uint8_t pin, uint8_t afr)
{
    uint32_t shift = (uint32_t)(pin & 7u) * 4u;
    uint32_t reg   = port + 0x20u + (uint32_t)(pin >> 3) * 4u;

    REG32(reg) = (REG32(reg) & ~(0xFu << shift)) | ((uint32_t)afr << shift);
}

/* from FUN_0800AABC: read the input data register, 1 if any masked bit is set */
int gpio_read(uint32_t port, uint16_t mask)
{
    return (REG16(port + 0x10) & mask) != 0;
}

/* set the bits in the port's BSRR (0x18), clear them in the BRR (0x28) */
void gpio_write(uint32_t port, uint16_t mask, int level)
{
    if (level)
        REG32(port + 0x18) = mask;
    else
        REG32(port + 0x28) = mask;
}

/* ---------------- timers ---------------------------------------------- */

static int timer_is_advanced(uint32_t tim)
{
    return (tim == TIM1_BASE) || (tim == TIM15_BASE) ||
           (tim == TIM16_BASE) || (tim == TIM17_BASE);
}

/* from FUN_0800AEC4: prescaler, auto reload, CR1 bits and the update event */
void timer_base_cfg(uint32_t tim, const timer_base_t *cfg)
{
    uint16_t cr1 = REG16(tim + 0x00);

    if ((tim == TIM1_BASE) || (tim == TIM2_BASE) || (tim == TIM3_BASE))
        cr1 = (uint16_t)((cr1 & 0xFF8Fu) | cfg->cr1_hi);

    if (tim == TIM6_BASE) {
        REG32(tim + 0x2C) = cfg->arr;
        REG16(tim + 0x28) = cfg->psc;
        REG16(tim + 0x00) = cr1;
    } else {
        REG16(tim + 0x00) = (uint16_t)((cr1 & 0xFCFFu) | cfg->cr1_ckd);
        REG32(tim + 0x2C) = cfg->arr;          /* ARR and RCR at once */
        REG16(tim + 0x28) = cfg->psc;
        if (timer_is_advanced(tim))
            REG16(tim + 0x30) = cfg->rcr;
    }
    REG16(tim + 0x14) = 1;                    /* EGR: UG */
}

/* from FUN_0800af4c: prescaler plus event generation register */
void timer_prescaler_egr(uint32_t tim, uint16_t psc, uint16_t egr)
{
    REG16(tim + 0x28) = psc;
    REG16(tim + 0x14) = egr;
}

/* from FUN_0800AF74: CR1 bit 7 (auto reload preload) */
void timer_arpe(uint32_t tim, int on)
{
    if (on)
        REG16(tim + 0x00) = (uint16_t)(REG16(tim + 0x00) | 0x80u);
    else
        REG16(tim + 0x00) = (uint16_t)(REG16(tim + 0x00) & (uint16_t)~0x80u);
}

/* from FUN_0800b1fc / FUN_0800b208 / FUN_0800b220 / FUN_0800b22c:
 * the compare preload enable bit of channel 1..4 */
void timer_ocpe(uint32_t tim, uint8_t ch, int on)
{
    uint32_t reg = (ch <= 2u) ? (tim + 0x18) : (tim + 0x1C);
    uint16_t bit = (ch <= 2u) ? (uint16_t)(1u << ((ch - 1u) * 8u + 3u))
                              : (uint16_t)(1u << ((ch - 3u) * 8u + 3u));

    if (on)
        REG16(reg) = (uint16_t)(REG16(reg) | bit);
    else
        REG16(reg) = (uint16_t)(REG16(reg) & (uint16_t)~bit);
}

/* from FUN_0800AFDC (ch1), FUN_0800B054 (ch2), FUN_0800B0F4 (ch3) and
 * FUN_0800B174 (ch4): the compare channel configuration of one channel. */
void timer_channel_cfg(uint32_t tim, uint8_t ch, const timer_chan_t *cfg)
{
    uint32_t ccer = REG16(tim + 0x20);
    uint32_t cr2  = REG16(tim + 0x04);

    switch (ch) {
    case 1:
        ccer &= ~0x0001u;
        ccer  = (ccer & 0xFFFDu) | cfg->ccer4 | cfg->ccer;
        if (timer_is_advanced(tim)) {
            ccer = ((ccer & 0xFFF7u) | cfg->ccer5) & 0xFFFBu;
            ccer = ccer | cfg->ccer2;
            cr2  = (cr2 & 0xFCFFu) | cfg->cr2 | cfg->cr2b;
        }
        REG16(tim + 0x04) = (uint16_t)cr2;
        REG16(tim + 0x18) = (uint16_t)((REG16(tim + 0x18) & 0xFF8Cu) | cfg->ccmr);
        REG32(tim + 0x34) = cfg->ccr;
        break;

    case 2:
        ccer &= ~0x0010u;
        ccer  = (ccer & 0xFFDFu) | ((uint32_t)cfg->ccer4 << 4) | ((uint32_t)cfg->ccer << 4);
        if (tim == TIM1_BASE) {
            ccer = ((ccer & 0xFF7Fu) | (cfg->ccer5 << 4)) & 0xFFBFu;
            ccer = ccer | (cfg->ccer2 << 4);
            cr2  = (cr2 & 0xFBFFu) | (cfg->cr2 << 2);
            cr2  = (cr2 & 0xF7FFu) | (cfg->cr2b << 2);
        } else if (tim == TIM15_BASE) {
            cr2  = (cr2 & 0xFBFFu) | (cfg->cr2 << 2);
        }
        REG16(tim + 0x04) = (uint16_t)cr2;
        REG16(tim + 0x18) = (uint16_t)((REG16(tim + 0x18) & 0x8CFFu) | (cfg->ccmr << 8));
        REG32(tim + 0x38) = cfg->ccr;
        break;

    case 3:
        ccer &= ~0x0100u;
        ccer  = (ccer & 0xFDFFu) | ((uint32_t)cfg->ccer4 << 8) | ((uint32_t)cfg->ccer << 8);
        if (tim == TIM1_BASE) {
            ccer = ((ccer & 0xF7FFu) | (cfg->ccer5 << 8)) & 0xFBFFu;
            ccer = ccer | (cfg->ccer2 << 8);
            cr2  = (cr2 & 0xCFFFu) | (cfg->cr2 << 4) | (cfg->cr2b << 4);
        }
        REG16(tim + 0x04) = (uint16_t)cr2;
        REG16(tim + 0x1C) = (uint16_t)((REG16(tim + 0x1C) & 0xFF8Cu) | cfg->ccmr);
        REG32(tim + 0x3C) = cfg->ccr;
        break;

    default: /* channel 4 */
        ccer &= ~0x1000u;
        if (tim == TIM1_BASE)
            cr2 = (cr2 & 0xBFFFu) | (cfg->cr2 << 6);
        REG16(tim + 0x04) = (uint16_t)cr2;
        REG16(tim + 0x1C) = (uint16_t)((REG16(tim + 0x1C) & 0x8CFFu) | (cfg->ccmr << 8));
        REG32(tim + 0x40) = cfg->ccr;
        ccer = (ccer & 0xDFFFu) | ((uint32_t)cfg->ccer4 << 12) | ((uint32_t)cfg->ccer << 12);
        break;
    }

    REG16(tim + 0x20) = (uint16_t)ccer;
}

/* from FUN_0800b1ec / FUN_0800b1f0 / FUN_0800b1f4 / FUN_0800b1f8.
 * The original writes 32 bits at TIMx_CCR1 (0x34) .. TIMx_CCR4 (0x40),
 * so a write also clears the next compare register (and, for channel 4,
 * touches the BDTR of an advanced timer).  Reproduced verbatim. */
void timer_pwm_set(uint32_t tim, uint8_t ch, uint32_t ccr)
{
    REG32(tim + 0x34u + 4u * (uint32_t)(ch - 1u)) = ccr;
}

/* from FUN_0800B244: the channel enable bits of CCER */
void timer_ccer(uint32_t tim, uint8_t ch, int on)
{
    uint16_t bit = (uint16_t)(1u << ((ch - 1u) * 4u));

    if (on)
        REG16(tim + 0x20) = (uint16_t)(REG16(tim + 0x20) | bit);
    else
        REG16(tim + 0x20) = (uint16_t)(REG16(tim + 0x20) & (uint16_t)~bit);
}

/* from FUN_0800AFA0: CR1 bit 0, the counter enable */
void timer_enable(uint32_t tim, int on)
{
    if (on)
        REG16(tim + 0x00) = (uint16_t)(REG16(tim + 0x00) | 1u);
    else
        REG16(tim + 0x00) = (uint16_t)(REG16(tim + 0x00) & (uint16_t)~1u);
}

/* from FUN_0800AFBC: BDTR bit 15, the main output enable */
void timer_moe(uint32_t tim, int on)
{
    if (on)
        REG16(tim + 0x44) = (uint16_t)(REG16(tim + 0x44) | 0x8000u);
    else
        REG16(tim + 0x44) = (uint16_t)(REG16(tim + 0x44) & (uint16_t)~0x8000u);
}

/* from FUN_0800B38C / FUN_0800B3C8: the interrupt enable register */
void timer_dier(uint32_t tim, uint16_t mask, int on)
{
    if (on)
        REG16(tim + 0x0C) = (uint16_t)(REG16(tim + 0x0C) | mask);
    else
        REG16(tim + 0x0C) = (uint16_t)(REG16(tim + 0x0C) & (uint16_t)~mask);
}

/* from FUN_0800b3c0: the status register is written with the inverted
 * mask, which clears the flags */
void timer_sr_clear(uint32_t tim, uint16_t mask)
{
    REG16(tim + 0x10) = (uint16_t)~mask;
}

/* from FUN_0800b3a4: the event generation register */
void timer_egr(uint32_t tim, uint16_t val)
{
    REG16(tim + 0x14) = val;
}

uint16_t timer_get16(uint32_t tim, uint32_t off) { return REG16(tim + off); }
void     timer_set16(uint32_t tim, uint32_t off, uint16_t v) { REG16(tim + off) = v; }
uint32_t timer_get32(uint32_t tim, uint32_t off) { return REG32(tim + off); }
void     timer_set32(uint32_t tim, uint32_t off, uint32_t v) { REG32(tim + off) = v; }

/* ---------------- ADC1 ----------------------------------------------- */

/* from FUN_0800A6A0: the configuration register CFGR1 */
void adc_cfg1(uint32_t adc, uint32_t clear_mask, uint32_t set_mask, uint8_t exten)
{
    uint32_t cfgr1 = REG32(adc + 0x0C);

    cfgr1 = (cfgr1 & clear_mask) | set_mask | ((uint32_t)exten << 13);
    REG32(adc + 0x0C) = cfgr1;
}

/* from FUN_0800a6f8: select a channel and set the sample time */
void adc_channel_add(uint32_t adc, uint8_t mask, uint32_t smpr)
{
    REG32(adc + 0x28) = REG32(adc + 0x28) | mask;   /* channel selection */
    REG32(adc + 0x14) = smpr;                       /* sample time       */
}

/* from FUN_0800A77C: the DMA request generation bits of CFGR1 */
void adc_dmngt(uint32_t adc, uint32_t v)
{
    uint32_t cfgr1 = REG32(adc + 0x0C);

    REG32(adc + 0x0C) = (cfgr1 & 0xFFFFFFFDu) | v;
}

/* from FUN_0800A760: DMA mode bit of CFGR1 */
void adc_dma_mode(uint32_t adc, int on)
{
    uint32_t cfgr1 = REG32(adc + 0x0C);

    if (on)
        REG32(adc + 0x0C) = cfgr1 | 1u;
    else
        REG32(adc + 0x0C) = cfgr1 & 0xFFFFFFFEu;
}

/* from FUN_0800A6DC: CR bit 0 enables the ADC, bit 1 starts the shutdown */
void adc_enable(uint32_t adc, int on)
{
    uint32_t cr = REG32(adc + 0x08);

    if (on)
        REG32(adc + 0x08) = cr | 1u;
    else
        REG32(adc + 0x08) = cr | 2u;
}

/* from FUN_0800a754: CR bit 2 */
void adc_stop(uint32_t adc)
{
    REG32(adc + 0x08) = REG32(adc + 0x08) | 4u;
}

/* from FUN_0800A704: start a conversion, wait for it and return the result
 * (the data register of this part sits at offset 0x40) */
uint32_t adc_start(uint32_t adc)
{
    uint32_t cr = REG32(adc + 0x08);

    REG32(adc + 0x08) = cr | 0x80000000u;
    while ((int)REG32(adc + 0x08) < 0)
        ;
    return REG32(adc + 0x40);
}

/* from FUN_0800A78C: status flag test, the conversion start flag of CR is
 * handled separately by the original */
int adc_ready(uint32_t adc, uint32_t mask)
{
    uint32_t src;

    if ((int)(mask << 7) < 0)
        src = REG32(adc + 0x08) & 0xFEFFFFFFu;
    else
        src = REG32(adc + 0x00);
    return (mask & src) != 0;
}

/* ---------------- DMA1 ----------------------------------------------- */

/* from FUN_0800A840: the four registers of a DMA channel.  The original
 * keeps bits 4..14 and 31 of the configuration register and replaces the
 * rest with the value given by the caller. */
void dma_channel_cfg(uint32_t ch, uint32_t cpar, uint32_t cmar,
                     uint16_t cntdr, uint32_t ccr)
{
    REG32(ch + 0x00) = (REG32(ch + 0x00) & 0xFFFF800Fu) | ccr;
    REG32(ch + 0x04) = cntdr;
    REG32(ch + 0x08) = cpar;
    REG32(ch + 0x0C) = cmar;
}

/* from FUN_0800A87C: the channel enable bit */
void dma_channel_enable(uint32_t ch, int on)
{
    if (on)
        REG32(ch + 0x00) = REG32(ch + 0x00) | 1u;
    else
        REG32(ch + 0x00) = REG32(ch + 0x00) & 0xFFFFFFFEu;
}

/* from FUN_0800A7B0: reset a channel and clear its interrupt flags */
void dma_channel_reset(uint32_t ch)
{
    uint32_t flags = 0;

    REG32(ch + 0x00) = REG32(ch + 0x00) & 0xFFFFFFFEu;
    REG32(ch + 0x00) = 0;
    REG32(ch + 0x04) = 0;
    REG32(ch + 0x08) = 0;
    REG32(ch + 0x0C) = 0;

    if (ch == DMA1_CH1)      flags = 0x0000000Fu;
    else if (ch == DMA1_CH2) flags = 0x000000F0u;
    else if (ch == DMA1_CH3) flags = 0x00000F00u;
    else if (ch == DMA1_CH4) flags = 0x0000F000u;
    else if (ch == DMA1_CH5) flags = 0x000F0000u;

    REG32(DMA1_BASE + 0x04) = REG32(DMA1_BASE + 0x04) | flags;
}

/* from FUN_0800a89c: the number of transfers */
void dma_set_cntdr(uint32_t ch, uint16_t n)
{
    REG32(ch + 0x04) = n;
}

/* from FUN_0800A8A0: the direction bit of the configuration register */
void dma_set_dir(uint32_t ch, uint32_t mask)
{
    REG32(ch + 0x00) = REG32(ch + 0x00) | mask;
}

/* from FUN_0800a8b8: the interrupt status register of the DMA */
int dma_flag(uint32_t mask)
{
    return (REG32(DMA1_BASE + 0x00) & mask) != 0;
}

/* from FUN_0800a8c8: the interrupt clear register of the DMA */
void dma_clear_flags(uint32_t mask)
{
    REG32(DMA1_BASE + 0x04) = mask;
}

/* ---------------- RCC and NVIC --------------------------------------- */

/* from FUN_0800AD5C: the AHB peripheral clock enables */
void rcc_ahb_enable(uint32_t mask, int on)
{
    if (on)
        REG32(RCC_BASE + 0x14) = REG32(RCC_BASE + 0x14) | mask;
    else
        REG32(RCC_BASE + 0x14) = REG32(RCC_BASE + 0x14) & ~mask;
}

/* from FUN_0800AD7C: the APB2 peripheral clock enables */
void rcc_apb2_enable(uint32_t mask, int on)
{
    if (on)
        REG32(RCC_BASE + 0x18) = REG32(RCC_BASE + 0x18) | mask;
    else
        REG32(RCC_BASE + 0x18) = REG32(RCC_BASE + 0x18) & ~mask;
}

/* from FUN_0800AD9C: the APB1 peripheral clock enables */
void rcc_apb1_enable(uint32_t mask, int on)
{
    if (on)
        REG32(RCC_BASE + 0x1C) = REG32(RCC_BASE + 0x1C) | mask;
    else
        REG32(RCC_BASE + 0x1C) = REG32(RCC_BASE + 0x1C) & ~mask;
}

/* from FUN_0800AB30: enable an interrupt in the NVIC and set its priority
 * (the original puts the two priority bits into the top of the byte) */
void nvic_enable(uint8_t irq, uint8_t prio)
{
    volatile uint8_t *ipr = (volatile uint8_t *)(NVIC_IPR + (uint32_t)(irq & 0x1Fu) * 4u);

    *ipr = (uint8_t)((*ipr & (uint8_t)~0xC0u) | (uint8_t)((prio & 3u) << 6));
    REG32(NVIC_ISER) = 1u << (irq & 0x1Fu);
}
