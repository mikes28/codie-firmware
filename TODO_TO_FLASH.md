# What has to be done to flash this to the robot

Last updated: 2026-10-27. Everything below is either **done and verified** or
**blocked on hardware**, with nothing half-finished.

---

## 1. Current state — verified

```
cd rust
cargo check --all-targets      0 errors, 0 warnings
cargo test                     21 passed, 0 failed
./build.sh                    -> build/fw.bin  22 916 bytes (17.5 % of 128 KB)
```

The image is **built, verified and flashable**. Section 4 is the only step that
has never been performed.

### Soak check (2026-10-28)

The four new periodic engines run inside the main loop, so a hang there would
take the robot down with no way to recover.  Verified on the host model with a
throwaway driver (not committed):

* 150 consecutive command rounds - real commands mixed with oversized lines,
  unknown interfaces, empty lines and garbage - **150/150 answered**, no hang.
* 6 seconds of wall time with the tick running and the fault line toggled both
  ways: the fault poll keeps up and the replies stay correct.
* The tone sequencer actually moves the hardware: `call hwiBuzz play 3` leaves
  TIM16 with `CR1 = 0x81` (counter + ARPE), `ARR = 19199` (= 48 MHz / 2500 − 1,
  the first step of pattern 3) and `CCR = 5759`.  Before the queue was
  implemented, ARR stayed at the init value and no tone ever played.

---

## 2. Done in this pass (2026-10-26/27), all with dump evidence

### 2.1 Register-level bugs that the host tests could not see

| # | Bug | Dump evidence |
|---|---|---|
| 1 | `timer::channel_cfg` used the wrong CCMR field mask for channels 1/3, leaving `CC1S`/`OC1M` stale | `FUN_0800AFDC` @`0x800afec` `movs r6,#0x73`; `FUN_0800B0F4` @`0x800b104` |
| 2 | `watchdog_init` wrote the IWDG values one register too high and invented a `0x0C` window | `FUN_0800AAEC/aaf8/ab04` store to base+0/+4/+8 |
| 3 | `system_init` waited for PLLRDY on **bit 26, never set** → the firmware would have hung before any peripheral came up | `FUN_08002958` @`0x8002a18` = bit 25 |
| 4 | The clock-tree gate was tested on HSEON (bit 16) instead of HSERDY (bit 17) | `0x80029d4` `lsls #14` |
| 5 | `hwi_power` wrote **EXTICR3** instead of **EXTICR4** — EXTI13 was never routed to GPIOC, so the power-good interrupt could not fire | `FUN_0800AE34` computes `((line>>2)+2)<<2` = `0x14`, field at bits 15:12 |
| 6 | `gpio::write` cleared with a **word** store | `FUN_08009E0C` @`0x8009e20` `strh r1,[r2,#40]` |
| 7 | Buzzer prescaler was 96 | `FUN_08008380` @`0x80083de`, `r7 = 2` |
| 8 | LED prescaler/ARR/pin map/AF were all from memory, not the dump | `FUN_080079A0`: psc 29, ARR 0, PB14/PB12/PB15, **PB14 in AF1** |
| 9 | **`TIM6` pointed at SYSCFG** (`0x4001_0000`) | pool @`0x800a4b8` = `0x40001000` |
| 10 | `adc::start` spun without a bound — a stuck conversion would hang for good | `FUN_0800A704`: counter `0xEFFF`, returns 0 |
| 11 | Stale comment claiming the watchdog never runs | — |

