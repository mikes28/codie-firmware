//! `main()` - the boot sequence and the main loop
//! (C: `fw/src/main.c`).

use core::sync::atomic::{AtomicU32, Ordering};

use crate::hal::addr;
use crate::mmio;
use crate::modules::{
    app_drive, app_lights, app_lightsense, app_sys, ble_com, drv_battery, hwi_adc, hwi_buzz,
    hwi_drive, hwi_enc, hwi_led, hwi_lightsense, hwi_line, hwi_mic, hwi_power, hwi_sonar,
};
use crate::proto::proxy;
use crate::system;

// --- reset cause words (C: main.c) -----------------------------------------
/// The two magic words of the original (0x200025A8 / 0x200017A8).
const RESET_MAGIC0: u32 = 0x19C3FE40;
const RESET_MAGIC1: u32 = 0xD7E5F7A8;

/// Two `static` cells hold the reset-cause magic left by the reset path.
/// The original used `static volatile uint32_t`; `AtomicU32` is the Rust
/// counterpart that is safe to touch from an interrupt.
static RESET_MAGIC0_CELL: AtomicU32 = AtomicU32::new(0);
static RESET_MAGIC1_CELL: AtomicU32 = AtomicU32::new(0);

// --- the power fail path registers (C: main.c) -----------------------------
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

/// The power fail path of the original (FUN_0800AB88): the clock tree back to
/// HSI and the GPIO setup of FUN_0800A964 (PC13/PC14 as outputs, then
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

    // PC13/PC14 as outputs (the original's FUN_0800A964)
    mmio::rmw32(addr::GPIOC + addr::GPIO_MODER, |v| (v & !0x1400_0000) | 0x1400_0000);
    mmio::w32(addr::GPIOC + addr::GPIO_BSRR, 0x4000); // PC14 high
}

// --- the command buffer (C: main.c) ------------------------------------------
/// The command buffer of the original: 0x28 = 40 bytes (0x20000FE8).
const CMD_BUF_SIZE: usize = 40;

/// `FUN_08009B14` - read one line out of the link into the command buffer.
///
/// Returns the number of characters read (0 if no line was received); on an
/// overflow the buffer is emptied and the buffer size is returned.
fn read_line(buf: &mut [u8; CMD_BUF_SIZE]) -> usize {
    let mut n = 0;

    loop {
        if n >= CMD_BUF_SIZE {
            buf[0] = 0;
            return CMD_BUF_SIZE; // overflow
        }

        let mut c = [0u8; 1];
        if ble_com::read(&mut c) <= 0 {
            ble_com::poll();
            continue; // nothing received yet
        }

        if c[0] == b'\n' {
            buf[n] = 0;
            return n;
        }
        buf[n] = c[0];
        n += 1;
    }
}

/// 96 MHz / 96000 = 1 kHz millisecond tick.
const SYSTICK_RELOAD: u32 = 96000;

/// The body of the original's `main()`.
pub fn run() -> ! {
    // 1. reset cause / power magic
    //
    // The original tests two magic words left in RAM by the reset path.  When
    // both match it clears them, runs the power fail mode (clock tree back to
    // HSI) and re-programs the SysTick for the slower HSI clock (reload 16000);
    // otherwise the tick is programmed with the full 96 MHz reload (96000).
    if RESET_MAGIC0_CELL.load(Ordering::Relaxed) == RESET_MAGIC0
        && RESET_MAGIC1_CELL.load(Ordering::Relaxed) == RESET_MAGIC1
    {
        RESET_MAGIC0_CELL.store(0, Ordering::Relaxed);
        RESET_MAGIC1_CELL.store(0, Ordering::Relaxed);
        power_fail_mode();
        // the original stops the SysTick here (its tick comes from TIM6);
        // the rebuild re-programs it for the HSI clock so that the
        // millisecond tick keeps running.
        system::sys_tick_init(16000);
    } else {
        // 2. the millisecond tick (the original uses TIM6, FUN_0800A464)
        system::sys_tick_init(SYSTICK_RELOAD);
    }

    // 3. the module inits, in the order of the original's interface table
    //    (0x0800D164)
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

    // 4. the battery pack init (the original's "hwInit")
    app_sys::hw_init();

    // 5. the greeting
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

    // 6. the main loop
    let mut s_cmdbuf = [0u8; CMD_BUF_SIZE];
    loop {
        let n = read_line(&mut s_cmdbuf);

        if n != 0 {
            // on an overflow the buffer is emptied (buf[0] = '\0'); the C
            // dispatches the empty line which answers "Invalid command!"
            if s_cmdbuf[0] == 0 {
                proxy::resp_message(proxy::LOG_ERR, "possible cmdBuf overflow", None);
            }
            // the bytes of the command line are ASCII, so from_utf8 is safe
            let cmd = core::str::from_utf8(&s_cmdbuf[..n]).unwrap_or("");
            proxy::dispatch(cmd);
        }

        // the periodic tasks (the original: FUN_080063EC)
        app_lights::tick();
        app_drive::tick();
        app_lightsense::tick();
    }
}
