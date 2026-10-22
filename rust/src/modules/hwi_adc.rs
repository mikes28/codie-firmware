//! hwiAdc - the raw ADC channels of the robot (C: `fw/modules/hwi_adc.c`).
//!
//! Original handler : FUN_080073DC (0x080073DD), registered as "hwiAdc"
//! Original init    : FUN_08007068 (hardware setup) + FUN_08007168 (start)
//! Original filter  : FUN_080071DC (average / mean deviation of a channel)
//!
//! Wire behaviour of the original (reproduced verbatim):
//!
//!   "get hwiAdc buf"  ->  the 32 raw samples of the DMA buffer, one per
//!                         line.  After the 9th, the 17th and the 25th value
//!                         an additional empty line is emitted.  The reply
//!                         carries NO "get hwiAdc buf" prefix - the original
//!                         writes the numbers straight to the output.
//!   every other op    ->  ignored, no reply.
//!
//! Hardware (all of it recovered from the original binary):
//!   ADC1 (0x40012400), four channels selected with the masks 0x02, 0x04,
//!   0x20 and 0x40 (ADC1_IN1, IN2, IN5 and IN6), GPIOA pins 1, 2, 5 and 6
//!   in analog mode, DMA1 channel 2 (0x4002001C) copies the results into a
//!   32 word buffer, 8 rounds of the four channels.

use crate::hal::{addr, adc, dma, gpio, rcc};
use crate::mmio;
use crate::modules::ble_com::{out_char, out_str};
use crate::proto::{fmt, proxy};
use crate::state::Global;

/// 4 channels x 8 rounds
const ADC_SAMPLES: usize = 32;
const ADC_CHANNELS: usize = 4;

/// the four ADC channels the original uses (the masks are the ones the
/// original keeps in its .data section)
const CHAN_MASK: [u8; ADC_CHANNELS] = [0x02, 0x04, 0x20, 0x40];
const CHAN_NUM: [u8; ADC_CHANNELS] = [1, 2, 5, 6];

/// The state of the module (the C file scope variables).
struct State {
    /// the DMA target: 32 half words, 8 rounds of the 4 channels
    samples: [u16; ADC_SAMPLES],
    /// 0/1 = not initialised, 2 = hardware set up, +4 = scan running
    /// (the original keeps this state in a byte of its .data)
    state: u8,
}

static S: Global<State> = Global::new(State { samples: [0; ADC_SAMPLES], state: 0 });

/// `hwiAdc_init`
pub fn init() {
    let st = S.get();

    if st.state < 2 {
        adc_hw_init(st.samples.as_ptr() as u32);
        st.state = 2;
    }
    if (st.state & 4) == 0 {
        adc_hw_start();
        st.state |= 4;
    }

    proxy::register(proxy::Iface { name: "hwiAdc", handler: handler });
}

/// `hwiAdc_read`
pub fn read(channel: u8) -> u16 {
    let st = S.get();
    let index = chan_index(channel);

    if index == 0xFF {
        return 0;
    }
    chan_average(st, index)
}

/// `hwiAdc_read_mv`
pub fn read_mv(channel: u8) -> u16 {
    // 12 bit converter, 3.3 V reference (the original leaves the
    // resolution bits of CFGR1 at their reset value = 12 bit)
    ((read(channel) as u32 * 3300) / 4095) as u16
}

/// the interface handler of the module
///
/// from FUN_080073DC
pub fn handler(op: u8, opname: &str, _args: &str) {
    if op != proxy::OP_GET {
        return;
    }
    if opname != "buf" {
        return;
    }

    let st = S.get();

    for i in 0..ADC_SAMPLES {
        let buf = fmt::int(sample(st, i) as i32);
        out_str(buf.as_str());
        out_char(b'\n');
        // the original adds an empty line every eight values
        //
        // NOTE: the C reconstruction tests `(i & 8u) == 0u` here, which
        // would emit an empty line after the 2nd .. 8th and the 17th ..
        // 24th value as well.  The original binary (FUN_080073DC) tests
        // `(i & 7) == 0` and the C comment of the same file says "after
        // 8, 16 and 24" - the mask of the C is a typo, the behaviour of
        // the original is reproduced here (the empty line comes after the
        // 9th, the 17th and the 25th value).
        if i != 0 && (i & 7) == 0 {
            out_char(b'\n');
        }
    }
}

// ------------------------------------------------------------------
// hardware
// ------------------------------------------------------------------

