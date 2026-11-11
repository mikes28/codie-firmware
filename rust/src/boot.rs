//! `main()` - the boot sequence and the main loop
//! (C: `fw/src/main.c`).

use core::sync::atomic::{AtomicU8, AtomicU32, Ordering};
use crate::hal::{addr, gpio, util};
use crate::mmio;
use crate::modules::{
    app_drive, app_lights, app_lightsense, app_sys, ble_com, drv_battery, hwi_adc, hwi_buzz,
    hwi_drive, hwi_enc, hwi_led, hwi_lightsense, hwi_line, hwi_mic, hwi_power, hwi_sonar, script,
};
use crate::proto::proxy;
use crate::state::Global;
use crate::system;

// ---------------------------------------------------------------------------
// Reset cause words (C: main.c)
// ---------------------------------------------------------------------------

/// The two magic words of the original (0x200025A8 / 0x200017A8).
const RESET_MAGIC0: u32 = 0x19C3FE40;
const RESET_MAGIC1: u32 = 0xD7E5F7A8;

/// Two cells hold the reset-cause magic left by the reset path.
///
/// The original used `static volatile uint32_t`; `AtomicU32` is the Rust
/// counterpart that is safe to touch from an interrupt.
///
/// These MUST NOT live in `.bss`: `Reset_Handler` zero-fills `.bss`, so a
/// `.bss` cell is 0 again on the boot that follows the watchdog-forced
/// reset, and the bootloader entry would be silently dead.  `.noinit` is
/// NOLOAD and no startup code touches it, which is what the original gets
/// for free by writing to fixed addresses (`0x200017A8` / `0x200025A8`)
/// outside every section the startup owns.
#[link_section = ".noinit"]
static RESET_MAGIC0_CELL: AtomicU32 = AtomicU32::new(0);
#[link_section = ".noinit"]
static RESET_MAGIC1_CELL: AtomicU32 = AtomicU32::new(0);
/// Bootloader entry state (matches dump RAM at 0x20000410).
///
/// 0 = normal operation
/// 2 = USB detected (PA12/VBUS went high)
/// 3 = ready for bootloader entry (button pressed while USB detected)
static BOOTLOADER_STATE: AtomicU8 = AtomicU8::new(0);

/// LED animation state (matches dump RAM at 0x2000035e).
///
/// 0 = off, 4 = red (bootloader entry in progress)
static LED_ANIMATION_STATE: AtomicU8 = AtomicU8::new(0);

/// Previous PA12 level for edge detection (USB VBUS).
static PA12_LAST: AtomicU8 = AtomicU8::new(0);

/// Previous PC13 level for button edge detection.
static PC13_LAST: AtomicU8 = AtomicU8::new(0);

/// `main_mark_reset_cause` - leave the magic words behind before a reset.
///
/// The original (`main_mark_reset_cause` of `main.c`) is called by hwiPower
/// (or any other module that triggers a reset); the next boot then enters the
/// power fail mode.
#[no_mangle]
pub extern "C" fn main_mark_reset_cause() {
    RESET_MAGIC0_CELL.store(RESET_MAGIC0, Ordering::Relaxed);
    RESET_MAGIC1_CELL.store(RESET_MAGIC1, Ordering::Relaxed);
}

