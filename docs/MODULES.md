# MODULES — per-module behaviour (16 interfaces)

One module per interface, named after the C file it comes from
(`fw/modules/*.c` → `rust/src/modules/*.rs`). Every module publishes itself in
the interface registry from its `init()` function.

Init order matches the dump's interface table (`0x0800D164`):
`hwiPower::power_hold_on()` first (PC12 power-hold latch), then the remaining
15 in table order, then `app_sys::hw_init()` (battery pack query), then
`system::watchdog_init()` and the greeting.

---

## hwi_adc — raw ADC channels

**C:** `fw/modules/hwi_adc.c` → **Rust:** `rust/src/modules/hwi_adc.rs`
**Original handler:** `FUN_080073DC` (0x080073DD)

**Wire behaviour:**
- `get hwiAdc buf` → the 32 raw samples of the DMA buffer, one per line. After
  the 9th, 17th and 25th value an additional empty line is emitted. No
  "get hwiAdc buf" prefix — numbers written straight to output. Radix 10.
- every other op → ignored, no reply.

**Hardware:** ADC1 (`0x40012400`), 4 channels (masks `0x02, 0x04, 0x20, 0x40` =
IN1, IN2, IN5, IN6), GPIOA pins 1/2/5/6 in analog mode, DMA1 CH2
(`0x4002001C`) copies results into a 32-word buffer, 8 rounds of the 4 channels.

**Periodic tick** (`hwi_adc::tick`, `FUN_080071C0`): tests the ADC conversion-
overrun flag (mask `0x10`) and clears it.

---

## hwi_led — ARGB LED strip

**C:** `fw/modules/hwi_led.c` → **Rust:** `rust/src/modules/hwi_led.rs`
**Original bring-up:** `FUN_080079A0` (0x080079A0)

**Wire behaviour:**
- `set hwiLed on <led> <0\|1>` — full on / off
- `set hwiLed pwm <led> <duty>` — scaled duty 0..255
- `set hwiLed rgb <led> <r> <g> <b>` — explicit RGB colour (GRB wire order)
- `get hwiLed get <led>` — current duty

**Hardware:** TIM15 (`0x40014000`) + DMA1 CH5 (`0x40020058`) single-wire ARGB
(WS2812-class). PB14 in AF1 carries the data. 12 LEDs.

**Dump evidence (verified):**
- GPIOB on, PB14 in AF1 (`FUN_0800AAC8(0x48000400, 14, 1)` @`0x8007a16`)
- TIM15 on, PSC = 29 (`movs r2,#29` @`0x80079b8`), ARR = 0
- PB15: ALT, open-drain, speed 3, no AF call
- PB12: INPUT (mode byte overwritten to 0), open-drain, speed 3, no AF
- PB14: INPUT, open-drain, speed 3, **pull-up**, AF1
- Only PB14 is routed to a timer function

**Bit encoding** (`FUN_080090A8`, MSB first): bit 0 → `0x0e`, bit 1 → `0x2e`.
With 3.2 MHz timer clock (96 MHz / 30) and ARR 0x307 (775), one bit = 242.5 µs.
12 LEDs × 3 bytes × 8 bits = 288 bits → 69.8 ms per frame. `PPRT`/`PPRET`
markers emitted at transfer start/end.

**Note:** the pin map (PB14/PB12/PB15 on GPIOB) and the physical strip wiring
are hardware unknowns the dump cannot answer.

---

## hwi_drive — two motor drivers

**C:** `fw/modules/hwi_drive.c` → **Rust:** `rust/src/modules/hwi_drive.rs`
**Original handler:** `FUN_08008B3C` (0x08008B3D)

**Wire behaviour:**
- `set hwiDrive dutyA <n>` — motor A duty. Argument parsed as **hexadecimal**,
  truncated to 16 bit, clamped to ±1000. No reply.
- `set hwiDrive dutyB <n>` — same for motor B.
- `get hwiDrive nFault` → `set hwiDrive nFault 0\n`. The original reads the
  fault input (GPIOC pin 14) but throws the result away and answers the
  literal "0". Op word is "set" (not "get").

**Hardware:** TIM3 (`0x40000400`) centre-aligned, PSC = 0, ARR = 1000, 4 PWM
channels (PWM mode 2) on GPIOC pins 6,7,8,9 (AF 0): CH1 = motor A forward,
CH2 = motor A reverse, CH3 = motor B forward, CH4 = motor B reverse.
GPIOC pin 2 = driver enable (high = enabled). GPIOC pin 1 = input pull-up.
GPIOC pin 14 = fault input.

