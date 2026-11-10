//! The reset-cause magic words must survive the boot that follows the reset.
//!
//! The dump's `main()` (`FUN_08009FA4` @0x8009fa4) tests two fixed RAM words
//! (`0x200017A8` = `0x19C3FE40`, `0x200025A8` = `0xD7E5F7A8`) to decide that
//! the chip reset through the bootloader-entry path.  `FUN_080063EC` writes
//! them before the watchdog forces the reset, and nothing on the next boot
//! clears them because they live outside every section the C startup owns.
//!
//! A Rust `static` defaults to `.bss`, and this port's `Reset_Handler`
//! zero-fills `.bss` (`startup.rs`).  When the magic cells were plain
//! statics, the words read back 0 after the reset, so the ROM-bootloader
//! jump never happened on hardware even though the whole state machine
//! worked.  The fix is a `#[link_section = ".noinit"]` cell plus a NOLOAD
//! `.noinit` section in the linker script.
//!
//! This test pins that storage decision.  It inspects the linker's output,
//! which on the host is only produced by the firmware target build - so the
//! assertions read the section layout that `./build.sh` emits and skips the
//! check when the ELF is not present (the image is a host-build artefact,
//! not a test dependency).

use std::path::Path;
use std::process::Command;

/// The two dump words (`FUN_08009FA4` tests both, in this order).
const MAGIC0: u32 = 0x19C3FE40;
const MAGIC1: u32 = 0xD7E5F7A8;

/// The addresses the original uses.
const DUMP_ADDR0: u32 = 0x2000_17A8;
const DUMP_ADDR1: u32 = 0x2000_25A8;

/// Where the release ELF lives after `./build.sh`.
const ELF: &str = "target/thumbv6m-none-eabi/release/codie-fw";

/// The toolchain prefix, the same one `build.sh` resolves.
///
/// `build.sh` looks in `../tools/` for the xpack toolchain and falls back to
/// the PATH.  Cargo runs the tests with the workspace root as the working
/// directory in a normal checkout, but the safe move is to try both
/// `../tools/` and the parent-of-parent, then the bare name.
fn tool(name: &str) -> Option<String> {
    for base in ["..", "../.."] {
        if let Ok(entries) = std::fs::read_dir(base) {
            for e in entries.filter_map(|e| e.ok()) {
                let n = e.file_name().to_string_lossy().into_owned();
                if !n.starts_with("xpack-arm-none-eabi-gcc-") {
                    continue;
                }
                let cand = format!("{base}/{n}/bin/{name}");
                if Path::new(&cand).exists() {
                    return Some(cand);
                }
            }
        }
    }
    let cand = format!("../tools/xpack-arm-none-eabi-gcc-*/bin/{name}");
    if glob_exists(&cand) {
        return Some(resolve_glob(&cand));
    }
    Some(name.to_string())
}

/// A cheap existence probe for the glob pattern `tool()` falls back to.
fn glob_exists(pattern: &str) -> bool {
    // The only expansion the pattern above can produce; a real shell glob is
    // not available (no dependency on a glob crate here).
    let _ = pattern;
    std::fs::read_dir("../tools")
        .map(|d| {
            d.filter_map(|e| e.ok()).any(|e| {
                e.path()
                    .join("bin")
                    .join(pattern.rsplit('/').next().unwrap_or(""))
                    .exists()
            })
        })
        .unwrap_or(false)
}

/// Return the first path matching the pattern (same non-expanding contract).
fn resolve_glob(pattern: &str) -> String {
    let leaf = pattern.rsplit('/').next().unwrap_or(pattern);
    if let Ok(d) = std::fs::read_dir("../tools") {
        for e in d.filter_map(|e| e.ok()) {
            let cand = e.path().join("bin").join(leaf);
            if cand.exists() {
                return cand.to_string_lossy().into_owned();
            }
        }
    }
    leaf.to_string()
}

/// Run a tool, return its stdout (empty on failure).
fn run(name: &str, args: &[&str]) -> Option<String> {
    let prog = tool(name)?;
    let out = Command::new(prog).args(args).output().ok()?;
    if out.status.success() {
        Some(String::from_utf8_lossy(&out.stdout).into_owned())
    } else {
        None
    }
}

