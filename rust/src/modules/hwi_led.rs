//! hwiLed - the ARGB LED strip (C: `fw/modules/hwi_led.c`).
//!
//! The dump drives a single-wire ARGB (WS2812/SK6812-class) strip with
//! **TIM15 + DMA1 channel 5**: the timer generates the bit window and
//! the DMA streams the per-bit CCR values into `TIM15_CCR1`, so the CPU never
//! touches the line while a frame is being sent.  PB14 carries the data in
//! **AF1**.
//!
//! Wire encoding, recovered verbatim from `FUN_080090A8` (@0x80090a8) - the
//! function the periodic light-program tick `FUN_08006000` calls to drive the
//! strip.  Each source byte expands into 8 CCR bytes, **MSB first**:
//!   bit = 0  ->  0x0e   (short high time)
//!   bit = 1  ->  0x2e   (long high time)
//! The dump contains a second, unrelated encoder (`FUN_08007830`, 0x60/0x04,
//! LSB first) that feeds a different buffer through `FUN_080078E4` /
//! `FUN_080077A0`; it is not on the light-program path.
//!
//! Frame layout the engine writes (12 LEDs × 3 bytes = 36 bytes per frame,
//! `FUN_080090A8` loops `iVar3 != 0xc` and steps its buffer by 0x18 = 24
//! entries = 3 bytes of source data per LED):
//!   GRB, MSB first, 24 bits per LED.
//!
//! Register evidence:
//!   `FUN_080079A0` @0x80079a0 - the bring-up: GPIOB on, PB14 in AF1
//!       (`FUN_0800AAC8(0x48000400, 0xe, 1)` @0x8007a14), TIM15 on, PSC 29
//!       (`movs r2,#29` @0x80079b8), ARR 0, DMA1 ch5 (base `0x40020058`)
//!       programmed to push 0x10 bytes from `0x20000530` into `0x40014034`
//!       (`&DAT_40014034` is the first word of the struct @0x8007a52).
//!   `FUN_08007B34` @0x8007b34 state 6 - the striped transfer: ARR `0x187`
//!       (`FUN_0800af54(0x40014000,0x66,&DAT,0x187)` @0x8007ba0), CCMR1
//!       `0x60` (OC1M = PWM mode 1, `FUN_0800AFDC`), CCER output enable
//!       on CH1, CCR1 `0xffff` (`FUN_0800B1EC` - the latch gap), DMA count
//!       `DAT_20000588 << 3`, BDTR.MOE, CR1.CEN.  This is the ARR the
//!       gate `CCR1 in [0x5a..0x187]` is written against.
//!   `FUN_080077A0` @0x80077a0 - the single-LED refresh the appLights engine
//!       uses: same timer setup, **ARR `0x308`** (`FUN_0800af54` @0x8007bb0),
//!       DMA count `8 << 3` = the `0x10` of the bring-up.
//!
//! The `PPRT` / `PPRET` markers are emitted when a striped transfer starts
//! and ends (`FUN_08007B34` @0x8007b36 / @0x8007ba8), exactly like the
//! original's output primitives.

use crate::hal::{addr, dma, gpio, rcc, timer};
use crate::modules::ble_com;
use crate::proto::proxy;
use crate::state::Global;

// TIM15 is the ARGB timer in the original (0x40014000)
const LED_TIMER: u32 = addr::TIM15;

// DMA1 channel 5 pushes the CCR stream (dump: base 0x40020058)
const LED_DMA: u32 = addr::DMA1_CH5;

// The data line: PB14 in AF1 (dump: FUN_0800AAC8(0x48000400, 0xe, 1))
const LED_PORT: u32 = addr::GPIOB;
const LED_PIN: u8 = 14;
const LED_AF: u8 = 1;

/// The prescaler of the LED timer: 29, so the timer clock is 96 MHz / 30
/// = 3.2 MHz (dump `FUN_080079A0` @0x80079b8 `movs r2,#29`).
const LED_PSC: u16 = 29;

/// The bit period: ARR = 0x308 - 1 = 775, so one bit takes 776 / 3.2 MHz =
/// **242.5 µs**.
///
/// Dump evidence: `FUN_080077A0` (@0x80077a0) - the function that arms the
/// single-outline transfer the appLights engine feeds - writes `ARR = 0x308`
/// through `FUN_0800af54(0x40014000, 0x308)`.  The timing cross-check is
/// decisive: with the 3.2 MHz timer clock a 288 bit frame (12 LEDs × 3 bytes
/// × 8 bits) then takes 242.5 µs × 288 = **69.8 ms**, and the state machine
/// stalls for exactly `0x46` = 70 ms around a striped transfer
/// (`FUN_08007B34` state 4) - a 0.2 % match, which no other ARR in the image
/// reproduces (0x187 -> 35.3 ms, 0x66 -> 9.3 ms, 0xffff -> 5.9 s).
///
/// This is **not** the WS2812 1.25 µs bit window - it is 242.5 µs, i.e. a
/// ~4 kHz bit rate, so the strip must be driven as a fixed-on/fixed-off
/// waveform rather than timed edges.
const LED_ARR: u16 = 0x308 - 1;