**Periodic tick** (`hwi_drive::tick`, `FUN_08008784`): edge-triggered, debounced
motor fault poll on GPIOA pin 12.

**Note:** motor output pin map and physical wiring are hardware unknowns.

---

## drv_battery — battery monitor

**C:** `fw/modules/drv_battery.c` → **Rust:** `rust/src/modules/drv_battery.rs`
**Original handler:** `FUN_08006F2D` (0x08006F2C)

**Wire behaviour (all byte-exact from the dump):**
- `get drvBattery voltage` → `set drvBattery voltage <hex>\n` (local ADC)
- `get drvBattery current` → `set drvBattery current <hex>\n` (local ADC)
- `get drvBattery remainingCapacity` → `set drvBattery remainingCapacity <hex>\n`
- `get drvBattery chargeStat` → `set drvBattery chargeStat 1\n` (PF6 probe)
- `get drvBattery temp` → `set drvBattery temp <hex>\n`
- `call drvBattery hwInit` → no reply; starts the BMS state machine (open:
  payload undecodable — see FINDINGS.md)

**Hardware:** GPIOF pin 6 = charge-state probe (pull-up / pull-down, 10 ms
delays). Values come from the local ADC; the original asks the BLE battery pack
for them (`FUN_08006D34`) but that path is not a BLE exchange over USART3 —
every frame builder ends in `FUN_080078E4` gated on the LED engine's DMA state
variable.

**Note:** the BMS query over the LED-DMA transport is documented-open rather
than guessed.

---

## hwi_sonar — ultrasonic distance

**C:** `fw/modules/hwi_sonar.c` → **Rust:** `rust/src/modules/hwi_sonar.rs`
**Original handler:** stub (4-byte `movs r0,#0; bx lr`)

**Wire behaviour:** stub — answers no op.

**Hardware:** TIM17 (`0x40014800`), GPIOB pin 9 AF2, IRQ 22 (prio 3), IRQ 9
(prio 2, DMA1 CH1). `ccer5 = 0` (confirmed by `FUN_08008F14`).

**Functions:** `init`, `busy` (measurement-running flag, gap closed in this
port), `echo_us`, `trigger`, `handler` (stub).

---

## ble_com — BLE module + console link

**C:** `fw/modules/bleCom.c` → **Rust:** `rust/src/modules/ble_com.rs`
**Original handler:** `FUN_0800837D` (0x0800837D)

**Wire behaviour:**
- `bleCom buf` / `write` / `echo` — console link operations
- Console error flags + "BUFOVF!!!!!\n" on ring overflow
- "BLE-RSP HELLO" / "BLE-RSP GAP SET MODE: " responses from the module

**Hardware:** USART3 (`0x40004800`, 14400 bit/s, 2 stop bits) = BLE AT channel.
RX ring 128 B, TX ring 64 B. Errors: FE → "BLE framing error", NE → "BLE USART
noise", ORE → "BLE USART overrun".

**All robot output goes to USART4** (460800) — `out_str`/`out_char` write to
CONSOLE_USART. This was swapped in the first rebuild and fixed.

---

## hwi_buzz — buzzer / tone engine

**C:** `fw/modules/hwi_buzz.c` → **Rust:** `rust/src/modules/hwi_buzz.rs`
**Original handler:** `FUN_08008491` (0x08008491)

**Wire behaviour:**
- `call hwiBuzz play <pattern>` — play one of 5 tone patterns (non-blocking
  queue)

**Hardware:** TIM16 (`0x40014400`) CH1 on PB8 AF2. **PSC = 2** (verified,
not 96), ARR = TIMCLK/freq − 1, CCR = ARR × duty / 100. TIMCLK = 48 MHz.

**Dump evidence:** `FUN_08008380` @`0x80083de: strh r7,[r1,#0]` with `r7 = 2`;
idle ARR = 24000 (`0x5DC0`).

**Periodic tick** (`hwi_buzz::tick`, `FUN_08008574`): queued tone sequencer.

---

## hwi_mic — microphone level

**C:** `fw/modules/hwi_mic.c` → **Rust:** `rust/src/modules/hwi_mic.rs`
**Original handler:** `FUN_0800962D` (0x0800962D)

**Wire behaviour:** stub — answers no op.

**Hardware:** ADC sensor group id 2 with a tracking filter (±64 window).

**Periodic:** mic sample at 500 ms (`FUN_080030D0`) — publishes through
`FUN_08005EC4(0x103,…)` internal bus slot; not ported (no bus counterpart).