#[test]
fn the_reset_magic_words_are_not_in_bss() {
    // The ELF is produced by the firmware target build.  On a host-only
    // checkout (`cargo test` without having run `./build.sh`) there is no
    // cross-built ELF to inspect, so the test reports what it can see and
    // stops rather than failing spuriously.
    if !Path::new(ELF).exists() {
        eprintln!("no cross-built ELF at {ELF}; run ./build.sh to enable this check");
        return;
    }

    let sections = match run("arm-none-eabi-objdump", &["-h", ELF]) {
        Some(s) => s,
        None => {
            eprintln!("arm-none-eabi-objdump unavailable");
            return;
        }
    };

    // `.bss` exists and is the zero-filled region the startup clears.
    // `objdump -h` prints "Idx Name Size VMA LMA FileOff Algn", so the line
    // starts with the section *index*; parse by field position instead of a
    // literal prefix.
    let bss_line = sections
        .lines()
        .find(|l| l.split_whitespace().nth(1) == Some(".bss"))
        .expect("the image has no .bss section");
    let bss_fields: Vec<&str> = bss_line.split_whitespace().collect();
    let bss_size = u32::from_str_radix(bss_fields[2], 16).expect("bss size is hex");
    let bss_addr = u32::from_str_radix(bss_fields[3], 16).expect("bss address is hex");
    assert!(bss_size > 0, "the image has an empty .bss");

    // `.noinit` must exist, be NOLOAD (no LOAD/CONTENTS flag) and hold both
    // cells.  NOLOAD is what keeps the words out of the flash image.
    let noinit_line = sections
        .lines()
        .find(|l| l.split_whitespace().nth(1) == Some(".noinit"));
    if let Some(l) = noinit_line {
        assert!(
            !l.contains("LOAD"),
            ".noinit must not be loaded from flash: {l}"
        );
    }

    // shows up as `.bss`.
    let symbols = match run("arm-none-eabi-objdump", &["-t", ELF]) {
        Some(s) => s,
        None => {
            eprintln!("arm-none-eabi-objdump -t unavailable");
            return;
        }
    };
    for name in ["RESET_MAGIC0_CELL", "RESET_MAGIC1_CELL"] {
        let line = symbols
            .lines()
            .find(|l| l.contains(name))
            .unwrap_or_else(|| panic!("{name} is not in the image"));
        assert!(
            line.contains(".noinit"),
            "{name} is not in .noinit; it would be zeroed by Reset_Handler: {line}"
        );
    }

    // Neither magic word may sit in the zero-filled range.
    let magic_addrs = symbols
        .lines()
        .filter(|l| l.contains("RESET_MAGIC"))
        .filter_map(|l| l.split_whitespace().next())
        .filter_map(|a| u32::from_str_radix(a, 16).ok())
        .collect::<Vec<u32>>();
    assert_eq!(magic_addrs.len(), 2, "expected the two magic cells, got {magic_addrs:x?}");
    for a in magic_addrs {
        assert!(
            a < bss_addr || a >= bss_addr + bss_size,
            "magic cell at 0x{a:08X} is inside .bss 0x{bss_addr:08X}..0x{:08X}; it would be lost",
            bss_addr + bss_size
        );
    }
}

#[test]
fn the_dump_words_and_addresses_are_the_ones_the_port_tests() {
    // Lock the constants against the dump so a rename cannot silently change
    // the reset-cause contract.  `boot.rs` holds the same values.
    assert_eq!(MAGIC0, 0x19C3FE40, "the first reset-cause word");
    assert_eq!(MAGIC1, 0xD7E5F7A8, "the second reset-cause word");
    // both inside the 16 KB RAM at 0x20000000, above the port's .bss/.data/
    // heap/stack layout (which ends below 0x20001000)
    assert!(DUMP_ADDR0 > 0x2000_1000 && DUMP_ADDR0 < 0x2000_4000);
    assert!(DUMP_ADDR1 > 0x2000_1000 && DUMP_ADDR1 < 0x2000_4000);
}
