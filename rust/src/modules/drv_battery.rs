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
use crate::modules::hwi_led;

// --- Constants (C: drv_battery.c defines) -----------------------------------
/// The BLE pack query: `call drvBattery hwInit` (FUN_08006E94).
///
/// The 8-byte frame {0xcc,0x4e,0x01,0,0,0,0,parity} rides the same single-wire
/// line as the LEDs (the second encoder, `FUN_080078E4`), armed with
/// `hwi_led::send_raw`.  The parity byte is `FUN_080001c4((0x73a+1000)<<5,
/// 0x271)` = 145 = 0x91.
const HW_INIT_FRAME: [u8; 8] = [0xcc, 0x4e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x91];
// The callback frame {0xcc,0x48,1} (FUN_08006CA0 -> FUN_08006C80(1)).
const HW_INIT_CB_FRAME: [u8; 3] = [0xcc, 0x48, 1];

/// The pack-poll frame {0xcc,0x44} (FUN_08006E5C -> FUN_08006CFC).
///
/// The dump's periodic poll.  The reconstruction registers the reply machine
/// from the hwInit path instead, so no callsite emits this frame; kept as
/// recovered protocol data.
#[allow(dead_code)]
const HW_POLL_FRAME: [u8; 2] = [0xcc, 0x44];

/// Reply frames of the 7-state reply machine (FUN_08006D34).
const HW_REPLY_B4: [u8; 2] = [0xcc, 0xb4];
const HW_REPLY_B8_0: [u8; 3] = [0xcc, 0xb8, 0x00];
const HW_REPLY_BE_0: [u8; 3] = [0xcc, 0xbe, 0];
const HW_REPLY_B8_1: [u8; 3] = [0xcc, 0xb8, 1];
/// The {0xcc,0xbe,1} variant of the machine's third frame.  Recovered from
/// the dump's index table; the machine only reaches {cc,0xbe,0}, so kept as
/// protocol data with no callsite.
#[allow(dead_code)]
const HW_REPLY_BE_1: [u8; 3] = [0xcc, 0xbe, 1];

/// The charge status pin of the pack: PF6 on port F.
const DRVB_GPIO_F: u32 = addr::GPIOF;
// PUPDR register offset.
const DRVB_PUPDR: u32 = DRVB_GPIO_F + 0x0C;
// IDR register offset.
const DRVB_IDR: u32 = DRVB_GPIO_F + 0x10;
// PF6 bit (bit 6) in the IDR.
const DRVB_PF6: u32 = 0x0040;
// PUPDR bit 12 (PF6 pull-up field).
const DRVB_PUPDR_UP: u32 = 0x1000;
// PUPDR bit 13 (PF6 pull-down field).
const DRVB_PUPDR_DOWN: u32 = 0x2000;

// --- State ------------------------------------------------------------------

/// The original's battery state block (0x200004A0).
struct State {
    s_temp_c10: i16,
    s_voltage_mv: u16,
    s_current_ma: i16,
    s_remaining: i16,
    hw_ab: u8,
    hw_cb: Option<fn() -> i32>,
}

