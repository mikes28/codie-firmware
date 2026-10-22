# Codie robot firmware - Rust port

A Rust port of the reconstructed C firmware in [`../fw`](../fw).  The port is
**functional**: the same register accesses, the same wire protocol, the same
bytes on the command link.  The C tree stays the reference implementation, the
original flash dump stays the ground truth.

* [`ARCHITECTURE.md`](ARCHITECTURE.md) - the flow graph of the files, what
  runs when and what each module does.
* the C original of every module is named in the header comment of the Rust
  file (`fw/modules/hwi_drive.c` -> `src/modules/hwi_drive.rs`).

## Build

```bash
cd rust
cargo build --release --target thumbv6m-none-eabi
```

The target `thumbv6m-none-eabi` (Cortex-M0+) is a **tier 3** target, so it
needs a nightly toolchain with a rebuilt core:

```bash
rustup toolchain install nightly
rustup component add rust-src
cargo +nightly build -Z build-std=core --release --target thumbv6m-none-eabi
```

With a stable toolchain that has the target installed
(`rustup target add thumbv6m-none-eabi`) the plain build above works as
well - the crate only needs `core`, no `std`, no allocator and no external
crate.

The image is linked with [`linker/stm32g071rb.ld`](linker/stm32g071rb.ld):
128 KB flash @ `0x08000000`, 16 KB RAM @ `0x20000000`, the stack at
`0x20004000` - the same memory map as the C rebuild.

```bash
# the raw image for the flasher
arm-none-eabi-objcopy -O binary \
    target/thumbv6m-none-eabi/release/codie build/fw.bin
# ... and flash it with the script of the C tree
../fw/flash.sh build/fw.bin
```

## Test

```bash
cd rust
cargo test
```

The tests run the **unmodified firmware** on the host, against four independent
references:

* `tests/protocol.rs` - the protocol layer against the exact bytes of the
  original (the Rust counterpart of `tools/protocol_test.c`).
* `tests/simulator.rs` - the whole firmware against a model of the
  peripherals (the Rust counterpart of `tools/host_sim.c`): the boot output,
  the replies, the motor duty registers and the baud divisors.
* `tests/binary.rs` - the port against the **dumped binary**: the interface
  table of the original (flash `0x0800D164`), its version block
  (`0x0800C65C`), its vector table, its IRQ handlers and the peripheral base
  addresses it uses.
* `tests/binary_ops.rs` - every op the original answers
  (`analysis/INTERFACE_OPS.txt`, extracted from the dump) with the exact
  bytes of the answer.
* `tests/c_equivalence.rs` - the port against the **C firmware**: the same
  scenario, the same bytes.  The facts are captured from the C simulator
  (`tools/capture_c_sim.sh` -> `tests/data/c_sim.txt`) and replayed.

The switch between the real peripherals and the model is in
[`src/mmio.rs`](src/mmio.rs): on the target the accesses are volatile loads
and stores, on the host they end up in [`src/sim/mod.rs`](src/sim/mod.rs).
No code is duplicated for the tests.

Two documents hold the proof of the parity with the C tree:
[`PARITY.md`](PARITY.md) (the function inventory) and
[`HAL_PARITY.md`](HAL_PARITY.md) (the register by register review of the
shared layers).

## What is where

| file | what |
|---|---|
| `src/lib.rs` | the crate root, the module tree |
| `src/mmio.rs` | the register access (volatile / simulated) |
| `src/hal/` | the low level helpers: GPIO, timers, USART, ADC, DMA, RCC, NVIC, ring buffer, utils |
| `src/proto/` | the command protocol: the parser, the interface registry, the replies, the number formatting |
| `src/modules/` | the 16 interfaces of the original firmware |
| `src/system.rs` | the clock tree and the millisecond tick |
| `src/startup.rs` | the vector table and the reset handler |
| `src/boot.rs` | `main()`: the boot sequence and the main loop |
| `src/state.rs` | the global state of the modules |
| `src/sim/` | the model of the peripherals (host only) |
| `tests/` | the protocol and the simulator tests |

## The rules of the port

* The C code is the reference: no "improvements", no new ops, no changed
  strings.  The quirks of the original (the hexadecimal reply values, the
  "set" op word in a "get" answer, the LED pattern engine, the swapped
  meaning of the two serial links) are reproduced on purpose.
* The register addresses are the ones of the **original binary**, not the
  ones of a data sheet (see `../MEMORY.md` section 2).
* The reply of the robot goes out on **USART4** (`0x40004C00`, 460800 bit/s,
  PC10/PC11); USART3 (`0x40004800`, 14400 bit/s) is the AT channel to the
  BLE module.
* No `alloc`, no `std` on the target, no external crate.
