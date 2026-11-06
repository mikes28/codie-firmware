# PROTOCOL — Codie robot command protocol

Recovered from the original flash dump (`stm32_flash.bin`) and reproduced
byte-for-byte in the Rust port (`rust/src/proto/{proxy,fmt}.rs`).

The C original functions: `FUN_0800A0AC` (parser), `FUN_0800A350` (dispatcher),
`FUN_0800A16C` (value reply), `FUN_0800A2D0` (system message), `FUN_0800A1C8`
(greeting), `FUN_080068D4`/`FUN_0800693C` (number formatting).

---

## 1. Link

| | USART4 (command + reply) | USART3 (BLE AT) |
|---|---|---|
| Baud | 460800 bit/s | 14400 bit/s |
| Stop bits | 2 | 2 |
| Pins | PC10 TX / PC11 RX (AF0) | PB1/PB13 (AF4), PC4/PC5 (AF1), PA2 input |
| Role | commands in, every reply out | AT / control channel to BLE module |

All robot output (`out_str`, `out_char`) goes to USART4. The command reader
also reads USART4 and clears its error flags there. USART3 only carries the
BLE module's setup traffic.

---

## 2. Request grammar

One line, space-separated:

```
<get|set|call> <interface> <op> [args]
```

- `get` — read a value (op code 1)
- `set` — write a value (op code 2)
- `call` — run an action (op code 3)
- `<interface>` — one of the 16 registered interfaces
- `<op>` — the operation name (interface-specific)
- `[args]` — the rest of the line, passed verbatim to the handler

Leading spaces are skipped; tokens end at the next space; everything after the
third token is `args`.

---

## 3. Replies

### 3.1 Value reply

```
<word> <interface> <op> [value]\n
```

- `<word>` — "get", "set" or "call" matching the request's op code
- `[value]` — present only when the handler has a value; the separator and
  value are both omitted when there is none (the C NULL-value case)

Example: `get drvBattery voltage` → `set drvBattery voltage 3a42\n`
(The op word in a "get" answer is hard-coded to "set" — that is what the
original does.)

### 3.2 System message

```
call :proxy message <level> <text>[extra]\n
```

- `<level>` — "???", "inf", "dbg", "wrn", "err"
- `[extra]` — appended directly after `<text>` with **no separator** (a quirk
  of the original)

Example: `call :proxy message err No interface:foo\n`

### 3.3 Greeting

```
call :proxy greeting "k":"v" ... "msg:<text>\"\n
```

Key/value pairs, each `"key":"value"`. A pair with a NULL value ends the
greeting — the C writes the opening quote and then breaks out of the loop, so
the closing quote is missing. The boot greeting sends no message:

```
call :proxy greeting "ver":"0.5.0hw2.1" "mat":"4" "date":"1602121419" "ttpms":"1000" "+ack":"0"\n
```

---

## 4. Number formatting

Values are formatted with a variable-radix itoa (`FUN_080068D4` unsigned,
`FUN_0800693C` signed):

- **Radix 16 (hex)** for essentially every reply — sensor values come out as
  hexadecimal.
- **Radix 10 (decimal)** only for `hwiAdc buf` and the `fixed`/`int` wrappers.
- Digits 10..35 use the original's `'W' + digit` trick → `'a'`..`'z'`.

Signed values wrap on overflow like the C (`i32::MIN` negates to itself).

---

## 5. Interface table

Registered at `0x0800D164` in the dump, 8 bytes per entry (name + handler),
16 entries, searched **backwards** by the dispatcher:

| # | interface | handler (dump) | Rust module |
|---|---|---|---|
| 0 | hwiAdc | `0x080073DD` | `hwi_adc` |
| 1 | hwiLed | `0x080090A5` | `hwi_led` |
| 2 | hwiDrive | `0x08008B3D` | `hwi_drive` |
| 3 | drvBattery | `0x08006F2D` | `drv_battery` |
| 4 | hwiSonar | `0x08009F89` | `hwi_sonar` |
| 5 | bleCom | `0x0800837D` | `ble_com` |
| 6 | hwiBuzz | `0x08008491` | `hwi_buzz` |
| 7 | hwiMic | `0x0800962D` | `hwi_mic` |
| 8 | hwiEnc | `0x08008DFD` | `hwi_enc` |
| 9 | hwiLine | `0x08009479` | `hwi_line` |
| 10 | hwiPower | `0x0800981D` | `hwi_power` |
| 11 | hwiLightSense | `0x080093A9` | `hwi_lightsense` |
| 12 | appLights | `0x08005F01` | `app_lights` |
| 13 | appDrive | `0x08003849` | `app_drive` |
| 14 | appSys | `0x080064F1` | `app_sys` |
| 15 | appLightSense | `0x08003855` | `app_lightsense` |

7 of the 16 handlers are 4-byte stubs in the original (`movs r0,#0; bx lr`):
hwiLed, hwiSonar, bleCom, hwiEnc, hwiLightSense, appLights, appSys. Their real
behaviour lives in the periodic task (`FUN_080063EC`) and the per-module
engines. The reconstruction exposes the engines as real operations.

---

## 6. Op reference (per interface)

| interface | op | args | reply | notes |
|---|---|---|---|---|
| hwiAdc | `buf` | none | 32 raw ADC samples, one per line; empty line after 9th/17th/25th | radix 10; no prefix |
| hwiLed | `on` / `pwm` | led, value | none | sets LED duty |
| hwiLed | `rgb` | led, r, g, b | none | GRB order |
| hwiDrive | `dutyA` | n (hex) | none | motor A duty, clamped ±1000 |
| hwiDrive | `dutyB` | n (hex) | none | motor B duty |
| hwiDrive | `nFault` | none | `set hwiDrive nFault 0\n` | always "0", op word "set" |
| drvBattery | `voltage` | none | `set drvBattery voltage <hex>\n` | from local ADC |
| drvBattery | `current` | none | `set drvBattery current <hex>\n` | from local ADC |
| drvBattery | `remainingCapacity` | none | `set drvBattery remainingCapacity <hex>\n` | from local ADC |
| drvBattery | `chargeStat` | none | `set drvBattery chargeStat 1\n` | PF6 probe |
| drvBattery | `temp` | none | `set drvBattery temp <hex>\n` | |
| drvBattery | `hwInit` | none | none | starts BMS query (open: payload undecodable) |
| hwiSonar | — | — | none | stub handler |
| bleCom | `buf` / `write` / `echo` | — | console link | |
| hwiBuzz | `play` | pattern | none | tone sequencer |
| hwiMic | — | — | none | stub handler |
| hwiEnc | `count` | none | encoder count | incl. combined 0x20 |
| hwiEnc | `reset` | none | none | |
| hwiLine | — | — | none | one bit per sensor |
| hwiPower | `pwrOff` | on\|1\|true | none | PC12 off, spin |
| hwiPower | `Vsens` | on\|1\|true | none | PC15 set/reset |
| hwiLightSense | — | — | none | stub handler |
| appLights | — | — | none | light program engine |
| appDrive | — | — | none | speed loop engine |
| appSys | version strings | none | 4 version strings | |
| appLightSense | — | — | none | stub handler |

`echo` sits in the string table but no code references it. Unknown interface →
`call :proxy message err No interface:<name>\n`. Malformed request →
`call :proxy message err Invalid command!\n`.

---

## 7. Error strings

"BLE framing error", "BLE USART noise", "BLE USART overrun", "USART framing
error", "possible cmdBuf overflow", "BUFOVF!!!!!", "Invalid command!",
"No interface:".
