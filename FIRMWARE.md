# Codie robot — original STM32 firmware (from `stm32_flash.bin`)

This document describes the **original robot firmware** as recovered from the
binary dump `stm32_flash.bin` (128 KB, captured over USB DFU on 2026-10-21).
It covers what the firmware does: its commands, protocol, per-module
behaviour, hardware register map, and boot sequence.

**This is documentation of the original C firmware**, not of the Rust port.
The Rust port lives in `rust/` and is tracked separately
(`FEATURES.md`, `rust/PROGRESS.md`, `STATUS_INTENDED_VS_REALITY.md`) — see
the link at the bottom of this page.

## Contents

- [Command protocol](#command-protocol)
- [Serial links (USART3 / USART4)](#serial-links)
- [Modules and their ops](#modules-and-their-ops)
- [Boot sequence](#boot-sequence)
- [Hardware register map](#hardware-register-map)
- [Clock tree](#clock-tree)
- [RAM layout](#ram-layout)
- [Interrupts](#interrupts)
- [Option bytes](#option-bytes)

---

## Command protocol

The firmware speaks a text command protocol over the **command link** (USART4,
460800 baud, PC10 TX / PC11 RX, AF0, 2 stop bits). Every line is:

```
<get|set|call> <interface> <op> [args…]
```

- `get` (op 1): read a value → `"<word> <iface> <op> [value]\n"`
- `set` (op 2): write a value → no reply
- `call` (op 3): run an action → no reply (except `:proxy greeting` / `:proxy message`)

The request parser is `FUN_0800A0AC`, the dispatcher `FUN_0800A350`. The
dispatcher searches the 16-entry interface table (flash `0x0800D164`)
backwards.

### Protocol-visible strings

| String | Where |
|--------|-------|
| `"get"`, `"set"`, `"call"` | op words (flash `0x0800B824`) |
| `"???"`, `"inf"`, `"dbg"`, `"wrn"`, `"err"` | log levels (flash `0x0800B834`) |
| `"BLE-boot"`, `"BLE-RSP HELLO"`, `"BLE-RSP GAP SET MODE: "`, `"PPRT"`, `"PPRE"`, `"ERR:BLE_serout_TO"`, `"+ack"` | BLE link |
| `"Invalid command!"`, `"No interface:"` | protocol errors |
| `"BUFOVF!!!!!"` | command buffer overflow |
| `"USART framing error"`, `"USART noise"`, `"USART overrun"` | USART errors |
| `"BLE framing error"`, `"BLE USART noise"`, `"BLE USART overrun"` | BLE link errors |
| `":proxy"` | protocol prefix (message proxied through BLE module) |

### Greeting

On boot the firmware prints on USART4:

```
call :proxy greeting "ver":"0.5.0hw2.1" "mat":"4" "date":"1602121419" "ttpms":"1000" "+ack":"0"
```

- `ver`: `0.5.0hw2.1` (app 0.5.0, hardware 2.1)
- `mat`: `4`
- `date`: `1602121419` (build stamp, 2020-10-07 21:43:39 UTC)
- `ttpms`: `1000` (claimed tick period; see clock tree below — the actual SysTick-derived tick differs)

---

## Serial links (USART3 / USART4)

The application **never uses USB** — USB is the ROM bootloader's DFU interface
only (0483:df11). All runtime communication is over two UARTs:

| | **USART3** `0x40004800` | **USART4** `0x40004C00` |
|---|---|---|
| Role | AT / control channel to the external BLE module | Robot's **command + reply link** |
| Pins | PB1 + PB13 (AF4), PC4 + PC5 (AF1), PA2 = input | **PC10 TX / PC11 RX (AF0)** |
| Baud | **14400**, 2 stop bits (CR2 = 0x2000) | **460800**, 2 stop bits |
| Traffic | BLE module setup traffic; reads `"BLE-RSP HELLO"` / `"BLE-RSP GAP SET MODE: "` | Commands in, **every** reply out (greeting, values, errors, `BUFOVF!!!!!`) |

All output primitives (`out_str` = `0x08009CCC`, `out_char` = `0x08009CA1`)
write to **USART4** (literal pool holds `0x40004C00`). The command reader
(`FUN_08009B14`) also reads USART4 and clears its own error flags.

USART3 error reports (`"BLE framing error"`, `"BLE USART noise"`, `"BLE USART
overrun"`) go out on USART4, where the commands come from.

### BLE link details
- 8 data bits, **2 stop bits** (CR2 = 0x2000), TE|RE (CR1 = 0x300), CR3 = 0x0C, UE set after init
- Pins: GPIOB1 + GPIOB13 → AF4, GPIOC4 + GPIOC5 → AF1, GPIOA2 = input
- RX ring 128 B, TX ring 64 B
- Errors: FE → `"BLE framing error"`, NE → `"BLE USART noise"`, ORE → `"BLE USART overrun"` (ICR bits 2/4/8)
- Ring overflow → `"BUFOVF!!!!!\n"`

### Command link details
- 8 data bits, **2 stop bits** (CR2 = 0), TE|RE (CR1 = 0x300), CR3 = 0x0000, UE set after init
- Pins: PC10 TX / PC11 RX (AF0)
- Error flags (framing/noise/overrun) cleared on USART4

---

## Modules and their ops

The firmware has 16 interfaces registered in a flash table at `0x0800D164`
(searched backwards by the dispatcher). 7 of the 16 handlers are 4-byte no-op
stubs in the original (`movs r0,#0; bx lr`); their real behaviour lives in the
periodic task `FUN_080063EC` and in per-module engines.

| # | Interface | Handler (flash) | Ops that do something | Notes |
|---|-----------|-----------------|----------------------|-------|
| 0 | hwiAdc | `0x080073DD` | `get buf` | 32 ADC samples, empty line after 9th/17th/25th value; decimal (radix 10) |
| 1 | hwiLed | `0x080090A5` | (stub in original) | Real behaviour: LED pattern engine via TIM15 + DMA1 ch5 |
| 2 | hwiDrive | `0x08008B3D` | `set dutyA`/`set dutyB` (hex), `get nFault` → `"0"` | TIM3 PWM, PSC=0, ARR=1000; duty clamped ±100 |
| 3 | drvBattery | `0x08006F2D` | `get voltage`/`current`/`remainingCapacity`/`chargeStat`/`temp`, `call hwInit` | Hex replies except `chargeStat`; `hwInit` starts BMS query over BLE link |
| 4 | hwiSonar | `0x08009F89` | (stub in original) | Sonar capture via TIM17 + DMA1 ch1 |
| 5 | bleCom | `0x0800837D` | (stub in original) | BLE module link over USART3 |
| 6 | hwiBuzz | `0x08008491` | `call play` (pattern) | Non-blocking tone sequencer via TIM16 |
| 7 | hwiMic | `0x0800962D` | (stub in original) | Mic sample via ADC, tracking filter, 500 ms |
| 8 | hwiEnc | `0x08008DFD` | (stub in original) | Wheel encoders via TIM1/TIM2 |
| 9 | hwiLine | `0x08009479` | (stub in original) | Line sensor via ADC |
| 10 | hwiPower | `0x0800981D` | `get pwrOff`/`Vsens`, `set Vsens on\|1\|true` | PC12 power hold, PC13 power-good, PC14/PC15 |
| 11 | hwiLightSense | `0x080093A9` | (stub in original) | Light sense via ADC |
| 12 | appLights | `0x08005F01` | (stub in original) | Light programs via TIM15 + DMA1 ch5 (ARGB strip) |
| 13 | appDrive | `0x08003849` | (stub in original) | Drive speed loop (200 ms) |
| 14 | appSys | `0x080064F1` | (stub in original) | Version block (greeting) |
| 15 | appLightSense | `0x08003855` | (stub in original) | (stub) |

### Op names that really exist in the original

- `buf` — hwiAdc (ADC buffer dump)
- `dutyA`, `dutyB`, `nFault` — hwiDrive
- `voltage`, `current`, `remainingCapacity`, `chargeStat`, `temp`, `hwInit` — drvBattery
- `pwrOff`, `Vsens` — hwiPower
- `echo` — in the string table but **no code references it**

The other 11 interfaces have no-op stub handlers in the original; the
reconstruction exposes their engines with documented, reconstructed op names
(e.g. hwiLed `on`/`pwm`/`rgb`).

### `drvBattery hwInit` (BMS query)

Sends an 8-byte frame on the BLE link:
`{0xcc, 0x4e, 0x01, 0x00, 0x00, 0x00, 0x00, parity}`
where parity = `FUN_080001c4((0x73a+1000)<<5, 0x271)` = `0x91`.

The state machine (`FUN_08006D34`) steps 1..7, sending `{cc,0xb4}`, `{cc,0xb8,x}`,
`{cc,0xbe,x}` frames and parsing 5 reply bytes into:
- temp: `_DAT_200004a2`
- voltage: `_DAT_200004a4`
- current: `_DAT_200004a6`
- remaining: `_DAT_200004a8`

---

## Boot sequence

```
Reset_Handler (0x08002904)
  ├── set SP = 0x20003FFF
  ├── copy .data (flash → RAM)
  ├── zero .bss
  ├── SystemInit()  (0x08002958): HSI16 → PLL×6 = 96 MHz
  ├── __libc_init_array()  (0x0800B6B0)
  ├── main()  (0x08009FA4)
  └── spin forever
```

`main()`:
1. `hwInit` — initialise hardware
2. Send the greeting on USART4
3. Loop: `{ read a line (40-byte buffer) → dispatch → periodic tasks }`

The periodic task `FUN_080063EC` runs 11 engines every iteration:
1. BLE link service (800 ms)
2. Motor fault poll (GPIOA PA12)
3. ADC/DMA poll (overrun flag clear)
4. Line sensor service (0x37 ms)
5. BLE AT re-send (2000 ms)
6. Light-sense sample
7. Light program step (160 ms)
8. Mic sample (500 ms)
9. Line sensor sample
10. Buzzer tone sequencer
11. Drive speed loop (200 ms)

Bootloader entry: `main()` tests two magic words in RAM
(`0x200025a8 == 0xD7E5F7A8` and `0x200017a8 == 0x19c3fe40`). If they match,
it runs power-fail mode and jumps to the ROM bootloader at
`0x1FFFC804` (MSP = `0x200014A8`).

---

## Hardware register map

All peripheral bases below are **taken from the decompiled binary** — they are
ground truth for this board, not necessarily matching any published ST datasheet.

| Peripheral | Base | Notes |
|------------|------|-------|
| GPIOA | `0x48000000` | MODER 0x00, OTYPER 0x04, OSPEEDR 0x08, PUPDR 0x0C, IDR 0x10, ODR 0x14, BSRR 0x18, LCKR 0x1C, AFR 0x20/0x24 |
| GPIOB | `0x48000400` | same layout, 0x400-byte spacing |
| GPIOC | `0x48000800` | same layout |
| GPIOF | `0x48001400` | same layout |
| TIM1 | `0x40012C00` | wheel encoders |
| TIM3 | `0x40000400` | motor drivers + GPIOC |
| TIM6 | `0x40001000` | tick counter (~65.5 ms update) |
| TIM15 | `0x40014000` | LED pattern engine + DMA1 ch5 |
| TIM16 | `0x40014400` | buzzer engine |
| TIM17 | `0x40014800` | sonar capture + DMA1 ch1 |
| USART1 | `0x40013800` | (not used by firmware) |
| USART3 | `0x40004800` | BLE module link (CR1 0x00, CR2 0x04, CR3 0x08, BRR 0x0C, ISR 0x1C, ICR 0x20, RDR 0x24, TDR 0x28) |
| USART4 | `0x40004C00` | command + reply link (same layout) |
| ADC1 | `0x40012400` (CCR 0x40012440) | battery / mic / light sensing |
| DMA1 | `0x40020000` | channel stride 0x14 |
| RCC | `0x40021000` | CR 0x00, CFGR 0x04, CIR 0x08, AHBENR 0x14, APB2ENR 0x18, APB1ENR 0x1C |
| FLASH | `0x40022000` | |
| IWDG | `0x40003000` | KR 0x00, PR 0x04, RLR 0x08 |
| SYSCFG | `0x40010000` | EXTICR 0x14-0x18 |
| Cortex-M SysTick | `0xE000E010` | |
| Cortex-M NVIC | `0xE000E100` | IPR base |
| Cortex-M SCB | `0xE000ED00` | |

### GPIO pins actually used

| Pin | Function |
|-----|----------|
| PB14 | LED data (TIM15 CCR1, AF1) |
| PB12, PB15 | LED (GPIO) |
| PC12 | Power hold (output) |
| PC13 | Power-good input + EXTI13 (falling edge) |
| PC14, PC15 | Vsens |
| PC10 | USART4 TX (AF0) |
| PC11 | USART4 RX (AF0) |
| PB1 | USART3 RX (AF4) |
| PB13 | USART3 TX (AF4) |
| PC4 | USART3 RX (AF1) |
| PC5 | USART3 TX (AF1) |
| PA2 | input (unused by app) |
| PA0, PA8, PA15 | Encoder (TIM1/TIM2, AF2); PA15 = supply enable |
| GPIOC 6-9 | Motor outputs (TIM3, AF0) |
| PC2 | Motor enable |
| PC1 | Motor input pull-up |
| PB5 | Buzzer |
| PB8 | Buzzer PWM (TIM16 CH1, AF2) |
| GPIOB pin 9 | Sonar (TIM17, AF2) |
| PA12 | Motor fault input / USB VBUS detect (read as GPIO) |
| PA10 | BLE link state (GPIOA mask 0x400) |
| ADC group 0x20 | hwiLine (sensor bar) |
| ADC group 0x40 | hwiLightSense (averaged over 8 samples) |
| ADC group id 2 | hwiMic (tracking filter) |
| GPIOF PF6 | Charge state (pull-up/pull-down probe) |

---

## Clock tree

`FUN_08002958` (`SystemInit`):
1. `CR |= HSION \| HSEON` (bits 0, 16)
2. Wait for `HSERDY` (bit 17, timeout 0x5000)
3. `CFGR &= 0xf8c0b80c`
4. If CR bit 18 (HSE bypass): `FLASH_ACR = 0x11`, wait `PLLRDY` (bit 25)
5. `CFGR = (CFGR \& 0xf8c0380c) \| 0x130002`
6. `CR |= 0x1010001`
7. Wait for `SWS == PLL`

Result: **HSI16 → PLL×6 = 96 MHz**, AHB = APB = 96 MHz.

Note: the greeting advertises `"ttpms":"1000"` but the dump's TIM6 (PSC 48,
ARR 0xFFFF) updates every ~65.5 ms, not 1 ms. The SysTick is used only as a
1 MHz-derived delay counter and a "spin forever" exit for the charge-state
probe — not as the system tick.

---

## RAM layout

| Region | Address | Size | Contents |
|--------|---------|------|----------|
| .data | `0x20000000` | ~0xF04 bytes | copied from flash `0x0800C720..0x0800D624` |
| .bss | `0x20000F08` | ~0x128 bytes | zero-filled at boot |
| Stack | `0x20003FFF` | top of 16 KB RAM | initial SP |

Key RAM variables:
- `0x20000410` — bootloader state (0/2/3)
- `0x2000035e` — LED animation state (0/4=red)
- `0x200004a2/4a4/4a6/4a8` — BMS temp/voltage/current/remaining
- `0x200025a8` / `0x200017a8` — reset magic words (`0xD7E5F7A8` / `0x19c3fe40`)
- `0x20000fe8` — 40-byte command buffer
- `0x20000a14` — greeting key/value array
- `0x20001018` — dump tick counter (TIM6)

---

## Interrupts

31 IRQs (0..30). Real handlers exist for 9 of them:

| IRQ | Dump handler | What |
|-----|-------------|------|
| 7 | `FUN_0800976C` | EXTI4_15, power-good (PC13) |
| 9 | `FUN_08008EE8` | TIM17 CC / DMA1 ch1 (sonar) |
| 11 | `FUN_08007734` | TIM15 + DMA1 ch5 (LED pattern engine) |
| 13 | `FUN_08008BF8` | TIM1 CC (wheel encoder) |
| 15 | `FUN_08008C20` | TIM2 CC (wheel encoder) |
| 17 | `FUN_0800A43C` | TIM6 update (tick counter increment) |
| 20 | `FUN_0800745C` | TIM15 update (LED sequencer state machine) |
| 22 | `FUN_08008E9C` | TIM17 update (sonar) |
| 29 | `FUN_08002DC8` | USART/line error paths |

The other 22 IRQs fall into `Default_Handler` (`FUN_08002954`, endless loop).

---

## Option bytes

`AA55 FF00 FF00 FF00 FF00 FF00 FF00 FF00` — magic `0x55AA`, user byte `0xFF`,
write-protection bytes all `0xFF` ⇒ **no sector write protection, no read
protection**.

---

## Related documentation

This page documents the **original firmware** as recovered from the binary
dump. The **Rust port** (a reimplementation in `rust/`) is documented
separately:

- [`FEATURES.md`](FEATURES.md) — implemented vs. missing features in the port
- [`rust/PROGRESS.md`](rust/PROGRESS.md) — register-level bug fixes and open items
- [`rust/STATUS_INTENDED_VS_REALITY.md`](STATUS_INTENDED_VS_REALITY.md) — intended behaviour vs. what the port actually does
- [`README.md`](README.md) — build, flash, and repository notes

## Relation to the Rust Port Rewrite

This document describes the **original firmware** as recovered from the binary dump `stm32_flash.bin`. The Rust port (a reimplementation in `rust/`) is documented separately:

- [`FEATURES.md`](rust/FEATURES.md) — per-feature implemented vs. missing list
- [`rust/PROGRESS.md`](rust/PROGRESS.md) — register-level bug fixes and open items
- [`rust/STATUS_INTENDED_VS_REALITY.md`](rust/STATUS_INTENDED_VS_REALITY.md) — intended behavior vs. actual port behavior
- [`rust/README.md`](rust/README.md) — build, flash, and verification instructions

The Rust port lives on the [`feat/autonomous`](https://github.com/mikes28/codie-firmware/tree/feat/autonomous) branch.
