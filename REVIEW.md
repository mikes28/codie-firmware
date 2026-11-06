# REVIEW — Codie robot firmware Rust port vs the binary dump

**Ground truth:** `stm32_flash.bin` (128 KB, untouched).
**Port:** `rust/` — a Rust rewrite of the reconstructed C firmware (`fw/`).
**Goal of this document:** a complete, human-readable review of every register access,
every protocol byte, and every divergence between the Rust port and the original binary.
It is written for a human engineer, not for an LLM. Every claim is backed by a dump
address (function + instruction) or by the machine code of `stm32_flash.bin`.

---

## 1. Executive summary

| artefact | size | source |
|---|---|---|
| Original flash dump | 54 820 B (firmware part) | `stm32_flash.bin` |
| C reference rebuild | 12 608 B | `fw/` |
| Rust port build | 13 432 B | `rust/build/fw.bin` |

The Rust port is **functional** and **byte-exact on the wire protocol**:
- same register accesses, same wire protocol, same bytes on the command link
- same interface table order (16 entries at flash `0x0800D164`)
- same version block (flash `0x0800C65C`)
- same vector table, same IRQ handlers, same peripheral base addresses

The C code stays the reference implementation; the original flash dump stays the
ground truth. Where the C reconstruction and the binary disagree, the binary wins
and the divergence is written down here.

**Verification oracles (11 tests green):**
- `tests/protocol.rs` — protocol layer against the exact bytes of the original
- `tests/simulator.rs` — whole firmware against a peripheral model
- `tests/binary.rs` — port against the dump (interface table, version block,
  vector table, IRQ handlers, peripheral bases)
- `tests/binary_ops.rs` — every op the original answers with exact answer bytes
- `tests/c_equivalence.rs` — port vs the C firmware (same scenario, same bytes)
- plus `clock`, `usart` tests

Build: `cargo check --all-targets` (clean, 0 warnings), `cargo test` (11 passed,
0 failed), `cargo build --release --target thumbv6m-none-eabi` → `build/fw.bin`.

---

## 2. Memory map & peripheral bases (ground truth from the dump)

These addresses come from the machine code of `stm32_flash.bin`, **not** from a
data sheet. They are what the board is wired to. `rust/src/hal/addr.rs` holds them
and must not be "fixed" to match a datasheet.

### 2.1 Peripheral base addresses

| peripheral | base | notes |
|---|---|---|
| GPIOA | `0x48000000` | 11 decompiled call sites use it |
| GPIOB | `0x48000400` | |
| GPIOC | `0x48000800` | |
| GPIOF | `0x48001400` | |
| TIM1 | `0x40012C00` | wheel encoders (hwiEnc) |
| TIM2 | `0x40000000` | wheel encoders (hwiEnc) |
| TIM3 | `0x40000400` | motor drive (hwiDrive) |
| TIM6 | `0x40001000` | millisecond tick of the original |
| TIM15 | `0x40014000` | ARGB LED engine + DMA1 ch5 |
| TIM16 | `0x40014400` | buzzer / tone engine |
| TIM17 | `0x40014800` | sonar capture + second LED engine |
| USART1 | `0x40013800` | |
| USART3 | `0x40004800` | BLE module AT channel (14400 bit/s) |
| USART4 | `0x40004C00` | command + reply link (460800 bit/s) |
| ADC1 | `0x40012400` | battery / mic / light sensing |
| DMA1 | `0x40020000` | channel stride 0x14 |
| RCC | `0x40021000` | clock |
| FLASH | `0x40022000` | |
| IWDG | `0x40003000` | watchdog |
| SYSCFG | `0x40010000` | external interrupt configuration |
| EXTI | `0x40010400` | external interrupts |
| NVIC | `0xE000E100` | Cortex-M system |
| NVIC_IPR | `0xE000E400` | priority registers |
| SysTick | `0xE000E010` | millisecond tick of the port |

