//! hwiPower - the power / battery switch (C: `fw/modules/hwi_power.c`).
//!
//! PC13 is the power-good sense input (EXTI13 on GPIOC), PC12 is the power
//! hold output, PC15 is the Vsens output and PC14 is an auxiliary line.
//! The `off()` path clears the hold line and then spins forever, kicking the
//! watchdog so the supply collapses.

use crate::modules::ble_com;
use crate::hal::{addr, gpio, rcc, util};
use crate::mmio;
use crate::proto::{fmt, proxy};
use crate::state::Global;

const PWR_PORT: u32 = addr::GPIOC;
const PWR_HOLD_PIN: u8 = 12; // PC12 - keeps the supply alive
const PWR_SENSE_PIN: u8 = 13; // PC13 - power good sense (input)
const PWR_VSENS_PIN: u8 = 15; // PC15 - "Vsens" line
const PWR_AUX_PIN: u8 = 14; // PC14 - auxiliary line

/// The IRQ the power-good line raises: EXTI4_15 (the dump's `FUN_0800AB30`
/// call at the end of `FUN_08009654`).
const PWR_SENSE_IRQ: u8 = 7;

/// The bit of the power-good line in the EXTI and GPIO registers.
const PWR_SENSE_MASK: u32 = 1u32 << PWR_SENSE_PIN;

struct State {
    power_ready: bool,
    /// the two edge counters of the dump's power-good ISR
    /// (`DAT_200008c9` = rising, `DAT_200008ca` = falling)
    falling_edges: u16,
    rising_edges: u16,
}

static S: Global<State> =
    Global::new(State { power_ready: false, falling_edges: 0, rising_edges: 0 });
/// `power_hold_on` - sets PC12 high to latch the power hold line.
///
/// Called first in boot, before anything that can hang.  The dump's
/// FUN_08006338 calls FUN_080098C4() (PC12 HIGH) as its 2nd step, right
/// after GPIO setup and before any module init that could hang.
pub fn power_hold_on() {
    // RCC: GPIOC (AHBENR 0x80000)
    rcc::ahb_en(0x80000, true);

    // PC12: output
    let cfg = gpio::PerPinCfg {
        mask: 1u32 << PWR_HOLD_PIN,
        mode: gpio::MODE_OUTPUT,
        speed: 0,
        otype: 0,
        pull: gpio::PULL_NONE,
    };
    gpio::config_per_pin(PWR_PORT, &cfg);

    // keep the supply alive
    gpio::set(PWR_PORT, 1u16 << PWR_HOLD_PIN);
}

/// `hwiPower_init`
pub fn init() {
    proxy::register(proxy::Iface { name: "hwiPower", handler: handler });
    if S.get().power_ready {
        return;
    }
    // RCC: GPIOC (AHBENR 0x80000) and SYSCFG (APB2ENR 1)
    rcc::ahb_en(0x80000, true);
    rcc::apb2_en(0x0001, true);

    let s = S.get();

    // PC13: input with pull down
    let cfg = gpio::PerPinCfg {
        mask: 1u32 << PWR_SENSE_PIN,
        mode: gpio::MODE_INPUT,
        speed: 1,
        otype: 0,
        pull: gpio::PULL_DOWN,
    };
    gpio::config_per_pin(PWR_PORT, &cfg);

    // PC12, PC14, PC15: outputs
    let cfg = gpio::PerPinCfg {
        mask: (1u32 << PWR_HOLD_PIN) | (1u32 << PWR_AUX_PIN) | (1u32 << PWR_VSENS_PIN),
        mode: gpio::MODE_OUTPUT,
        speed: 0,
        otype: 0,
        pull: gpio::PULL_NONE,
    };
    gpio::config_per_pin(PWR_PORT, &cfg);

    // SYSCFG: EXTI13 belongs to GPIOC (field value 2).
    //
    // Dump: FUN_0800AE34 (0x800ae34) computes the register offset as
    // `((line >> 2) + 2) << 2`, so for line 13 it is ((13 >> 2) + 2) << 2
    // = (3 + 2) << 2 = 0x14 = EXTICR4, and the field shift as `(line & 3) << 2`
    // = (13 & 3) << 2 = 4 -> `[SYSCFG + 0x14]` bits 7:4.
    // The previous code used shift 12 (bits 15:12 = EXTI15 field) - wrong field,
    // so EXTI13 interrupt was never routed to GPIOC.
    mmio::rmw32(addr::SYSCFG + 0x14, |v| (v & !(0xFu32 << 4)) | (2u32 << 4));

    // keep the supply alive
    gpio::set(PWR_PORT, 1u16 << PWR_HOLD_PIN);

    // arm the falling edge of the power good line - poked directly, like the
    // C: `mmio::rmw32(hal::addr::EXTI + 0x0C, ...)` and `... + 0x08`.
    //
    // The dump's init ends with `FUN_0800AB30` (the NVIC enable) after these
    // two writes, which is IRQ 7 = EXTI4_15 - the interrupt this module's
    // power-good line is routed to.  Without arming it the handler below never
    // runs and a power loss goes unnoticed until the next poll.
    mmio::rmw32(addr::EXTI + addr::EXTI_FTSR, |v| v | PWR_SENSE_MASK);
    mmio::rmw32(addr::EXTI + addr::EXTI_RTSR, |v| v & !PWR_SENSE_MASK);
    crate::hal::nvic::enable(PWR_SENSE_IRQ, 2);

    s.power_ready = true;
}

