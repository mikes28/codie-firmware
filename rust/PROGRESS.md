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
| the tests | `tests/{protocol,simulator,binary,binary_image,binary_ops,c_equivalence,clock,usart}.rs` | done, 10 tests green |
| the documentation | `README.md`, `ARCHITECTURE.md` (the flow graph), `PARITY.md`, `HAL_PARITY.md` | done |

**Everything is green:**

```
cargo check --all-targets      clean, no warnings
cargo test                     10 passed, 0 failed
cargo build --release --target thumbv6m-none-eabi   -> rust/build/fw.bin (13 544 B)
python3 rust/verify_features.py rust/build/fw.bin   -> OK: every protocol visible
                                                        string of the original is present
tools/run_sim.sh               -> all behavioural checks passed (the C did not move)
```

The image was optimized (15 456 -> 13 432 bytes, see `OPTIMIZATION.md`) and
**after** the optimization everything was checked against the dump again -
the whole suite is green, the image carries the surface of the original and
the C firmware still produces the same facts.

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
| the **Rust** rebuild | 13 432 (was 15 456 before the optimization) |

The Rust image is 824 bytes bigger than the C one (+6.5 %): the `no_std`
machinery of `core` and the bigger code of the Rust (the `Option` handling,
the bounds checks).  The protocol strings are stored once - the dead second
copy was dropped by the optimization (see `OPTIMIZATION.md`).

---

## 6. The deliberate differences to the C reconstruction

| what | why |
|---|---|
| `hwiAdc`'s "buf" op emits the extra empty line after the 9th, the 17th and the 25th value (`(i & 7) == 0`) | that is what the **original binary** does - `analysis/decomp/FUN_080073DC_0x080073DC.c` shows `(bVar3 & 7) == 0`; the C reconstruction has a typo (`& 8u`) that contradicts its own comment |
| `hwiPower::vsens()` reads back PC15 | the C declares `hwiPower_vsens()` in `codie.h` but never implements it; the reading of the "Vsens" line is the natural meaning.  It is not used by the protocol |
| the `opname == NULL` guards of the C handlers are gone | the Rust dispatcher always hands over a non empty op name |
| the 32 bit compare register writes of `hal::timer::pwm_set` and the "whole byte" output type of `hal::gpio::config` are kept as they are | they are the quirks of the original (the C helper of the `hwi_a_hal` slice clears the GPIO bits at offset `0x28`, the one of the `hwi_b_hal` slice at `0x1C`) |
| the original shifts the output type of the GPIO by `2*pin` for the even pins (the C and the Rust use `pin`) | the decompilation of `FUN_0800A964` in `analysis/decomp/ALL.c` is wrong here; the review in `HAL_PARITY.md` derived the truth from the machine code of the dump.  The port follows the C (the reconstruction) |

## 7. The bug-fix pass against the dump (2026-10-26)

A whole-dump cross-check of the port against the machine code of
`stm32_flash.bin` (see `FINDINGS.md` at the repo root for the full evidence
trail) found and **fixed** these register-level defects, none of which the
host tests could see:

