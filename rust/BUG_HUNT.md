# Bug hunt: the Rust port against the dumped firmware

**The dump is the ground truth.** `stm32_flash.bin` is the raw flash image of the
original firmware (code at `0x08000000`, so a function at `0x0800B3FC` is at file
offset `0xB3FC`).  `fw/` is only a reconstruction of it — where the C and the dump
disagree, **the dump wins** and the Rust has to be fixed.

## Why this matters

`fw/` was verified with the *host simulator* (`tools/host_sim.c`), which wraps the
USART primitives with `ld --wrap`.  The simulator therefore never sees what the
firmware actually writes into the USART control registers — the wrapped
`hwi_usart_init` writes its own values.  So a register value that the C gets
wrong is invisible in that test harness and only bites on real hardware.  The
Rust port inherits every such error from the C.

## How to read the dump

```bash
B=tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin
python3 -c "
d=open('stm32_flash.bin','rb').read()
start,end=0x0800B3FC,0x0800B560
open('/tmp/f.bin','wb').write(d[start-0x08000000:end-0x08000000])"
$B/arm-none-eabi-objdump -D -b binary -m arm -M force-thumb \
    --adjust-vma=0xb3fd /tmp/f.bin | awk '/b3fd:/,/b4c0:/'
```

Read the literal pools too (they are at the end of the function):
`python3 -c "..."` on `off = addr - 0x08000000`, 4 bytes little endian.

Function addresses: `analysis/decomp/FUNCS.txt`.

## Already found (verify these yourselves)

**`hal::usart::init` (rust/src/hal/usart.rs) — two wrong constants, taken from
the C:**

The original (`FUN_0800B3FC`) is a generic init that takes a config struct from
the caller.  `FUN_08007F7C` (the BLE link) passes:
`cfg[0]=baud, cfg[2]=0x2000 (CR2), cfg[4]=0x0C (CR1), cfg[5]=0x300 (CR3)`,
and `FUN_0800B3FC` computes

```text
CR1 = (CR1 & 0xE9F3) | cfg[3] | cfg[1] | cfg[4]   =  ... | 0x000C   -> RE + TE
CR2 = (CR2 & 0xCFFF) | cfg[2]                     =  ... | 0x2000   -> 2 stop bits
CR3 = (CR3 & 0xFCFF) | cfg[5]                     =  ... | 0x0300
```

The Rust (and the C) writes `CR1 |= 0x0300` and `CR3 |= 0x000C`.

* `0x000C` = USART_CR1 bits 2,3 = **RE (receiver enable) + TE (transmitter
  enable)**.
* `0x0300` = bits 8,9 = **PEIE (parity error interrupt enable) + PS (odd
  parity)**.

So the C/Rust never enables the transmitter or the receiver, and enables a
parity interrupt instead — the link would be completely dead on the robot.  The
mask `0xE9F3` clears bit 10 (PCE), so the parity itself stays off, which is why
the host model did not notice.

Confirm this from the machine code, then fix `hal::usart::init` to match the
dump: `CR1 |= 0x000C` (RE|TE) and `CR3 |= 0x0300`, keeping the masks.

## What to check, in priority order

1. **Every register value** the Rust writes: the baud divisor (`FUN_0800B3FC`:
   `brr = clock/baud; if (remainder >= baud/2) brr++`, and `brr & 0xFFF0` when
   CR1 bit 15 — OVER8 — is set), the GPIO configuration, the timer setup
   (`FUN_0800AEC4`, `FUN_0800AFDC`…), the ADC, the DMA, the RCC, the NVIC, the
   SysTick, the IWDG.
2. **The order** of the register writes (a write twice on purpose in the
   original must stay twice).
3. **The clock tree** (`FUN_08002958`) — a wrong clock makes every baud rate and
   timer frequency wrong.  Compare the Rust's `system::system_init()` with the
   machine code, instruction by instruction.
4. **Anything the C never implemented** that the dump does (whole functions,
   register writes, error handling).
5. **The startup** (`0x08002904`): the .data copy, the .bss clear, the call
   order, the initial SP.

## Rules

* `fw/` is read only.  `stm32_flash.bin` is read only.
* The Rust must end up **bit-compatible with the dump**.  Where the C disagrees
  with the dump, the Rust follows the dump and the difference is documented
  (in the module, in a comment that names the dump function and the offset of
  the instruction).
* Do not change the wire protocol, the op list, or the behaviour that the tests
  already prove — the tests are the safety net.
* Every change: `cd rust && cargo check --all-targets` and
  `cd rust && cargo test` (both have to stay green), plus the target build
  and the image check:
  ```bash
  cd rust && PATH=/tmp/rust-official/bin:$PATH cargo build --release --target thumbv6m-none-eabi
  cd rust && mkdir -p build && ../tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin/arm-none-eabi-objcopy -O binary target/thumbv6m-none-eabi/release/codie-fw build/fw.bin
  cd .. && python3 rust/verify_features.py rust/build/fw.bin
  ```
* Report: every bug, the dump evidence (the instruction addresses), the fix, and
  what you could not verify.
