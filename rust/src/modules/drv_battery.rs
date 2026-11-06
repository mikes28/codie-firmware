//! drvBattery - the battery pack (C: `fw/modules/drv_battery.c`).
//!
//! Battery pack query flow (original state block 0x200004A0, fields at +2
//! temp, +4 voltage, +6 current, +8 remaining).  The pack is a BLE device:
//! the host speaks to it over USART3 (the BLE link) and parses the binary
//! reply - the pack values never come from the local ADC (hwiAdc).
//!
//!   FUN_08006E94 (0x08006e94) - "hwInit": waits for the BLE line to be idle
//!     (FUN_08007980(0)) and no callback registered (_DAT_200004ac == 0), then
//!     emits the 8-byte frame {0xcc,0x4e,0x01,0,0,0,0,parity} and registers the
//!     reply callback 0x8006ca1 (state -> 1).  Called by the
//!     "call drvBattery hwInit" command (FUN_08006F2C, op==3).
//!   FUN_08006E5C (0x08006e5c) - periodic: emits {0xcc,0x44} via
//!     FUN_08006CFC (0x08006cfc) and registers FUN_08006D34 (callback slot
//!     0x8006d35) as the reply handler (state -> 1).
//!   FUN_08006F0C (0x08006f0c) - runs the registered callback each loop.
//!   FUN_08006D34 (0x08006d34) - the 7-state reply machine: sends
//!     {cc,0xb4} (FUN_08006D18, 0x08006d18), {cc,0xb8,..} (FUN_08006C60,
//!     0x08006c60), {cc,0xbe,..} (FUN_08006CDC, 0x08006cdc), and parses the
//!     BMS frame via FUN_08007D7C into 0x200004a2/044/046/048.
//!
//! PORT STATUS: this flow is not implemented - the C reconstruction's
//! drvBattery_hwInit only zeroes the state (see fw/modules/drv_battery.c),
//! so the fields below stay 0 on the simulated and real robot.  The missing
//! half is a ble_com line-callback hook: the dump dispatches the BLE reply
//! through FUN_08007D7C into the registered callback; the rebuild's
//! ble_check_line is private and only matches the ASCII "BLE-RSP HELLO" /
//! "BLE-RSP GAP" lines, so it cannot deliver a binary BMS frame to this
//! module.  TX would be possible via ble_com::write, but without the RX parse
//! the request is unpaired and is dropped (matching the C).
//!

use crate::hal::{addr, util};
use crate::mmio;
use crate::proto::{fmt, proxy};
use crate::state::Global;

// --- Constants (C: drv_battery.c defines) -----------------------------------

/// The charge status pin of the pack: PF6 on port F.
const DRVB_GPIO_F: u32 = addr::GPIOF;
/// PUPDR register offset.
const DRVB_PUPDR: u32 = DRVB_GPIO_F + 0x0C;
/// IDR register offset.
const DRVB_IDR: u32 = DRVB_GPIO_F + 0x10;
/// PF6 bit (bit 6) in the IDR.
const DRVB_PF6: u32 = 0x0040;
/// PUPDR bit 12 (PF6 pull-up field).
const DRVB_PUPDR_UP: u32 = 0x1000;
/// PUPDR bit 13 (PF6 pull-down field).
const DRVB_PUPDR_DOWN: u32 = 0x2000;

// --- State ------------------------------------------------------------------

/// The original's battery state block (0x200004A0).
struct State {
    /// +2 : temperature in 1/10 degC
    s_temp_c10: i16,
    /// +4 : pack voltage in mV
    s_voltage_mv: u16,
    /// +6 : pack current in mA
    s_current_ma: i16,
    /// +8 : remaining capacity
    s_remaining: i16,
}

static S: Global<State> = Global::new(State {
    s_temp_c10: 0,
    s_voltage_mv: 0,
    s_current_ma: 0,
    s_remaining: 0,
});

// --- Low level helpers ------------------------------------------------------

