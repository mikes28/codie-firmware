# Progress of the Rust port

**Checkpoint of the Rust port of the Codie robot firmware** (`rust/`), the
C reference implementation is `fw/` (untouched, read only).

---

## 1. Where we are

| slice | file(s) | state |
|---|---|---|
| crate skeleton, linker script, cargo config | `Cargo.toml`, `.cargo/config.toml`, `linker/stm32g071rb.ld` | done |
| register access (volatile / simulated) | `src/mmio.rs` | done |
| the low level HAL | `src/hal/{addr,gpio,timer,usart,adc,dma,rcc,nvic,ring,util}.rs` | done |
| the protocol layer | `src/proto/{proxy,fmt}.rs` | done |
| the module state helper | `src/state.rs` | done |
| the clock tree and the tick | `src/system.rs` | done |
| the peripheral model (host only) | `src/sim/mod.rs` | done |
| `hwiAdc` | `src/modules/hwi_adc.rs` | done |
| `hwiDrive` | `src/modules/hwi_drive.rs` | done |
| `hwiEnc` | `src/modules/hwi_enc.rs` | done |
| `hwiSonar` | `src/modules/hwi_sonar.rs` | done |
| `hwiLed` | `src/modules/hwi_led.rs` | done |
| `hwiBuzz` | `src/modules/hwi_buzz.rs` | done |
| `hwiMic` | `src/modules/hwi_mic.rs` | done |
| `hwiLine` | `src/modules/hwi_line.rs` | done |
| `hwiPower` | `src/modules/hwi_power.rs` | done |
| `hwiLightSense` | `src/modules/hwi_lightsense.rs` | done |
| `bleCom` (both serial links, `out_str`/`out_char`) | `src/modules/ble_com.rs` | done |
| `drvBattery` | `src/modules/drv_battery.rs` | done |
| `appSys` | `src/modules/app_sys.rs` | done |
| `appDrive` | `src/modules/app_drive.rs` | done |
| `appLights` | `src/modules/app_lights.rs` | done |
| `appLightSense` | `src/modules/app_lightsense.rs` | done |
| the vector table and the reset handler | `src/startup.rs` | ported, in the build/fix loop |
| `main()` - the boot sequence and the main loop | `src/boot.rs` | ported, in the build/fix loop |
| the firmware binary target | `src/main.rs` | done |
| the tests | `tests/protocol.rs`, `tests/simulator.rs` | written, not yet green |
| the documentation | `README.md`, `ARCHITECTURE.md` | done |

**Open at this checkpoint:** make the crate compile, make the tests pass and
produce the flashable image.  That work is running (agent `MakeItRun`).

---

## 2. How to build and test

```bash
cd rust

# the host build and the tests (a plain stable toolchain is enough)
cargo check --all-targets
cargo test

# the firmware image (the target is tier 3: nightly + -Z build-std=core, or
# a toolchain that has the target installed)
cargo build --release --target thumbv6m-none-eabi
```

The toolchain that was used here: a stable rustc 1.98 with the
`thumbv6m-none-eabi` standard library, unpacked in `/tmp/rust-official`:

```bash
PATH=/tmp/rust-official/bin:$PATH cargo build --release --target thumbv6m-none-eabi
```

The image is linked with `linker/stm32g071rb.ld` (128 KB flash @ `0x08000000`,
16 KB RAM @ `0x20000000`, the stack at `0x20004000`) - the same memory map as
the C rebuild.  `build.sh` does build + objcopy + (optionally) flash.

The C tree is **not** touched by any of this.

---

## 3. The architecture

[`ARCHITECTURE.md`](ARCHITECTURE.md) has the flow graph: the layers, the boot
sequence, the main loop, the request path, the file table, the 16 interfaces,
the interrupts, the memory map and the list of the reproduced quirks.

The short version:

```text
startup.rs  -> boot.rs -> modules/*  -> proto/proxy.rs -> proto/fmt.rs
                    |         |              |
                    |         +-> hal/* ------+-> mmio.rs -> (the STM32)
                    +-> system.rs
```

`mmio.rs` is the only switch between the firmware and the test world: on the
target it is a volatile load/store, on the host it goes into the model of
`src/sim/`.  No firmware code is duplicated for the tests.

---

## 4. The deliberate differences to the C reconstruction

| what | why |
|---|---|
| `hwiAdc`'s "buf" op emits the extra empty line after the 9th, the 17th and the 25th value (`(i & 7) == 0`) | that is what the **original binary** does - `analysis/decomp/FUN_080073DC_0x080073DC.c` shows `(bVar3 & 7) == 0`; the C reconstruction has a typo (`& 8u`) that contradicts its own comment |
| `hwiPower::vsens()` reads back PC15 | the C declares `hwiPower_vsens()` in `codie.h` but never implements it; the reading of the "Vsens" line is the natural meaning.  It is not used by the protocol |
| the `opname == NULL` guards of the C handlers are gone | the Rust dispatcher always hands over a non empty op name |
| the 32 bit compare register writes of `hal::timer::pwm_set` and the "whole byte" output type of `hal::gpio::config` are kept as they are | they are the quirks of the original (the C helper of the `hwi_a_hal` slice clears the GPIO bits at offset `0x28`, the one of the `hwi_b_hal` slice at `0x1C`) |

---

## 5. What is verified

* `cargo check --all-targets` - the whole crate, the library, the binary and
  both test files.
* `cargo test`:
  * `tests/protocol.rs` - the protocol layer against the exact bytes of the
    original (the greeting, the value reply, the unknown interface, the
    invalid command, the command buffer overflow, the message with the extra
    field, the log level words) plus the number formatting (hexadecimal, the
    signed variant, the fixed point variant).  The Rust counterpart of
    `tools/protocol_test.c`.
  * `tests/simulator.rs` - the whole firmware against the model of the
    peripherals: the boot output ("BLE-boot" + the greeting) on the command
    link, the quiet AT link, the replies of `drvBattery` and of the error
    paths, the motor duty registers (`TIM3_CCR1 = 0x200`, `TIM3_CCR3 = 0x100`)
    and the baud divisors of the two links.  The Rust counterpart of
    `tools/host_sim.c`.
* the image check of the original: `python3 tools/verify_features.py <image>`
  - every protocol visible string of the original has to be in the image.

## 6. What is still open

* the on-hardware test (flash the image and check that the robot boots and
  answers over the command link) - the same open point as the C rebuild.
* the pin map of the LED matrix and of the motor outputs is the one of the
  part family, it could not be recovered from the binary (see the comments in
  `src/modules/hwi_led.rs` and `src/modules/hwi_drive.rs`).
* the battery values still come from the local ADC; the original asks the BLE
  battery pack for them (`FUN_08006D34`).
