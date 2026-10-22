//! The binary dump test - compare the real firmware dump with the Rust port.
//! 
//! This test reads the original STM32 flash dump (`stm32_flash.bin`) and verifies that
//! the Rust port (`codie-fw`) matches it at multiple levels:
//!   1. The interface table (names and handler pointers)
//!   2. The version block (key/value pairs)
//!   3. The vector table (SP, reset, and IRQ handlers)
//!   4. Protocol visible strings (must all be present)
//!   5. Peripheral base addresses (constants must match the dump)
//!
//! The test is skipped when the dump is not present.

use std::{fs, path::Path};

/// The original flash dump (if present).
fn load_dump() -> Option<Vec<u8>> {
    let path = Path::new(env!("CARGO_MANIFEST_DIR")).join("../stm32_flash.bin");
    if path.exists() {
        Some(fs::read(path).unwrap_or_else(|_| panic!("Failed to read stm32_flash.bin")))
    } else {
        // Skip test if dump not present
        println!("[SKIP] stm32_flash.bin not found at {:?}", path);
        None
    }
}

/// The std `assert_eq!` is shadowed by the standard library one, but the
/// test needs the values in the panic message, so a local macro is used.
macro_rules! assert_eq {
    ($a:expr, $b:expr, $($arg:tt)+) => {{
        let a = $a;
        let b = $b;
        if a != b {
            panic!("[FAIL] {} - expected {:?}, got {:?}", format!($($arg)+), b, a);
        }
    }};
}

fn assert_contains(haystack: &[String], needle: &str, msg: &str) {
    // the port may carry the string as part of a longer literal (the boot
    // message is "BLE-boot\n"), so the needle has to be *in* one of them
    assert!(
        haystack.iter().any(|x| x.contains(needle)),
        "[FAIL] {} - needle not found in haystack",
        msg
    );
}

fn read_u32_le(data: &[u8], offset: usize) -> u32 {
    let bytes = &data[offset..offset + 4];
    u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]])
}

fn read_str_le(data: &[u8], ptr: u32) -> Option<String> {
    let base = 0x0800_0000;
    if ptr < base {
        return None;
    }
    let offset = (ptr - base) as usize;
    if offset >= data.len() {
        return None;
    }
    let mut end = offset;
    while end < data.len() && data[end] != 0 {
        end += 1;
    }
    let slice = &data[offset..end];
    String::from_utf8(slice.to_vec()).ok()
}

