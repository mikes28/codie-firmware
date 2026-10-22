//! The low level hardware helpers of the original firmware.
//!
//! Every function here is the Rust counterpart of one of the helpers of
//! `fw/modules/hwi_a_hal.c` or `fw/modules/hwi_b_hal.c`; the original
//! function is named in the comment above it.  The register addresses are
//! the ones the *original binary* uses (see `../MEMORY.md` section 2) - they
//! are not the ones a data sheet would suggest.

pub mod addr;
pub mod adc;
pub mod dma;
pub mod gpio;
pub mod nvic;
pub mod rcc;
pub mod ring;
pub mod timer;
pub mod usart;
pub mod util;
