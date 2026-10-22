//! The firmware image.
//!
//! The library (`src/lib.rs`) holds the whole firmware and is what the
//! tests use; this binary is what is linked into the flash image: it
//! provides the `main` symbol of the original firmware and keeps the vector
//! table of the library in the image.

#![cfg_attr(target_os = "none", no_std)]
#![cfg_attr(target_os = "none", no_main)]

/// The vector table of the library, kept in the image.
///
/// The table lives in the section `.vector_table`, which the linker script
/// marks with `KEEP`; this reference makes sure that the object file that
/// holds it is pulled into the image.
#[cfg(target_os = "none")]
#[used]
static VECTOR_TABLE: &'static [codie::startup::Vector; 48] = &codie::startup::VECTORS;

/// The `main()` of the original firmware (`FUN_08009FA4`).
///
/// The reset handler of `codie::startup` calls `codie::boot::run()` - the
/// same body - after it has set up the C runtime and the clock tree.
#[cfg(target_os = "none")]
#[no_mangle]
pub extern "C" fn main() -> ! {
    codie::boot::run()
}

/// The host build of the binary is only a compile check: the firmware itself
/// runs in the tests of the library.
#[cfg(not(target_os = "none"))]
fn main() {
    println!("the Codie firmware is built for the target thumbv6m-none-eabi");
}