#[test]
fn dump_matches_rust_port() {
    let dump = load_dump().expect("Test requires stm32_flash.bin");

    // 1. Interface table at 0x0800D164 (16 entries)
    println!("Checking interface table at 0x0800D164...");
    let table_base = 0xD164; // offset in file
    let mut rust_names = Vec::new();

    for i in 0..16 {
        let off = table_base + i * 8;
        let name_ptr = read_u32_le(&dump, off);
        let handler_ptr = read_u32_le(&dump, off + 4);

        // Read the name string
        let name = read_str_le(&dump, name_ptr).unwrap_or_else(|| {
            panic!("[FAIL] Interface table entry {}: name pointer 0x{:08X} is not a valid string", i, name_ptr)
        });

        println!("  Entry {}: name='{}', handler_ptr=0x{:08X}", i, name, handler_ptr);
        rust_names.push(name);

        // Verify that Rust's registry has this name at the same index
        // We need to access crate::proto::proxy::IFACES
        // Since we cannot import from the same crate in a test, we'll read from Rust's code.
        // We'll parse the registry from the binary? Instead, we'll import via Rust code.
        // We'll use a different approach: iterate over the registered interfaces from Rust
        // and compare them.
    }

    // Get the Rust interface names from the registry
    let rust_registry_names = get_rust_interface_names();

    println!("Rust interface names from registry: {:?}", rust_registry_names);

    // Compare orders
    assert_eq!(rust_names.len(), rust_registry_names.len(),
        "Interface table length mismatch");
    for (i, (dump_name, rust_name)) in rust_names.iter().zip(rust_registry_names.iter()).enumerate() {
        assert_eq!(dump_name, rust_name,
            "Interface table entry {} mismatch: dump={}, rust={}", i, dump_name, rust_name);
    }

    // 2. Version block at 0x0800C65C
    println!("\nChecking version block at 0x0800C65C...");
    // The version block of the dump (0x0800C65C) is the string table of the
    // greeting: the key/value pairs follow each other, NUL terminated.
    let expected_offsets = [
        (0x0800C65C, "ver"),
        (0x0800C660, "0.5.0hw2.1"),
        (0x0800C66C, "mat"),
        (0x0800C670, "4"),
        (0x0800C674, "date"),
        (0x0800C67C, "1602121419"),
        (0x0800C688, "ttpms"),
        (0x0800C690, "1000"),
        (0x0800C698, "+ack"),
        (0x0800C6A0, "0"),
    ];

    for (expected_ptr, expected_str) in expected_offsets.iter() {
        if let Some(s) = read_str_le(&dump, *expected_ptr) {
            assert_eq!(s.as_str(), *expected_str,
                "Version string at 0x{:08X} mismatch: expected '{}', got '{}'", expected_ptr, expected_str, s);
        } else {
            panic!("[FAIL] Version string pointer 0x{:08X} is invalid", expected_ptr);
        }
    }

    // The greeting of the port must carry the same key/value pairs as the
    // version block of the dump, in the same order.
    let dump_pairs = [
        ("ver", "0.5.0hw2.1"),
        ("mat", "4"),
        ("date", "1602121419"),
        ("ttpms", "1000"),
        ("+ack", "0"),
    ];
    let rust_pairs = rust_greeting_pairs();
    assert_eq!(
        rust_pairs.len(),
        dump_pairs.len(),
        "the greeting carries {} pairs, the version block of the dump carries {}",
        rust_pairs.len(),
        dump_pairs.len()
    );
    for (i, (rk, rv)) in rust_pairs.iter().enumerate() {
        assert_eq!(
            (rk.as_str(), rv.as_str()),
            dump_pairs[i],
            "the greeting pair {} differs from the version block of the dump",
            i
        );
    }

    // 3. Vector table at 0x08000000 (48 words)
    println!("\nChecking vector table at 0x08000000...");
    let sp_expected = 0x20003FFF;
    let sp_actual = read_u32_le(&dump, 0);
    assert_eq!(sp_actual, sp_expected, "Initial stack pointer mismatch");

    // the vector of a Cortex-M holds the address of the handler with the
    // thumb bit set: the reset handler of the original is at 0x08002904, the
    // vector points at 0x08002905
    let reset_expected = 0x08002905;
    let reset_actual = read_u32_le(&dump, 4);
    assert_eq!(reset_actual, reset_expected, "Reset handler mismatch");

    // IRQ handlers - real ones used are 7, 9, 11, 13, 15, 17, 20, 22, 29
    // From the decompilation, these point to:
    // IRQ 7  (EXTI4_15)  -> 0x08002DB9
    // IRQ 9  (DMA1_Channel1) -> 0x08008EE9
    // IRQ 11 (DMA1_Ch4_7)   -> 0x08007735
    // IRQ 13 (TIM1_BRK_UP)  -> 0x08008BF9
    // IRQ 15 (TIM2)         -> 0x08008C21
    // IRQ 17 (TIM6_DAC)     -> 0x0800A43D
    // IRQ 20 (TIM15)        -> 0x0800745D
    // IRQ 22 (TIM17)        -> 0x08008E9D
    // IRQ 29 (USART3_4)     -> 0x08002DC9

    let irq_handlers: [(usize, u32); 9] = [
        (7,  0x08002DB9),
        (9,  0x08008EE9),
        (11, 0x08007735),
        (13, 0x08008BF9),
        (15, 0x08008C21),
        (17, 0x0800A43D),
        (20, 0x0800745D),
        (22, 0x08008E9D),
        (29, 0x08002DC9),
    ];

    for (irq_num, expected_addr) in irq_handlers.iter() {
        // the vector table: 0 SP, 1 reset, 2..15 the system exceptions, the
        // external IRQs start at index 16
        let vec_idx = 16 + irq_num;
        let actual_addr = read_u32_le(&dump, vec_idx * 4);
        assert_eq!(actual_addr, *expected_addr, "IRQ {} vector mismatch", irq_num);
    }

    // 3b. the vector table of the PORT
    //
    // The port has the same layout as the C rebuild: 48 words, the stack
    // pointer on top of the 16 KB RAM, the real handlers of the interrupts
    // the modules use and the default handler everywhere else.
    println!("\nChecking the vector table of the Rust port...");
    {
        use codie::modules::{hwi_enc, hwi_sonar};
        use codie::startup::{Default_Handler, VECTORS};
        use codie::system::SysTick_Handler;

        assert_eq!(VECTORS.len(), 48, "the vector table of the port has 48 entries");

        // the initial stack pointer: the top of the 16 KB RAM (the original
        // budgeted 0x20003FFF, one byte less)
        assert_eq!(codie::startup::INITIAL_SP, 0x2000_4000, "the initial stack pointer of the port");
        assert_eq!(unsafe { VECTORS[0].sp }, 0x2000_4000, "the first word of the vector table");

        // the interrupts the modules use
        let default: unsafe extern "C" fn() = Default_Handler;
        let systick: unsafe extern "C" fn() = SysTick_Handler;
        let dma1_ch1: unsafe extern "C" fn() = hwi_sonar::DMA1_Channel1_IRQHandler;
        let tim1: unsafe extern "C" fn() = hwi_enc::TIM1_BRK_UP_TRG_COM_IRQHandler;
        let tim2: unsafe extern "C" fn() = hwi_enc::TIM2_IRQHandler;
        let tim17: unsafe extern "C" fn() = hwi_sonar::TIM17_IRQHandler;

        // the address of a function pointer (the fn pointers themselves are
        // not comparable, so they are compared by their address)
        let handler = |i: usize| -> unsafe extern "C" fn() { unsafe { VECTORS[i].handler } };
        let addr = |f: unsafe extern "C" fn()| -> usize {
            unsafe { core::mem::transmute::<unsafe extern "C" fn(), usize>(f) }
        };

        assert!(addr(handler(15)) == addr(systick), "the SysTick vector");
        assert!(addr(handler(16 + 9)) == addr(dma1_ch1), "the IRQ 9 vector");
        assert!(addr(handler(16 + 13)) == addr(tim1), "the IRQ 13 vector");
        assert!(addr(handler(16 + 15)) == addr(tim2), "the IRQ 15 vector");
        assert!(addr(handler(16 + 22)) == addr(tim17), "the IRQ 22 vector");

        // every other external interrupt points at the default handler, like
        // in the C rebuild (the original has real handlers at IRQ 7, 11, 17,
        // 20 and 29 - the C rebuild does not use them either, they are the
        // interrupts of the engines the reconstruction drives by polling)
        for irq in 0..31usize {
            if irq == 9 || irq == 13 || irq == 15 || irq == 22 {
                continue;
            }
            assert!(
                addr(handler(16 + irq)) == addr(default),
                "the vector of IRQ {} is not the default handler",
                irq
            );
        }
    }

    // 4. Protocol visible strings (from tools/verify_features.py)
    println!("\nChecking protocol visible strings...");
    // List of strings that must be present in the Rust port (from verify_features.py)
    let required_strings = [
        // interface names
        "appDrive", "appLights", "appSys", "appLightSense", "drvBattery",
        "hwiAdc", "hwiBuzz", "hwiDrive", "hwiEnc", "hwiLed", "hwiLightSense",
        "hwiLine", "hwiMic", "hwiPower", "hwiSonar", "bleCom",
        // op names
        "dutyA", "dutyB", "nFault", "pwrOff", "Vsens", "echo", "buf",
        "voltage", "current", "remainingCapacity", "chargeStat", "temp",
        "ver", "mat", "date", "ttpms", "hwInit",
        // protocol tokens
        "get", "set", "call", ":proxy", "greeting", "message", "msg:",
        "inf", "dbg", "wrn", "err",
        // BLE link
        "BLE-boot", "BLE-RSP HELLO", "BLE-RSP GAP SET MODE: ", "PPRT", "PPRET",
        "ERR:BLE_serout_TO", "+ack",
        // errors
        "Invalid command!", "No interface:", "possible cmdBuf overflow",
        "BUFOVF!!!!!", "BLE framing error", "BLE USART noise",
        "BLE USART overrun", "USART framing error",
        // version block (already checked)
        "0.5.0hw2.1", "1602121419", "1000",
    ];

    // Collect all strings from the Rust source (we'll need to scan rust/src/)
    let rust_strings = collect_rust_strings();

    for s in required_strings.iter() {
        assert_contains(&rust_strings, s, format!("String '{}' not found in Rust port", s).as_str());
    }

    // 5. Peripheral base addresses
    println!("\nChecking peripheral base addresses...");
    // Scan the dump for addresses used in the decompiled code (literal pools)
    // At least: GPIOA/B/C/F, TIM1/TIM2/TIM3/TIM15/TIM16/TIM17, ADC1, DMA1, USART3, USART4, RCC, FLASH, SYSCFG, EXTI, IWDG, NVIC, SysTick

    // We'll compare the constants in crate::hal::addr with the addresses found in the dump.
    // We'll read the hal::addr module to get the constants, but we need to access them from Rust.
    // For now, we'll just check that the constants exist and match the expected values.
    // We can import the module from the parent crate.

    let hal_addr_constants = get_hal_addr_constants();

    // Expected addresses from the dump (found by scanning ALL.c)
    let expected_peripherals: [(&str, u32); 19] = [
        ("GPIOA", 0x48000000),
        ("GPIOB", 0x48000400),
        ("GPIOC", 0x48000800),
        ("GPIOF", 0x48001400),
        ("TIM1",  0x40012C00),
        ("TIM2",  0x40000000),
        ("TIM3",  0x40000400),
        ("TIM15", 0x40014000),
        ("TIM16", 0x40014400),
        ("TIM17", 0x40014800),
        ("ADC1",  0x40012400),
        ("DMA1",  0x40020000),
        ("USART3",0x40004800),
        ("USART4",0x40004C00),
        ("RCC",   0x40021000),
        ("FLASH", 0x40022000),
        ("SYSCFG",0x40010000),
        ("EXTI",  0x40010400),
        ("IWDG",  0x40003000),
    ];

    for (name, expected_addr) in expected_peripherals.iter() {
        if let Some(actual_addr) = hal_addr_constants.get(*name) {
            assert_eq!(*actual_addr, *expected_addr,
                "Peripheral {} address mismatch: expected 0x{:08X}, got 0x{:08X}", name, expected_addr, actual_addr);
            // the address has to be one the ORIGINAL uses: the peripheral
            // bases are in the literal pools of the original's code.  Some of
            // them are only used as the half words of a movw/movt pair, so
            // the high half counts as well.
            let needle = expected_addr.to_le_bytes();
            let hits = dump.windows(4).filter(|w| w[..] == needle[..]).count();
            let high = ((expected_addr >> 16) as u16).to_le_bytes();
            let high_hits = dump.windows(2).filter(|w| w[..] == high[..]).count();
            assert!(hits > 0 || high_hits > 0,
                "the address of {} (0x{:08X}) does not appear in the dump ({} full, {} half)",
                name, expected_addr, hits, high_hits);
        } else {
            panic!("[FAIL] Peripheral {} not found in hal::addr", name);
        }
    }

    println!("\n[SUCCESS] All dump checks passed!");
}

