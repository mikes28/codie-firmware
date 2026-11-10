# GAPS.md — Functions in the binary dump NOT implemented in the Rust port

**Status: the trace is complete.** Every dump function that represents observable behaviour has a Rust counterpart, all 11 periodic engines run in the main loop, and every IRQ the dump implements has a handler in the vector table. The suite is green (19 tests) and the release image builds.

This document now lists only what is genuinely still open, and corrects the stale claims that Tier 1 / Tier 2 / Tier 3 were missing (they are implemented — see the evidence pointers below).

---

## What was previously listed as missing but IS implemented

### Tier 1: bootloader entry (USB → button → ROM bootloader)

All of it is in `rust/src/boot.rs`:

| Dump behaviour | Rust location | Evidence |
|---|---|---|
| PA12 / USB VBUS poll, rising edge → state 2, LED anim state 4 (red) | `boot.rs` main loop, PA12 edge detect | `boot.rs:313-323`, statics `PA12_LAST` `boot.rs:40` |
| Button (PC13) press → state 2 → 3 | `boot.rs` main loop, PC13 edge detect + `link_alt_callback` | `boot.rs:325-333`, `boot.rs:361-365` |
| state 3 && LED state 4 → write magic words, spin on watchdog until reset | `boot.rs` main loop, magic-word block | `boot.rs:337-346` |
| Magic words 0xD7E5F7A8 / 0x19C3FE40 left in RAM | `RESET_MAGIC0_CELL` / `RESET_MAGIC1_CELL` | `boot.rs:20-27`, checked at `boot.rs:170-182` |
| RAM state `0x20000410` (bootloader state), `0x2000035e` (LED anim state) | `BOOTLOADER_STATE` / `LED_ANIMATION_STATE` | `boot.rs:28-38` |

So the user-described sequence (long press → LED animation → plug USB → every second LED red → press button → bootloader on PC) is traced end-to-end. What remains is only the **on-hardware verification** (flash the image, plug USB, press button, confirm the ROM bootloader appears).

### Tier 2: periodic engines (all 11 ported)

`boot.rs` main loop calls all 11 engines every iteration (`boot.rs:290-301`):

| # | Dump function | Rust equivalent | Status |
|---|---|---|---|
| 1 | `FUN_080096EC` BLE link service (800 ms) | `ble_com::link_service_tick()` | ✅ |
| 2 | `FUN_08008784` motor fault poll (PA12) | `hwi_drive::tick()` | ✅ |
| 3 | `FUN_080071C0` ADC/DMA poll | `hwi_adc::tick()` | ✅ |
| 4 | `FUN_08009EA0` line sensor service (0x37 ms) | `ble_com::line_sensor_service_tick()` | ✅ |
| 5 | `FUN_08008130` BLE AT re-send (2000 ms) | `ble_com::ble_at_resend_tick()` | ✅ |
| 6 | `FUN_08009340` light-sense sample | `app_lightsense::tick()` | ✅ |
| 7 | `FUN_08006000` light program step | `app_lights::tick()` | ✅ |
| 8 | `FUN_080030D0` mic sample (500 ms) | `hwi_mic::tick()` | ✅ |
| 9 | `FUN_080094F8` line sensor sample | `hwi_line::tick()` | ✅ |
| 10 | `FUN_08008574` buzzer tone sequencer | `hwi_buzz::tick()` | ✅ |
| 11 | `FUN_08003330` drive speed loop | `app_drive::tick()` | ✅ |

Plus `drv_battery::tick()` (battery pack reply machine) and the two app-light programs ported from `FUN_080039D8` (`legacy_chase`, program 5) and `FUN_08004CD8` (`legacy_sine`, program 6).

### Tier 3: IRQ handlers (all ported)

`rust/src/startup.rs` vector table (`startup.rs:89-140`) serves every IRQ the dump implements; none fall through to `Default_Handler`:

| IRQ | Dump handler | Rust handler | Status |
|---|---|---|---|
| 7 | EXTI4_15 power-good | `hwi_power::EXTI4_15_IRQHandler` | ✅ |
| 9 | TIM17 CC / DMA ch1 | `hwi_sonar::DMA1_Channel1_IRQHandler` | ✅ |
| 11 | TIM15 + DMA ch5 LED | `hwi_led::dma_ch5_irq` | ✅ |
| 13 | TIM1 CC | `hwi_enc::TIM1_BRK_UP_TRG_COM_IRQHandler` | ✅ |
| 15 | TIM2 CC | `hwi_enc::TIM2_IRQHandler` | ✅ |
| 17 | TIM6 update | `system::tim6_tick` | ✅ |
| 20 | TIM15 update LED sequencer | `hwi_led::tim15_update_irq` | ✅ |
| 22 | TIM17 update | `hwi_sonar::TIM17_IRQHandler` | ✅ |
| 29 | serial error path | `ble_com::usart_error_irq` | ✅ |

---

## What is genuinely still open

### P0: on-hardware verification

Flash `rust/build/fw.bin` and confirm the robot boots and answers over the command link. Same open point as the C rebuild. The bootloader-entry sequence (Tier 1 above) can only be validated on real hardware (PA12 VBUS, PC13 button, watchdog reset → ROM bootloader).

### P0: pin map

The LED matrix and motor-output pin maps come from the part family, not the binary (see comments in `src/modules/hwi_led.rs` and `src/modules/hwi_drive.rs`). Not recoverable from `stm32_flash.bin`.

### P1: battery values from the BLE pack

The original asks the BLE battery pack for voltage/current/remaining (`FUN_08006D34`). `drv_battery::hw_init()` now sends the 8-byte `{0xcc,0x4e,1,0,0,0,0,0x91}` frame on the LED line (`hwi_led::send_raw`) and `drv_battery::tick()` runs the reply machine that parses replies into voltage/current/remaining — but a real pack is needed to exercise the reply path. Until then, values come from the local ADC.

### P2: 64-bit division helpers

`FUN_08002578` and `FUN_080021A8` (64-bit division) are not ported — Rust's `i128`/`u128` division covers them; no behaviour is lost. Not needed for the port.

### RAM state not tracked (cosmetic, not needed for bootloader entry)

| Address | Description | Rust equivalent |
|---|---|---|
| 0x20000400 | Power off state (0/1/2) | not tracked (`power_fail_mode` handles the path) |
| 0x200006f0 / 6f4 / 6f8 / 0fc0 | PA12 callback ptr / level / last level | `PA12_LAST` tracked locally in `boot.rs`; callback ptr not modelled |
| 0x200008c8 | Power sense state struct | not tracked |

These are not needed for any traced behaviour; the bootloader state (`0x20000410`) and LED animation state (`0x2000035e`) that matter ARE tracked.

---

## Implementation priority (accurate)

1. **On-hardware test** — flash, boot, answer over the link; validate the USB→button→bootloader sequence.
2. **Pin map** — confirm LED matrix / motor pins against the actual board.
3. **BLE battery pack** — exercise `drv_battery` reply path with a real pack.
4. **Nothing else** — all dump functions with observable behaviour are traced and ported.