/// The power fail path of the original (`FUN_0800AB88`): the clock tree back
/// to HSI and the GPIO setup of `FUN_0800A964` (PC13/PC14 as outputs, then
/// GPIOC BSRR = 0x4000 -> PC14 high).
fn power_fail_mode() {
    // RCC register offsets (from the original's main.c)
    const RCC_CR: u32 = 0x00;
    const RCC_CFGR: u32 = 0x04;
    const RCC_CIR: u32 = 0x08;
    const RCC_AHBENR: u32 = 0x14;
    const RCC_AUX0: u32 = 0x2C;
    const RCC_CSR: u32 = 0x30;
    const RCC_CCIPR: u32 = 0x34;

    mmio::rmw32(addr::RCC + RCC_CR, |v| (v & 0xFEF2_FFFF) | 0x0000_0001);
    mmio::rmw32(addr::RCC + RCC_CFGR, |v| v & 0x08C0_B80C);
    mmio::rmw32(addr::RCC + RCC_AUX0, |v| v & 0xFFFF_FFF0);
    mmio::rmw32(addr::RCC + RCC_CSR, |v| v & 0xFFFF_FEAC);
    mmio::rmw32(addr::RCC + RCC_CCIPR, |v| v & 0xFFFF_FFFE);
    mmio::w32(addr::RCC + RCC_CIR, 0);

    mmio::rmw32(addr::RCC + RCC_AHBENR, |v| v | 0x0008_0000);

    // PC13/PC14 as outputs (the original's `FUN_0800A964`)
    mmio::rmw32(addr::GPIOC + addr::GPIO_MODER, |v| (v & !0x1400_0000) | 0x1400_0000);
    mmio::w32(addr::GPIOC + addr::GPIO_BSRR, 0x4000); // PC14 high

    // The power-fail tail of the original `main()` (`FUN_08009F8C` @`0x08009f8c`,
    // called from the reset-cause path of `main()` at `0x08009fa4`).  The dump:
    //   if (isCurrentModePrivileged()) setMSP(0x200014a8);
    //   (*0x1fffc804)();
    // 0x200014a8 is the power-fail stack top and 0x1FFFC804 is a platform ROM
    // table slot: its target is not visible in the dump, so it is read and
    // called verbatim at runtime.  Arm-only: cfg-gated so the host build
    // (cargo test, x86) does not need to assemble `msr`.
    #[cfg(target_arch = "arm")]
    unsafe {
        core::arch::asm!("msr MSP, {0}", in(reg) 0x200014A8u32, options(nostack));
        let entry = core::ptr::read_volatile(0x1FFFC804u32 as *const extern "C" fn());
        entry();
    }
}

// ---------------------------------------------------------------------------
// Command buffer (C: main.c)
// ---------------------------------------------------------------------------

/// The command buffer of the original: 0x28 = 40 bytes (0x20000FE8).
const CMD_BUF_SIZE: usize = 40;

/// The line state of the command link: the bytes received so far.
///
/// The original keeps its 40-byte command buffer in RAM (`DAT_20000fe8`) and
/// the ring count lives in the peripheral, so a partially received line
/// survives a call that finds the ring empty.  The length has to persist
/// between calls; otherwise stale bytes accumulate until the overflow fires
/// and every command is answered with "Invalid command!".
struct LineState {
    buf: [u8; CMD_BUF_SIZE],
    len: usize,
}

static LINE: Global<LineState> = Global::new(LineState { buf: [0; CMD_BUF_SIZE], len: 0 });

/// `FUN_08009B14` - read the bytes of one line out of the link.
///
/// NON-BLOCKING, like the original: at `0x08009b25` it reads the ring count
/// and when it is zero it takes the branch at `0x08009b29` that only polls
/// the USART error flags (`0x08009ba7`..`0x08009bdf`) and returns 0.  The
/// bytes of a partially received line stay in the buffer; the next call
/// continues where this one stopped.
fn read_line() -> usize {
    ble_com::poll();

    let st = LINE.get();

    loop {
        if st.len >= CMD_BUF_SIZE {
            st.buf[0] = 0;
            st.len = 0;
            return CMD_BUF_SIZE; // overflow
        }

        let mut c = [0u8; 1];
        if ble_com::read(&mut c) <= 0 {
            return 0; // nothing received - the original returns 0 here
        }

        if c[0] == b'\n' {
            let n = st.len;
            st.len = 0;
            return n;
        }
        st.buf[st.len] = c[0];
        st.len += 1;
    }
}

// ---------------------------------------------------------------------------
// run() - the body of the original's main()
// ---------------------------------------------------------------------------

/// 96 MHz / 96000 = 1 kHz millisecond tick.
const SYSTICK_RELOAD: u32 = 96000;