/// The CCR values of a `0` and a `1` bit.
///
/// Dump evidence, `FUN_080090A8` (@0x80090a8) - the function the periodic
/// light-program tick `FUN_08006000` drives the strip through:
/// ```text
/// bit == 0  ->  0x0e
/// bit == 1  ->  0x2e
/// ```
/// With the 3.2 MHz timer clock and the 242.5 µs bit window these give
///   0x0e = 14 -> 15/3.2 MHz = 4.7 µs high   (the *short* pulse, a `0`)
///   0x2e = 46 -> 47/3.2 MHz = 14.7 µs high  (the *long* pulse, a `1`)
/// i.e. a ~4 kHz serial rate with a 4.7/14.7 µs high-time code, matching the
/// 70 ms frame stall of the state machine.
/// A *second*, independent encoder exists in the dump - `FUN_08007830`
/// (@0x8007830) uses 0x60/0x04 and emits LSB first - but it feeds a
/// different buffer (`DAT_20000530`) through `FUN_080078E4`/`FUN_080077A0`
/// and is not on the light-program path.  The constants here are the ones
/// the appLights engine actually uses.
const BIT_ZERO: u8 = 0x0e;
const BIT_ONE: u8 = 0x2e;

/// The number of LEDs on the strip: 12 (dump `FUN_080090A8` @0x80090a8 loops
/// `iVar3 != 0xc`).
pub const LED_COUNT: usize = 12;

/// The bytes one LED needs: GRB order (dump `FUN_080090A8` steps its source
/// buffer by 3 bytes per LED).
const LED_BYTES: usize = 3;

/// The CCR entries for the whole strip (8 per byte, 24 per LED).
const CCR_ENTRIES: usize = LED_COUNT * LED_BYTES * 8;

/// The trailing low period that latches the frame: WS2812 needs > 50 µs of
/// low after the last bit.  The original achieves it by pausing the transfer
/// for 0x46 = 70 ms (`FUN_08007B34` state 4) / 0x13b = 315 ms (state 5)
/// before and after the striped transfer; [`send_frame`] re-arms the timer
/// with the counter reset afterwards, which latches on hardware.

/// The CCR stream the DMA feeds into TIM15_CCR1.
///
/// Aligned to 4 bytes so the DMA does not have to handle a misaligned source.
#[repr(align(4))]
struct CcrStream([u8; CCR_ENTRIES]);

/// The state of the module (the C file scope variables).
struct State {
    led_ready: bool,
    /// the duty of each LED, 0..255 (the original's `led_duty[]`)
    led_duty: [u8; LED_COUNT],
    /// the last RGB triple written per LED (the original's `DAT_20000530`
    /// frame buffer, GRB order)
    led_rgb: [[u8; LED_BYTES]; LED_COUNT],
    /// the CCR stream of the pending frame
    ccr: CcrStream,
}

static S: Global<State> = Global::new(State {
    led_ready: false,
    led_duty: [0; LED_COUNT],
    led_rgb: [[0; LED_BYTES]; LED_COUNT],
    ccr: CcrStream([0; CCR_ENTRIES]),
});

