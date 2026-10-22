//! The **image** of the port against the dump of the original firmware.
//!
//! The other tests of the crate check the *behaviour* of the port (the bytes
//! on the command link, the register values).  This one checks the **image**
//! that is flashed to the robot:
//!
//!   1. the protocol visible strings of the original are in the image,
//!   2. the image fits into the 128 KB of the part,
//!   3. the vector table of the image: 48 words, the stack pointer, the
//!      reset vector, the SysTick and the IRQ entries,
//!   4. the peripheral bases of the port are in the image (they are in the
//!      literal pools of the original's code, so they have to be in the dump
//!      as well).
//!
//! The image is the release build:
//!
//! ```text
//! cd rust
//! PATH=/tmp/rust-official/bin:$PATH cargo build --release --target thumbv6m-none-eabi
//! mkdir -p build
//! ../tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin/arm-none-eabi-objcopy -O binary \
//!     target/thumbv6m-none-eabi/release/codie-fw build/fw.bin
//! ```
//!
//! When the image is not there the test skips itself with a message - the
//! other tests of the crate do not need it.

use std::fs;
use std::path::{Path, PathBuf};

use codie::hal::addr;

fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("the crate lives in <repo>/rust")
        .to_path_buf()
}

fn image_path() -> PathBuf {
    repo_root().join("rust/build/fw.bin")
}

fn dump_path() -> PathBuf {
    repo_root().join("stm32_flash.bin")
}

/// The protocol visible strings of the original.
///
/// The list is the one of the C tool (`tools/verify_features.py`) - it is
/// read from that file, so that the two checks cannot drift apart.
fn protocol_strings() -> Vec<String> {
    let path = repo_root().join("tools/verify_features.py");
    let text = fs::read_to_string(&path).expect("tools/verify_features.py");
    let start = text.find("MUST = [").expect("the MUST list of the C tool");
    let end = text[start..].find(']').expect("the end of the MUST list") + start;
    text[start..end]
        .lines()
        .filter_map(|l| {
            let l = l.trim().trim_start_matches('\'').trim_end_matches(',');
            let l = l.trim();
            if l.starts_with('\'') && l.ends_with('\'') && l.len() >= 2 {
                Some(l[1..l.len() - 1].to_string())
            } else {
                None
            }
        })
        .collect()
}

/// The maximal printable runs of a file (the method of the C tool).
fn printable_runs(data: &[u8]) -> Vec<String> {
    let mut out = Vec::new();
    let mut cur = Vec::new();
    for &b in data {
        if (0x20..0x7F).contains(&b) {
            cur.push(b);
        } else {
            if cur.len() >= 2 {
                out.push(String::from_utf8_lossy(&cur).into_owned());
            }
            cur.clear();
        }
    }
    if cur.len() >= 2 {
        out.push(String::from_utf8_lossy(&cur).into_owned());
    }
    out
}

fn read_u32_le(data: &[u8], offset: usize) -> u32 {
    u32::from_le_bytes([
        data[offset],
        data[offset + 1],
        data[offset + 2],
        data[offset + 3],
    ])
}

