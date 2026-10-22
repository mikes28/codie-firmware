//! The 16 interfaces of the original firmware.
//!
//! One module per interface, named after the C file it comes from.  Every
//! module publishes itself in the interface registry of the protocol layer
//! (`crate::proto::proxy`) from its `init()` function.

pub mod app_drive;
pub mod app_lights;
pub mod app_lightsense;
pub mod app_sys;
pub mod ble_com;
pub mod drv_battery;
pub mod hwi_adc;
pub mod hwi_buzz;
pub mod hwi_drive;
pub mod hwi_enc;
pub mod hwi_lightsense;
pub mod hwi_led;
pub mod hwi_line;
pub mod hwi_mic;
pub mod hwi_power;
pub mod hwi_sonar;