/// `FUN_080079A0` - the ARGB bring-up.
pub fn init() {
    if S.get().led_ready {
        return;
    }
    proxy::register(proxy::Iface { name: "hwiLed", handler });

    // RCC: GPIOA/GPIOB and the LED timer (APB2ENR 0x20000) - the values of
    // the original.
    rcc::ahb_en(0x0006_0000, true);
    rcc::apb2_en(0x0002_0000, true);
    // DMA1 needs its clock too - the dump's turns it on from `FUN_080071C0`'s
    // neighbour before it touches the channel registers.
    rcc::ahb_en(0x0000_0001, true);

    // The data line: PB14, alternate function AF1, open drain, fast edge.
    let cfg = gpio::PerPinCfg {
        mask: 1u32 << LED_PIN,
        mode: gpio::MODE_ALT,
        speed: 3,
        otype: 1,
        pull: gpio::PULL_NONE,
    };
    gpio::config_per_pin(LED_PORT, &cfg);
    gpio::set_afr(LED_PORT, LED_PIN, LED_AF);

    // TIM15 base: PSC 29, ARR = the 1.25 µs bit period, CR1 = 0.
    let base = timer::BaseCfg {
        psc: LED_PSC,
        cr1_hi: 0,
        arr: LED_ARR as u32,
        cr1_ckd: 0,
        rcr: 0,
    };
    timer::base_cfg(LED_TIMER, &base);

    // Channel 1: PWM mode 1 (OC1M = 6 -> low byte of CCMR1 = `0x60`), CC1E
    // set (`FUN_08007B34` state 6 -> `FUN_0800AFDC` then `FUN_0800B3C8`).
    // `channel_cfg` clears OC1PE as part of its 0x73 mask, so it must NOT be
    // called again afterwards - that would set bit 3 and turn the mode into
    // 0x07 (PWM mode 2) on the wire.
    let ch = timer::ChanCfg {
        ccmr: 0x60,
        ccer: 0x0001, // CC1E - the channel output is enabled
        ccer2: 0,
        ccer3: 0,
        ccer4: 0,
        ccer5: 0,
        ccr: 0,
        cr2: 0,
        cr2b: 0,
    };
    timer::channel_cfg(LED_TIMER, 1, &ch);

    // BDTR.MOE - the advanced timer output enable (FUN_0800AFBC).
    timer::moe(LED_TIMER, true);

    // The DMA channel: memory -> peripheral, 8 bit at a time, memory
    // incrementing, circular off, high priority (the dump's
    // `FUN_0800A840` struct: `0x3000 | 1 | 0x80` = MEM2MEM off, MSIZE/PSIZE
    // 00, MINC, CIRC off, DIR read-from-memory, TCIE off, priority 11).
    dma::channel_cfg(LED_DMA, LED_TIMER + addr::TIM_CCR1, S.get().ccr.0.as_ptr() as u32, 0, 0x0000_3081);
    dma::channel_enable(LED_DMA, true);

    timer::start(LED_TIMER); // FUN_0800AFA0
    S.get().led_ready = true;
}

/// Expand the frame buffer into the CCR stream (the dump's `FUN_080090A8`).
///
/// **MSB first**: bit `0x80` of a byte becomes the first CCR entry of the
/// eight, bit `0x01` the last - the order `FUN_080090A8` tests its masks in
/// (`& 0x80`, `& 0x40`, ... `& 1`).  A byte of a LED therefore occupies CCR
/// entries `[8*byte] .. [8*byte+7]`, and the buffer is stepped 3 bytes
/// (= 24 entries) per LED.
fn fill_ccr_stream(st: &mut State) {
    let mut idx = 0usize;
    for led in 0..LED_COUNT {
        for b in 0..LED_BYTES {
            let byte = st.led_rgb[led][b];
            let mut shift = 8;
            while shift > 0 {
                shift -= 1;
                st.ccr.0[idx] = if byte & (1 << shift) != 0 { BIT_ONE } else { BIT_ZERO };
                idx += 1;
            }
        }
    }
}

/// Send one frame: program the DMA count and re-arm the timer.
///
/// The dump does exactly this in `FUN_08007B34` state 6:
/// ```text
/// FUN_0800af54(0x40014000, 0x66, ...);  // timer base re-stated
/// FUN_0800AFDC(0x40014000, ...);         // CCMR1 = 0x60
/// FUN_0800B3C8(0x40014000, 0x100, 1);    // CCER CH1 output enable
/// FUN_0800a89c(0x40020058, n << 3);      // DMA count (the *LED count* << 3)
/// FUN_0800b1ec(0x40014000, 0xffff);      // CCR1 = 0xffff (reset time)
/// FUN_0800A87C(0x40020058, 1);           // DMA channel on
/// _DAT_40014024 = 0;                     // CNT = 0
/// FUN_0800AFA0(0x40014000, 1);           // CEN
/// FUN_0800b3a4(0x40014000, 1);           // UG
/// FUN_0800AFBC(0x40014000, 1);           // MOE
/// ```
fn send_frame() {
    // `CCR_ENTRIES` is already the byte count (12 LEDs × 3 bytes × 8 bits =
    // 288), so the DMA is programmed with it directly.  The dump's
    // `DAT_20000588 << 3` performs the same scaling on an LED *count* before
    // handing it to the same register - doing it again here would send eight
    // copies of the frame.
    dma::set_cntdr(LED_DMA, CCR_ENTRIES as u16);
    timer::pwm_set(LED_TIMER, 1, 0xFFFF); // the latch/reset pulse
    timer::set32(LED_TIMER, addr::TIM_CNT, 0);
    dma::channel_enable(LED_DMA, true);
    timer::egr(LED_TIMER, 1); // UG so the reload registers take effect
    timer::start(LED_TIMER);
    timer::moe(LED_TIMER, true);
}