pub fn run() -> ! {
    // ------------------------------------------------------------------
    // Phase 1: reset cause / power magic
    //
    // The original tests two magic words left in RAM by the reset path.  When
    // both match it clears them, runs the power fail mode (clock tree back to
    // HSI) and re-programs the SysTick for the slower HSI clock (reload 16000);
    // otherwise the tick is programmed with the full 96 MHz reload (96000).
    // ------------------------------------------------------------------
    if RESET_MAGIC0_CELL.load(Ordering::Relaxed) == RESET_MAGIC0
        && RESET_MAGIC1_CELL.load(Ordering::Relaxed) == RESET_MAGIC1
    {
        RESET_MAGIC0_CELL.store(0, Ordering::Relaxed);
        RESET_MAGIC1_CELL.store(0, Ordering::Relaxed);
        power_fail_mode();
        // The original stops the SysTick here (its tick comes from TIM6);
        // the rebuild re-programs it for the HSI clock so that the
        // millisecond tick keeps running.
        system::sys_tick_init(16000);
    } else {
        system::sys_tick_init(SYSTICK_RELOAD);
    }

    // ------------------------------------------------------------------
    // Phase 2: the original's own tick source (TIM6)
    //
    // Run in addition to the SysTick so its cadence and counter stay
    // observable (`system::dump_tick`).  The periodic engines are gated on
    // the SysTick's `millis()`, not on this.  The dump's TIM6 (PSC 48, ARR
    // 0xFFFF) is a ~65.5 ms update, not 1 kHz - the SysTick is the deliberate
    // better-behaved choice.
    // ------------------------------------------------------------------
    system::tim6_tick_init();

    // ------------------------------------------------------------------
    // Phase 3: the module inits
    //
    // hwi_power::power_hold_on() MUST be first: it sets the PC12 power-hold
    // latch that keeps the supply alive after the power button is released.
    // Everything after it can hang (e.g. ADC ADEN wait) and cut power.
    // Dump `FUN_08006338`: `FUN_08009654()` then `FUN_080098C4()` (PC12 HIGH)
    // 2nd, before any other init.
    //
    // The remaining inits run in the order of the original's interface table
    // (0x0800D164).  hwi_power::init() is at position 11 to keep the
    // interface registry order matching the dump.
    // ------------------------------------------------------------------
    hwi_power::power_hold_on();

    hwi_adc::init();
    hwi_led::init();
    hwi_drive::init();
    drv_battery::init();
    hwi_sonar::init();
    ble_com::init();
    hwi_buzz::init();
    hwi_mic::init();
    hwi_enc::init();
    hwi_line::init();
    hwi_power::init();
    hwi_lightsense::init();
    app_lights::init();
    app_drive::init();
    app_sys::init();
    app_lightsense::init();
    script::init();

    // ------------------------------------------------------------------
    // Phase 4: the battery pack init (the original's "hwInit")
    // ------------------------------------------------------------------
    app_sys::hw_init();

    // ------------------------------------------------------------------
    // Phase 5: watchdog bring-up + greeting
    //
    // The dump calls `FUN_0800A640` (watchdog) from `main()` before the
    // module inits; this port calls it after (the hardware-prep order was
    // adjusted so power_hold_on runs first).  `system::watchdog_init()`
    // reproduces the whole IWDG sequence.
    // ------------------------------------------------------------------
    system::watchdog_init();
    proxy::resp_greeting(
        &[
            ("ver", Some("0.5.0hw2.1")),
            ("mat", Some("4")),
            ("date", Some("1602121419")),
            ("ttpms", Some("1000")),
            ("+ack", Some("0")),
        ],
        None,
    );

    // ------------------------------------------------------------------
    // Phase 6: the main loop
    //
    // Each iteration: read a line -> dispatch -> periodic ticks -> watchdog
    // kick.  In the original this is `FUN_080063EC` (the periodic task,
    // @`0x80063ec`) called from the `do {...} while (1)` at `0x08009fa4`,
    // which runs 14 things in order.  The rebuild collapses the hwi_* polls
    // into the on-demand app ticks below; the genuinely unported pieces are
    // the battery pack query and the STOP-mode idle timers.
    //
    // The engines that CAN run without the BLE link:
    //   hwi_adc::tick   FUN_080071C0 - the ADC overrun flag service
    //   hwi_drive::tick FUN_08008784 - the motor fault input (GPIOA 12)
    //   hwi_line::tick  FUN_080094F8 - the line sensor sample window
    //   hwi_buzz::tick  FUN_08008574 - the queued tone sequencer
    // ------------------------------------------------------------------
    loop {
        // 6a. read a line from the command link
        let n = read_line();
        let s_cmdbuf = &LINE.get().buf;

        if n != 0 {
            // On overflow the buffer is emptied (buf[0] = '\0'); the C
            // dispatches the empty line which answers "Invalid command!".
            if s_cmdbuf[0] == 0 {
                proxy::resp_message(proxy::LOG_ERR, "possible cmdBuf overflow", None);
            }
            // The C hands the line over as a NUL-terminated C string and does
            // not validate anything; the bytes of the command line are ASCII
            // (they come from the link), so the conversion is unchecked.
            debug_assert!(core::str::from_utf8(&s_cmdbuf[..n]).is_ok());
            // SAFETY: the bytes come from the serial link, the protocol is
            // ASCII - the C does the same without a check.
            let cmd = unsafe { core::str::from_utf8_unchecked(&s_cmdbuf[..n]) };
            proxy::dispatch(cmd);
        }

        // 6b. periodic ticks
        hwi_adc::tick();
        hwi_drive::tick();
        hwi_line::tick();
        hwi_buzz::tick();
        hwi_mic::tick();
        app_lights::tick();
        app_lightsense::tick();
        app_drive::tick();
        drv_battery::tick();
        script::tick();
        ble_com::link_service_tick();
        ble_com::ble_at_resend_tick();
        ble_com::line_sensor_service_tick();

        // 6c2. USB VBUS / button detect + bootloader entry (from dump's
        //     FUN_08008784 + FUN_0800626C + FUN_080063EC bootloader check).
        //
        // PA12 = USB VBUS detect.  When USB is plugged in, PA12 goes high,
        // advancing bootloader state 0 -> 2 and LED animation state to 4 (red).
        // When state == 2 and the button (PC13) is pressed, advance to state 3.
        // When state == 3 && LED state == 4, write the magic words and enter
        // an infinite loop - the watchdog resets the chip, and on the next
        // boot main() sees the magic words and jumps to the ROM bootloader.
        {
            // PA12 edge detect (USB VBUS)
            let pa12 = gpio::read(addr::GPIOA, 0x1000) as u8;
            let pa12_last = PA12_LAST.load(Ordering::Relaxed);
            if pa12 != pa12_last {
                PA12_LAST.store(pa12, Ordering::Relaxed);
                if pa12 != 0 {
                    // USB plugged in: state 0 -> 2, LED animation state = 4
                    BOOTLOADER_STATE.store(2, Ordering::Relaxed);
                    LED_ANIMATION_STATE.store(4, Ordering::Relaxed);
                }
            }

            // PC13 button detect: advance state 2 -> 3 on press
            let pc13 = gpio::read(addr::GPIOC, 0x2000) as u8;
            let pc13_last = PC13_LAST.load(Ordering::Relaxed);
            if pc13 != pc13_last {
                PC13_LAST.store(pc13, Ordering::Relaxed);
                if BOOTLOADER_STATE.load(Ordering::Relaxed) == 2 {
                    BOOTLOADER_STATE.store(3, Ordering::Relaxed);
                }
            }

            // Bootloader entry: state 3 && LED state 4 -> write magic words,
            // spin on watchdog until reset, then main() jumps to ROM bootloader.
            if BOOTLOADER_STATE.load(Ordering::Relaxed) == 3
                && LED_ANIMATION_STATE.load(Ordering::Relaxed) == 4
            {
                RESET_MAGIC0_CELL.store(RESET_MAGIC0, Ordering::Relaxed);
                RESET_MAGIC1_CELL.store(RESET_MAGIC1, Ordering::Relaxed);
                loop {
                    util::iwdg_kick();
                    core::hint::spin_loop();
                }
            }
        }

        // 6c. watchdog refresh
        //
        // The original: `FUN_0800AB10`, called once per loop iteration from
        // the dump's `main()` at `0x0800A020`.  `util::iwdg_kick` writes
        // IWDG_KR = 0xAAAA.
        util::iwdg_kick();
    }
}

/// `FUN_0800626C` - the link alt callback, fired by `ble_com::link_service_tick`
/// when link flag 2 is raised (power-good low edge).  Advances bootloader
/// state 2 -> 3 (USB detected -> button pressed equivalent).
pub fn link_alt_callback() {
    if BOOTLOADER_STATE.load(Ordering::Relaxed) == 2 {
        BOOTLOADER_STATE.store(3, Ordering::Relaxed);
    }
}