#[test]
fn the_image_carries_the_surface_of_the_original() {
    let image_path = image_path();
    if !image_path.exists() {
        println!(
            "[SKIP] the image is not built ({} is missing) - see the header of this file",
            image_path.display()
        );
        return;
    }
    let dump_path = dump_path();
    if !dump_path.exists() {
        println!("[SKIP] the dump is not there ({} is missing)", dump_path.display());
        return;
    }

    let image = fs::read(&image_path).expect("the image");
    let dump = fs::read(&dump_path).expect("the dump");

    // --- 1. the protocol visible strings --------------------------------
    //
    // The C tool (`tools/verify_features.py`) extracts the *maximal
    // printable runs* of the image.  The Rust image stores its string
    // literals merged (the Rust compiler puts them into one `.rodata` blob
    // without a NUL between them), so the maximal runs of the Rust image
    // are long concatenations ("hwiDrivehwiPowergreeting...").  The
    // strings are therefore looked for as *substrings* of the image - they
    // are there, byte for byte.
    let dump_runs = printable_runs(&dump);
    let mut missing = Vec::new();
    for s in protocol_strings() {
        if dump_runs.iter().any(|r| r == &s) && !image.windows(s.len()).any(|w| w == s.as_bytes())
        {
            missing.push(s);
        }
    }
    assert!(
        missing.is_empty(),
        "the image does not carry these protocol visible strings of the original: {:?}",
        missing
    );

    // --- 2. the size -----------------------------------------------------
    assert!(
        image.len() <= 0x20000,
        "the image is {} bytes, the flash of the part is 128 KB",
        image.len()
    );
    assert!(
        image.len() < dump.len(),
        "the image is {} bytes, the dump is {} - the rebuild cannot be bigger than the original",
        image.len(),
        dump.len()
    );

    // --- 3. the vector table of the image --------------------------------
    assert!(image.len() >= 48 * 4, "the image has no vector table");
    let sp = read_u32_le(&image, 0);
    let reset = read_u32_le(&image, 4);
    assert_eq!(
        sp, 0x2000_4000,
        "the initial stack pointer of the image is not the top of the 16 KB RAM"
    );
    assert!(
        (0x0800_0000..0x0802_0000).contains(&reset),
        "the reset vector 0x{:08X} is not in the flash",
        reset
    );
    assert_eq!(
        reset & 1,
        1,
        "the reset vector 0x{:08X} has no thumb bit",
        reset
    );
    // the SysTick and the four interrupts the modules use
    for (index, what) in [
        (15usize, "SysTick"),
        (16 + 9, "IRQ 9 (DMA1 channel 1)"),
        (16 + 13, "IRQ 13 (TIM1)"),
        (16 + 15, "IRQ 15 (TIM2)"),
        (16 + 22, "IRQ 22 (TIM17)"),
    ] {
        let addr = read_u32_le(&image, index * 4);
        assert!(
            (0x0800_0000..0x0802_0000).contains(&addr),
            "the vector of {} is 0x{:08X}, not in the flash",
            what,
            addr
        );
    }
    // the default handler of the interrupts the firmware does not use
    let default = read_u32_le(&image, 16 * 4);
    for irq in 0..31usize {
        if irq == 9 || irq == 13 || irq == 15 || irq == 22 {
            continue;
        }
        let addr = read_u32_le(&image, (16 + irq) * 4);
        assert_eq!(
            addr, default,
            "the vector of IRQ {} is 0x{:08X}, not the default handler 0x{:08X}",
            irq, addr, default
        );
    }

    // --- 4. the peripheral bases -----------------------------------------
    //
    // They are in the literal pools of the original's code, so they have to
    // occur in the dump - and in the image of the port.
    let bases: [(&str, u32); 19] = [
        ("GPIOA", addr::GPIOA),
        ("GPIOB", addr::GPIOB),
        ("GPIOC", addr::GPIOC),
        ("GPIOF", addr::GPIOF),
        ("TIM1", addr::TIM1),
        ("TIM2", addr::TIM2),
        ("TIM3", addr::TIM3),
        ("TIM15", addr::TIM15),
        ("TIM16", addr::TIM16),
        ("TIM17", addr::TIM17),
        ("ADC1", addr::ADC1),
        ("DMA1", addr::DMA1),
        ("USART3", addr::USART3),
        ("USART4", addr::USART4),
        ("RCC", addr::RCC),
        ("FLASH", addr::FLASH),
        ("SYSCFG", addr::SYSCFG),
        ("EXTI", addr::EXTI),
        ("IWDG", addr::IWDG),
    ];
    for (name, base) in bases {
        let needle = base.to_le_bytes();
        let in_dump = dump.windows(4).any(|w| w == needle)
            || dump.windows(2).any(|w| w == &needle[2..]);
        assert!(in_dump, "the address of {} (0x{:08X}) is not in the dump", name, base);
        let in_image = image.windows(4).any(|w| w == needle)
            || image.windows(2).any(|w| w == &needle[2..]);
        assert!(
            in_image,
            "the address of {} (0x{:08X}) is not in the image",
            name,
            base
        );
    }

    println!(
        "the image: {} bytes, the vector table at 0x08000000 (SP 0x{:08X}, reset 0x{:08X})",
        image.len(),
        sp,
        reset
    );
}