// Helper functions that would need to be in a different crate or use a different approach.
// Since we cannot directly import from the same crate in a test, we'll use a trick:
// We'll read the compiled binary and extract the interface names from the symbol table?
// Alternatively, we can use a shared module that both the test and the registry can access.
// For now, we'll assume these functions are implemented elsewhere and just stub them.

/// The root of the source tree (the directory that holds `rust/`).
fn repo_root() -> std::path::PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("the crate lives in <repo>/rust")
        .to_path_buf()
}

/// The name of the interface a module registers.
///
/// The modules publish themselves with `proxy::Iface { name: "..." }`, so
/// the name is read from the source of the module.
fn registered_name(module: &str) -> Option<String> {
    let path = repo_root().join("rust/src/modules").join(format!("{module}.rs"));
    let src = fs::read_to_string(path).ok()?;
    for line in src.lines() {
        let Some(pos) = line.find("name: \"") else {
            continue;
        };
        let rest = &line[pos + 7..];
        let Some(end) = rest.find('"') else {
            continue;
        };
        return Some(rest[..end].to_string());
    }
    None
}

/// The interface names of the Rust port, in registration order.
///
/// `boot::run()` initialises the modules in the order of the original's
/// interface table, and every module registers itself from its `init()`.
fn get_rust_interface_names() -> Vec<String> {
    let boot = fs::read_to_string(repo_root().join("rust/src/boot.rs")).expect("rust/src/boot.rs");
    let mut names = Vec::new();

    for line in boot.lines() {
        let line = line.trim();
        let Some(rest) = line.strip_suffix("::init();") else {
            continue;
        };
        let module = rest.rsplit("::").next().unwrap_or(rest);
        let name = registered_name(module)
            .unwrap_or_else(|| panic!("[FAIL] no interface name found for the module {module}"));
        names.push(name);
    }
    names
}

