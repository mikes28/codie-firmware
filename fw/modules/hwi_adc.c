/* hwi_adc.c - hwiAdc: the raw ADC channels of the robot.
 *
 * Original handler : FUN_080073DC (0x080073DD), registered as "hwiAdc"
 * Original init    : FUN_08007068 (hardware setup) + FUN_08007168 (start)
 * Original filter  : FUN_080071DC (average / mean deviation of a channel)
 *
 * Wire behaviour of the original (reproduced verbatim):
 *
 *   "get hwiAdc buf"  ->  the 32 raw samples of the DMA buffer, one per
 *                         line.  After the 9th, the 17th and the 25th value
 *                         an additional empty line is emitted.  The reply
 *                         carries NO "get hwiAdc buf" prefix - the original
 *                         writes the numbers straight to the output.
 *   every other op    ->  ignored, no reply.
 *
 * Hardware (all of it recovered from the original binary):
 *   ADC1 (0x40012400), four channels selected with the masks 0x02, 0x04,
 *   0x20 and 0x40 (ADC1_IN1, IN2, IN5 and IN6), GPIOA pins 1, 2, 5 and 6
 *   in analog mode, DMA1 channel 2 (0x4002001C) copies the results into a
 *   32 word buffer, 8 rounds of the four channels.
 */
#include "codie.h"
#include "hwi_a_hal.h"
#include <string.h>

#define ADC_SAMPLES     32u   /* 4 channels x 8 rounds                     */
#define ADC_CHANNELS    4u

/* the four ADC channels the original uses (the masks are the ones the
 * original keeps in its .data section) */
static const uint8_t s_chan_mask[ADC_CHANNELS] = { 0x02u, 0x04u, 0x20u, 0x40u };
static const uint8_t s_chan_num[ADC_CHANNELS]  = { 1u, 2u, 5u, 6u };

/* the DMA target: 32 half words, 8 rounds of the 4 channels */
static volatile uint16_t s_samples[ADC_SAMPLES];

/* 0/1 = not initialised, 2 = hardware set up, +4 = scan running
 * (the original keeps this state in a byte of its .data) */
static uint8_t s_state;

/* forward */
static void hwiAdc_handler(uint8_t op, const char *opname, const char *args);

static const iface_t s_iface = { "hwiAdc", hwiAdc_handler };

/* ------------------------------------------------------------------ *
 * hardware
 * ------------------------------------------------------------------ */

/* from FUN_08007068: the ADC1 setup */
static void adc_hw_init(void)
{
    gpio_cfg_t gpio;

    /* GPIOA: PA1, PA2, PA5 and PA6 as analog inputs */
    gpio.pins   = 0x0066u;
    gpio.mode   = 3u;      /* analog */
    gpio.ospeed = 1u;
    gpio.otype  = 0u;
    gpio.pupd   = 0u;

    rcc_ahb_enable(0x00020000u, 1);         /* FUN_0800AD5C: ADC clock  */
    gpio_config(GPIOA_BASE, &gpio);         /* FUN_0800A964             */

    /* FUN_0800A67C -> FUN_0800ADBC: the original pulses bit 9 of the RCC
     * register at offset 0x0C (set and immediately cleared again). */
    *(volatile uint32_t *)(RCC_BASE + 0x0Cu) |= 0x00000200u;
    *(volatile uint32_t *)(RCC_BASE + 0x0Cu) &= ~0x00000200u;

    rcc_apb2_enable(0x00000200u, 1);        /* FUN_0800AD7C             */

    /* FUN_0800ac0c: the ADC clock prescaler (CFGR1) and the second clock
     * control register of the RCC. */
    {
        volatile uint32_t *cfgr1 = (volatile uint32_t *)(RCC_BASE + 0x04u);
        volatile uint32_t *cfgr2 = (volatile uint32_t *)(RCC_BASE + 0x30u);
        *cfgr1 = 0x00004000u | (*cfgr1 & 0xFFFFBFFFu);
        *cfgr2 = 0x00000100u | (*cfgr2 & 0xFFFFFEFFu);
    }

    /* FUN_0800AE14: the original enables bit 8 of the register block at
     * 0x40010000 before the DMA is started. */
    *(volatile uint32_t *)0x40010000u |= 0x00000100u;

    /* DMA1 channel 2 (0x4002001C): the data register of the ADC is the
     * source, the sample buffer the destination, 32 transfers.
     * The configuration word is the original's (see FUN_0800A840). */
    dma_channel_cfg(DMA1_CH2, ADC1_BASE + 0x40u, (uint32_t)(uintptr_t)s_samples,
                    (uint16_t)ADC_SAMPLES, 0x000015A0u);

    /* the ADC configuration register (FUN_0800A6A0): only bit 13 is set by
     * the original, everything else keeps its reset value. */
    adc_cfg1(ADC1_BASE, 0xFFFFD203u, 0u, 1u);

    /* the four channels with their sample times (FUN_0800a6f8).  The
     * original writes the sample time register as a whole, so the last
     * write (6) is the one that stays. */
    adc_channel_add(ADC1_BASE, s_chan_mask[0], 4u);
    adc_channel_add(ADC1_BASE, s_chan_mask[1], 6u);
    adc_channel_add(ADC1_BASE, s_chan_mask[2], 6u);
    adc_channel_add(ADC1_BASE, s_chan_mask[3], 6u);

    adc_dmngt(ADC1_BASE, 2u);               /* FUN_0800A77C             */
}