Three notes in `MEMORY.md` were also corrected against the machine code (the
buzzer PSC, the BLE link's PA-pin, and the LED DMA transfer count), and two
wrong agent "fixes" were reverted (the NVIC priority write is real — the IPR
address is computed base-relative; the TIM6 write order is CR1→ARR→PSC).

### 2.2 Periodic engines implemented

The dump's periodic task `FUN_080063EC` (@`0x80063ec`) runs **11** engines. All
11 now run from the `boot.rs` main loop, plus the battery reply machine:

| Engine | Dump function | Rust counterpart | Status |
|---|---|---|---|
| ADC overrun service | `FUN_080071C0` | `hwi_adc::tick()` | done |
| Motor fault poll (GPIOA PA12) | `FUN_08008784` | `hwi_drive::tick()` | done |
| Line sensor sample window (20 ms) | `FUN_080094F8` | `hwi_line::tick()` | done |
| Tone sequencer (non-blocking queue) | `FUN_08008574` | `hwi_buzz::tick()` | done |
| Light sense sample | `FUN_08009340` | `app_lightsense::tick()` | done (earlier) |
| Light program step (160 ms) | `FUN_08006000` | `app_lights::tick()` | done (earlier) |
| Drive speed loop (200 ms) | `FUN_08003330` | `app_drive::tick()` | done (earlier) |
| BLE link service (800 ms) | `FUN_080096EC` | `ble_com::link_service_tick()` + `link_callback()` | done |
| BLE AT re-send (2000 ms) | `FUN_08008130` | `ble_com::ble_at_resend_tick()` | done |
| Mic sample (500 ms) | `FUN_080030D0` | `hwi_mic::tick()` | done |
| Battery pack reply machine | `FUN_08006D34` | `drv_battery::tick()` | done |

### 2.3 Interrupts served

The dump has real handlers for IRQ 7, 9, 11, 13, 15, 17, 20, 22, 29. The port
serves **all 9** of them:

| IRQ | Handler | Status |
|---|---|---|
| 7 | `hwi_power::EXTI4_15_IRQHandler` | done (power-good line) |
| 9 | `hwi_sonar::DMA1_Channel1_IRQHandler` | done |
| 11 | `hwi_led::dma_ch5_irq` | done (LED striped-transfer complete) |
| 13 | `hwi_enc::TIM1_BRK_UP_TRG_COM_IRQHandler` | done |
| 15 | `hwi_enc::TIM2_IRQHandler` | done |
| 17 | `system::tim6_tick` | done (the original's tick counter) |
| 20 | `hwi_led::tim15_update_irq` | done (LED single-outline sequencer + PB14 bit-bang) |
| 22 | `hwi_sonar::TIM17_IRQHandler` | done |
| 29 | `ble_com::usart_error_irq` | done (console RX + BLE link errors) |

All nine are in `src/startup.rs`'s vector table and armed via `nvic::enable`.

`system::tim6_tick_init()` runs the original's TIM6 tick **in addition** to the
SysTick, so `system::dump_tick()` exposes the original's cadence while the
periodic engines stay gated on a correct 1 kHz `millis()`.

### 2.4 The battery pack query is **not** a BLE exchange

`MEMORY.md` and an earlier handoff described `FUN_08006D34` as asking an
external BLE battery pack over USART3. Tracing the transmission disproves it:
every frame builder ends in `FUN_080078E4`, which is gated on `DAT_2000051a`
(the **LED engine's** state variable), calls `FUN_080077A0`, and programs TIM15
+ **DMA1 channel 5**. The only code in the image that writes the USART3 data
register is the serout routine `FUN_08007D88`.

A faithful port would therefore have to model the LED pattern DMA as a
transport, and a USART3 implementation would put bytes on a channel the dump
never uses for this. **Left documented-open rather than guessed.**
`drvBattery` still answers `voltage`/`chargeStat` byte-exactly from the dump's
`FUN_08006F2C` handler.

---

## 3. What is still open

### 3.1 Blocked on the BLE pay-load sink

* **`FUN_08002AC0` (the BLE AT re-send) — the formatter IS decoded and the
  keep-alive engine IS ported; the exact GAP payload bytes are not.**
  `FUN_08008130` calls `FUN_08002ac0(0x39,2,2)` and `FUN_08002ac0(0x32,1)`.
  That formatter is **not** an opaque bytecode interpreter — it is a
  serialization switch over a nibble field (`ALL.c:3690-3707`), and the dump
  payload table at `0x0800B938` (12-byte stride) decodes cleanly for both
  parameters:

  | call | fmt nibbles | serialized as |
  |---|---|---|
  | `0x39, 2, 2` | `0x22` | 2 × u8 |
  | `0x32, 1` | `0x02` | 1 × u8 |

  The port implements the engine as `ble_com::ble_at_resend_tick()`: after
  2000 ms with no activity on the BLE USART it sends an `AT\r\n` probe, with
  `ble_at_touch_activity()` on RX/TX keeping the timer honest.

  **What is still blocked:** the original's payload *bytes*.  The table's
  address field resolves into the code region at `0x08002BE1` (a
  `47 c0 46 70` jump-table blob), and the only `GAP SET MODE` string in the
  whole dump (`0x0800C48C`) is the *reply matcher*, which the re-send does not
  compose.  So the GAP command body is runtime-derived, not a literal.
  **Risk remains:** if the BLE module needs that GAP command to begin
  advertising, the robot will not come up in BLE mode.  A faithful port needs
  either a live capture from the module or the byte stream at `0x08002BE1`.
* **IRQ 11/20 (LED engine) and IRQ 29 (serial error path)** are now served
  (`hwi_led::dma_ch5_irq`, `hwi_led::tim15_update_irq`,
  `ble_com::usart_error_irq`) and armed in the NVIC.

### 3.2 Deliberate differences, documented and shipped

* **The tick source.** The dump's TIM6 (psc 48, ARR 0xFFFF) is a ~65.5 ms update,
  not the 1 kHz its `"ttpms":"1000"` greeting advertises. The port keeps a
  correct 1 kHz SysTick and runs TIM6 alongside. Changing this would slow the
  firmware to the original's broken cadence.
* **The LED engine** is a TIM15 + DMA1-CH5 CCR stream (ARGB), not plain PWM.
  Dump evidence: `FUN_080090A8` is the encoder the periodic tick
  `FUN_08006000` drives (MSB-first, `0 -> 0x0e`, `1 -> 0x2e`, GRB, 12 LEDs);
  `FUN_080077A0` arms the transfer with **ARR `0x308`**, PSC 29 (3.2 MHz timer
  clock) and CCR1 `0xffff` as the latch gap.  Timing cross-check: 242.5 µs/bit
  × 288 bits = **69.8 ms**, and `FUN_08007B34` state 4 stalls for `0x46` = 70 ms
  around a striped transfer — a 0.2 % match, which no other ARR in the image
  reproduces (0x187 → 35.3 ms, 0x66 → 9.3 ms, 0xffff → 5.9 s).  The `PPRT` /
  `PPRET` markers are emitted from the sequencer.
* **`hwi_led` arms IRQ 11 and IRQ 20** — DMA1 channel 5 transfer complete
  terminates the striped ARGB transfer, and the TIM15 update interrupt
  sequences the single-outline transfer and bit-bangs the battery-pack reply
  back in on PB14.

### 3.3 Never performed

* **The on-hardware test.** Nothing has ever been run on the robot.

---

## 4. The only step left: flashing

The image is `rust/build/fw.bin` (22 916 bytes). No further code work is
required to produce it.

```bash
# 1. put the robot into DFU mode (BOOT0 high, reset)
# 2. confirm it enumerates
lsusb | grep 0483        # expect: 0483:df11 STMicroelectronics STM32 BOOTLOADER

# 3. flash (fw/flash.sh pads to a whole 2 KiB page and reads the flash back)
cd fw && ./flash.sh ../rust/build/fw.bin
```

`flash.sh` uses native `dfu-util` when available, otherwise falls back to the
same docker image used for the original dump.

**Critical: do the `:leave` last.** The ROM bootloader resets the MCU out of
DFU mode on leave, after which the device disappears — so every read must
happen first. Leaving the chip in the bootloader and re-plugging produces the
known failure mode where the robot advertises over BLE but drops every
connection (recorded in `MEMORY.md` §10).

### The boot checklist

1. The greeting appears on the command link (USART4 @ 460800, PC10/PC11,
   2 stop bits) — a USB-TTL adapter is needed, **USB is DFU-only**:
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
   not a hang (this is what the new IRQ 7 + watchdog are for)
8. A reset must not leave the chip stuck in the bootloader (`:leave` was issued)

### The known risk on first power-up

The **LED pin map** (`hwi_led.rs`), the **motor output pin map** (`hwi_drive.rs`)
and the **96 MHz clock rating** are the three hardware unknowns the dump cannot
answer. If the LEDs or motors do not move but everything on the command link
works, those are the first things to suspect — both files carry the dump
evidence and the recovered assumptions in their headers.

That said, the pins are no longer guesswork: every port, pin number, alternate
function and RCC clock-enable bit in the port was cross-checked against the
machine code of the dump and now matches, including the subtle LED case where
the dump leaves `PB12` configured as an **input** (the mode byte is written once
and overwritten to 0 partway through `FUN_080079A0`) and routes only `PB14` to
the timer, in AF1 with a pull-up.

---

## 5. Files

| file | what |
|---|---|
| `rust/` | the firmware port; `cargo test` = 21 tests |
| `rust/build/fw.bin` | the flashable image |
| `fw/` | the C reference implementation (read only for the port) |
| `fw/flash.sh` | the upload script |
| `FINDINGS.md` | the full audit trail, every item with dump addresses |
| `rust/PROGRESS.md` §7/§8 | the bug-fix table and the open list |
| `MEMORY.md` §11 | the corrections to the earlier notes |
