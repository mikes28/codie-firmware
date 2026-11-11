# Codie robot — Bluetooth / BLE link commands

The robot talks to an external BLE module over **USART3** at **14400 baud,
8N2** (CR2 = 0x2000, TE|RE enabled). Pins: **PB1/PB13 (AF4)** and
**PC4/PC5 (AF1)**, with **PA2** as an input. RX ring is 128 B, TX ring is
64 B.

**USART3 is the BLE AT link.** All robot output (greeting, command replies,
error messages) goes out on **USART4** (the command link at 460800 baud),
not on USART3. USART3 only carries traffic to/from the BLE module.

---

## Link configuration

| Setting | Value |
|---------|-------|
| Peripheral | USART3 @ `0x40004800` |
| Baud | 14400 (BRR = 0x3840) |
| Data bits | 8 |
| Stop bits | 2 (CR2 = 0x2000) |
| Parity | None |
| Flow control | None (CR3 = 0x0300) |
| Pins TX | PB13 (AF4) / PC5 (AF1) |
| Pins RX | PB1 (AF4) / PC4 (AF1) |
| PA2 | Input (unused by app) |
| RX buffer | 128 bytes |
| TX buffer | 64 bytes |

Error reporting (reported on USART4, the command link):
- Framing error (FE) → `"BLE framing error"`
- Noise error (NE) → `"BLE USART noise"`
- Overrun error (ORE) → `"BLE USART overrun"`
- RX ring overflow → `"BUFOVF!!!!!\n"`

---

## What the robot sends to the BLE module

The robot initiates the link on boot:

| Direction | Bytes | Meaning |
|-----------|-------|---------|
| Robot → Module | `"BLE-boot\n"` | Boot announcement, sent at startup |

The original also attempts to read `"BLE-RSP HELLO"` and `"BLE-RSP GAP SET MODE: "` replies from the module.

### AT keep-alive (`FUN_08008130`)

After **2000 ms of no activity** on the BLE link, the firmware re-sends a keep-alive probe:

```
AT\r\n
```

Activity (RX or TX on USART3) resets the 2000 ms timer via `ble_at_touch_activity()`.

The exact GAP command payload the original sends is **not recoverable** from the dump — the formatter (`FUN_08002AC2`) decodes nibble-switch serialization, but the payload bytes themselves are runtime-derived from a table at `0x08002BE1` that the dump does not fully resolve. The port substitutes a plain `AT\r\n` probe, which any AT-mode BLE module answers with `OK`.

---

## What the BLE module sends to the robot

| From Module | Bytes | Meaning |
|-------------|-------|---------|
| Module → Robot | `"BLE-RSP HELLO\n"` | BLE module hello response |
| Module → Robot | `"BLE-RSP GAP SET MODE: ..."` | GAP set mode response |
| Module → Robot | *(raw AT responses)* | e.g. `OK`, `ERROR` to AT commands |

The robot matches `"BLE-RSP HELLO"` and `"BLE-RSP GAP SET MODE: "` as reply matchers on the BLE link.

---

## LED markers on the command link

The LED engine prints markers on **USART4** (the command link), not on USART3:

| Marker | When |
|--------|------|
| `PPRT\n` | LED pattern transfer starts |
| `PPRET\n` | LED pattern transfer ends |

These are not BLE traffic — they are emitted on the command link when the LED DMA transfer runs.

---

## BLE-related op names on the command link

The command link (USART4) exposes no dedicated `bleCom` ops — the `bleCom` handler in the original is a 4-byte no-op stub. BLE-related traffic is observed indirectly:

- `get drvBattery voltage` / `current` / `remainingCapacity` / `chargeStat` / `temp` — values that, on the original, come from the BLE battery pack query (`drvBattery hwInit` sends `{0xcc,0x4e,1,0,0,0,0,0x91}` over the LED line, then parses the pack's binary replies)
- `call :proxy message err BLE framing error` — USART3 framing error reported on the command link
- `call :proxy message err BLE USART noise` — USART3 noise error
- `call :proxy message err BLE USART overrun` — USART3 overrun error

---

## Protocol command reference (USART4, 460800 baud)

All commands are text lines: `<get\|set\|call> <interface> <op> [args]`

Replies: `"<word> <iface> <op> [value]\n"` or `"call :proxy message <level> <text>[extra]\n"`

| Command | Direction | Meaning |
|---------|-----------|---------|
| `get hwiAdc buf` | Robot → PC | 32 ADC samples, hex, empty line after 9th/17th/25th value |
| `get hwiDrive dutyA` / `dutyB` | Robot → PC | Motor duty (hex) |
| `get hwiDrive nFault` | Robot → PC | Always `"0"` |
| `set hwiDrive dutyA <hex>` | PC → Robot | Motor A duty (parsed as hex) |
| `set hwiDrive dutyB <hex>` | PC → Robot | Motor B duty (parsed as hex) |
| `get drvBattery voltage` | Robot → PC | Battery voltage (hex) |
| `get drvBattery current` | Robot → PC | Battery current (hex) |
| `get drvBattery remainingCapacity` | Robot → PC | Battery remaining (hex) |
| `get drvBattery chargeStat` | Robot → PC | Battery charge status (hex) |
| `get drvBattery temp` | Robot → PC | Battery temperature (hex) |
| `call drvBattery hwInit` | PC → Robot | Start BMS query over BLE link |
| `get hwiPower pwrOff` / `Vsens` | Robot → PC | Power control / sense |
| `set hwiPower Vsens on\|1\|true` | PC → Robot | Set Vsens (PC15) |
| `get hwiLed on\|pwm\|rgb <led>` | Robot → PC | LED state |
| `set hwiLed on\|pwm\|rgb <led> <args>` | PC → Robot | Set LED |
| `call hwiBuzz play <pattern>` | PC → Robot | Play tone pattern |
| `call :proxy greeting` | PC → Robot | Get boot greeting |
| `call :proxy message ...` | PC → Robot | System message |

---

## BLE link state machine (original)

The BMS battery pack query (`drvBattery hwInit`) runs a 7-state reply machine:

1. Wait for BLE link idle
2. Send `{cc,0x4e,1,0,0,0,0,0x91}` (hwInit frame)
3. Wait for callback `{cc,0x48,1}`
4. Send `{cc,0xb4}`
5. Send `{cc,0xb8,0}`
6. Send `{cc,0xbe,0}`
7. Parse replies into temp/voltage/current/remaining

---

## What is NOT recoverable from the dump

- The exact **GAP command payload bytes** the BLE AT re-send sends. The formatter is decoded (nibble-switch serialization), but the payload table at `0x08002BE1` resolves to runtime-derived bytes, not literals. The only `GAP SET MODE` string in the dump is the *reply matcher*, not the command body.
- Whether the BLE module requires the GAP command to begin advertising. If it does, the robot will not advertise on BLE without the original payload bytes (needs a live capture from the module).

---

## Files

| File | Contents |
|------|----------|
| `stm32_flash.bin` | Original 128 KB flash dump |
| `analysis/decomp/` | Ghidra decompilation of all 378 functions |
| `rust/src/modules/ble_com.rs` | Rust port of the BLE link (AT keep-alive, link service, line service, USART error IRQ) |
| `rust/src/modules/drv_battery.rs` | Rust port of the BMS battery query state machine |
| `FINDINGS.md` | Register-level findings, including verified BLE USART config |
| `MEMORY.md` §4 | USB and the two serial links, with the two P0 bugs that were fixed |