/// from FUN_08007068: the ADC1 setup
///
/// NOTE: the C reads the address of the sample buffer itself
/// (`(uint32_t)(uintptr_t)s_samples`); the Rust port gets the address
/// from the caller, because the buffer lives inside the global state.
fn adc_hw_init(samples: u32) {
    let gpio = gpio::Cfg {
        // GPIOA: PA1, PA2, PA5 and PA6 as analog inputs
        pins: 0x0066,
        mode: gpio::MODE_ANALOG,
        ospeed: 1,
        otype: 0,
        pupd: 0,
    };

    rcc::ahb_en(0x0002_0000, true); // FUN_0800AD5C: ADC clock
    gpio::config(addr::GPIOA, &gpio); // FUN_0800A964

    // FUN_0800A67C -> FUN_0800ADBC: the original pulses bit 9 of the RCC
    // register at offset 0x0C (set and immediately cleared again).
    mmio::rmw32(addr::RCC + 0x0C, |v| v | 0x0000_0200);
    mmio::rmw32(addr::RCC + 0x0C, |v| v & !0x0000_0200);

    rcc::apb2_en(0x0000_0200, true); // FUN_0800AD7C

    // FUN_0800ac0c: the ADC clock prescaler (CFGR1) and the second clock
    // control register of the RCC.
    mmio::rmw32(addr::RCC + 0x04, |v| 0x0000_4000 | (v & 0xFFFF_BFFF));
    mmio::rmw32(addr::RCC + 0x30, |v| 0x0000_0100 | (v & 0xFFFF_FEFF));

    // FUN_0800AE14: the original enables bit 8 of the register block at
    // 0x40010000 before the DMA is started.
    mmio::rmw32(addr::SYSCFG, |v| v | 0x0000_0100);

    // DMA1 channel 2 (0x4002001C): the data register of the ADC is the
    // source, the sample buffer the destination, 32 transfers.
    // The configuration word is the original's (see FUN_0800A840).
    dma::channel_cfg(
        addr::DMA1_CH2,
        addr::ADC1 + 0x40,
        samples,
        ADC_SAMPLES as u16,
        0x0000_15A0,
    );

    // the ADC configuration register (FUN_0800A6A0): only bit 13 is set by
    // the original, everything else keeps its reset value.
    adc::cfg1(addr::ADC1, 0xFFFF_D203, 0, 1);

    // the four channels with their sample times (FUN_0800a6f8).  The
    // original writes the sample time register as a whole, so the last
    // write (6) is the one that stays.
    adc::channel_add(addr::ADC1, CHAN_MASK[0], 4);
    adc::channel_add(addr::ADC1, CHAN_MASK[1], 6);
    adc::channel_add(addr::ADC1, CHAN_MASK[2], 6);
    adc::channel_add(addr::ADC1, CHAN_MASK[3], 6);

    adc::dmngt(addr::ADC1, 2); // FUN_0800A77C
}

/// from FUN_08007168: start the continuous scan
fn adc_hw_start() {
    let _ = adc::start(addr::ADC1); // FUN_0800A704: one conversion
    adc::enable(addr::ADC1, true); // FUN_0800A6DC: CR.ADEN
    while !adc::ready(addr::ADC1, 1) {
        // FUN_0800A78C: wait for ADEN
    }
    dma::channel_enable(addr::DMA1_CH2, true); // FUN_0800A87C
    adc::dma_mode(addr::ADC1, true); // FUN_0800A760: CFGR1.DMNGT
    adc::stop(addr::ADC1); // FUN_0800a754: CR bit 2
}

// ------------------------------------------------------------------
// reading
// ------------------------------------------------------------------

/// index of an ADC channel number in the configured list, 0xFF if the
/// channel is not scanned
fn chan_index(channel: u8) -> u8 {
    for i in 0..ADC_CHANNELS {
        if CHAN_NUM[i] == channel {
            return i as u8;
        }
    }
    0xFF
}

/// read one sample of the DMA buffer
///
/// The C buffer is `volatile` (the DMA writes it in the background), the
/// Rust port reads it the same way.
#[inline]
fn sample(st: &State, i: usize) -> u16 {
    // SAFETY: the buffer is a static, the index is below ADC_SAMPLES.
    unsafe { core::ptr::read_volatile(st.samples.as_ptr().add(i)) }
}

/// from FUN_080071DC with mode 0: the average of the eight samples of one
/// channel.  The buffer holds the channels interleaved (round robin), so
/// the samples of a channel are four words apart.
fn chan_average(st: &State, index: u8) -> u16 {
    let mut sum: u32 = 0;

    for i in 0..8 {
        sum += sample(st, index as usize + i * ADC_CHANNELS) as u32;
    }

    ((sum + 4) >> 3) as u16
}