/// `FUN_08002DD4` - delay in ms.  The original spins on the millisecond tick
/// with a bounded spin count; the reconstruction uses `hal::util::delay_ms`
/// which does the same thing.
fn drv_delay_ms(ms: u32) {
    util::delay_ms(ms);
}

/// `FUN_08007CFC` - the charge state of the pack.
/// 0 = the pin is pulled low (charging), 1 = the pin is floating,
/// 2 = the pin is driven high.
///
/// The charge status pin is PF6; the register accesses are done directly
/// (mmio::r32 / mmio::w32) because the low level HAL is not part of the
/// shared contract for this module - just like the C.
fn read_charge_state() -> i32 {
    // PF6: pull-up
    mmio::rmw32(DRVB_PUPDR, |v| (v & !DRVB_PUPDR_UP) | DRVB_PUPDR_UP);
    drv_delay_ms(10);
    if mmio::r32(DRVB_IDR) & DRVB_PF6 != 0 {
        // high with pull-up -> 0
        let state = 0;
        // no pull
        mmio::rmw32(DRVB_PUPDR, |v| v & !DRVB_PUPDR_UP);
        return state;
    }

    // PF6: pull-down
    mmio::rmw32(DRVB_PUPDR, |v| (v & !DRVB_PUPDR_UP) | DRVB_PUPDR_DOWN);
    drv_delay_ms(10);
    let state = if mmio::r32(DRVB_IDR) & DRVB_PF6 != 0 {
        2 // high with pull-down -> 2
    } else {
        1 // low -> 1
    };
    // no pull
    mmio::rmw32(DRVB_PUPDR, |v| v & !DRVB_PUPDR_UP);
    state
}

// --- Public API -------------------------------------------------------------

/// `drvBattery_init`
pub fn init() {
    let s = S.get();
    s.s_temp_c10 = 0;
    s.s_voltage_mv = 0;
    s.s_current_ma = 0;
    s.s_remaining = 0;
    proxy::register(proxy::Iface { name: "drvBattery", handler: handler });
}

/// `drvBattery_hwInit`
pub fn hw_init() {
    let s = S.get();
    s.s_temp_c10 = 0;
    s.s_voltage_mv = 0;
    s.s_current_ma = 0;
    s.s_remaining = 0;
}

/// `drvBattery_voltage_mv`
pub fn voltage_mv() -> i32 {
    S.get().s_voltage_mv as i32
}

/// `drvBattery_current_ma`
pub fn current_ma() -> i32 {
    S.get().s_current_ma as i32
}

/// `drvBattery_remaining_pct`
pub fn remaining_pct() -> i32 {
    S.get().s_remaining as i32
}

/// `drvBattery_charge_state`
pub fn charge_state() -> i32 {
    read_charge_state()
}

/// `drvBattery_temp_c10`
pub fn temp_c10() -> i32 {
    S.get().s_temp_c10 as i32
}

/// The interface handler of the module
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    if _op == proxy::OP_GET {
        if _opname == "voltage" {
            let value = fmt::num(16, S.get().s_voltage_mv as u32);
            proxy::resp_value(proxy::OP_SET, "drvBattery", _opname, value.as_str());
        } else if _opname == "temp" {
            let value = fmt::num_s(16, S.get().s_temp_c10 as i32);
            proxy::resp_value(proxy::OP_SET, "drvBattery", _opname, value.as_str());
        } else if _opname == "current" {
            let value = fmt::num_s(16, S.get().s_current_ma as i32);
            proxy::resp_value(proxy::OP_SET, "drvBattery", _opname, value.as_str());
        } else if _opname == "remainingCapacity" {
            let value = fmt::num_s(16, S.get().s_remaining as i32);
            proxy::resp_value(proxy::OP_SET, "drvBattery", _opname, value.as_str());
        } else if _opname == "chargeStat" {
            let value = fmt::num(16, charge_state() as u32);
            proxy::resp_value(proxy::OP_SET, "drvBattery", _opname, value.as_str());
        }
        return;
    }

    if _op == proxy::OP_CALL {
        if _opname == "hwInit" {
            hw_init();
        }
    }
}