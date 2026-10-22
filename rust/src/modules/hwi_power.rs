//! hwiPower - the power / battery switch (C: `fw/modules/hwi_power.c`).
//!
//! PC13 is the power-good sense input (EXTI13 on GPIOC), PC12 is the power
//! hold output, PC15 is the Vsens output and PC14 is an auxiliary line.
//! The `off()` path clears the hold line and then spins forever, kicking the
//! watchdog so the supply collapses.

use crate::hal::{addr, gpio, rcc, util};
use crate::mmio;
use crate::proto::{fmt, proxy};
use crate::state::Global;

const PWR_PORT: u32 = addr::GPIOC;
const PWR_HOLD_PIN: u8 = 12; // PC12 - keeps the supply alive
const PWR_SENSE_PIN: u8 = 13; // PC13 - power good sense (input)
const PWR_VSENS_PIN: u8 = 15; // PC15 - "Vsens" line
const PWR_AUX_PIN: u8 = 14; // PC14 - auxiliary line

struct State {
    power_ready: bool,
}

static S: Global<State> = Global::new(State { power_ready: false });

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

    // SYSCFG: EXTI13 belongs to GPIOC (field value 2) - poked directly,
    // like the C: `mmio::rmw32(hal::addr::SYSCFG + 0x0C, ...)`.
    mmio::rmw32(addr::SYSCFG + 0x0C, |v| (v & !(0xFu32 << 4)) | (2u32 << 4));

    // keep the supply alive
    gpio::set(PWR_PORT, 1u16 << PWR_HOLD_PIN);

    // arm the falling edge of the power good line - poked directly, like the
    // C: `mmio::rmw32(hal::addr::EXTI + 0x0C, ...)` and `... + 0x08`.
    mmio::rmw32(addr::EXTI + 0x0C, |v| v | (1u32 << PWR_SENSE_PIN));
    mmio::rmw32(addr::EXTI + 0x08, |v| v & !(1u32 << PWR_SENSE_PIN));

    s.power_ready = true;
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