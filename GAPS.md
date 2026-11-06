# GAPS.md — Functions in the binary dump NOT implemented in the Rust port

This document lists every function from `stm32_flash.bin` (378 total) that has
no counterpart in `rust/src/`, and every behavioral gap where the Rust port
does not reproduce the dump's functionality.

## Tier 1: Critical Gaps (bootloader entry, USB detection, button → bootloader)

These are required for the hardware behavior the user described:
> Turn on (long press button) → LED animation → Plug USB → LEDs change (every
> second LED on) → Press button again → LEDs red → Bootloader appears on PC

### Gap 1: PA12 / USB VBUS detection

**Dump:** `FUN_08008784` (@0x08008784) polls GPIOA PA12 every main-loop
iteration. When PA12 goes HIGH (USB VBUS), it calls callback
`FUN_08006294(2)`, which sets `DAT_20000410 = 2` (bootloader state = USB
detected) and `FUN_08005FBC(4)` (LED animation state = 4 = red).

**Rust:** `hwi_drive::tick()` polls PA12 but treats it as a motor fault input.
The USB VBUS detection behavior (state machine advancement) is **not**
implemented.

**Fix needed:** In `boot::run()` main loop, poll GPIOA PA12. On rising edge,
set bootloader state = 2 and LED animation state = 4.

### Gap 2: Button press → bootloader state 2 → 3

**Dump:** `FUN_0800626C` (@0x0800626C) checks if `DAT_20000410 == 2`, sets it
to 3, calls `FUN_08005EC4(4, 0, 3, 0x60)` (LED animation), and
`FUN_08006130()` (sets `_DAT_20000374 = 1`).

**Rust:** Not implemented at all. No button press detection, no state
advancement from 2 to 3.

**Fix needed:** Detect button press (PC13/EXTI13 falling edge, or GPIO poll).
When in state 2, advance to state 3 and set LED animation to state 4 (red).

### Gap 3: Bootloader entry (state 3 + LED state 4 → magic words → reset)

**Dump:** `FUN_080063EC` (@0x080063EC) checks:
```
if (DAT_20000410 == 3 && FUN_08006124() == 4)  // state 3 + LED state 4
{
    DAT_200025A8 = 0xD7E5F7A8;   // magic word 0
    DAT_200017A8 = 0x19C3FE40;   // magic word 1
    while (1) { FUN_0800ab10(); } // kick watchdog forever → reset
}
// On reset, main() (FUN_08009FA4) sees magic words match, then:
//   FUN_08009F8C() → set MSP=0x200014A8, call *(0x1FFFC804) = ROM bootloader
```

**Rust:** `boot::run()` main loop calls `util::iwdg_kick()` but does NOT check
bootloader state, does NOT write magic words, does NOT enter infinite loop.

**Fix needed:** In `boot::run()` main loop, after watchdog kick, check if
bootloader state == 3 and LED state == 4. If so, write magic words to RAM and
enter infinite loop (watchdog will reset → main() → bootloader).

---

## Tier 2: Periodic Engines (4 of 11 not ported)

The dump's `FUN_080063EC` calls 11 engines. The Rust port calls 7:

| # | Dump Function | Rust Equivalent | Status |
|---|---|---|---|
| 1 | `FUN_080096EC` (BLE link service, 800ms) | None | **MISSING** |
| 2 | `FUN_08008784` (motor fault poll) | `hwi_drive::tick()` | ✅ Ported |
| 3 | `FUN_080071C0` (ADC/DMA poll) | `hwi_adc::tick()` | ✅ Ported |
| 4 | `FUN_08009EA0` (line sensor service) | None | **MISSING** |
| 5 | `FUN_08008130` (BLE AT re-send, 2000ms) | None | **MISSING** (payload undecodable) |
| 6 | `FUN_08009340` (light-sense sample) | `app_lightsense::tick()` | ✅ Ported |
| 7 | `FUN_08006000` (light program step) | `app_lights::tick()` | ✅ Ported |
| 8 | `FUN_080030D0` (mic sample, 500ms) | None | **MISSING** |
| 9 | `FUN_080094F8` (line sensor sample) | `hwi_line::tick()` | ✅ Ported |
| 10 | `FUN_08008574` (buzzer tone sequencer) | `hwi_buzz::tick()` | ✅ Ported |
| 11 | `FUN_08003330` (drive speed loop) | `app_drive::tick()` | ✅ Ported |

**Missing engines need:** BLE link service callback hook in `ble_com` before
they can be ported (they publish over the BLE data channel).

---

## Tier 3: IRQ Handlers (3 of 9 not ported)