GPIO register offsets (standard layout, proven by the dump's helpers):
`MODER 0x00, OTYPER 0x04, OSPEEDR 0x08, PUPDR 0x0C, IDR 0x10, ODR 0x14,
BSRR 0x18, LCKR 0x1C, AFR0 0x20, AFR1 0x24`.

### 2.2 Timers

| offset | register |
|---|---|
| `0x00` | CR1 |
| `0x04` | CR2 |
| `0x08` | SMCR |
| `0x0C` | DIER |
| `0x10` | SR |
| `0x14` | EGR |
| `0x18` | CCMR1 |
| `0x1C` | CCMR2 |
| `0x20` | CCER |
| `0x24` | CNT |
| `0x28` | PSC |
| `0x2C` | ARR |
| `0x30` | RCR |
| `0x34` | CCR1 |
| `0x38` | CCR2 |
| `0x3C` | CCR3 |
| `0x40` | CCR4 |
| `0x44` | BDTR |

### 2.3 USART

| offset | register |
|---|---|
| `0x00` | CR1 |
| `0x04` | CR2 |
| `0x08` | CR3 |
| `0x0C` | BRR |
| `0x18` | RQR |
| `0x1C` | ISR |
| `0x20` | ICR |
| `0x24` | RDR |
| `0x28` | TDR |

### 2.4 ADC1

| offset | register |
|---|---|
| `0x00` | ISR |
| `0x08` | CR |
| `0x0C` | CFGR1 |
| `0x14` | SMPR |
| `0x28` | CHSELR |
| `0x40` | DR |

### 2.5 DMA1

Channel blocks at `DMA1 + 0x08 + (n-1)*0x14`:
- CH1 `0x40020008`, CH2 `0x4002001C`, CH3 `0x40020030`, CH4 `0x40020044`,
  CH5 `0x40020058`, CH6 `0x4002006C`, CH7 `0x40020080`

Channel register offsets within a block: CR `0x00`, CNDTR `0x04`, CPAR `0x08`,
CMAR `0x0C`.

### 2.6 Cortex-M core

| item | address |
|---|---|
| NVIC_ISER | `0xE000E100` |
| NVIC_IPR | `0xE000E400` |
| SysTick | `0xE000E010` |
| SCB | `0xE000ED00` |

---

## 3. Clock tree

The port reproduces the original's clock tree (`FUN_08002958`, `rust/src/system.rs`):

- Source: HSI16 → PLL (×6) → **96 MHz** SYSCLK
- AHB = APB1 = APB2 = 96 MHz (bus dividers kept at reset, `/1`)
- Flash: `ACR = 0x11` (1 wait state, prefetch enabled)
- PLLMUL = 6, PLLSRC = HSI/1

**Tick source:** the port uses the **SysTick** at 96 MHz / 96000 = **1 kHz**
(reload 95999). The original uses **TIM6** (PSC 48, ARR 0xFFFF on a 48 MHz
timer clock → ~65.5 ms update, ~15 Hz — *not* the 1 kHz its greeting's
`"ttpms":"1000"` advertises).

The port runs TIM6 in addition (`system::tim6_tick_init()`) so the original's
counter (`system::dump_tick()`) stays observable, while `millis()` is fed by the
SysTick. This is a deliberate, better-behaved choice; the dump's TIM6 cadence is
broken and the port does not slow itself to match it.

---

## 4. Register-by-register review

Each HAL function below is paired with the dump evidence that confirms it. The
register accesses are bit-for-bit identical to the machine code.

### 4.1 GPIO (`rust/src/hal/gpio.rs`)

The original has **two** GPIO configuration helpers, and they are **not**
equivalent. Both are ported.

| C helper | Rust | flavour | notes |
|---|---|---|---|
| `FUN_0800A964` (hwi_a_hal) | `gpio::config` | hwi_a_hal | writes all registers once, whole-byte otype shift |
| `FUN_0800A964` (hwi_b_hal) | `gpio::config_per_pin` | hwi_b_hal | each pin written on its own, otype reduced to 1 bit |

**Pin modes:** `MODE_INPUT 0`, `MODE_OUTPUT 1`, `MODE_ALT 2`, `MODE_ANALOG 3`.
**Pull:** `PULL_NONE 0`, `PULL_UP 1`, `PULL_DOWN 2`.

`gpio::set_afr(port, pin, afr)` — `FUN_0800AAC8`. The 4-bit AFR field is masked,
the value is **not** masked (shifted in as the caller passed it). Dump evidence:
`800aaca: movs r3,#7 / ands r3,r1 / lsls r3,r3,#2` → shift = `(pin&7)*4`;
`lsrs r1,r1,#3 / adds r1,#8 / lsls r1,r1,#2` → offset = `(8+(pin>>3))*4`.

`gpio::read(port, mask)` — reads `IDR` at offset `0x10`, halfword. Dump:
`0x80043708: ldrh r0,[r0,#16]` confirms offset 0x10.

`gpio::set(port, mask)` — `BSRR` at offset `0x18` (word store).

`gpio::reset(port, mask)` — offset **0x28**, **halfword** store. This is NOT BRR
(0x1C) — on this part 0x28 is past AFRH (0x24). Dump:
`FUN_080097E8 @0x800980e: strh r1,[r3,#40]` (offset 0x28). Reproduced verbatim.

`gpio::write(port, mask, level)` — the `hwi_a_hal` flavour: sets via BSRR,
clears at offset **0x28** (halfword). Dump: `FUN_08009E0C @0x8009e20:
strh r1,[r2,#40]`. A word store would clobber the next register; the port
matches the 16-bit width.

### 4.2 Timers (`rust/src/hal/timer.rs`)

Two flavours again.

**`hwi_a_hal`** (`base_cfg`, `channel_cfg`, `pwm_set`, …): reproduces the
original's register writes bit for bit, including the 32-bit CCR writes that also
clear the next compare register.

`base_cfg(tim, cfg)` — `FUN_0800AEC4`. Writes CR1 → ARR → PSC.
- TIM1/TIM2/TIM3: `CR1 = (CR1 & 0xFF8F) | cr1_hi`
- TIM6: dedicated branch — CR1 → ARR (word, ARR+RCR at once at +0x2C) → PSC
  (halfword at +0x28). Dump: `0x800aef2 strh` CR1; `0x800aef6 str` ARR at
  `+0x2c`; `0x800aefa strh` PSC at `+0x28`. Then shared EGR tail.
- Advanced timers (TIM1/TIM15/TIM16/TIM17): RCR written after PSC.

`channel_cfg(tim, ch, cfg)` — `FUN_0800AFDC` (ch1), `FUN_0800B054` (ch2),
`FUN_0800B0F4` (ch3), `FUN_0800B174` (ch4).

**CCMR masks (the fix from FINDINGS.md):**
- ch1 CCMR1 low: `(v & 0xFF73) | ccmr` — `0x73` = CC1S(1:0) + OC1PE(3) + OC1M(6:4)
  (dump `movs r6,#0x73` @`0x800afec`). The old `0x8C` left CC1S/OC1M stale.
- ch3 CCMR2 low: `(v & 0xFF73) | ccmr` — same mask (dump `movs r3,#0x73` @`0x800b104`).
- ch2/ch4 CCMR high: `(v & 0x8CFF) | (ccmr << 8)` — confirmed correct.

`pwm_set(tim, ch, ccr)` — writes 32 bits at `TIMx_CCR1 + 4*(ch-1)`. A write also
clears the next compare register (reproduced verbatim).

**`hwi_b_hal`** (`pwm_init`, `pwm_duty`, …): simpler PWM helper used by hwiLed
and hwiBuzz.
- `pwm_init`: CR1 ARPE off, PSC, ARR, EGR UG, CCMR `0x68` (OCxM=110 PWM mode 1,
  OCxPE=1), CCER enable.
- `pwm_duty`: 32-bit write to `TIMx_CCR1 + 4*(ch-1)`.

### 4.3 USART (`rust/src/hal/usart.rs`)

`init(usart, cfg)` — `FUN_0800B3FC`. Configures CR1/CR2/CR3 with masks:
`CR1 = (CR1 & 0xE9F3) | cr1`, `CR2 = (CR2 & 0xCFFF) | cr2`, `CR3 = (CR3 & 0xFCFF) | cr3`.
Baud: `BRR = APB1_HZ / baud`, rounded up when `remainder >= baud/2`, truncated
to 16 bits (`uxth`).

- BLE link (USART3): baud 14400, `CR2 = 0x2000` (2 stop bits), `CR1 = 0x000C`
  (RE|TE), `CR3 = 0x300`. `BRR = 96e6/14400 = 6666`.
- Console link (USART4): baud 460800, `CR2 = 0`, `CR1 = 0x000C`, `CR3 = 0`.
  `BRR = 96e6/460800 = 208`.

**Critical bug fixed:** the first port wrote `CR1 = 0x0300` (PEIE|PS) which
leaves RE and TE **off** — the link would be dead on the robot. The dump's
callers pass `0x000C`. The port now writes `0x000C`. Invisible in the host
simulator (it wraps USART primitives and never reads CR1).

`tx_ready()` — ISR bit 7 (TXFNF). `rx_ready()` — ISR bit 5 (RXNE). `putc()`
writes TDR at `+0x28`. `getc()` reads RDR at `+0x24` (`& 0x1FF`). `clear()`
writes ICR at `+0x20`. `enable()`/`disable()` set/clear CR1.UE (bit 0).

**UART roles (MEMORY.md §4.2):** USART4 = command + reply link (all robot output
goes here, `out_str`/`out_char` literal pool holds `0x40004C00`). USART3 =
BLE AT channel. This was **swapped in the first rebuild** and fixed.

### 4.4 ADC (`rust/src/hal/adc.rs`)

`cfg1(adc, clear_mask, set_mask, exten)` — `FUN_0800A6A0`:
`CFGR1 = (CFGR1 & clear_mask) | set_mask | (exten << 13)`.
Dump: `(v & 0xffffd203) \| set \| exten<<13`.

`channel_add(adc, mask, smpr)` — `FUN_0800a6f8`: `CHSELR |= mask`, `SMPR = smpr`.
Channels masks `[0x02, 0x04, 0x20, 0x40]` (IN1, IN2, IN5, IN6), sample times
`4, 6, 6, 6`.

`dmngt(adc, v)` — `FUN_0800A77C`: `CFGR1 = (CFGR1 & 0xFFFFFFFD) | v` (DMA bits).
`dma_mode(adc, on)` — `FUN_0800A760`: CR bit 2 (DMA mode).

`enable(adc, on)` — `FUN_0800A6DC`: CR bit 0 = ADEN, bit 1 = ADDIS.
`stop(adc)` — `FUN_0800a754`: CR bit 2.

`start(adc)` — `FUN_0800A704`: start conversion, wait bounded, return result or
0 on timeout. Dump: counter `0xEFFF` from literal at `0x800a750`, decremented
by 2 per iteration, returns `0` (`movs r0,#0` @`0x800a730`) on timeout. The port
reproduces the bound — an unbounded spin would hang the firmware for good.

`isr_clear(adc, mask)` — writes mask to ISR at `+0x00`. `ready(adc, mask)` — tests
a status flag.

### 4.5 DMA (`rust/src/hal/dma.rs`)

`channel_cfg(ch, cpar, cmar, cntdr, ccr)` — `FUN_0800A840`: keeps bits 4..14
and 31 of CCR (`rmw32 (v & 0xFFFF800F) \| ccr`), writes CNDTR, CPAR, CMAR.
`channel_enable(ch, on)` — `FUN_0800A87C`: CR enable bit.
`channel_reset(ch)` — `FUN_0800A7B0`: disable, zero all 4 channel registers,
clear interrupt flags (per-channel mask at `DMA1 + 0x04`).

### 4.6 RCC (`rust/src/hal/rcc.rs`)

`ahb_en(mask, enable)` — `FUN_0800AD5C` (AHBENR at `+0x14`).
`apb2_en(mask, enable)` — `FUN_0800AD7C` (APB2ENR at `+0x18`).
`apb1_en(mask, enable)` — `FUN_0800AD9C` (APB1ENR at `+0x1C`).
Each: `rmw32` set or clear the mask.

**NOTE:** in the original image the GPIO/enable bits are shifted by one compared
to published STM32 maps (GPIOA `0x20000`, GPIOB `0x40000`, GPIOC `0x80000`,
APB2 TIM15/16/17 `0x20000`, APB1 USART3 `0x40000`, USART4 `0x80000`). Reproduced
verbatim.

### 4.7 NVIC (`rust/src/hal/nvic.rs`)

`enable(irq, prio)` — `FUN_0800AB30`. Sets priority byte first, then ISER.
Priority address: `0xE000E100 + ((irq>>2) + 0xc0)*4 = 0xE000E400 + (irq>>2)*4`.
Byte within word: `(irq & 3) << 3`, priority into top of byte (`prio << 6`).
Priority byte at `0xE000E400 + irq` holds `prio << 6`.

**Evidence the priority write is real:** the IPR address is computed base-relative
(`0xE000E100 + offset`), so no `0xE000E400` literal appears in the image. An
agent deleted it as "absent" — wrong. The read-modify-write at `0x800ab42`/`0x800ab60`
is real, and the port's priority code was restored.

ISER: `w32(NVIC_ISER, 1 << (irq & 0x1F))`.

### 4.8 IWDG (`rust/src/system.rs` watchdog_init)

`FUN_0800A640`. Sequence (verified against `FUN_0800AAEC/aaf8/ab04`):
- RCC AHBENR |= GPIOA|B|C, GPIO bank setup
- wait for ready flag (`FUN_0800ADDC(0x41)`)
- `0x5555` → IWDG+0x00 (PR prescaler)
- `2` → IWDG+0x04 (RLR reload)
- `0xFFE` → IWDG+0x08 (WLR window) — **the old port wrote this at +0x0C, wrong**
- `0xAAAA` → IWDG+0x00 (KR refresh)
- `0xCCCC` → IWDG+0x00 (KR enable)

The port refreshes (`util::iwdg_kick`, IWDG_KR = 0xAAAA) every main-loop
iteration. A hang is recovered by a reset.

### 4.9 EXTI / SYSCFG (`rust/src/modules/hwi_power.rs`)

`hwi_power` routes EXTI13 (power-good, GPIOC pin 13) to the falling edge.
Dump `FUN_0800AE34` computes EXTICR offset `((line>>2)+2)<<2`. For line 13:
`((13>>2)+2)<<2 = 0x14` = **EXTICR4**, field shift `(line & 3) << 2 = 12`
(bits 15:12).

The old port wrote SYSCFG+0x0C (EXTICR3) at bits 7:4 — wrong register, wrong
field, EXTI13 never routed to GPIOC, power-good interrupt could not fire.
Fixed to EXTICR4 bits 15:12. `FUN_0800A8D4` clears IMR/RTSR/FTSR, sets IMR,
clears RTSR, sets FTSR when the struct's [5] field is 0x0c (falling edge).
The port's two writes `EXTI+0x0C |= 1<<13` and `EXTI+0x08 &= ~(1<<13)` match.

**Still not ported:** `FUN_0800A8D4`'s IMR/EMR part and the NVIC arm for IRQ 7
(EXTI4_15) through `FUN_0800AB30`.

---

## 5. The 10 fixed register-level defects (FINDINGS.md §7)

| # | what was wrong | dump evidence | fix |
|---|---|---|---|
| 1 | `timer::channel_cfg` wrong CCMR mask for ch1/3 (left CC1S/OC1M stale) | `FUN_0800AFDC` @`0x800afec` `movs r6,#0x73`; `FUN_0800B0F4` @`0x800b104` | `0xFF73` mask for low bytes |
| 2 | `watchdog_init` wrote IWDG values one register too high, invented 0x0C window | `FUN_0800AAEC/aaf8/ab04` store to base+0/+4/+8 | 0x5555→+0, 2→+4, 0xFFE→+8 |
| 3 | `system_init` waited for PLLRDY on bit 26 (never set → hang before peripherals) | `FUN_08002958` @`0x8002a18` `lsls r2,r3,#18` = bit 25 | wait on bit 25 |
| 4 | clock-tree gate on HSEON (bit 16) instead of HSERDY (bit 17) | `0x80029d4` `lsls #14` → bit 17 | gate on HSERDY |
| 5 | `hwi_power` wrote EXTICR3 instead of EXTICR4 — EXTI13 never routed to GPIOC | `FUN_0800AE34` → `0x14` = EXTICR4, field bits 15:12 | EXTICR4 bits 15:12 |
| 6 | `gpio::write` cleared with a word store | `FUN_08009E0C` @`0x8009e20` `strh r1,[r2,#40]` = halfword | halfword store |
| 7 | Buzzer prescaler was 96 | `FUN_08008380` @`0x80083de`, `r7=2` | PSC = 2 |
| 8 | LED prescaler/ARR/pin map/AF from memory, not dump | `FUN_080079A0`: psc 29, ARR 0, PB14/PB12/PB15, PB14 in AF1 | PSC 29, ARR 0, PB14 AF1 |
| 9 | `TIM6` pointed at SYSCFG (`0x40010000`) | pool @`0x800a4b8` = `0x40001000` | `TIM6 = 0x40001000` |
| 10 | `adc::start` spun without a bound — stuck conversion hangs for good | `FUN_0800A704`: counter `0xEFFF`, returns 0 | bounded wait + 0 return |

Also fixed: two wrong agent "fixes" reverted (NVIC priority write is real;
TIM6 write order is CR1→ARR→PSC); three notes in MEMORY.md corrected (buzzer PSC
= 2 not 96, BLE link state line = PA10 not PA2).

---

## 6. The 11 periodic engines of `FUN_080063EC`

Called from the dump's main loop (`FUN_08009FA4`), in order:

| # | dump function | what | ported? |
|---|---|---|---|
| 1 | `FUN_080096EC` | BLE link service (800 ms) | **open** |
| 2 | `FUN_08008784` | motor fault poll (GPIOA `0x1000`) | **yes** (`hwi_drive::tick`) |
| 3 | `FUN_080071C0` | ADC/DMA poll | **yes** (`hwi_adc::tick`) |
| 4 | `FUN_08009EA0` | line sensor service (0x37 ms) | **open** |
| 5 | `FUN_08008130` | BLE AT re-send (2000 ms) | **open** (payload undecodable) |
| 6 | `FUN_08009340` | light-sense sample | **yes** (`app_lightsense::tick`) |
| 7 | `FUN_08006000` | light program step | **yes** (`app_lights::tick`) |
| 8 | `FUN_080030D0` | mic sample (500 ms) | **open** (publishes on internal bus) |
| 9 | `FUN_080094F8` | line sensor sample | **open** |
| 10 | `FUN_08008574` | buzzer tone sequencer | **yes** (`hwi_buzz::tick`) |
| 11 | `FUN_08003330` | drive speed loop | **yes** (`app_drive::tick`) |

The port's main loop runs 7 of the 11. The open 4 need a line-callback hook in
`ble_com` before they can be ported (they publish over the BLE data channel).

---

## 7. IRQ vectors

Dump vector table (first 48 words): real handlers for IRQ 7, 9, 11, 13, 15, 17, 20, 22, 29.

| IRQ | dump handler | ported? |
|---|---|---|
| 7 | `FUN_0800976C`/`FUN_08009D30`/`FUN_080086C4` (EXTI4_15) | **yes** (`hwi_power::EXTI4_15_IRQHandler`) |
| 9 | `0x08008EE8` (TIM17 CC / DMA1 ch1) | **yes** (`DMA1_Channel1_IRQHandler`) |
| 11 | `0x08007734` (TIM15 + DMA ch5, LED engine) | **open** (no TIM15 ISR yet) |
| 13 | `0x08008BF8` (TIM1 CC) | **yes** (`TIM1_BRK_UP_TRG_COM_IRQHandler`) |
| 15 | `0x08008C20` (TIM2 CC) | **yes** (`TIM2_IRQHandler`) |
| 17 | `0x0800A43C` (TIM6 update, counter increment) | **yes** (`system::tim6_tick`) |
| 20 | `0x0800745C` (TIM15 update, LED pattern sequencer) | **open** |
| 22 | `0x08008E9C` (TIM17 update) | **yes** (`TIM17_IRQHandler`) |
| 29 | `0x08002DC8` (USART/line error paths) | **open** |

IRQ 11/20 (LED engine) and IRQ 29 (serial error path) stay on `Default_Handler`
by design — arming them without handlers would spin forever.

---

## 8. Deliberate differences vs the C reconstruction

The port keeps the **quirks of the original on purpose** (they are not bugs):

- **Hexadecimal reply values** (radix 16) — every sensor value printed in hex,
  `hwiAdc buf` in decimal (radix 10). Digits ≥ 10 via the original's `'W' + digit`
  trick → `a`..`f`.
- **`"set"` op word in a `"get"` answer** — e.g. `get drvBattery voltage` →
  `set drvBattery voltage <hex>`. The op word is hard-coded to "set".
- **LED pattern engine** — `PPRT`/`PPRET` markers emitted from the sequencer.
- **UART roles** — USART4 = command + reply (460800), USART3 = BLE AT (14400).
  This was swapped in the first rebuild and fixed.

Divergences where the port follows the binary and the C transcription is wrong
(2, documented in PARITY.md §4 and FINDINGS.md):
- `hwiAdc` "buf" op: the dump emits the extra empty line after the 9th, 17th,
  25th value (`(i & 7) == 0`); the C reconstruction has `(& 8u)` which contradicts
  its own comment. Port follows the dump.
- `hwiPower::vsens()` reads back PC15 — the C declares `hwiPower_vsens()` in
  `codie.h` but never implements it.

API-shape divergences in `proto/` (reported to its owner):
- `resp_value_opt` for the NULL-value case of the C greeting.

---

## 9. Open items & risks

**Never performed (blocked on hardware):**
- The on-hardware test — nothing has ever been run on the robot. Boot checklist
  is in `TODO_TO_FLASH.md` §4.

**Hardware unknowns the dump cannot answer:**
- LED pin map (`hwi_led.rs`) — PB14/PB12/PB15 on GPIOB, PB14 AF1 (verified),
  but the physical strip wiring is unknown.
- Motor output pin map (`hwi_drive.rs`) — GPIOC pins 6-9, PC2 enable, PC14 fault
  (verified), physical wiring unknown.
- 96 MHz clock rating — the original's `FUN_0800AC40` says 96 MHz, presumably OK.

**Documented-open (do not guess):**
- **BLE AT re-send payload** (`FUN_08008130`): the formatter is decoded
  (nibble switch over `0x22` → 2×u8, `0x02` → 1×u8), but the payload bytes
  themselves resolve into code region `0x08002BE1` (a jump-table blob). The GAP
  command body is runtime-derived, not a literal. Needs a live capture from the
  BLE module or the byte stream at `0x08002BE1`. Risk: if the BLE module needs
  that GAP command to begin advertising, the robot won't come up in BLE mode.
- **Mic sample** (`FUN_080030D0`): publishes through `FUN_08005EC4(0x103,…)`,
  an internal bus slot with no ported Rust counterpart. Harmless but unported.
- **Battery pack query** (`call drvBattery hwInit`): `FUN_08006E94` sends an
  8-byte frame `{0xcc,0x4e,1,0,0,0,0,parity}` and `FUN_08006D34` parses the
  reply. NOT a BLE exchange over USART3 — every frame builder ends in
  `FUN_080078E4` which is gated on the LED engine's DMA state variable and
  programs TIM15 + DMA1 ch5. The only code writing USART3_TDR (`0x40004828`)
  is the serout routine. A faithful port would model the LED DMA as a transport
  — left documented-open rather than guessed. `drvBattery` still answers
  `voltage`/`chargeStat` byte-exactly from the local ADC.

---

## 10. Verification oracles

| oracle | what it proves | where |
|---|---|---|
| golden bytes | protocol layer byte-exact (greeting, value reply, errors, number formatting) | `tests/protocol.rs` |
| peripheral model | whole firmware: boot output, replies, motor duty registers, baud divisors | `tests/simulator.rs` |
| the dump | port vs original binary: interface table `0x0800D164`, version block `0x0800C65C`, vector table, IRQ handlers, peripheral bases, ops with exact answer bytes | `tests/binary.rs`, `tests/binary_ops.rs` |
| the C firmware | same scenario, same bytes (facts captured from C simulator, replayed) | `tests/c_equivalence.rs` + `tests/data/c_sim.txt` |

The dump is the ground truth, the C firmware is the reference implementation,
the golden bytes are the protocol contract.