---

## hwi_enc — wheel encoders

**C:** `fw/modules/hwi_enc.c` → **Rust:** `rust/src/modules/hwi_enc.rs`
**Original handler:** stub

**Wire behaviour:**
- `get hwiEnc count` → encoder count (incl. combined `0x20`)
- `call hwiEnc reset` → reset

**Hardware:** TIM1 (`0x40012C00`) + TIM2 (`0x40000000`), GPIOA pins 0/8 AF2,
pin 15 = supply enable. CCMR1 `0x21`, CCER `0xB`, CR2 `7`, SMCR `0x50`.
IRQ 13 (prio 2), IRQ 15 (prio 2).

**Dump evidence:** `FUN_08008C48` matches; encoder pins GPIOA 0/8 AF2, pin 15
supply enable.

---

## hwi_line — line sensors

**C:** `fw/modules/hwi_line.c` → **Rust:** `rust/src/modules/hwi_line.rs`
**Original handler:** `FUN_08009479` (0x08009479)

**Wire behaviour:** one bit per sensor.

**Hardware:** PA4 output = sensor bar, ADC groups `0x20`/`0x40`, deltas vs
calibration. GPIOC pin 13 = line sensor.

**Periodic:** line sensor sample (`FUN_080094F8`) — not ported; line sensor
service (`FUN_08009EA0`) — not ported.

---

## hwi_power — power management

**C:** `fw/modules/hwi_power.c` → **Rust:** `rust/src/modules/hwi_power.rs`
**Original handler:** `FUN_0800981D` (0x0800981D)

**Wire behaviour:**
- `call hwiPower pwrOff <on\|1\|true>` — PC12 power-hold off, then spin (never
  returns)
- `call hwiPower Vsens <on\|1\|true>` — PC15 set / reset (set op)

**Hardware:** PC12 = power-hold output, PC13 = power-good input + EXTI13
(falling edge, SYSCFG EXTICR4 bits 15:12), PC14/PC15 (Vsens).

**Verified:** EXTI13 routed to GPIOC via EXTICR4 (the old port used EXTICR3 —
wrong register, fixed). IRQ 7 (EXTI4_15) served.

---

## hwi_lightsense — light sensor

**C:** `fw/modules/hwi_lightsense.c` → **Rust:** `rust/src/modules/hwi_lightsense.rs`
**Original handler:** stub

**Wire behaviour:** stub — answers no op.

**Hardware:** ADC group id 4, averaged over 8 samples.

---

## app_lights — light program engine

**C:** `fw/modules/app_lights.c` → **Rust:** `rust/src/modules/app_lights.rs`
**Original handler:** stub

**Wire behaviour:** stub — real behaviour is the periodic engine.

**Hardware:** drives the ARGB strip via `hwi_led`. `PPRT`/`PPRET` markers from
the sequencer.

**Periodic tick** (`app_lights::tick`, `FUN_08006000`): light program step at
160 ms (`_DAT_2000035c = 0xa0` when zero).

---

## app_drive — drive speed loop

**C:** `fw/modules/app_drive.c` → **Rust:** `rust/src/modules/app_drive.rs`
**Original handler:** stub

**Wire behaviour:** stub — real behaviour is the periodic engine.

**Periodic tick** (`app_drive::tick`, `FUN_08003330`): speed loop, `err / 4`,
clamp ±100.

---

## app_sys — system info

**C:** `fw/modules/app_sys.c` → **Rust:** `rust/src/modules/app_sys.rs`
**Original handler:** `FUN_080064F1` (0x080064F1)

**Wire behaviour:** 4 version strings.

**Functions:** `hw_init` (calls `drv_battery::hw_init`, battery pack query),
`init` (registers "appSys").

---

## app_lightsense — light-sense app

**C:** `fw/modules/app_lightsense.c` → **Rust:** `rust/src/modules/app_lightsense.rs`
**Original handler:** stub

**Wire behaviour:** stub — real behaviour is the periodic engine.

**Periodic tick** (`app_lightsense::tick`, `FUN_08009340`): light-sense sample.

---

## The 4 unported periodic engines

| engine | dump function | why open |
|---|---|---|
| BLE link service | `FUN_080096EC` (800 ms) | needs ble_com line-callback hook |
| Line sensor sample | `FUN_080094F8` | needs ble_com line-callback hook |
| BLE AT re-send | `FUN_08008130` (2000 ms) | payload bytes undecodable |
| Mic sample | `FUN_080030D0` (500 ms) | internal bus slot, no Rust counterpart |