static S: Global<State> = Global::new(State {
    s_temp_c10: 0,
    s_voltage_mv: 0,
    s_current_ma: 0,
    s_remaining: 0,
    hw_ab: 0,
    hw_cb: None,
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

pub fn init() {
    let s = S.get();
    s.s_temp_c10 = 0;
    s.s_voltage_mv = 0;
    s.s_current_ma = 0;
    s.s_remaining = 0;
    s.hw_cb = None;
    s.hw_ab = 0;
    proxy::register(proxy::Iface { name: "drvBattery", handler: handler });
}

/// `drvBattery_hwInit` - the BLE pack query (FUN_08006E94).
///
/// Sends the 8-byte frame {0xcc,0x4e,0x01,0,0,0,0,0x91} over the single-wire
/// line via `hwi_led::send_raw` (the second encoder) and registers the
/// callback that sends {0xcc,0x48,1} once the line is idle again.  Does
/// nothing while the line is busy or a callback is already registered,
/// matching the dump (`FUN_08007980` == 0 && `_DAT_200004ac` == 0).
pub fn hw_init() {
    if hwi_led::link_busy() {
        return;
    }
    let s = S.get();
    if s.hw_cb.is_some() {
        return;
    }
    s.s_temp_c10 = 0;
    s.s_voltage_mv = 0;
    s.s_current_ma = 0;
    s.s_remaining = 0;
    s.hw_ab = 1;
    let _ = s;
    hwi_led::send_raw(&HW_INIT_FRAME, 0);
    S.get().hw_cb = Some(hw_init_cb);
}

/// The hwInit callback (dump `FUN_08006CA0`, registered at 0x8006ca1).
///
/// Runs from `tick()` when the line is idle.  Sends {0xcc,0x48,1}, then
/// clears itself.
fn hw_init_cb() -> i32 {
    if hwi_led::link_busy() {
        return 1;
    }
    let s = S.get();
    let next = match s.hw_ab {
        1 => {
            hwi_led::send_raw(&HW_INIT_CB_FRAME, 0);
            s.hw_ab = 2;
            1
        }
        2 => {
            s.hw_ab = 1; // reply machine picks up from state 1
            s.hw_cb = None;
            0
        }
        _ => {
            s.hw_cb = None;
            0
        }
    };
    next
}

/// The 7-state pack-reply machine (dump `FUN_08006D34`).  Runs from `tick()`
/// when the line is idle.  Sends the {cc,0xb4}/{cc,0xb8,x}/{cc,0xbe,x}
/// command frames and parses the replies captured by the TIM15 update
/// interrupt's PB14 bit-bang (`reply_take`).
fn reply_machine() -> i32 {
    let hw_ab = S.get().hw_ab;
    // States 4 and 7 retrieve a reply captured by the update interrupt
    // (`reply_take` returns `Some` only in phase 16).  They only read, never
    // send, so phase 16 being "busy" must not block them - otherwise
    // `reply_take` is unreachable and the reply machine stalls at 4/7.
    if hw_ab != 4 && hw_ab != 7 && hwi_led::link_busy() {
        return 1;
    }
    let s = S.get();
    match hw_ab {
        1 => {
            hwi_led::send_raw(&HW_REPLY_B4, 0);
            s.hw_ab = 2;
            1
        }
        2 => {
            hwi_led::send_raw(&HW_REPLY_B8_0, 0);
            s.hw_ab = 3;
            1
        }
        3 => {
            hwi_led::send_raw(&HW_REPLY_BE_0, 0);
            s.hw_ab = 4;
            1
        }
        4 => {
            if let Some(reply) = hwi_led::reply_take() {
                s.s_temp_c10 = reply[2] as i8 as i16;
                s.s_voltage_mv =
                    ((reply[3] as u16 & 3) << 8 | reply[3] as u16) as u16;
                s.s_current_ma =
                    (((reply[5] as u16 + (reply[4] as u16) << 8) as u32)
                        .wrapping_mul(0x271)
                        >> 6) as i16;
                s.s_remaining =
                    (((reply[7] as u16 + (reply[6] as u16) << 8) as u32)
                        .wrapping_mul(0x271)
                        >> 5) as i16;
                s.s_remaining = s.s_remaining.wrapping_sub(1000);
                s.hw_ab = 6;
            }
            1
        }
        6 => {
            hwi_led::send_raw(&HW_REPLY_B8_1, 0);
            s.hw_ab = 7;
            1
        }
        7 => {
            if let Some(reply) = hwi_led::reply_take() {
                s.s_current_ma =
                    (((reply[5] as u16 + (reply[4] as u16) << 8) as u32)
                        .wrapping_mul(0x271)
                        >> 6) as i16;
                s.s_remaining =
                    (((reply[7] as u16 + (reply[6] as u16) << 8) as u32)
                        .wrapping_mul(0x271)
                        >> 5) as i16;
                s.s_remaining = s.s_remaining.wrapping_sub(1000);
                s.hw_ab = 0;
                s.hw_cb = None;
            }
            1
        }
        _ => {
            s.hw_cb = None;
            0
        }
    }
}

/// `drvBattery_tick` - runs the registered pack-query callback when the line
/// is idle (dump `FUN_08006F0C`).  Called once per main-loop iteration.
pub fn tick() {
    let s = S.get();
    let cb = s.hw_cb;
    let ab = s.hw_ab;
    let _ = s;
    if let Some(cb) = cb {
        if cb() == 0 {
            S.get().hw_cb = None;
        }
        return;
    }
    if ab != 0 && ab < 8 {
        reply_machine();
    }
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
