//! The global state of the modules.
//!
//! The C original keeps the state of a module in file scope variables.  In
//! Rust the same is done with a [`Global`] cell: the firmware runs in a
//! single thread (the main loop) and the interrupts touch their own state
//! only, so the usual `static mut` rules are relaxed with one `unsafe impl
//! Sync` instead of scattering `unsafe` over the code.

use core::cell::UnsafeCell;

/// A global cell for the single threaded firmware.
pub struct Global<T>(UnsafeCell<T>);

// SAFETY: the firmware is single threaded - the main loop is the only
// thread of the target build, and the host build (the simulator) runs the
// firmware in one thread as well.  The interrupts of the target build only
// touch the state of their own module.
unsafe impl<T> Sync for Global<T> {}

impl<T> Global<T> {
    /// A new global cell.
    pub const fn new(value: T) -> Self {
        Global(UnsafeCell::new(value))
    }

    /// The mutable content of the cell.
    #[inline]
    pub fn get(&self) -> &mut T {
        unsafe { &mut *self.0.get() }
    }
}
