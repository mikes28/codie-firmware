# BUILD_FLASH — build, test, flash

## 1. Build

```
cd /home/mikes/Documents/codie-firmware-dump/rust

# host build + tests (stable toolchain is enough)
cargo check --all-targets
cargo test

# firmware image (thumbv6m-none-eabi is tier 3)
cargo build --release --target thumbv6m-none-eabi

# raw binary for the flasher
arm-none-eabi-objcopy -O binary \
    target/thumbv6m-none-eabi/release/codie build/fw.bin
```

The image is linked with `linker/stm32g071rb.ld`: 128 KB flash @ `0x08000000`,
16 KB RAM @ `0x20000000`, stack at `0x20004000` — same memory map as the C
rebuild.

Profile settings (`release`): `opt-level = "z"`, `lto = true`,
`codegen-units = 1`, `panic = "abort"`, `overflow-checks = false` (the C
firmware relies on 32-bit wraparound).

## 2. Test

```
cd rust
cargo test
```

11 tests, 0 failed, across 5 test files:
- `tests/protocol.rs` — protocol layer vs exact bytes of the original
- `tests/simulator.rs` — whole firmware vs peripheral model
- `tests/binary.rs` — port vs dump (interface table `0x0800D164`, version block
  `0x0800C65C`, vector table, IRQ handlers, peripheral bases)
- `tests/binary_ops.rs` — every op with exact answer bytes
- `tests/c_equivalence.rs` — port vs C firmware (same scenario, same bytes)
- plus `clock`, `usart` tests

The host build compiles the same code with `std` and routes every register
access into `src/sim/mod.rs` (peripheral model). No code is duplicated for the
tests.

## 3. Flash

```
# 1. put the robot into DFU mode (BOOT0 high, reset)
# 2. confirm it enumerates
lsusb | grep 0483   # expect: 0483:df11 STMicroelectronics STM32 BOOTLOADER

# 3. flash (pads to a whole 2 KiB page and reads the flash back)
cd /home/mikes/Documents/codie-firmware-dump/fw && ./flash.sh ../rust/build/fw.bin
```

`flash.sh` uses native `dfu-util` when available, otherwise falls back to the
same docker image used for the original dump.

**Critical: do the `:leave` last.** The ROM bootloader resets the MCU out of
DFU mode on leave, after which the device disappears — so every read must
happen first. Leaving the chip in the bootloader and re-plugging produces the
known failure mode where the robot advertises over BLE but drops every
connection.

## 4. Boot checklist

1. The greeting appears on the command link (USART4 @ 460800, PC10/PC11,
   2 stop bits) — a USB-TTL adapter is needed, USB is DFU-only:
   ```
   call :proxy greeting "ver":"0.5.0hw2.1" "mat":"4" "date":"1602121419" "ttpms":"1000" "+ack":"0"
   ```
2. `get drvBattery voltage` → `set drvBattery voltage <hex>`
3. `get drvBattery chargeStat` → `set drvBattery chargeStat 1`
4. `set hwiDrive dutyA 200` → motor A spins, no reply
5. `set hwiDrive dutyB 100` → motor B spins, no reply
6. `get hwiAdc buf` → the four channel dumps in hex, with the empty line after
   the 9th/17th/25th value
7. Power-loss behaviour: cutting the supply must produce the brown-out path,
   not a hang (IRQ 7 + watchdog)
8. A reset must not leave the chip stuck in the bootloader (`:leave` was issued)

## 5. Known risks on first power-up

- **LED pin map** (`hwi_led.rs`) — PB14/PB12/PB15 on GPIOB, PB14 AF1 (verified
  against the dump); physical strip wiring unknown.
- **Motor output pin map** (`hwi_drive.rs`) — GPIOC pins 6-9, PC2 enable, PC14
  fault (verified); physical wiring unknown.
- **96 MHz clock rating** — the original's `FUN_0800AC40` says 96 MHz,
  presumably OK.

## 6. Size

| image | bytes |
|---|---|
| Dumped original (firmware part) | 54 820 |
| C rebuild | 12 608 |
| Rust rebuild | 13 432 (+824 B / +6.5% vs C) |

The Rust image is bigger due to `no_std` machinery and `Option`/bounds-check
code. A dead second copy of the protocol strings was dropped by the optimization
(see `rust/OPTIMIZATION.md`).
