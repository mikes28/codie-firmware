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

use crate::hal::{addr, dma, gpio, nvic, rcc, timer, util};
use crate::mmio;
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

struct State {
    led_ready: bool,
    /// the duty of each LED, 0..255 (the original's `led_duty[]`)
    led_duty: [u8; LED_COUNT],
    /// the last RGB triple written per LED (the original's `DAT_20000385`
    /// frame buffer, GRB order)
    led_rgb: [[u8; LED_BYTES]; LED_COUNT],
    /// the CCR stream of the pending frame
    ccr: CcrStream,
    /// the single-outline sequencer (dump `DAT_2000051a` and friends)
    seq: Sequencer,
    /// global brightness multiplier applied to all LED outputs (0..255, 255 = full)
    brightness: u8,
}

static S: Global<State> = Global::new(State {
    led_ready: false,
    led_duty: [0; LED_COUNT],
    led_rgb: [[0; LED_BYTES]; LED_COUNT],
    ccr: CcrStream([0; CCR_ENTRIES]),
    seq: Sequencer {
        state: 0,
        reply: [0; 8],
        reply_byte: 0,
        reply_bit: 0,
        reply_expect: 0,
        stage: 0,
        stamp: 0,
        block: [0; 5],
        stream: [0; SECOND_STREAM_SIZE],
    },
    brightness: 255,
});

/// The state of the second-encoder / single-outline sequencer.
///
/// Mirrors the dump's `DAT_2000051a` (sequencer phase), `DAT_2000058a`
/// (the reply bytes bit-banged in on PB14), `DAT_20000589` (the reply
/// byte count the sequencer should capture), `DAT_20000594`/`DAT_20000596`
/// (reply byte/bit index), `DAT_2000051c` (the sequencer stage), and
/// `DAT_20000528` (the uptime stamp of the last phase change).
struct Sequencer {
    state: u8,        // DAT_2000051a: 0 = idle
    reply: [u8; 8],   // DAT_2000058a
    reply_byte: u8,   // _DAT_20000594
    reply_bit: u8,    // _DAT_20000596
    reply_expect: u8, // DAT_20000589
    stage: u32,       // _DAT_2000051c
    stamp: u32,       // DAT_20000528
    block: [u16; 5],  // DAT_20000510
    stream: [u8; SECOND_STREAM_SIZE],
}



