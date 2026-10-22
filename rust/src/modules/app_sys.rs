//! appSys - the system interface (C: `fw/modules/app_sys.c`).
//!
//! Original handler: 0x080064F1 (the entry in the interface table at
//! 0x0800D164) - in the original this handler is only a stub ("movs r0,#0;
//! bx lr"), the version block of the firmware is published in the greeting
//! (the key/value array in .data at 0x20000A14, the copy of the string table
//! at 0x0800C65C).
//!
//! The rebuild answers the version block as console ops as well, using the
//! very same strings that the original sends in the greeting:
//!   get ver   -> "0.5.0hw2.1"
//!   get mat   -> "4"
//!   get date  -> "1602121419"
//!   get ttpms -> "1000"
//!   call hwInit -> the hardware init (the original implements the same call
//!                   op in drvBattery, FUN_08006F2C)

use crate::modules::drv_battery;
use crate::proto::proxy;

// the version block of the original (0x0800C65C .. 0x0800C698)
const APP_SYS_VER: &str = "0.5.0hw2.1";
const APP_SYS_MAT: &str = "4";
const APP_SYS_DATE: &str = "1602121419";
const APP_SYS_TTPMS: &str = "1000";

/// the interface handler of the module
///
/// Original: `appSys_handler` (0x080064F1).
pub fn handler(op: u8, opname: &str, args: &str) {
    // (void)args;  - the original ignores the args of the request
    let _ = args;

    if op == proxy::OP_GET {
        if opname == "ver" {
            proxy::resp_value(proxy::OP_GET, "appSys", opname, APP_SYS_VER);
        } else if opname == "mat" {
            proxy::resp_value(proxy::OP_GET, "appSys", opname, APP_SYS_MAT);
        } else if opname == "date" {
            proxy::resp_value(proxy::OP_GET, "appSys", opname, APP_SYS_DATE);
        } else if opname == "ttpms" {
            proxy::resp_value(proxy::OP_GET, "appSys", opname, APP_SYS_TTPMS);
        }
        return;
    }

    if op == proxy::OP_CALL {
        if opname == "hwInit" {
            hw_init();
        }
    }
}

/// `hw_init` - the application level hardware init.
///
/// The original asks the battery pack for its values here (FUN_08006E94)
/// before the app starts.
pub fn hw_init() {
    drv_battery::hw_init();
}

/// `appSys_init`
///
/// Original: `appSys_init` (registers the interface under "appSys").
pub fn init() {
    proxy::register(proxy::Iface { name: "appSys", handler: handler });
}