/// `hwiLed_set` - set one LED to a full or zero duty.
pub fn set(led: u8, on: bool) -> i32 {
    if !S.get().led_ready {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    S.get().led_duty[led as usize] = if on { 255 } else { 0 };
    let v = S.get().led_duty[led as usize];
    S.get().led_rgb[led as usize] = [v, v, v];
    fill_and_send();
    0
}

/// `hwiLed_set_pwm` - set one LED to a scaled duty (0..255).
pub fn set_pwm(led: u8, duty: u8) -> i32 {
    if !S.get().led_ready {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    S.get().led_duty[led as usize] = duty;
    S.get().led_rgb[led as usize] = [duty, duty, duty];
    fill_and_send();
    0
}

/// `hwiLed_set_rgb` - set one LED to an explicit RGB colour.
///
/// The wire order is GRB (WS2812): the green byte goes out first, then red,
/// then blue.  The `app_lights` programs and the dump's own frame buffer use
/// exactly that order.
pub fn set_rgb(led: u8, r: u8, g: u8, b: u8) -> i32 {
    if !S.get().led_ready {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    S.get().led_rgb[led as usize] = [g, r, b];
    S.get().led_duty[led as usize] = (r as u16 / 3 + g as u16 / 3 + b as u16 / 3) as u8;
    fill_and_send();
    0
}

fn fill_and_send() {
    let st = S.get();
    fill_ccr_stream(st);
    send_frame();
}

/// `hwiLed_pattern_request` - prints "PPRT\n" (the transfer starts).
pub fn pattern_request() {
    ble_com::out_str("PPRT\n");
}

/// `hwiLed_pattern_done` - prints "PPRET\n" (the transfer ended).
pub fn pattern_done() {
    ble_com::out_str("PPRET\n");
}

/// The byte offset of the second whitespace separated token of `s` (the
/// value the C reaches with `args + 1` after its `hw_parse_args`).
fn skip_word(s: &str) -> usize {
    match s.find(' ') {
        Some(i) => i + 1,
        None => s.len(),
    }
}

/// `hwiLed_get` - the current duty of a LED.
pub fn get(led: u8) -> i32 {
    if led as usize >= LED_COUNT {
        return -1;
    }
    S.get().led_duty[led as usize] as i32
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */

/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    use crate::hal::util;
    use crate::proto::{fmt, proxy};

    let mut v = [0i32; 4];
    let n = util::parse_args(args, &mut v);

    if opname == "on" {
        let led = if n > 0 && v[0] >= 0 { v[0] as u8 } else { 0 };
        if op == proxy::OP_GET {
            let buf = fmt::int(get(led));
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
            return;
        }
        // the second token is the flag, exactly like the C
        let on = if n > 1 { util::is_on(&args[skip_word(args)..]) } else { true };
        set(led, on);
        let buf = fmt::int(if on { 1 } else { 0 });
        proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        return;
    }

    if opname == "pwm" {
        let led = if n > 0 && v[0] >= 0 { v[0] as u8 } else { 0 };
        if op == proxy::OP_GET {
            let buf = fmt::int(get(led));
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
            return;
        }
        let mut duty = if n > 1 { v[1] } else { 0 };
        if duty < 0 {
            duty = 0;
        }
        if duty > 255 {
            duty = 255;
        }
        set_pwm(led, duty as u8);
        let buf = fmt::int(duty);
        proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        return;
    }

    if opname == "rgb" {
        let led = if n > 0 && v[0] >= 0 { v[0] as u8 } else { 0 };
        if op == proxy::OP_GET {
            let buf = fmt::int(get(led));
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
            return;
        }
        let mut ch = [0i32; 3];
        for i in 0..3 {
            ch[i] = if n > 1 + i { v[1 + i].clamp(0, 255) } else { 0 };
        }
        set_rgb(led, ch[0] as u8, ch[1] as u8, ch[2] as u8);
        // the reply is the packed colour, hexadecimal - the radix every other
        // value reply in this firmware uses
        let packed = ((ch[0] as u32) << 16) | ((ch[1] as u32) << 8) | ch[2] as u32;
        let buf = fmt::num(16, packed);
        proxy::resp_value(op, "hwiLed", opname, buf.as_str());
        return;
    }

    if op == proxy::OP_GET {
        // count the dark LEDs, exactly like the C reconstruction
        let mut off = 0i32;
        for i in 0..LED_COUNT {
            if S.get().led_duty[i] == 0 {
                off += 1;
            }
        }
        let buf = fmt::int(off);
        proxy::resp_value(op, "hwiLed", opname, buf.as_str());
    }
}