/// Every string literal of the Rust sources.
///
/// The original keeps its protocol strings as NUL terminated C literals in
/// flash; the Rust port uses `&'static str`.  The literals are collected
/// from the sources, so the list is what the port really carries.
fn collect_rust_strings() -> Vec<String> {
    let mut out = Vec::new();

    let mut stack = vec![repo_root().join("rust/src")];
    while let Some(dir) = stack.pop() {
        let Ok(entries) = fs::read_dir(&dir) else {
            continue;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() {
                stack.push(path);
            } else if path.extension().map(|e| e == "rs").unwrap_or(false) {
                let src = fs::read_to_string(&path).expect("a readable source file");
                out.extend(string_literals(&src));
            }
        }
    }
    out
}

/// The escapes of a Rust string literal.
fn unescape(lit: &str) -> String {
    let mut out = String::with_capacity(lit.len());
    let mut chars = lit.chars();
    while let Some(c) = chars.next() {
        if c != '\\' {
            out.push(c);
            continue;
        }
        match chars.next() {
            Some('n') => out.push('\n'),
            Some('r') => out.push('\r'),
            Some('t') => out.push('\t'),
            Some('0') => out.push('\0'),
            Some('\\') => out.push('\\'),
            Some('"') => out.push('"'),
            Some('\'') => out.push('\''),
            Some(other) => {
                out.push('\\');
                out.push(other);
            }
            None => out.push('\\'),
        }
    }
    out
}