| IRQ | Dump Handler | Rust Equivalent | Status |
|---|---|---|---|
| 7 | `FUN_0800976C` (EXTI4_15, power-good) | `hwi_power::EXTI4_15_IRQHandler` | ✅ Ported |
| 9 | `FUN_08008EE8` (TIM17 CC / DMA ch1) | `DMA1_Channel1_IRQHandler` | ✅ Ported |
| 11 | `FUN_08007734` (TIM15 + DMA ch5, LED) | None | **MISSING** (no TIM15 ISR) |
| 13 | `FUN_08008BF8` (TIM1 CC) | `TIM1_BRK_UP_TRG_COM_IRQHandler` | ✅ Ported |
| 15 | `FUN_08008C20` (TIM2 CC) | `TIM2_IRQHandler` | ✅ Ported |
| 17 | `FUN_0800A43C` (TIM6 update) | `system::tim6_tick` | ✅ Ported |
| 20 | `FUN_0800745C` (TIM15 update, LED sequencer) | None | **MISSING** |
| 22 | `FUN_08008E9C` (TIM17 update) | `TIM17_IRQHandler` | ✅ Ported |
| 29 | `FUN_08002DC8` (serial error path) | None | **MISSING** |

IRQ 11/20 (LED engine) and IRQ 29 (serial error) stay on `Default_Handler`
by design — arming them without handlers would spin forever.

---

## Tier 4: Individual Functions Not in Rust

The following dump functions have no direct Rust counterpart (they are either
inlined into larger Rust functions, or represent behavior not yet ported):

| Dump Address | Function | Size | Why not ported |
|---|---|---|---|
| 0x08002AC0 | FUN_08002ac0 | 2 | BLE AT formatter entry (opaque bytecode template) |
| 0x08002AC2 | FUN_08002AC2 | 274 | BLE AT formatter (payload undecodable) |
| 0x08000474 | FUN_08000474 | 922 | String/format processing (inlined in proto/fmt.rs) |
| 0x08000818 | FUN_08000818 | 614 | BLE/protocol processing (inlined in ble_com/proto) |
| 0x08000BFC | FUN_08000BFC | 596 | Memory copy/fill (inlined) |
| 0x08000E5C | FUN_08000E5C | 932 | ADC/DMA processing (inlined in hwi_adc) |
| 0x08002578 | FUN_08002578 | 6464 | Large function |
| 0x080021A8 | FUN_080021A8 | 6966 | Large function |
| 0x080039D8 | FUN_080039D8 | 13696 | Large function |
| 0x080043D4 | FUN_080043D4 | 5069 | Large function |
| 0x08004CD8 | FUN_08004CD8 | 12937 | Large function |
| 0x08006AB4 | FUN_08006AB4 | 2248 | Large function |
| 0x0800745C | FUN_0800745C | 3243 | TIM15 LED sequencer state machine |
| 0x080090A8 | FUN_080090A8 | 3017 | LED frame encoder (inlined in hwi_led::fill_ccr_stream) |
| 0x08007B34 | FUN_08007B34 | 1869 | LED DMA transfer (inlined in hwi_led::send_frame) |
| 0x08009F8C | FUN_08009F8C | 264 | Power fail tail (inlined in boot::power_fail_mode) |
| 0x0800A558 | FUN_0800A558 | 90 | Timer counter read (inlined) |
| 0x08009094 | FUN_08009094 | 146 | Sonar busy (inlined in hwi_sonar::busy) |

---

## Tier 5: RAM State Variables Not in Rust

The dump uses these RAM variables that the Rust port does not track:

| Address | Description | Rust Equivalent |
|---|---|---|
| 0x20000410 | Bootloader state (0/2/3) | **NOT TRACKED** |
| 0x2000035e | LED animation state | **NOT TRACKED** |
| 0x20000374 | LED engine flag | **NOT TRACKED** |
| 0x20000400 | Power off state (0/1/2) | **NOT TRACKED** |
| 0x200006f0 | PA12 callback function ptr | **NOT TRACKED** |
| 0x200006f4 | PA12 current level | **NOT TRACKED** |
| 0x200006f8 | PA12 callback | **NOT TRACKED** |
| 0x20000fc0 | PA12 last level | **NOT TRACKED** |
| 0x200008c8 | Power sense state struct | **NOT TRACKED** |
| 0x20001018 | Dump tick (TIM6 counter) | `system::dump_tick()` ✅ |

---

## Implementation Priority

### P0 (must have for bootloader entry):
1. **PA12/USB VBUS detection** in `boot::run()` main loop
2. **Button press detection** (PC13/EXTI13 or GPIO poll)
3. **Bootloader state machine** (0→2→3)
4. **Magic word write + infinite loop** in `boot::run()`
5. **LED animation state** (0=off, 2=pattern, 4=red) tracking

### P1 (nice to have):
6. Remaining periodic engines (BLE link service, line sensor service, mic sample)
7. Missing IRQ handlers (11/20 LED, 29 serial error)
8. BLE AT re-send (`FUN_08008130`)

### P2 (documentation only):
9. Large unanalyzed functions (0x08002578, 0x080021A8, etc.)
10. RAM state variables not needed for bootloader entry
