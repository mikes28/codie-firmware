//! Memory mapped register access.
//!
//! On the target the functions below are plain volatile loads and stores on
//! the peripheral addresses of the original firmware.  On the host they are
//! routed into [`crate::sim`], the model of the peripherals, so that the
//! *same* firmware code can be run by `cargo test`.
//!
//! The C original used `(*(volatile uint32_t *)(addr))` macros; the
//! behaviour is identical, the unsafe is confined to this module.

/// Read a 32 bit register.
#[inline]
pub fn r32(addr: u32) -> u32 {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::read_volatile(addr as *const u32) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::read32(addr)
    }
}

/// Write a 32 bit register.
#[inline]
pub fn w32(addr: u32, value: u32) {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::write_volatile(addr as *mut u32, value) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::write32(addr, value)
    }
}

/// Read a 16 bit register.
#[inline]
pub fn r16(addr: u32) -> u16 {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::read_volatile(addr as *const u16) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::bus_read16(addr)
    }
}

/// Write a 16 bit register.
#[inline]
pub fn w16(addr: u32, value: u16) {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::write_volatile(addr as *mut u16, value) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::bus_write16(addr, value)
    }
}

/// Read a 8 bit register.
#[inline]
pub fn r8(addr: u32) -> u8 {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::read_volatile(addr as *const u8) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::bus_read8(addr)
    }
}

/// Write a 8 bit register.
#[inline]
pub fn w8(addr: u32, value: u8) {
    #[cfg(target_os = "none")]
    {
        unsafe { core::ptr::write_volatile(addr as *mut u8, value) }
    }
    #[cfg(not(target_os = "none"))]
    {
        crate::sim::bus_write8(addr, value)
    }
}

/// Read/modify/write a 32 bit register.
#[inline]
pub fn rmw32(addr: u32, f: impl FnOnce(u32) -> u32) {
    let v = r32(addr);
    w32(addr, f(v));
}

/// Read/modify/write a 16 bit register.
#[inline]
pub fn rmw16(addr: u32, f: impl FnOnce(u16) -> u16) {
    let v = r16(addr);
    w16(addr, f(v));
}

/// Read/modify/write a 8 bit register.
#[inline]
pub fn rmw8(addr: u32, f: impl FnOnce(u8) -> u8) {
    let v = r8(addr);
    w8(addr, f(v));
}