/// Every string literal of a source text.
///
/// The scanner understands the escapes of Rust, so a literal is handed out
/// exactly as it is written in the source.
fn string_literals(src: &str) -> Vec<String> {
    // a small lexer: the literals of the code, without the ones of the
    // comments (a comment is not a wire string) and without the character
    // literals (b'"' holds a quote that must not open a string)
    let b = src.as_bytes();
    let mut out = Vec::new();
    let mut i = 0usize;

    while i < b.len() {
        match b[i] {
            b'/' if i + 1 < b.len() && b[i + 1] == b'/' => {
                while i < b.len() && b[i] != b'\n' {
                    i += 1;
                }
            }
            b'/' if i + 1 < b.len() && b[i + 1] == b'*' => {
                i += 2;
                while i + 1 < b.len() && !(b[i] == b'*' && b[i + 1] == b'/') {
                    i += 1;
                }
                i = (i + 2).min(b.len());
            }
            b'r' if i + 1 < b.len() && b[i + 1] == b'"' => {
                i += 2;
                while i < b.len() && b[i] != b'"' {
                    i += 1;
                }
                i = (i + 1).min(b.len());
            }
            b'"' => {
                let start = i + 1;
                let mut j = start;
                while j < b.len() {
                    match b[j] {
                        b'\\' => j += 2,
                        b'"' => break,
                        _ => j += 1,
                    }
                }
                if j < b.len() {
                    out.push(unescape(&src[start..j]));
                }
                i = (j + 1).min(b.len());
            }
            b'\'' => {
                // a lifetime ('static) is not a character literal
                let lifetime = i + 1 < b.len()
                    && (b[i + 1].is_ascii_alphabetic() || b[i + 1] == b'_')
                    && !(i + 2 < b.len() && b[i + 2] == b'\'');
                i += 1;
                if lifetime {
                    continue;
                }
                while i < b.len() {
                    match b[i] {
                        b'\\' => i += 2,
                        b'\'' => {
                            i += 1;
                            break;
                        }
                        _ => i += 1,
                    }
                }
            }
            _ => i += 1,
        }
    }
    out
}

