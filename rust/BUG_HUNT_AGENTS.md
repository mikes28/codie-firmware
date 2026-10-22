# The four bug-hunt agents

Each agent gets one slice.  The shared context is `rust/BUG_HUNT.md` (read it first:
the goal, the disassembly recipe, the known finding, the rules, the verification commands).

Rules for every agent (from the brief, repeated because they are non-negotiable):

* `fw/` and `stm32_flash.bin` are **read only**.
* **The dump wins.**  Where the C disagrees with the dump, the Rust follows the dump
  and the fix gets a comment naming the dump function and the instruction address.
* Do not change the wire protocol, the op list or the behaviour the tests already
  prove — the tests are the safety net.
* After every change: `cd rust && cargo check --all-targets`,
  `cd rust && cargo test` (7 tests have to stay green) and the image check
  (`rust/verify_features.py`).
* **Fix what you find, don't just report it.**  A finding that is not fixed is a
  half result.
* Report: every bug, the dump evidence (instruction addresses), the fix, and what
  could not be verified.

---

## Agent 1 — `HuntUsart` : the serial links

Own files: `rust/src/hal/usart.rs`, `rust/src/modules/ble_com.rs`,
`rust/src/hal/addr.rs` (the USART offsets only).

1. The known finding: `hal::usart::init` writes `CR1 |= 0x0300` and `CR3 |= 0x000C`;
   the dump's `FUN_0800B3FC` (called from `FUN_08007F7C`) computes
   `CR1 |= 0x000C` (RE + TE) and `CR3 |= 0x0300`.  Confirm from the machine code and fix it.
   Then check the **second** caller (the console link, `FUN_08009A04` or whatever
   `analysis/decomp/FUNCS.txt` says) and make sure both callers are covered.
2. The baud divisor: `brr = clock/baud`, `if (remainder >= baud/2) brr++`, and the
   `& 0xFFF0` adjustment when CR1 bit 15 (OVER8) is set — the instruction at
   `0x800b495` (`lsls r3, r7, #28` / `lsrs r6, r3, #29` / `ldr r5, [pc] @0x800b4bc`
   / `ands r7, r5` / `orrs r7, r6`) implements exactly that.  Check whether the
   Rust's `usart::init` and the clock constant reproduce it, or whether the
   special case is only reachable for one of the two links.
3. The USART offsets: the Rust assumes the G4 layout (CR1 0x00, CR2 0x04, CR3 0x08,
   BRR 0x0C, ISR 0x1C, ICR 0x20, RDR 0x24, TDR 0x28).  Confirm every offset from the
   machine code of the dump (the instructions at `0x800b409..0x800b43b` read
   `[r4+0]`, `[r4+4]`, `[r4+8]`, `[r4+12]`, `[r4+20]`, `[r4+36]`).
4. The error handling: the flags the original reports and the ICR masks
   (`FUN_08007EC8`, `FUN_08008130`), the receive ring, the transmit timeout.
   Compare with `ble_com.rs`.
5. Which peripheral clock does the baud divisor really use?  `FUN_0800AC40` computes
   it from the RCC registers at run time.  Compare with `hal::addr::APB1_HZ`
   (96 MHz) and check what the C's constant actually is — `MEMORY.md` §4.4 says the
   C used 48 MHz and was then changed to 96 MHz.

## Agent 2 — `HuntCore` : the clock, the startup, the timer clock and the ADC/DMA

Own files: `rust/src/system.rs`, `rust/src/startup.rs`,
`rust/src/hal/{timer,adc,dma,rcc,nvic}.rs`, `rust/src/hal/addr.rs` (the rest).

1. `FUN_08002958` (at `0x08002958`) vs `system::system_init()` — instruction by
   instruction, including the two spin loops (the PLL ready timeout 0x5000, the
   unbounded waits after it) and the exact masks on CR/CFGR/AUX0/CSR/CCIPR/CIR.
   A wrong clock is a dead robot: the baud rates and every timer frequency depend
   on it.
2. The startup at `0x08002904`: the .data copy, the .bss clear, the call order
   (`SystemInit`, then `main`), the initial SP.  Compare with the Rust's
   `Reset_Handler`.  Also check the vector table's IRQ numbers (the original uses
   7, 9, 11, 13, 15, 17, 20, 22, 29 — which of them does the port use and why do
   the others not matter?).
