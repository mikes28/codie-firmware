# BOOTLOADER_ENTRY.md — Bootloader entry sequence (dump → Rust port)

This document describes the complete bootloader entry sequence as recovered
from `stm32_flash.bin` and implemented in the Rust port.

## The user-observable sequence

1. **Turn on** (long press button) → supply comes up, PC12 power-hold latch
   keeps it alive → LED animation runs
2. **Plug USB** → PA12 (VBUS) goes HIGH → LEDs change (every second LED on)
3. **Press button again** → LEDs go red → bootloader entry armed
4. **Bootloader appears on PC** as `0483:df11` STM32 BOOTLOADER

## How it works in the binary dump

The sequence is implemented as a state machine across three RAM variables and
the periodic task `FUN_080063EC`:

### RAM state variables

| Address | Name | Values | Meaning |
|---|---|---|---|
| `0x20000410` | Bootloader state | 0/2/3 | 0=normal, 2=USB detected, 3=ready for entry |
| `0x2000035e` | LED animation state | 0/1/2/4 | 0=off, 4=red |
| `0x200025a8` | Magic word 0 | 0xD7E5F7A8 | Written before bootloader entry |
| `0x200017a8` | Magic word 1 | 0x19C3FE40 | Written before bootloader entry |

### Step 1: USB detected → state 2

`FUN_08008784` (@0x08008784) polls **GPIOA PA12** (USB VBUS) every main-loop
iteration. It's edge-triggered: when PA12 changes level, it calls the callback
`FUN_08006294` (`_DAT_200006f8`) with argument 1 (PA12 low) or 2 (PA12 high).

`FUN_08006294` (@0x08006294), when called with argument 2 (USB plugged in):
- Sets `DAT_20000410 = 2` (bootloader state = USB detected)
- Calls `FUN_08005FBC(4)` → sets LED animation state to **4** (red)
- Calls `FUN_08005EC4(4, 0, 2)` → LED animation callback

This is why plugging USB changes the LED pattern: state advances to 2 and LED
animation state becomes 4 (red).

### Step 2: Button pressed → state 3

`FUN_0800626C` (@0x0800626C) checks if `DAT_20000410 == 2`, then:
- Sets `DAT_20000410 = 3` (bootloader state = ready for entry)
- Calls `FUN_08005EC4(4, 0, 3, 0x60)` → LED animation continues
- Calls `FUN_08006130()` → sets `_DAT_20000374 = 1` (LED engine flag)

This is why pressing the button again makes LEDs go red: state advances to 3
and the LED animation is in state 4 (red).

### Step 3: Bootloader entry

`FUN_080063EC` (@0x080063EC), the periodic task, checks at the end of each
iteration:

```c
if (DAT_20000410 == 3 && FUN_08006124() == 4)  // state 3 + LED state 4
{
    DAT_200025a8 = 0xD7E5F7A8;   // magic word 0
    DAT_200017a8 = 0x19C3FE40;   // magic word 1
    while (1) { FUN_0800ab10(); } // kick watchdog forever → reset
}
```

`FUN_08006124` (@0x08006124) returns `_DAT_2000035e` (LED animation state).

When both conditions are true:
1. Magic words are written to RAM
2. Infinite loop kicks the watchdog → watchdog reset
3. On reset, `main()` (`FUN_08009FA4`) sees magic words match
4. `main()` runs `power_fail_mode()` (`FUN_0800ab88`): clock back to HSI,
   GPIOC setup, PC14 high
5. `main()` calls `FUN_08009F8C`: sets MSP = 0x200014a8, reads function
   pointer from `0x1FFFC804`, calls it → **ROM bootloader** (DFU, 0483:df11)

## How it's implemented in the Rust port

The Rust port (`rust/src/boot.rs`) implements the same state machine:

### State variables

```rust
static BOOTLOADER_STATE: AtomicU8 = AtomicU8::new(0);   // 0/2/3
static LED_ANIMATION_STATE: AtomicU8 = AtomicU8::new(0); // 0/4
static PA12_LAST: AtomicU8 = AtomicU8::new(0);          // edge detect
static PC13_LAST: AtomicU8 = AtomicU8::new(0);          // edge detect
```

### Main loop logic (in `boot::run()`)

Each iteration of the main loop:

1. **PA12 poll** (USB VBUS detect): read `GPIOA + IDR`, mask `0x1000`
   - On rising edge: `BOOTLOADER_STATE = 2`, `LED_ANIMATION_STATE = 4`

2. **PC13 poll** (button detect): read `GPIOC + IDR`, mask `0x2000`
   - On any edge when `BOOTLOADER_STATE == 2`: `BOOTLOADER_STATE = 3`

3. **Bootloader entry check**: if `BOOTLOADER_STATE == 3 && LED_ANIMATION_STATE == 4`:
   - Write `RESET_MAGIC0 = 0x19C3FE40` to `RESET_MAGIC0_CELL`
   - Write `RESET_MAGIC1 = 0xD7E5F7A8` to `RESET_MAGIC1_CELL`
   - Enter infinite loop kicking the watchdog → watchdog reset

4. **On next boot**: `main()` sees magic words match, runs `power_fail_mode()`
   (clock to HSI, GPIOC setup, PC14 high), sets MSP = 0x200014a8, calls
   `*(0x1FFFC804)` → ROM bootloader

### Magic words

| Name | Address | Value |
|---|---|---|
| `RESET_MAGIC0` | `0x200025A8` | `0x19C3FE40` |
| `RESET_MAGIC1` | `0x200017A8` | `0xD7E5F7A8` |

## Verification

- `cargo check --all-targets` — clean, 0 warnings, 0 errors
- `cargo test` — 12 passed, 0 failed
- `cargo build --release --target thumbv6m-none-eabi` — success

## Hardware requirements

| Item | Pin | Function |
|---|---|---|
| USB VBUS detect | GPIOA PA12 | High when USB plugged in |
| Button press | GPIOC PC13 | Button press detection |
| Power hold | GPIOC PC12 | Output, keeps supply alive |
| ROM bootloader | — | `0x1FFFC804` reset handler, DFU 0483:df11 |