/// `FUN_080079A0` - the ARGB bring-up.
pub fn init() {
    let st = S.get();
    st.led_ready = true;
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
    // NVIC: DMA1 channel 5 transfer complete (IRQ 11) terminates the
    // striped transfer; TIM15 update (IRQ 20) sequences the single-outline
    // phases and bit-bangs the battery-pack reply in on PB14.
    nvic::enable(11, 2);
    nvic::enable(20, 2);

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
    let brightness = st.brightness;
    let mut idx = 0usize;
    for led in 0..LED_COUNT {
        let (r, g, b) = apply_brightness(st.led_rgb[led][0], st.led_rgb[led][1], st.led_rgb[led][2], brightness);
        let bytes = [g, r, b]; // GRB order on wire
        for byte in bytes {
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
    timer::set16(LED_TIMER, addr::TIM_ARR, LED_ARR); // LED bit period
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
/// The CCR buffer of the second encoder (dump `DAT_20000530`): 8 entries per
/// source byte, enough for the longest pack frame (8 bytes).
const SECOND_STREAM_SIZE: usize = 64;

/// Expand `src` into `dst` with the dump's second encoder (`FUN_08007830`):
/// LSB first, `0x60` for a clear bit and `4` for a set bit, with bit 7 of
/// each byte inverted (set -> `0x60`, clear -> `4`).
fn fill_ccr_stream_lsb(src: &[u8], dst: &mut [u8]) {
    let mut di = 0usize;
    for &byte in src {
        for shift in 0..7 {
            dst[di] = if byte & (1u8 << shift) != 0 { 4 } else { 0x60 };
            di += 1;
        }
        dst[di] = if byte & 0x80 != 0 { 0x60 } else { 4 };
        di += 1;
    }
}

/// `FUN_080078E4` / `FUN_080077A0` - send `bytes` over the LED line with the
/// second encoder, armed as TIM15's single-outline DMA transfer (ARR 0x308,
/// CCR1 = 0, DMA count = `len << 3`).  `reply` is the number of reply bytes
/// the update interrupt should bit-bang back on PB14 afterwards (0 = none;
/// values >= 0x80 mean "no reply").
///
/// Returns false while the sequencer is busy (a transfer is in progress).
pub fn send_raw(bytes: &[u8], reply: u8) -> bool {
    let ptr = S.get();
    if ptr.seq.state != 0 || bytes.len() > SECOND_STREAM_SIZE / 8 {
        return false;
    }
    let seq = &mut ptr.seq;
    fill_ccr_stream_lsb(bytes, &mut seq.stream);
    seq.reply_expect = reply;
    seq.reply_byte = 0;
    seq.reply_bit = 0;
    let stream_ptr = seq.stream.as_ptr() as u32;
    let count = (bytes.len() * 8) as u16;
    // FUN_080077A0: arm the single-outline transfer.
    timer::set16(LED_TIMER, addr::TIM_CNT, 0);
    timer::sr_clear(LED_TIMER, 1); // UIF
    timer::sr_clear(LED_TIMER, 2); // CC1IF
    timer::dier(LED_TIMER, 2, false); // CC1IE off
    timer::one_pulse(LED_TIMER, true); // OPM on (FUN_0800af90 with 8)
    ptr.seq.stamp = util::millis();
    ptr.seq.stage = 0;
    timer::set16(LED_TIMER, addr::TIM_ARR, 0x308);
    timer::pwm_set(LED_TIMER, 1, 0);
    timer::dier(LED_TIMER, 1, true);  // UIE on
    timer::moe(LED_TIMER, true);
    timer::enable(LED_TIMER, true);
    timer::egr(LED_TIMER, 1); // UG
    // CC1E on, CC1NE on (FUN_0800B3C8(TIM15, 0x100, 1))
    mmio::rmw16(LED_TIMER + addr::TIM_CCER, |v| v | 0x0001 | 0x0100);
    // FUN_0800b260: the sequencer block (DAT_20000510)
    ptr.seq.block = [0, 0, 1, 0, 0];
    // Reprogram the DMA channel to stream the pack frame (the dump's
    // FUN_080078E4 selects the buffer per transfer).
    dma::set_cntdr(LED_DMA, count);
    mmio::w32(LED_DMA + 0x0C, stream_ptr); // CMAR -> seq.stream
    ptr.seq.state = 2;
    core::sync::atomic::compiler_fence(core::sync::atomic::Ordering::SeqCst);
    true
}

/// Take the reply bytes captured by the update interrupt's PB14 bit-bang,
/// once the sequencer reached phase 16.  None while idle or busy.
pub fn reply_take() -> Option<[u8; 8]> {
    let st = S.get();
    if st.seq.state == 16 {
        Some(st.seq.reply)
    } else {
        None
    }
}
/// Test hook: drive the single-outline sequencer as if `tim15_update_irq`
/// (IRQ 20) had run to completion - the interrupt that bit-bangs the
/// battery-pack reply bits in on PB14 and parks the sequencer in phase 16,
/// so that [`reply_take`] returns `Some`.  That interrupt never fires in
/// host tests, so without this hook `reply_take` always returns `None` and
/// the pack-reply machine stalls at states 4 and 7.
///
/// If `reply` is `Some`, up to 8 bytes are stored as the captured reply and
/// the sequencer is left in phase 16, so [`reply_take`] returns `Some`.
/// If `reply` is `None`, the transfer completes with no reply and the
/// sequencer returns to idle (phase 0).
///
/// No-ops (returns false) while a transfer is mid-flight (any state other
/// than 0 or 2), matching the hardware where the interrupt only completes a
/// transfer that was armed by [`send_raw`].
pub fn simulate_update_irq(reply: Option<&[u8]>) -> bool {
    let ptr = S.get();
    match reply {
        Some(bytes) => {
            // Injecting a captured reply requires the sequencer to be idle
            // or armed; it parks the sequencer in phase 16 holding the reply,
            // exactly as the update interrupt would after bit-banging PB14.
            if ptr.seq.state != 0 && ptr.seq.state != 2 {
                return false;
            }
            let n = bytes.len().min(8);
            ptr.seq.reply[..n].copy_from_slice(&bytes[..n]);
            ptr.seq.reply_expect = n as u8;
            ptr.seq.reply_byte = n as u8;
            ptr.seq.reply_bit = 0;
            ptr.seq.state = 16;
            core::sync::atomic::compiler_fence(core::sync::atomic::Ordering::SeqCst);
        }
        None => {
            // Completing a transfer with no reply: armed (2) -> idle (0),
            // or a held reply (16, already consumed by reply_take) -> idle (0).
            if ptr.seq.state != 0 && ptr.seq.state != 2 && ptr.seq.state != 16 {
                return false;
            }
            ptr.seq.state = 0;
        }
    }
    true
}
/// Test hook: reset the single-outline sequencer to idle (phase 0), clearing
/// any captured reply.  Module statics persist across host tests, so tests
/// that drive the sequencer need this to start from a clean state.
pub fn reset() {
    let ptr = S.get();
    ptr.seq.state = 0;
    ptr.seq.reply = [0; 8];
    ptr.seq.reply_byte = 0;
    ptr.seq.reply_bit = 0;
    ptr.seq.reply_expect = 0;
}

/// The sequencer is idle (no single-outline transfer in progress).
pub fn sequencer_idle() -> bool {
    S.get().seq.state == 0
}
/// The single-wire line is busy while a transfer is in progress (dump
/// `FUN_08007980`: `1 < DAT_2000051a`).
pub fn link_busy() -> bool {
    S.get().seq.state > 1
}
/// Mutable access to the per-LED RGB buffer (for use by legacy light
/// programs that write the frame buffer directly).
pub(crate) fn led_rgb_mut() -> &'static mut [[u8; LED_BYTES]; LED_COUNT] {
    &mut S.get().led_rgb
}


/// IRQ 11 - DMA1 channel 5 transfer complete (and the error flag).

///
/// The dump's `FUN_08007734`: when the sequencer is in phase 7 it stops the
/// striped transfer, then clears the DMA flags.
#[no_mangle]
pub extern "C" fn dma_ch5_irq() {
    if dma::flag(0x20000) {
        if S.get().seq.state == 7 {
            dma::channel_enable(LED_DMA, false);
            // CC1NE off (FUN_0800B3C8(TIM15, 0x100, 0))
            mmio::rmw16(LED_TIMER + addr::TIM_CCER, |v| v & !0x0100);
            timer::sr_clear(LED_TIMER, 2); // CC1IF
            timer::dier(LED_TIMER, 2, false); // CC1IE off
        }
        dma::clear_flags(0x20000);
    }
    if dma::flag(0x40000) {
        dma::clear_flags(0x40000);
    }
}

/// IRQ 20 - TIM15 update.
///
/// The dump's `FUN_0800745C`: sequences the single-outline transfer through
/// its post-transfer phases and bit-bangs the battery-pack reply bits in on
/// PB14 while phase 14 runs.
#[no_mangle]
pub extern "C" fn tim15_update_irq() {
    let tim = LED_TIMER;
    // update interrupt pending and enabled (FUN_0800B3A8)
    if mmio::r16(tim + addr::TIM_SR) & 1 == 0 || mmio::r16(tim + addr::TIM_DIER) & 1 == 0 {
        return;
    }
    let ptr = S.get();
    let state = ptr.seq.state;
    let reply_expect = ptr.seq.reply_expect;

    match state {
        2 => {
            // re-arm phase: stop, reconfigure CCR1/CCMR for capture, restart.
            timer::moe(tim, false);
            timer::dier(tim, 1, false); // UIE off
            timer::sr_clear(tim, 1); // UIF
            timer::sr_clear(tim, 2); // CC1IF
            // FUN_0800B2C0(TIM15, &[0,0,1,0,0]) -> AE5C + b270:
            mmio::rmw16(tim + addr::TIM_CCER, |v| (v & !0xfffe) | 0x0001);
            mmio::rmw16(tim + addr::TIM_CCMR1, |v| (v & 0xff03) | 0x0001);
            timer::enable(tim, false);
            timer::set16(tim, addr::TIM_ARR, 0xFFFF);
            timer::dier(tim, 2, true); // CC1IE on
            timer::moe(tim, false);
            timer::enable(tim, true);
            ptr.seq.stamp = util::millis();
            ptr.seq.state = 4;
        }
        4 => {
            timer::enable(tim, false);
            mmio::rmw16(tim + addr::TIM_CCER, |v| v & 0xfffd); // CC1P off
            timer::enable(tim, true);
            ptr.seq.state = 5;
        }
        5 => {
            timer::enable(tim, false);
            timer::dier(tim, 2, false); // CC1IE off
            timer::dier(tim, 1, false); // UIE off
            ptr.seq.state = if reply_expect != 0 && reply_expect < 0x80 { 14 } else { 0 };
        }
        14 => {
            let bit = gpio::read(addr::GPIOB, 0x4000); // PB14
            let byte = &mut ptr.seq.reply[ptr.seq.reply_byte as usize];
            if bit {
                *byte |= 1u8 << ptr.seq.reply_bit;
            } else {
                *byte &= !(1u8 << ptr.seq.reply_bit);
            }
            ptr.seq.reply_bit = ptr.seq.reply_bit.wrapping_add(1);
            let done_byte = ptr.seq.reply_bit == 8;
            if done_byte {
                if ptr.seq.reply_expect == ptr.seq.reply_byte.wrapping_add(1) {
                    timer::moe(tim, false);
                    timer::enable(tim, false);
                    timer::dier(tim, 2, false);
                    ptr.seq.state = 16;
                } else {
                    ptr.seq.reply_byte = ptr.seq.reply_byte.wrapping_add(1);
                    ptr.seq.reply_bit = 0;
                }
            }
            if ptr.seq.state != 16 {
                timer::pwm_set(tim, 1, 6);
                timer::moe(tim, true);
                ptr.seq.state = 13;
            }
        }
        13 => {
            timer::moe(tim, false);
            timer::pwm_set(tim, 1, 0x18);
            ptr.seq.state = 14;
        }
        // phases 6, 7, 12 are stall/terminal states the dump parks in;
        // phase 16 is the post-bit-bang terminal state.
        _ => {}
    }
    timer::sr_clear(tim, 2); // CC1IF (FUN_0800b3c0(TIM15, 2))
}

/// `hwiLed_set_rgb` - set one LED to an explicit RGB colour.
///
/// The wire order is GRB (WS2812): the green byte goes out first, then red,
pub fn set_rgb(led: u8, r: u8, g: u8, b: u8) -> i32 {
    if !S.get().led_ready {
        init();
    }
    if led as usize >= LED_COUNT {
        return -1;
    }
    let ptr = S.get();
    ptr.led_rgb[led as usize] = [g, r, b];
    ptr.led_duty[led as usize] = (r as u16 / 3 + g as u16 / 3 + b as u16 / 3) as u8;
    fill_and_send();
    0
}

fn fill_and_send() {
    let ptr = S.get();
    fill_ccr_stream(&mut *ptr);
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

/// Apply the global brightness multiplier to an RGB triple.
/// Each channel is scaled by `brightness / 255` so that brightness=255
/// is full intensity and brightness=0 is off.
fn apply_brightness(r: u8, g: u8, b: u8, brightness: u8) -> (u8, u8, u8) {
    let mul = brightness as u32;
    (
        ((r as u32) * mul / 255) as u8,
        ((g as u32) * mul / 255) as u8,
        ((b as u32) * mul / 255) as u8,
    )
}

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
    if opname == "argb" {
        if op == proxy::OP_GET {
            if n > 1 {
                let second = &args[skip_word(args)..];
                let second_tok = second.split_whitespace().next().unwrap_or("");
                if second_tok == "brightness" {
                    let buf = fmt::int(S.get().brightness as i32);
                    proxy::resp_value(op, "hwiLed", opname, buf.as_str());
                    return;
                }
                let led_idx = second_tok.parse::<u8>().unwrap_or(0).min(LED_COUNT as u8 - 1);
                let rgb = S.get().led_rgb[led_idx as usize];
                let packed = ((rgb[0] as u32) << 16) | ((rgb[1] as u32) << 8) | rgb[2] as u32;
                let buf = fmt::num(16, packed);
                proxy::resp_value(op, "hwiLed", opname, buf.as_str());
                return;
            }
            let buf = fmt::int(S.get().brightness as i32);
            proxy::resp_value(op, "hwiLed", opname, buf.as_str());
            return;
        }
        if op == proxy::OP_SET && n > 1 {
            let second = &args[skip_word(args)..];
            let second_tok = second.split_whitespace().next().unwrap_or("");
            if second_tok == "brightness" {
                if n > 2 {
                    let rest = second.split_whitespace().nth(1).unwrap_or("0");
                    let b = rest.parse::<i32>().unwrap_or(255).clamp(0, 255) as u8;
                    S.get().brightness = b;
                }
                let buf = fmt::int(S.get().brightness as i32);
                proxy::resp_value(op, "hwiLed", opname, buf.as_str());
                return;
            }
            if n >= 4 {
                let mut ch = [0i32; 3];
                for i in 0..3 {
                    ch[i] = if n > 2 + i { v[2 + i].clamp(0, 255) } else { 0 };
                }
                let r = ch[0] as u8;
                let g = ch[1] as u8;
                let b = ch[2] as u8;
                if v[0] < 0 {
                    for i in 0..LED_COUNT {
                        S.get().led_rgb[i] = [g, r, b];
                    }
                } else {
                    let led_idx = v[0] as usize;
                    if led_idx < LED_COUNT {
                        S.get().led_rgb[led_idx] = [g, r, b];
                    }
                }
                fill_and_send();
                let packed = ((r as u32) << 16) | ((g as u32) << 8) | b as u32;
                let buf = fmt::num(16, packed);
                proxy::resp_value(op, "hwiLed", opname, buf.as_str());
                return;
            }
        }
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