3. The **timer clock**: the original's buzzer engine uses
   `SystemCoreClock/2`, and `MEMORY.md` says the timer clock is 2xAPB on this
   family.  Check `HWI_TIMCLK_HZ` in the C against the dump (`FUN_0800849C`) and
   against `hal::addr::TIMCLK_HZ` in the Rust.  A wrong value makes the buzzer
   tone, the sonar tick and the LED PWM wrong.
4. `FUN_0800AEC4`, `FUN_0800AFDC`, `FUN_0800B054`, `FUN_0800B0F4`, `FUN_0800B174`
   (the timer base and channel configuration) vs `hal::timer` — the offsets, the
   order, the 32-bit compare register writes.
5. The ADC (`FUN_0800A6A0`, `FUN_0800a6f8`, `FUN_0800A77C`, `FUN_0800A760`,
   `FUN_0800A6DC`, `FUN_0800A704`, `FUN_0800A78C`) and the DMA
   (`FUN_0800A840`, `FUN_0800A87C`, `FUN_0800A7B0`) vs `hal::adc` and `hal::dma`.

## Agent 3 — `HuntModules` : the GPIO, the module register writes and the IRQ handlers

Own files: `rust/src/hal/gpio.rs`, `rust/src/modules/*.rs`,
`rust/src/proto/*.rs` (only where the register writes are concerned).

1. `FUN_0800A964` (the GPIO configuration) vs `hal::gpio::config` and
   `hal::gpio::config_per_pin` — including the odd part (the output type byte is
   shifted as a whole, `MEMORY.md` and `rust/HAL_PARITY.md` disagree with
   `analysis/decomp/ALL.c` here: derive the truth from the *machine code*).
2. The pin maps of the modules: which pins does each module really configure
   (GPIOA/B/C/F, which pin numbers, which alternate function)?  Compare
   `hwi_adc`, `hwi_drive`, `hwi_enc`, `hwi_sonar`, `hwi_led`, `hwi_buzz`,
   `hwi_mic`, `hwi_line`, `hwi_power`, `hwi_lightsense`, `ble_com` with the dump.
   A wrong pin number is a silent no-op on the robot.
3. The RCC clock enables: which bits does each module really set
   (`FUN_0800AD5C`, `FUN_0800AD7C`, `FUN_0800AD9C`) and does the Rust set the same
   bits?  A missing clock enable is a dead peripheral.
4. The interrupt handlers: `FUN_08008BF8`, `FUN_08008C20` (the encoders),
   `FUN_08008E9C`, `FUN_08008EE8` (the sonar) vs `hwi_enc.rs` and `hwi_sonar.rs` —
   the flags they read, the flags they clear, the registers they touch.
5. `FUN_0800AAC8` (the alternate function helper) — the Rust has two flavours and
   the HalParity review could not settle which one the original uses.  Settle it
   from the machine code.

## Agent 4 — `HuntGaps` : what the dump does and the port does not do at all

Own files: any, but only to add what is missing (`rust/src/modules/*.rs`,
`rust/src/boot.rs`, `rust/src/proxy`, reports).

1. Walk the function list of the dump (`analysis/decomp/FUNCS.txt`, 378 functions)
   and find the ones that the port does not implement at all, and that matter on
   hardware: register writes, waits, error paths, peripheral setups.
   `rust/PARITY.md` lists the function parity already — check it against the dump
   and extend it.
2. The stubs: the 12 interfaces whose handler in the original is a 4-byte stub
   (`movs r0,#0; bx lr`).  Their real behaviour lives in the periodic task
   (`FUN_080063EC`) and the module engines.  Does the port's tick do what the
   original's periodic task does?
3. `FUN_0800AB88` (the power fail path) vs `boot.rs::power_fail_mode`.
   `FUN_0800AB10` (the watchdog), `FUN_0800AB30` (the NVIC), `FUN_0800A084`
   (the "on/1/true" test), `FUN_0800AC40` (the clock helper).
4. `FUN_08009B14` (the command reader) vs `boot.rs::read_line` — including the
   0x28 buffer and the overflow behaviour, and `FUN_080063EC` (the periodic task)
   vs the port's three ticks.
5. `FUN_08006D34` (the battery values from the BLE pack) — the port leaves the
   battery state empty.  Is that reachable on the robot, and what does the
   original do exactly?