| # | what | the dump evidence |
|---|---|---|
| 1 | `hal::timer::channel_cfg` used the wrong CCMR field mask for the low byte of channels 1 and 3 (left CC1S/OC1M stale) | `FUN_0800AFDC` @0x800afec `movs r6,#0x73`; `FUN_0800B0F4` @0x800b104 |
| 2 | `system::watchdog_init` wrote the IWDG values one register too high and invented a 0x0C window | `FUN_0800AAEC/aaf8/ab04` store to base+0/+4/+8, and `FUN_0800AB10/ab20` to base+0 |
| 3 | `system::system_init` waited for PLLRDY on **bit 26** (never set → the firmware would hang before any peripheral came up) | `FUN_08002958` @0x8002a18 `lsls r2,r3,#18` → 0x0200_0000 = bit 25, `tst`/`beq` |
| 4 | `system::system_init` gated the clock-tree path on HSEON (bit 16) instead of HSERDY (bit 17) | `FUN_08002958` @0x80029d4 `lsls r2,r2,#14` → bit 17, `bpl` |
| 5 | `hwi_power` wrote the SYSCFG EXTICR of the **wrong register and the wrong field** - EXTI13 was never routed to GPIOC | `FUN_08009654` @0x80096b0 `movs r1,#13` → `FUN_0800AE34`; that helper computes `((line>>2)+2)<<2` = **EXTICR4** with the field at `(line&3)<<2` = bits 15:12 |
| 6 | `gpio::write()` cleared the `hwi_a_hal` GPIO group with a **word** store | `FUN_08009E0C` @0x8009e20 `strh r1,[r2,#40]` = a halfword |
| 7 | `hwi_buzz`'s tone timer prescaler was 96 | `FUN_08008380` @0x80083de `strh r7,[r1,#0]` with `r7 = 2` (set at 0x80083aa); the idle ARR is 24000 (`0x80083e2`, pool 0x5DC0) |
| 8 | `hwi_led`'s timer prescaler/ARR and its pin map were from memory, not from the dump | `FUN_080079A0`: psc = 29 (`0x8007a24`), ARR = 0 (`0x8007a26`), pins PB14/PB12/PB15 on GPIOB, **PB14 in AF1** (`0x8007a16`) |
| 9 | `hal::addr::TIM6` pointed at **SYSCFG** (`0x4001_0000`), so `timer::base_cfg(TIM6, ..)` reconfigured the wrong peripheral | `FUN_0800A464` @0x800a464 loads its base from the pool at 0x800a4b8 = **0x40001000**; `FUN_0800A43C` @0x800a43c uses `&DAT_40001000` |
| 10 | `hal::adc::start` waited for ADSTART with **no bound** - a stuck conversion would hang the firmware permanently | `FUN_0800A704` @0x800a704 bound counter `0x0000_EFFF` (pool 0x800a750), decremented by 2, and returns 0 on timeout (`movs r0,#0` @0x800a730) |

Also corrected three claims that the machine code does not support:

* `MEMORY.md` says the buzzer timer uses "PSC 96"; the dump uses **2**
  (`FUN_08008380` @0x80083de) - see item 7 above.
* `MEMORY.md` says the BLE link's state line is **PA2**; the dump maps a
  GPIOA mask of `0x400` = **PA10** (`FUN_08007F7C`, `0x80 << 3`). The code was
  already right; only the note was.
* An agent claimed the NVIC priority write is absent from the dump because no
  `0xE000E400` literal exists in the image. It does exist in effect:
  `FUN_0800AB30` computes the IPR address **base-relative**
  (`0xE000E100 + ((irq>>2) + 0xc0) << 2`), so the constant never appears
  literally. The read-modify-write at 0x800ab42/0x800ab60 is real and the
  port's priority code was restored.

## 8. What is still open

* the on-hardware test (flash the image and check that the robot boots and
  answers over the command link) - the same open point as the C rebuild.
* the pin map of the LED matrix and of the motor outputs is the one of the
  part family, it could not be recovered from the binary (see the comments in
  `src/modules/hwi_led.rs` and `src/modules/hwi_drive.rs`).
* the battery values still come from the local ADC; the original asks the BLE
  battery pack for them (`FUN_08006D34`).
* **7 of the dump's 11 periodic engines have no port counterpart** - the
  dump's `FUN_080063EC` also calls `FUN_080096EC` (BLE service, 800 ms),
  `FUN_08008784` (motor fault poll), `FUN_080071C0` (ADC/DMA poll),
  `FUN_08009EA0` (line sensor service), `FUN_08008130` (BLE AT re-send,
  2000 ms), `FUN_080030D0` (mic sample, 500 ms), `FUN_080094F8` (line
  sensor sample) and `FUN_08008574` (buzzer tone sequencer). The port's main
  loop only runs `app_lights`, `app_drive` and `app_lightsense`. Several of
  them publish over the BLE data channel, so they need a line-callback hook
  in `ble_com` before they can be ported.
* **the BLE pack query** (`call drvBattery hwInit` → `FUN_08006E94` sends an
  8-byte `{0xcc,0x4e,1,0,0,0,0,parity}` frame and `FUN_08006D34` parses the
  reply into voltage/current/remaining) is not implemented; it needs the same
  callback hook.
* **5 IRQ vectors fall to `Default_Handler`** - the dump has real handlers for
  IRQ 7, 11, 17, 20 and 29 (the port implements 9, 13, 15 and 22). Arming
  them without handlers would spin forever, so they stay unarmed until the
  corresponding engines are ported.
* the dump's millisecond tick comes from **TIM6** (psc 48, ARR 0xFFFF), which
  is a ~65.5 ms update period, not 1 ms - the port's 1 kHz SysTick is the
  deliberate and better-behaved choice; see `FINDINGS.md` §OPEN-7.
