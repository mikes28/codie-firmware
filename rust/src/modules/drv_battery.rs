//! drvBattery - the battery pack (C: `fw/modules/drv_battery.c`).

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