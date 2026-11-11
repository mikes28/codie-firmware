# Script engine — autonomous on-robot programs

Run small programs on the robot without Bluetooth or USB after they are
loaded.  A program (script) is a sequence of bytecode bytes stored in RAM,
executed one step per main-loop tick.  Pressing the button on top twice
within 500 ms starts the script automatically — like LEGO robots.

## Table of contents

- [How it works](#how-it-works)
- [Loading a script](#loading-a-script)
- [Running a script](#running-a-script)
- [Double-press to start](#double-press-to-start)
- [Bytecode reference](#bytecode-reference)
- [Protocol commands](#protocol-commands)
- [Examples](#examples)
- [Behavior notes](#behavior-notes)

## How it works

The script engine lives in `src/modules/script.rs` and runs inside the
firmware's main loop (`boot::run()`).  Every iteration it executes at most
one bytecode instruction, so:

- The watchdog is always kicked on time.
- The command link keeps answering while a script plays.
- A `WAIT` instruction holds execution without freezing the CPU.

State is kept in a single 128-byte buffer plus a program counter.  The
engine is registered as the `script` interface in the protocol registry,
so you talk to it with the same `get`/`set`/`call` protocol every other
module uses.

## Loading a script

Send the bytecode bytes over the command link (USB or Bluetooth):

```
set script load <n1> <n2> <n3> ...
```

Each `<n>` is a decimal number 0–255 representing one bytecode byte.  Up
to **128 bytes** are stored; anything beyond is truncated.

Example — turn LED 0 red, wait half a second, turn it off:

```
set script load 1 0 255 0 0 3 500 0 1 0 0 0 0 0
```

That is: `SET_LED(0,255,0,0)`, `WAIT(500)`, `SET_LED(0,0,0,0)`, `STOP`.

Check what is stored:

```
get script
```

Reply:

```
set script 1 0 255 0 0 3 500 0 1 0 0 0 0 0
```

Clear a stored script:

```
set script clear
```

## Running a script

### Manually

Start the stored script:

```
call script run
```

Stop a running script:

```
call script stop
```

### Double-press the button

Press the button on top twice within 500 ms → the script starts
automatically.  This is the "LEGO robot" behaviour: load once, then
trigger anytime without a phone or computer.

- A single press does nothing.
- Two presses too far apart (> 500 ms) do nothing.
- The script must be loaded before the double-press; otherwise there is
  nothing to run.

## Bytecode reference

All values are unsigned bytes (0–255).  Multi-byte values are little-endian
(low byte first).

| Opcode | Mnemonic | Args | Meaning |
|--------|----------|------|---------|
| `0x00` | `STOP` | — | Stop execution.  The script becomes inactive until `run` or another double-press. |
| `0x01` | `SET_LED` | `<led> <r> <g> <b>` | Set LED `<led>` (0–11) to RGB color.  Wires GRB order on the strip. |
| `0x02` | `SET_DUTY` | `<motor> <duty>` | Set motor A=`0` / B=`1` to duty `<duty>` (0–255). |
| `0x03` | `WAIT` | `<ms_lo> <ms_hi>` | Pause `ms_lo + ms_hi*256` milliseconds before the next instruction. |
| `0x04` | `LOOP` | — | Jump the program counter back to byte 0 (repeat the script forever). |

An unknown opcode stops the script silently.

### Timing

One instruction executes per main-loop iteration (roughly every few
milliseconds).  `WAIT` does not busy-spin: the program counter is held
until the millisecond tick passes the deadline, so the watchdog and command
link keep working during the wait.

## Examples

### Blink LED 3 red forever

```
# SET_LED(3,255,0,0)  WAIT(250)  SET_LED(3,0,0,0)  WAIT(250)  LOOP
set script load 1 3 255 0 0 3 250 0 1 3 0 0 0 3 250 0 4
call script run
```

Then double-press the button any time to restart the blink pattern.

### Motor A forward half speed for 1 second, then stop

```
# SET_DUTY(0,128)  WAIT(1000)  SET_DUTY(0,0)  STOP
set script load 2 0 128 3 232 3 2 0 0 0
call script run
```

`1000` decimal = `0x03E8`, low byte `0xE8`=232, high byte `0x03`=3.

### Chase: one red LED wandering across the strip

Requires a longer script; load more bytes and use `LOOP` to repeat.

## Protocol commands

| Command | Meaning |
|---------|---------|
| `get script` | Reply with the stored bytes as decimal numbers |
| `set script load <n> ...` | Store `<n>...` as the script (max 128 bytes) |
| `set script clear` | Erase the stored script |
| `call script run` | Start executing the stored script |
| `call script stop` | Stop a running script |

## Behavior notes

- **No Bluetooth needed while playing.**  Once loaded, the script lives in
  RAM and runs from the main-loop tick.  Disconnect the phone; the script
  keeps running until it hits `STOP`, `LOOP`s forever, or you reset the
  robot.
- **RAM is volatile.**  Resetting or power-cycling the robot erases the
  script.  Reload it over USB/Bluetooth after boot.
- **One script at a time.**  Loading a new script replaces the old one.
- **Script starts inactive.**  After `load`, nothing happens until you call
  `run` or double-press the button.
- **Double-press gate:** presses must be within 500 ms of each other.  The
  timer starts on the first press; if the second press arrives later, the
  first press is forgotten and you must press twice again.
- **Button polarity:** PC13 is active-low (pressed pulls the pin low).

## Files

| File | Contents |
|------|----------|
| `rust/src/modules/script.rs` | Script engine implementation (VM, double-press detector, protocol handler) |
| `rust/tests/script.rs` | 10 tests: opcodes, WAIT timing, double-press, truncation, protocol |
| `rust/src/boot.rs` | Wires `script::init()` and `script::tick()` into the firmware main loop |
