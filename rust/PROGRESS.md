# Progress of the Rust port

**State of the Rust port of the Codie robot firmware** (`rust/`), the C
reference implementation is `fw/` (untouched, read only).

---

## 1. Where we are

| slice | file(s) | state |
|---|---|---|
| crate skeleton, linker script, cargo config | `Cargo.toml`, `.cargo/config.toml`, `linker/stm32g071rb.ld` | done |
| register access (volatile / simulated) | `src/mmio.rs` | done |
| the low level HAL | `src/hal/{addr,gpio,timer,usart,adc,dma,rcc,nvic,ring,util}.rs` | done, reviewed register by register (`HAL_PARITY.md`) |
| the protocol layer | `src/proto/{proxy,fmt,catalog}.rs` | done, reviewed (`HAL_PARITY.md`) |
| the module state helper | `src/state.rs` | done |
| the clock tree and the tick | `src/system.rs` | done, reviewed |
| the peripheral model (host only) | `src/sim/mod.rs` | done |
| the 16 interfaces | `src/modules/*.rs` | done, the function parity is proven (`PARITY.md`) |
| the vector table and the reset handler | `src/startup.rs` | done |
| `main()` - the boot sequence and the main loop | `src/boot.rs` | done |
| the firmware binary target | `src/main.rs` | done |
| the tests | `tests/{protocol,simulator,binary,binary_ops,c_equivalence}.rs` | done, 7 tests green |
| the documentation | `README.md`, `ARCHITECTURE.md` (the flow graph), `PARITY.md`, `HAL_PARITY.md` | done |

**Everything is green:**

```
cargo check --all-targets      clean, no warnings
cargo test                     7 passed, 0 failed
cargo build --release --target thumbv6m-none-eabi   -> rust/build/fw.bin (15 456 B)
python3 tools/verify_features.py rust/build/fw.bin  -> OK: every protocol visible
                                                        string of the original is present
```

---

## 2. The four oracles

The port is checked against four independent references:

| oracle | what it proves | where |
|---|---|---|
| **the golden bytes** | the protocol layer is byte exact (the greeting, the value reply, the error messages, the number formatting) | `tests/protocol.rs` |
| **the peripheral model** | the whole firmware: the boot output, the replies, the motor duty registers, the baud divisors | `tests/simulator.rs` |
| **the dump** | the port against the **original binary**: the interface table at `0x0800D164` (the same 16 names in the same order), the version block at `0x0800C65C`, the vector table (the stack pointer, the reset vector, the IRQ handlers), the ops of every interface with their exact answer bytes, the peripheral base addresses (every one of them appears in the dump) | `tests/binary.rs`, `tests/binary_ops.rs` |
| **the C firmware** | the same scenario, the same bytes: the facts are captured from the C simulator (`tools/host_sim.c`) and replayed by the port | `tests/c_equivalence.rs` + `tests/data/c_sim.txt` |

The dump is the ground truth, the C firmware is the reference implementation,
the golden bytes are the contract of the protocol.

---

## 3. The function parity with the C tree

[`PARITY.md`](PARITY.md) is the proof: one row per C function (238 symbols,
199 of them defined in the tree and 40 weak aliases), with the Rust
counterpart and the state.  **No behaviour of the C is missing in the port.**
The gaps that were found and closed:

* `hwi_sonar`: the compare register configuration of the capture channel had
  `ccer5 = 0x0008` (that would have enabled the channel 2 of TIM17); the C
  has `0` and the binary confirms it.
* `hwi_sonar`: the "measurement running" flag (`sonar_busy` of the C) had no
  counterpart - `hwi_sonar::busy()` now.
* `resp_value` / `resp_greeting`: the C can be called with a NULL value (the
  separator and the value are then left out, the greeting ends without the
  closing quote).  The Rust now expresses that case (`resp_value_opt`, the
  `Option` values of the greeting).
* `gpio::set_afr`: the two helpers of the C are not the same (the one of
  `hwi_a_hal` does not mask the value, the one of `hwi_b_hal` does) - the
  Rust has both now.
* the debug build aborted where the C wraps around (the channel arithmetic
  of the timers, the `i32::MIN` of `atoi`, the reload of the SysTick).

---

## 4. How to build and test

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

The C tree is **not** touched by any of this.  The only change outside
`rust/` is `tools/host_sim.c`: the C simulator got a "facts mode"
(`C_SIM_FACTS=<file>`) that writes the observable behaviour of the C firmware
into the fixture of the equivalence test.  The default transcript of
`tools/run_sim.sh` is unchanged.

---

## 5. The size

| image | bytes |
|---|---|
| the dumped original (the firmware part of the flash) | 54 820 |
| the C rebuild | 12 608 |
| the **Rust** rebuild | 15 456 |

The Rust image is 2 848 bytes bigger than the C one (+23 %): about 600 B are
the protocol strings, which the Rust carries twice - once as the `&str`
literals of the code and once NUL terminated in `src/proto/catalog.rs`, so
that every string stays individually recoverable in the flash exactly like
the string table of the original (that is what makes
`tools/verify_features.py` pass on the Rust image).  The rest is the `no_std`
machinery and the bigger `core` code.

---

## 6. The deliberate differences to the C reconstruction

| what | why |
|---|---|
| `hwiAdc`'s "buf" op emits the extra empty line after the 9th, the 17th and the 25th value (`(i & 7) == 0`) | that is what the **original binary** does - `analysis/decomp/FUN_080073DC_0x080073DC.c` shows `(bVar3 & 7) == 0`; the C reconstruction has a typo (`& 8u`) that contradicts its own comment |
| `hwiPower::vsens()` reads back PC15 | the C declares `hwiPower_vsens()` in `codie.h` but never implements it; the reading of the "Vsens" line is the natural meaning.  It is not used by the protocol |
| the `opname == NULL` guards of the C handlers are gone | the Rust dispatcher always hands over a non empty op name |
| the 32 bit compare register writes of `hal::timer::pwm_set` and the "whole byte" output type of `hal::gpio::config` are kept as they are | they are the quirks of the original (the C helper of the `hwi_a_hal` slice clears the GPIO bits at offset `0x28`, the one of the `hwi_b_hal` slice at `0x1C`) |
| the original shifts the output type of the GPIO by `2*pin` for the even pins (the C and the Rust use `pin`) | the decompilation of `FUN_0800A964` in `analysis/decomp/ALL.c` is wrong here; the review in `HAL_PARITY.md` derived the truth from the machine code of the dump.  The port follows the C (the reconstruction) |

## 7. What is still open

* the on-hardware test (flash the image and check that the robot boots and
  answers over the command link) - the same open point as the C rebuild.
* the pin map of the LED matrix and of the motor outputs is the one of the
  part family, it could not be recovered from the binary (see the comments in
  `src/modules/hwi_led.rs` and `src/modules/hwi_drive.rs`).
* the battery values still come from the local ADC; the original asks the BLE
  battery pack for them (`FUN_08006D34`).
