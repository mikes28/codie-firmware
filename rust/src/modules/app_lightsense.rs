//! appLightSense - the ambient light driven dimming of the LED array
//! (C: `fw/modules/app_lightsense.c`).
//!
//! Original handler: 0x08003855 (FUN_08003854) - like the appDrive handler
//! it only stamps the shared application time (FUN_08003040) and returns 0,
//! it answers no ops.
//!
//! The module itself belongs to the application block around 0x08003000; the
//! ambient light sensors are read through the hwiLightSense module
//! (FUN_080093A9) and the result drives the brightness of the LED array.
//! The rebuild runs that in the periodic tick (the original calls the
//! application tick functions from FUN_080063EC in main's loop).

use crate::modules::{hwi_led, hwi_lightsense};
use crate::proto::proxy;
use crate::state::Global;

const APP_LS_SENSORS: usize = 2;
const APP_LS_LEDS: usize = 11;

/// The state of the module (the C file scope variables).
struct State {
    level: [u16; APP_LS_SENSORS],
    brightness: u8,
}

static S: Global<State> = Global::new(State {
    level: [0; APP_LS_SENSORS],
    brightness: 0,
});

/// the interface handler of the module
///
/// Original: `appLightSense_handler` - a stub that only refreshes the
/// application timestamp and answers no op.
pub fn handler(_op: u8, _opname: &str, _args: &str) {
    /* the original's handler only refreshes the application timestamp */
}

/// `appLightSense_tick`
///
/// from the application tick of the original: read the ambient light and
/// dim the LED array accordingly
pub fn tick() {
    let st = S.get();
    let mut sum: u32 = 0;

    for i in 0..APP_LS_SENSORS {
        st.level[i] = hwi_lightsense::read(i as u8);
        // sum is uint32_t, s_level[i] is uint16_t -> zero-extended in C.
        sum = sum.wrapping_add(st.level[i] as u32);
    }
    // the average of the sensors, 0..255
    let brightness = ((sum / (APP_LS_SENSORS as u32)) >> 2) as u8;
    st.brightness = brightness;

    for i in 0..APP_LS_LEDS {
        hwi_led::set_pwm(i as u8, brightness);
    }
}

/// `appLightSense_init`
pub fn init() {
    let st = S.get();
    for i in 0..APP_LS_SENSORS {
        st.level[i] = 0;
    }
    st.brightness = 0;
    proxy::register(proxy::Iface {
        name: "appLightSense",
        handler: handler,
    });
}