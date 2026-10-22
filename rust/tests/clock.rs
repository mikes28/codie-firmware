//! The clock tree of the port against the dump (`FUN_08002958`, at
//! `0x08002958`).
//!
//! This is the one function that runs before anything else on the robot and
//! that no host test can exercise (there is no real PLL on the host), so it
//! gets its own test file: the register model of `crate::sim` is global and
//! a clock test in a file that also runs the firmware would race with it.

use codie::hal::addr;

/// The clock setup must not wait for a flag it clears itself.
///
/// `FUN_08002958` waits for bit 22 of the RCC CR (`0x0400_0000`) after
/// switching the PLL on.  The first version of this port waited for bit 25
/// (`0x0200_0000`) - the same bit the function clears a dozen instructions
/// earlier (`CR &= 0xFFFBFFFF`) - so the firmware would have hung there and
/// never brought up a single peripheral.  The dump is the truth:
/// `0x08002a17: lsls r2, r3, #18` -> `0x0400_0000`, then `tst / beq`.
#[test]
fn the_clock_setup_waits_for_the_flag_the_dump_waits_for() {
    codie::sim::reset();

    // the model of the RCC: the state a real chip has once the PLL is on
    // (bit 16 = the PLL enabled, bit 22 = the PLL ready - the flag the
    // original waits for)
    codie::sim::write32(addr::RCC + 0x00, 0x0001_0000 | 0x0400_0000);
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
    assert_ne!(cr & 0x0001_0000, 0, "the PLL is off after the setup");
    assert_ne!(cr & 0x0100_0000, 0, "the PLL is not running after the setup");
    assert_eq!(cfgr & 0x3, 2, "the system clock is not the PLL");
    assert_eq!(cfgr & 0xC, 8, "SWS does not report the PLL");
}
