//! Codie robot firmware - Rust port of the reconstructed C firmware.
//!
//! The firmware is a port of `../fw` (the C reconstruction of the original
//! STM32 flash dump).  Every module keeps the behaviour, the register
//! accesses and the wire protocol of its C counterpart; the C file it comes
//! from is named in the module documentation.
//!
//! Layering (bottom to top):
//!
//! ```text
//!   mmio      volatile register access (real on the target, simulated on
//!             the host so that `cargo test` can run the firmware)
//!   hal       the low level helpers of the original: GPIO, timers, USART,
//!             ADC, DMA, RCC/NVIC, ring buffers, small string/number utils
//!   proto     the command protocol: request parser, interface registry,
//!             the reply helpers and the number formatting
//!   modules   the 16 interfaces of the original firmware (hwi*, app*,
//!             drvBattery, bleCom)
//!   system    the clock tree and the millisecond tick
//!   startup   the vector table and the reset handler
//!   boot      main(): the boot sequence and the main loop
//! ```
//!
//! The host build (`cargo test`) compiles the same code with `std` enabled
//! and routes every register access into [`sim`], a small model of the
//! peripherals the firmware uses.

#![cfg_attr(target_os = "none", no_std)]
#![cfg_attr(target_os = "none", no_main)]
#![deny(unsafe_op_in_unsafe_fn)]
#![allow(clippy::missing_safety_doc)]

pub mod boot;
pub mod hal;
pub mod mmio;
pub mod modules;
pub mod proto;
pub mod startup;
pub mod state;
pub mod system;

#[cfg(not(target_os = "none"))]
pub mod sim;

// ---------------------------------------------------------------------------
// panic handler (only needed for the bare metal build)
// ---------------------------------------------------------------------------
#[cfg(target_os = "none")]
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    // the original firmware has no panic path either: spin
    loop {
        core::hint::spin_loop();
    }
}