/* from FUN_08007168: start the continuous scan */
static void adc_hw_start(void)
{
    adc_start(ADC1_BASE);                   /* FUN_0800A704: one conversion */
    adc_enable(ADC1_BASE, 1);               /* FUN_0800A6DC: CR.ADEN      */
    while (!adc_ready(ADC1_BASE, 1u))        /* FUN_0800A78C: wait for ADEN */
        ;
    dma_channel_enable(DMA1_CH2, 1);        /* FUN_0800A87C               */
    adc_dma_mode(ADC1_BASE, 1);             /* FUN_0800A760: CFGR1.DMNGT  */
    adc_stop(ADC1_BASE);                    /* FUN_0800a754: CR bit 2     */
}

void hwiAdc_init(void)
{
    if (s_state < 2u) {
        adc_hw_init();
        s_state = 2u;
    }
    if ((s_state & 4u) == 0u) {
        adc_hw_start();
        s_state |= 4u;
    }
    iface_register(&s_iface);
}

/* ------------------------------------------------------------------ *
 * reading
 * ------------------------------------------------------------------ */

/* index of an ADC channel number in the configured list, 0xFF if the
 * channel is not scanned */
static uint8_t chan_index(uint8_t channel)
{
    uint8_t i;

    for (i = 0; i < ADC_CHANNELS; i++) {
        if (s_chan_num[i] == channel)
            return i;
    }
    return 0xFFu;
}

/* from FUN_080071DC with mode 0: the average of the eight samples of one
 * channel.  The buffer holds the channels interleaved (round robin), so
 * the samples of a channel are four words apart. */
static uint16_t chan_average(uint8_t index)
{
    uint32_t sum = 0;
    uint8_t  i;

    for (i = 0; i < 8u; i++)
        sum += s_samples[index + (uint8_t)(i * ADC_CHANNELS)];

    return (uint16_t)((sum + 4u) >> 3);
}

uint16_t hwiAdc_read(uint8_t channel)
{
    uint8_t index = chan_index(channel);

    if (index == 0xFFu)
        return 0;
    return chan_average(index);
}

uint16_t hwiAdc_read_mv(uint8_t channel)
{
    /* 12 bit converter, 3.3 V reference (the original leaves the
     * resolution bits of CFGR1 at their reset value = 12 bit) */
    return (uint16_t)(((uint32_t)hwiAdc_read(channel) * 3300u) / 4095u);
}

/* ------------------------------------------------------------------ *
 * wire interface
 * ------------------------------------------------------------------ */

/* from FUN_080073DC */
static void hwiAdc_handler(uint8_t op, const char *opname, const char *args)
{
    char    buf[10];
    uint8_t i;

    (void)args;

    if (op != OP_GET)
        return;
    if (strcmp(opname, "buf") != 0)
        return;

    for (i = 0; i < (uint8_t)ADC_SAMPLES; i++) {
        fmt_int(buf, (int)sizeof(buf), (int32_t)s_samples[i]);
        out_str(buf);
        out_char('\n');
        /* the original adds an empty line every eight values (the test
         * is on bit 3 of the index, so after 8, 16 and 24) */
        if (i != 0u && (i & 8u) == 0u)
            out_char('\n');
    }
}
