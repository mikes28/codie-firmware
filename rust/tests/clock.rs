//! The clock tree of the port against the dump (`FUN_08002958`, at
//! `0x08002958`).
//!
//! This is the one function that runs before anything else on the robot and
//! that no host test can exercise (there is no real PLL on the host), so it
//! gets its own test file: the register model of `crate::sim` is global and
//! a clock test in a file that also runs the firmware would race with it.

use codie::hal::addr;

/// The clock setup must not wait for a flag the hardware never raises.
///
/// `FUN_08002958` (0x08002958) sets HSEON (CR bit 16, 0x0001_0000), then
/// waits for HSERDY (CR bit 17, 0x0002_0000): the outer wait at 0x080029b8
/// (bounded at 0x5000 spins; 0x080029ce `bne` 0x080029b8) and the gate at
/// 0x080029d4 (`lsls r2, #14` -> 0x0002_0000, bpl).  After PLLON (CR bit 24,
/// 0x0100_0000) it polls PLLRDY (CR bit 25, 0x0200_0000) at 0x08002a18
/// (`lsls r2, r3, #18` -> 0x0200_0000, `tst; beq`).  The model below pre-sets
/// exactly those two ready flags - not the enable bits (16/24) the setup
/// itself raises and the assertions check.  The first port revision polled
/// bit 26 (0x0400_0000) instead of bit 25; the hardware never raises it, so
/// the firmware hung and never brought up a single peripheral.
#[test]
fn the_clock_setup_waits_for_the_flag_the_dump_waits_for() {
    codie::sim::reset();

    // the model of the RCC: the ready flags the dump's waits poll (bit 17 =
    // HSERDY, bit 25 = PLLRDY) - not the enable bits (16/24) the setup raises
    codie::sim::write32(addr::RCC + 0x00, 0x0002_0000 | 0x0200_0000);
    codie::sim::write32(addr::RCC + 0x04, 0x0000_0008); // CFGR: SWS = PLL
    codie::sim::write32(addr::FLASH + 0x00, 0);

    // the clock setup has to terminate; if it waited for a flag the model
    // never sets, this call would never return
    codie::system::system_init();

    let cr = codie::sim::read32(addr::RCC + 0x00);
    let cfgr = codie::sim::read32(addr::RCC + 0x04);

    // the flash latency the original writes (1 wait state + prefetch)
    assert_eq!(
        codie::sim::read32(addr::FLASH + 0x00) & 0x7,
        1,
        "the flash latency is not 1 wait state"
    );
    // the PLL is on and the system clock runs from it
    assert_ne!(cr & 0x0001_0000, 0, "the HSE is not on after the setup");
    assert_ne!(cr & 0x0100_0000, 0, "the PLL is not running after the setup");
    assert_eq!(cfgr & 0x3, 2, "the system clock is not the PLL");
    assert_eq!(cfgr & 0xC, 8, "SWS does not report the PLL");
}