/// `FUN_0800976C` - the power-good interrupt, IRQ 7 (EXTI4_15).
///
/// The dump's IRQ-7 entry (`FUN_08002DB8`) calls this, then the BLE link
/// service and the idle-GAP re-emission.  It is edge-switching: it tests the
/// pending bit of PC13, reads the pin, and then flips which edge arms the line
/// so that the *next* edge is the opposite one:
/// ```text
/// 0x800976e: iVar1 = FUN_0800a948(0x2000)          ; the EXTI pending bit
/// 0x8009778: if (iVar1 == 0) return
/// 0x800977e:   iVar1 = FUN_08009758()               ; read PC13
/// 0x8009782:   if (pin low) {                      ; pin low = the supply went
///   0x800978a:     stamp 0x200008d8                ;  away -> debounce low
///   0x8009790:     DAT_200008ca++                   ;  the low-edge counter
///   0x8009796:     FTSR &= ~0x2000, RTSR |= 0x2000  ;  arm the rising edge
/// } else {
///   0x80097a4:     stamp 0x200008d0
///   0x80097ac:     DAT_200008c9++                   ;  the high-edge counter
///   0x80097b8:     FTSR |= 0x2000, RTSR &= ~0x2000  ;  arm the falling edge
/// }
/// 0x80097c2:   FUN_0800a958(0x2000)                 ; clear the pending bit
/// ```
/// The port reproduces the register effects and the two counters, which are
/// the observable state of the module.
#[no_mangle]
pub extern "C" fn EXTI4_15_IRQHandler() {
    let pr = mmio::r32(addr::EXTI + addr::EXTI_PR);
    if pr & PWR_SENSE_MASK == 0 {
        return; // not our line
    }

    let st = S.get();
    let level_high = gpio::read(addr::GPIOC, PWR_SENSE_MASK as u16);

    if level_high {
        st.rising_edges += 1;
        // pin high -> DAT_200008c9++ (link flag 1), arm falling edge
        ble_com::raise_link_flag();
        // arm the falling edge next
        mmio::rmw32(addr::EXTI + addr::EXTI_FTSR, |v| v | PWR_SENSE_MASK);
        mmio::rmw32(addr::EXTI + addr::EXTI_RTSR, |v| v & !PWR_SENSE_MASK);
    } else {
        st.falling_edges += 1;
        // pin low -> DAT_200008ca++ (link flag 2), arm rising edge
        ble_com::raise_link_flag2();
        // arm the rising edge next
        mmio::rmw32(addr::EXTI + addr::EXTI_FTSR, |v| v & !PWR_SENSE_MASK);
        mmio::rmw32(addr::EXTI + addr::EXTI_RTSR, |v| v | PWR_SENSE_MASK);
    }

    // clear the pending bit
    mmio::w32(addr::EXTI + addr::EXTI_PR, PWR_SENSE_MASK);
}

// from FUN_080097E8 - cut the power.  The original clears the power
// hold line and then spins forever (the watchdog is kicked, the supply
// collapses and the board resets).
/// `hwiPower_off` - cuts the power, never returns.
pub fn off() -> ! {
    gpio::reset(PWR_PORT, 1u16 << PWR_HOLD_PIN);
    loop {
        util::iwdg_kick();
    }
}

/// `hwiPower_vsens`
///
/// The C reconstruction declares this in `codie.h` but never defines a body
/// for it (it is reported nowhere and never called).  The Rust port reads the
/// Vsens output line (PC15) back, which is the natural meaning of the name.
pub fn vsens() -> u16 {
    if gpio::read(PWR_PORT, 1u16 << PWR_VSENS_PIN) {
        1
    } else {
        0
    }
}

// ------------------------------------------------------------------ *
// interface handler
// ------------------------------------------------------------------ */
// The C handler guards `opname == 0` (a missing token); the Rust
// dispatcher always yields a non-empty opname, so that null check is gone.
/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    // call hwiPower pwrOff  ->  switch the robot off
    if op == proxy::OP_CALL && opname == "pwrOff" {
        off();
    }

    // set hwiPower Vsens <on|1|true|off>
    if op == proxy::OP_SET && opname == "Vsens" {
        // the "on"/"1"/"true" test is the original's FUN_0800A084; note
        // that, unlike hwiLed, this tests `args` directly (not `args + 1`).
        if util::is_on(args) {
            gpio::set(PWR_PORT, 1u16 << PWR_VSENS_PIN);
            let buf = fmt::int(1);
            proxy::resp_value(op, "hwiPower", opname, buf.as_str());
        } else {
            gpio::reset(PWR_PORT, 1u16 << PWR_VSENS_PIN);
            let buf = fmt::int(0);
            proxy::resp_value(op, "hwiPower", opname, buf.as_str());
        }
        return;
    }

    if op == proxy::OP_GET {
        // report the state of the power good line (PC13)
        let v = if gpio::read(PWR_PORT, 1u16 << PWR_SENSE_PIN) { 1 } else { 0 };
        let buf = fmt::int(v);
        proxy::resp_value(op, "hwiPower", opname, buf.as_str());
    }
}