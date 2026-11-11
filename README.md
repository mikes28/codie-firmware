# Codie robot — STM32 firmware dump (Rust port)

Raw dump of the robot's STM32 bootloader flash, captured over USB DFU on 2026-10-21, rebuilt as a Rust port in [`rust/`](rust/). Same register map and wire protocol as the original dump.

**New on this branch (`feat/autonomous`):** an on-robot script engine. Load a small bytecode program over USB or Bluetooth, then run it on the robot with no wireless needed — start it manually or by double-pressing the power button on top (LEGO-robot style).

## Script engine (`rust/src/modules/script.rs`)

A script is up to 128 bytes of bytecode stored in RAM, executed one step per main-loop tick.

| Opcode | Args | Meaning |
|--------|------|---------|
| `0x00` STOP | — | Halt execution |
| `0x01` SET_LED | `<led> <r> <g> <b>` | Set LED 0–11 to RGB color |
| `0x02` SET_DUTY | `<motor> <duty>` | Set motor A=`0` / B=`1` duty 0–255 |
| `0x03` WAIT | `<ms_lo> <ms_hi>` | Wait `ms_lo + ms_hi×256` ms (non-blocking) |
| `0x04` LOOP | — | Jump back to byte 0 (repeat forever) |

### Protocol commands

```
set script load <n1> <n2> ...   # store up to 128 bytes as the script
set script clear                # erase the stored script
call script run                 # start executing
call script stop                # stop a running script
get script                      # read back the stored bytes
```

### Double-press to start

Press the power button (PC13) twice within 500 ms → the stored script starts automatically. A single press, or two presses too far apart, does nothing.

### Example — blink LED 3 red forever

```
set script load 1 3 255 0 0 3 250 0 1 3 0 0 0 3 250 0 4
call script run
```

That's: `SET_LED(3,255,0,0)`, `WAIT(250)`, `SET_LED(3,0,0,0)`, `WAIT(250)`, `LOOP`.

## Basic functions (already ported)

| Feature | What it does |
|---------|--------------|
| Command protocol | `get`/`set`/`call` `<interface> <op> [args]` over USART4 (460800) and USART3 BLE link |
| hwiLed | `on`/`pwm`/`rgb`/`argb` — ARGB LED strip via TIM15+DMA ch5 |
| hwiDrive | `dutyA`/`dutyB` motor duty, `nFault` fault flag |
| hwiBuzz | `play <pattern>` non-blocking tone sequencer |
| drvBattery | `voltage`/`current`/`remainingCapacity`/`chargeStat`/`temp`/`hwInit` |
| hwiAdc | `get buf` — 32-channel ADC sample dump |
| hwiPower | `pwrOff`/`Vsens` power control |
| appLights | Light programs: 4 solid red, 5 chase, 6 sine fader |

## Build & test

```bash
cd rust
cargo test        # 21 tests, green
cargo build       # firmware image, build/fw.bin
```

- Target: Cortex-M0+, STM32F0x1 / G0x1-class (128 KB flash @ `0x08000000`, 16 KB RAM @ `0x20000000`)
- No HAL, no dynamic allocation
- `cargo test` is green; image builds at ~24 KB (17 % of flash)

## Flashing

Put the robot in DFU mode (BOOT0 high, reset), then:

```bash
cd rust && ./build.sh          # build build/fw.bin
cd fw && ./flash.sh ../rust/build/fw.bin   # upload via dfu-util
```

USB is **DFU-only**: the application does not enable a USB peripheral, so the board shows no CDC/serial device when running normally. The command link answers over USART4 (460800, PC10/PC11) — a USB-TTL adapter is needed.

## What remains

- **On-hardware flash test** — nothing has ever been run on the actual robot.
- **BLE battery pack values** — `drvBattery` can query a BLE pack over the LED line, but a real pack is needed to exercise the reply path.
- **Exact BLE GAP payload** — the keep-alive engine sends `AT\r\n`; the original's exact GAP command bytes are unrecoverable from the dump.

## Repository notes

- [`FEATURES.md`](FEATURES.md) — full per-feature implemented-vs-missing list against the dump
- [`GAPS.md`](GAPS.md) — historical gap list, now closed except the items above
- [`rust/PROGRESS.md`](rust/PROGRESS.md) — register-level bug fixes and open items
- [`STATUS_INTENDED_VS_REALITY.md`](STATUS_INTENDED_VS_REALITY.md) — intended behavior vs. what the port actually does