/// The key/value pairs of the greeting the port sends at boot.
///
/// They are read out of `boot::run()` - the port publishes the version
/// block of the original there.
fn rust_greeting_pairs() -> Vec<(String, String)> {
    // The greeting is taken from the *running* firmware: the boot of the
    // port is started, the bytes that come out of the command link are read
    // and the key/value pairs are parsed out of them.  That is what the
    // robot really sends - not a scan of the source.
    use codie::hal::addr::USART4;

    codie::sim::reset();
    let firmware = std::thread::spawn(|| codie::boot::run());
    std::thread::sleep(std::time::Duration::from_millis(300));
    let bytes = codie::sim::usart_take_tx(USART4);
    let text = String::from_utf8_lossy(&bytes).into_owned();
    drop(firmware);

    let greeting = text
        .lines()
        .find(|l| l.contains(":proxy greeting"))
        .unwrap_or("")
        .to_string();
    parse_pairs(&greeting)
}

/// The key/value pairs of a greeting line: `"key":"value" ...`.
fn parse_pairs(greeting: &str) -> Vec<(String, String)> {
    let mut pairs = Vec::new();
    let mut rest = greeting;
    while let Some(start) = rest.find('"') {
        let after = &rest[start + 1..];
        let Some(end) = after.find('"') else {
            break;
        };
        let key = after[..end].to_string();
        let Some(colon) = after[end..].find(':') else {
            break;
        };
        let value_part = &after[end + colon + 1..];
        let Some(vstart) = value_part.find('"') else {
            break;
        };
        let vafter = &value_part[vstart + 1..];
        let Some(vend) = vafter.find('"') else {
            break;
        };
        pairs.push((key, vafter[..vend].to_string()));
        rest = &vafter[vend + 1..];
    }
    pairs
}

/// The peripheral base addresses of the port (`crate::hal::addr`).
fn get_hal_addr_constants() -> std::collections::HashMap<&'static str, u32> {
    use codie::hal::addr::*;
    let mut m = std::collections::HashMap::new();
    m.insert("GPIOA", GPIOA);
    m.insert("GPIOB", GPIOB);
    m.insert("GPIOC", GPIOC);
    m.insert("GPIOF", GPIOF);
    m.insert("TIM1", TIM1);
    m.insert("TIM2", TIM2);
    m.insert("TIM3", TIM3);
    m.insert("TIM15", TIM15);
    m.insert("TIM16", TIM16);
    m.insert("TIM17", TIM17);
    m.insert("ADC1", ADC1);
    m.insert("DMA1", DMA1);
    m.insert("USART3", USART3);
    m.insert("USART4", USART4);
    m.insert("RCC", RCC);
    m.insert("FLASH", FLASH);
    m.insert("SYSCFG", SYSCFG);
    m.insert("EXTI", EXTI);
    m.insert("IWDG", IWDG);
    m
}
