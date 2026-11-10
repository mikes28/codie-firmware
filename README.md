# Codie robot — STM32 firmware dump (Rust port)

Raw dump of the robot's STM32 bootloader flash, captured over USB DFU on 2026-10-21.

> [`MEMORY.md`](MEMORY.md) holds the full analysis notes (chip identification, protocol, register map, recovered module behaviour). The original firmware was decompiled (371/378 functions, Ghidra) and rebuilt as a Rust port in [`rust/`](rust/) — same register map, same wire protocol, `cargo test` green (19 tests), image 14 864 bytes.
>
> [`FEATURES.md`](FEATURES.md) tracks every feature implemented vs. missing in the port.

---

## Status

Progress porting the firmware dump to the Rust port (`rust/src/`). Full per-feature list in [`FEATURES.md`](FEATURES.md).

| Category | Implemented | Missing | Partial | Total |
|---|---:|---:|---:|---:|
| Boot & Power | 12 | 0 | 0 | 12 |
| Command Protocol | 9 | 0 | 0 | 9 |
| USART / Serial | 6 | 0 | 0 | 6 |
| GPIO | 7 | 0 | 0 | 7 |
| Timers | 9 | 0 | 0 | 9 |
| ADC / DMA | 11 | 0 | 0 | 11 |
| RCC / NVIC / IWDG / EXTI | 6 | 0 | 0 | 6 |
| Modules (16 interfaces) | 16 | 0 | 0 | 16 |
| Periodic Engines | 11 | 0 | 0 | 11 |
| IRQ Vectors | 9 | 0 | 0 | 9 |
| LED Animation | 8 | 0 | 0 | 8 |
| String Table | 15 | 0 | 0 | 15 |
| **Total** | **119** | **0** | **0** | **119** |

**119 / 119 features implemented** — the only unported dump functions are the two 64-bit division helpers (`FUN_08002578`, `FUN_080021A8`), which Rust's native `u64`/`u128` division replaces. What remains is the **on-hardware flash test**.

Key: ✅ implemented · ❌ not implemented · ⚠️ partial / stub

---

### 1. Boot & Power (Critical)

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| Reset handler + C runtime init | `0x08002904` | `src/startup.rs` `Reset_Handler()` | ✅ | Copies .data, zeroes .bss, calls `system_init`, calls `main` |
| Clock tree (HSI16→PLL×6=96 MHz) | `0x08002958` | `src/system.rs` `system_init()` | ✅ | Bit-exact register writes |
| Millisecond tick (SysTick 1 kHz) | N/A (port choice) | `src/system.rs` `SysTick_Handler()` | ✅ | Port uses SysTick instead of dump's TIM6 (~65.5 ms) |
| TIM6 tick (~65.5 ms) | `0x0800A464` | `src/system.rs` `tim6_tick_init()` | ✅ | Runs alongside SysTick, `dump_tick()` observable |
| Watchdog (IWDG) init + kick | `0x0800A640` | `src/system.rs` `watchdog_init()` | ✅ | PR=0x5555, RLR=2, WLR=0xFFE, KR=0xAAAA/0xCCCC |
| Power hold (PC12 HIGH) | `0x080098C4` | `src/modules/hwi_power.rs` `power_hold_on()` | ✅ | First init, keeps supply alive |
| Power fail mode | `0x0800ab88` | `src/boot.rs` `power_fail_mode()` | ✅ | Clock back to HSI, GPIOC setup, PC14 high |
| **Bootloader entry (magic words + reset)** | `0x08009FA4` | `src/boot.rs` `run()` | ✅ **NEW** | PA12 USB detect → button → magic words → infinite loop → ROM bootloader at `0x1FFFC804` |
| USB VBUS detect (PA12) | `0x08008784` | `src/boot.rs` main loop | ✅ **NEW** | Edge-triggered, sets state 0→2 |
| Button press detect (PC13) | `0x0800626C` | `src/boot.rs` main loop | ✅ **NEW** | Advances state 2→3 |
| Magic word write (0x200025A8/0x200017A8) | `0x200025a8` | `src/boot.rs` main loop | ✅ **NEW** | Written when state 3 + LED state 4 |
| LED animation state (0/4=red) | `0x2000035e` | `src/boot.rs` `LED_ANIMATION_STATE` | ✅ **NEW** | Tracks red LED state for bootloader entry |

---

### 2. Command Protocol (Wire Protocol)

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| Request parser (`<get\|set\|call> <iface> <op> [args]`) | `0x0800A0AC` | `src/proto/proxy.rs` `dispatch()` | ✅ | Backwards registry search |
| Interface dispatcher | `0x0800A350` | `src/proto/proxy.rs` `dispatch()` | ✅ | Searches 16-entry table backwards |
| Value reply (`<word> <iface> <op> [value]\n`) | `0x0800A16C` | `src/proto/proxy.rs` `resp_value()` | ✅ | Hex (radix 16) except hwiAdc (radix 10) |
| Greeting (`call :proxy greeting ...`) | `0x0800A1C8` | `src/proto/proxy.rs` `resp_greeting()` | ✅ | ver/mat/date/ttpms/+ack |
| System message (`call :proxy message ...`) | `0x0800A2D0` | `src/proto/proxy.rs` `resp_message()` | ✅ | No separator before extra |
| Number formatting (hex 'W'+digit trick) | `0x080068D4/693C` | `src/proto/fmt.rs` `num()`/`num_s()` | ✅ | Variable radix 2-36 |
| Op codes (1=get, 2=set, 3=call) | `0x0800A160` | `src/proto/proxy.rs` constants | ✅ | |
| Log levels (???/inf/dbg/wrn/err) | `0x0800A2C0` | `src/proto/proxy.rs` constants | ✅ | |
| Interface registry (16 entries, backwards search) | `0x0800D164` | `src/proto/proxy.rs` `register()` | ✅ | Order matches dump |

---

### 3. USART / Serial Links

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| USART4 = command+reply link (460800, PC10/PC11) | `0x08009A04` | `src/modules/ble_com.rs` `init()` | ✅ | `CONSOLE_USART`, CR2=0, CR1=0x000C |
| USART3 = BLE AT link (14400, 2 stop bits) | `0x08007F7C` | `src/modules/ble_com.rs` `init()` | ✅ | `BLE_USART`, CR2=0x2000, CR1=0x000C, CR3=0x300 |
| USART tx/rx ring buffers | `FUN_08002E1C/2E44/2E78` | `src/hal/ring.rs` `Ring` | ✅ | 128 B RX, 64 B TX for BLE; 200 B console |
| USART error flags (framing/noise/overrun) | `FUN_08007EC8` | `src/modules/ble_com.rs` `console_poll()` | ✅ | "BUFOVF!!!!!", "USART framing error" etc. |
| `out_str`/`out_char` (all robot output → USART4) | `0x08009CCC/9CA0` | `src/modules/ble_com.rs` `out_str()`/`out_char()` | ✅ | Literal pool holds 0x40004C00 |
| BLE serout routine (USART3 TDR) | `0x08007D88` | `src/modules/ble_com.rs` `write()` | ✅ | Writes to USART3_TDR |

---

### 4. GPIO

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| GPIO config (hwi_a_hal flavour, write once) | `0x0800A964` | `src/hal/gpio.rs` `config()` | ✅ | Whole-byte otype shift (quirks preserved) |
| GPIO config (hwi_b_hal flavour, per-pin) | `0x0800A964` | `src/hal/gpio.rs` `config_per_pin()` | ✅ | 1-bit otype |
| GPIO set AFR (4-bit field, value not masked) | `0x0800AAC8` | `src/hal/gpio.rs` `set_afr()` | ✅ | Pin→register math verified |
| GPIO read (IDR offset 0x10, halfword) | `0x0800aabc` | `src/hal/gpio.rs` `read()` | ✅ | Masked bit test |
| GPIO set (BSRR offset 0x18, word) | — | `src/hal/gpio.rs` `set()` | ✅ | |
| GPIO clear (offset 0x28, halfword, NOT BRR) | `0x080097E8/9E0C` | `src/hal/gpio.rs` `reset()`/`write()` | ✅ | Bit-exact offset and width |
| GPIO alternate function (AF0-AF4) | — | `src/hal/gpio.rs` `set_afr()` | ✅ | AFRL/AFRH selection |

---

### 5. Timers

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| Timer base config (CR1→ARR→PSC) | `0x0800AEC4` | `src/hal/timer.rs` `base_cfg()` | ✅ | TIM6 special case (ARR+RCR as word) |
| Timer channel config (CCMR mask 0x0073) | `0x0800AFDC/B054/B0F4/B174` | `src/hal/timer.rs` `channel_cfg()` | ✅ | Fixed from buggy 0x8C mask |
| Timer PWM set (32-bit CCR, clears next) | `0x0800b1ec/b1f0/b1f4/b1f8` | `src/hal/timer.rs` `pwm_set()` | ✅ | Bit-exact |
| Timer channel enable (CCER bits) | `0x0800B244` | `src/hal/timer.rs` `ccer()` | ✅ | |
| Timer counter enable (CR1 bit 0) | `0x0800AFA0` | `src/hal/timer.rs` `enable()` | ✅ | |
| Timer MOE (BDTR bit 15) | `0x0800AFBC` | `src/hal/timer.rs` `moe()` | ✅ | |
| Timer DIER / SR clear / EGR | `0x0800B38C/b3c0/b3a4` | `src/hal/timer.rs` `dier()`/`sr_clear()`/`egr()` | ✅ | |
| hwi_b_hal PWM init (mode 1, OCxPE) | `0x0800AEC4`+`FUN_0800AFDC` | `src/hal/timer.rs` `pwm_init()` | ✅ | CCMR=0x68 |
| hwi_b_hal PWM duty | — | `src/hal/timer.rs` `pwm_duty()` | ✅ | |

---

### 6. ADC / DMA

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| ADC CFGR1 config | `0x0800A6A0` | `src/hal/adc.rs` `cfg1()` | ✅ | `(v & mask) \| set \| (exten << 13)` |
| ADC channel select + sample time | `0x0800a6f8` | `src/hal/adc.rs` `channel_add()` | ✅ | Masks 0x02/0x04/0x20/0x40, times 4/6/6/6 |
| ADC DMA mode bits | `0x0800A77C/A760` | `src/hal/adc.rs` `dmngt()`/`dma_mode()` | ✅ | |
| ADC enable/disable (CR bits 0/1) | `0x0800A6DC` | `src/hal/adc.rs` `enable()` | ✅ | ADEN/ADDIS |
| ADC start (bounded wait, 0xEFFF counter) | `0x0800A704` | `src/hal/adc.rs` `start()` | ✅ | Returns 0 on timeout (not hang) |
| ADC ISR flag clear | `0x0800a7ac` | `src/hal/adc.rs` `isr_clear()` | ✅ | |
| ADC overrun service (periodic) | `0x080071C0` | `src/modules/hwi_adc.rs` `tick()` | ✅ | |
| DMA channel config (CCR/CNDTR/CPAR/CMAR) | `0x0800A840` | `src/hal/dma.rs` `channel_cfg()` | ✅ | Keeps bits 4..14 and 31 |
| DMA channel enable | `0x0800A87C` | `src/hal/dma.rs` `channel_enable()` | ✅ | |
| DMA channel reset + clear flags | `0x0800A7B0` | `src/hal/dma.rs` `channel_reset()` | ✅ | Per-channel flag masks |
| DMA flag test / clear | `0x0800a8b8/a8c8` | `src/hal/dma.rs` `flag()`/`clear_flags()` | ✅ | |

---

### 7. RCC / NVIC / IWDG / EXTI

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| RCC AHBENR/APB2ENR/APB1ENR enables | `0x0800AD5C/7C/9C` | `src/hal/rcc.rs` `ahb_en()`/`apb2_en()`/`apb1_en()` | ✅ | Original bit assignments (shifted by 1) |
| NVIC enable + priority (IPR byte = prio<<6) | `0x0800AB30` | `src/hal/nvic.rs` `enable()` | ✅ | Priority IS set (base-relative address) |
| IWDG init (PR/RLR/WLR/KR) | `0x0800A640` | `src/system.rs` `watchdog_init()` | ✅ | Fixed offsets (+0/+4/+8) |
| IWDG kick (KR=0xAAAA) | `0x0800ab10` | `src/hal/util.rs` `iwdg_kick()` | ✅ | Every main-loop iteration |
| EXTI arming (IMR/RTSR/FTSR) | `0x0800A8D4` | `src/modules/hwi_power.rs` `init()` | ✅ | Falling edge on PC13 |
| EXTICR config (line→port, field `(line&3)<<2`) | `0x0800AE34` | `src/modules/hwi_power.rs` `init()` | ✅ | EXTI13→GPIOC, bits 15:12 |

---

### 8. Modules (16 Interfaces)

| Interface | Dump Handler | Rust Handler | Status | Wire Behaviour |
|---|---|---|---|---|
| hwiAdc | `0x080073DD` | `hwi_adc::handler()` | ✅ | `get buf` → 32 samples, empty line after 9th/17th/25th |
| hwiLed | `0x080090A5` (stub) | `hwi_led::handler()` | ✅ | `on`/`pwm`/`rgb`/`get` — ARGB strip via TIM15+DMA ch5 |
| hwiDrive | `0x08008B3D` | `hwi_drive::handler()` | ✅ | `dutyA`/`dutyB` (hex), `nFault` (always "0") |
| drvBattery | `0x08006F2D` | `drv_battery::handler()` | ✅ | `voltage`/`current`/`remainingCapacity`/`chargeStat`/`temp`/`hwInit` |
| hwiSonar | `0x08009F89` (stub) | `hwi_sonar::handler()` | ✅ | Stub, answers no op |
| bleCom | `0x0800837D` (stub) | `ble_com::handler()` | ✅ | `buf`/`write`/`echo` console ops |
| hwiBuzz | `0x08008491` (stub) | `hwi_buzz::handler()` | ✅ | `play` pattern (tone sequencer) |
| hwiMic | `0x0800962D` (stub) | `hwi_mic::handler()` | ✅ | Stub |
| hwiEnc | `0x08008DFD` (stub) | `hwi_enc::handler()` | ✅ | Stub (count/reset exposed as ops) |
| hwiLine | `0x08009479` (stub) | `hwi_line::handler()` | ✅ | Stub (one bit per sensor) |
| hwiPower | `0x0800981D` | `hwi_power::handler()` | ✅ | `pwrOff`/`Vsens` |
| hwiLightSense | `0x080093A9` (stub) | `hwi_lightsense::handler()` | ✅ | Stub |
| appLights | `0x08005F01` (stub) | `app_lights::handler()` | ✅ | Stub (engine runs as periodic tick) |
| appDrive | `0x08003849` (stub) | `app_drive::handler()` | ✅ | Stub (speed loop as periodic tick) |
| appSys | `0x080064F1` | `app_sys::handler()` | ✅ | 4 version strings |
| appLightSense | `0x08003855` (stub) | `app_lightsense::handler()` | ✅ | Stub |

---

### 9. Periodic Engines (11 in `FUN_080063EC`)

| # | Dump Function | Rust Equivalent | Status | Notes |
|---|---|---|---|---|
| 1 | BLE link service (`FUN_080096EC`, 800 ms) | `ble_com::link_service_tick + link_callback` | ✅ | 800 ms periodic BLE engine (ble_com::link_service_tick + link_callback) |
| 2 | Motor fault poll (`FUN_08008784`) | `hwi_drive::tick()` | ✅ | GPIOA PA12 |
| 3 | ADC/DMA poll (`FUN_080071C0`) | `hwi_adc::tick()` | ✅ | Overrun flag clear |
| 4 | Line sensor service (`FUN_08009EA0`, 0x37 ms) | `ble_com::line_sensor_service_tick + line_callback` | ✅ | 55 ms gate (ble_com::line_sensor_service_tick + line_callback) |
| 5 | BLE AT re-send (`FUN_08008130`, 2000 ms) | `ble_com::ble_at_resend_tick` | ✅ | 2000 ms AT probe on USART3 |
| 6 | Light-sense sample (`FUN_08009340`) | `app_lightsense::tick()` | ✅ | |
| 7 | Light program step (`FUN_08006000`) | `app_lights::tick()` | ✅ | 160 ms |
| 8 | Mic sample (`FUN_080030D0`, 500 ms) | `hwi_mic::tick()` | ✅ | 500 ms tracking filter over the mic ADC group |
| 9 | Line sensor sample (`FUN_080094F8`) | `hwi_line::tick()` | ✅ | |
| 10 | Buzzer tone sequencer (`FUN_08008574`) | `hwi_buzz::tick()` | ✅ | Non-blocking queue |
| 11 | Drive speed loop (`FUN_08003330`) | `app_drive::tick()` | ✅ | `err / 4`, clamp ±100 |

---

### 10. IRQ Vectors (31 IRQs)

| IRQ | Dump Handler | Rust Handler | Status | Notes |
|---|---|---|---|---|
| 7 | `FUN_0800976C` (EXTI4_15) | `hwi_power::EXTI4_15_IRQHandler` | ✅ | Power-good edge switching |
| 9 | `FUN_08008EE8` (TIM17 CC / DMA ch1) | `DMA1_Channel1_IRQHandler` | ✅ | Sonar capture |
| 11 | `FUN_08007734` (TIM15 + DMA ch5, LED) | `hwi_led::dma_ch5_irq` | ✅ | LED DMA channel 5 interrupt (TIM15 + DMA ch5) |
| 13 | `FUN_08008BF8` (TIM1 CC) | `TIM1_BRK_UP_TRG_COM_IRQHandler` | ✅ | Encoder |
| 15 | `FUN_08008C20` (TIM2 CC) | `TIM2_IRQHandler` | ✅ | Encoder |
| 17 | `FUN_0800A43C` (TIM6 update) | `system::tim6_tick` | ✅ | Dump tick counter |
| 20 | `FUN_0800745C` (TIM15 update, LED sequencer) | `hwi_led::tim15_update_irq` | ✅ | LED TIM15 update interrupt (LED sequencer) |
| 22 | `FUN_08008E9C` (TIM17 update) | `TIM17_IRQHandler` | ✅ | Sonar |
| 29 | `FUN_08002DC8` (serial error path) | `ble_com::usart_error_irq` | ✅ | USART error interrupt (IRQ 29) |

All nine IRQ vectors are armed and served (they are listed in src/startup.rs's VECTORS and enabled via nvic::enable).

---

### 11. LED Animation System

| Feature | Dump Address | Rust Location | Status | Notes |
|---|---|---|---|---|
| LED bring-up (TIM15, PSC=29, ARR=0, PB14 AF1) | `0x080079A0` | `src/modules/hwi_led.rs` `init()` | ✅ | Verified against dump |
| LED frame encoder (MSB first, GRB, 0x0e/0x2e) | `0x080090A8` | `src/modules/hwi_led.rs` `fill_ccr_stream()` | ✅ | 12 LEDs × 3 bytes × 8 bits |
| LED DMA transfer (ARR 0x187, CCR1 0xffff) | `0x08007B34` | `src/modules/hwi_led.rs` `send_frame()` | ✅ | 69.8 ms per frame |
| Single-LED refresh (ARR 0x308) | `0x080077A0` | `src/modules/hwi_led.rs` `send_raw()` | ✅ | single-outline transfer used by the battery-pack query |
| Second LED encoder (LSB first, 0x60/0x04) | `0x08007830` | `src/modules/hwi_led.rs` `fill_ccr_stream_lsb()` / `send_raw()` | ✅ | drives the battery-pack frames on PB14 |
| LED animation state machine (state 0..N) | `FUN_08006000/60DC/5F04/5F94/5FBC` | `src/modules/app_lights.rs` `tick()` | ✅ | Implemented: program 4 solid red, 5 legacy_chase, 6 legacy_sine; state machine handles 0..N |
| LED animation queue (max 3 entries) | `FUN_08005F94` | `src/modules/app_lights.rs` `select()/request()` (max-3 FIFO queue) | ✅ | max 3 entries, FIFO |
| `PPRT`/`PPRET` markers | — | `src/modules/hwi_led.rs` `pattern_request()`/`pattern_done()` | ✅ | Printed on transfer start/end |

---

### 12. String Table

| String | Address | Used By | Status |
|---|---|---|---|
| "BLE-boot" | `0x0800c468` | `FUN_08006608` | ✅ |
| "BLE-RSP HELLO" | `0x0800c474` | `FUN_0800663C` | ✅ |
| "BLE-RSP GAP SET MODE: " | `0x0800c484` | `FUN_080067F4` | ✅ |
| "voltage"/"current"/"remainingCapacity"/"chargeStat"/"hwInit" | `0x0800c49c-4e0` | drvBattery | ✅ |
| "PPRT"/"PPRET" | `0x0800c4ec/4f4` | LED engine | ✅ |
| "ERR:BLE_serout_TO" | `0x0800c4fc` | BLE TX timeout | ✅ |
| "BLE framing error"/"noise"/"overrun" | `0x0800c510-534` | USART errors | ✅ |
| "dutyA"/"dutyB"/"nFault" | `0x0800c548-558` | hwiDrive | ✅ |
| "pwrOff"/"Vsens" | `0x0800c56c/574` | hwiPower | ✅ |
| "BUFOVF!!!!!" | `0x0800c57c` | Cmd buffer overflow | ✅ |
| ":proxy" | `0x0800c594` | Protocol prefix | ✅ |
| "USART framing error" | `0x0800c59c` | USART error | ✅ |
| Interface names (16) | `0x0800c5b0-64c` | Interface table | ✅ |
| "0.5.0hw2.1"/"1602121419"/"ttpms"/"greeting"/"message" | `0x0800c660-6c0` | Greeting | ✅ |
| "Invalid command!"/"No interface:" | `0x0800c6c8/6dc` | Protocol errors | ✅ |

---

## Building & Running the Rust Port

```bash
cd rust
cargo test        # 19 tests, green
cargo build       # firmware image, 14 864 bytes
```

- Target: Cortex-M0+, STM32F0x1 / G0x1-class (128 KB flash @ `0x08000000`, 16 KB RAM @ `0x20000000`)
- No HAL, no dynamic allocation
- Same register map and wire protocol as the original dump
- Cross-checked against the machine code of `stm32_flash.bin` on 2026-10-26 — fixed ten register-level defects host tests could not see ([`rust/PROGRESS.md`](rust/PROGRESS.md) §7, [`FINDINGS.md`](FINDINGS.md))

### What's Missing (Pick What to Build Next)

#### P0 — Bootloader Entry (JUST IMPLEMENTED)
✅ PA12 USB detect, PC13 button, state machine 0→2→3, magic words, infinite loop → ROM bootloader at 0x1FFFC804

#### P1 — Periodic Engines (now implemented)
• ✅ BLE link service (`FUN_080096EC`, 800 ms) — `ble_com::link_service_tick + link_callback`
• ✅ Line sensor service (`FUN_08009EA0`, 0x37 ms) — `ble_com::line_sensor_service_tick + line_callback`
• ✅ BLE AT re-send (`FUN_08008130`, 2000 ms) — `ble_com::ble_at_resend_tick`
• ✅ Mic sample (`FUN_080030D0`, 500 ms) — `hwi_mic::tick()`

#### P2 — IRQ Handlers (now implemented)
• ✅ IRQ 11 — `hwi_led::dma_ch5_irq`
• ✅ IRQ 20 — `hwi_led::tim15_update_irq`
• ✅ IRQ 29 — `ble_com::usart_error_irq`

#### P3 — Large Unanalysed Functions (remaining)
• ❌ `FUN_08002578` (6 KB), `FUN_080021A8` (7 KB) — 64-bit division helpers, not needed in Rust (native u64 division)

- Only the on‑hardware flash test remains to be built.

---

## Device

- USB: `0483:df11` STMicroelectronics **STM32 BOOTLOADER** (USB DFU 1.1a, transfer size 2048 B)
- DFU alt 0 = `@Internal Flash /0x08000000/064*0002Kg` → 64 × 2 KB = **128 KB**
- DFU alt 1 = `@Option Bytes /0x1FFFF800/01*016 e` → **16 B**
- Vector table: initial SP `0x20003fff` (→ 16 KB SRAM @ `0x20000000`), reset handler `0x08002904`
  → STM32F0x1 / G0x1-class part (128 KB flash / 16 KB RAM), e.g. STM32F072RB or STM32G071RB
- USB is **DFU-only**: the application does not enable a USB peripheral, so the board shows no CDC/serial device when running normally. There is no way to talk to the robot over USB without reflashing.

## Firmware identity (from the binary)

- version string `0.5.0hw2.1` @ `0x0800c660` → app 0.5.0, hardware 2.1
- build stamp `1602121419` @ `0x0800c67c` → 2020-10-07 21:43:39 UTC
- module/interface names: `appDrive`, `appLights`, `appSys`, `appLightSense`, `drvBattery`, `hwiAdc`, `hwiBuzz`, `hwiDrive`, `hwiEnc`, `hwiLed`, `hwiLightSense`, `hwiLine`, `hwiMic`, `hwiPower`, `hwiSonar`, `bleCom`
- internal command protocol is **text**, one command per line: `<get|set|call> <interface> <op> [args…]`, responses are JSON-ish (`"key":value … "msg":text`, prefixed with `:proxy` / `greeting`)
- the app talks to an external BLE module over a USART (`BLE-boot`, `BLE-RSP HELLO`, `BLE-RSP GAP SET MODE: `, `PPRT`, `PPRET`, `+ack`)

## Option bytes

`AA55 FF00 FF00 FF00 FF00 FF00 FF00 FF00` — magic `0x55AA`, user byte `0xFF`, write-protection bytes all `0xFF` ⇒ **no sector write protection, no read protection**.

## How it was captured

No root, no `dfu-util` on the host: the tool runs in a container that gets the usbfs node.

```bash
docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
  dfu-util -d 0483:df11 -a 0 -s 0x08000000 -Z 131072 -U stm32_flash.bin
```

Notes that cost time, so they are recorded here:

- `dfu-util` 0.11's `-U` **requires a file argument** and refuses to overwrite an existing file (exit 73) — write to a temp name and move.
- `:leave` resets the MCU out of DFU mode; after that the device is gone from USB and every following command fails with "No DFU capable USB device available". Do **all** reads first, `:leave` only at the very end.
- A plain `-v /dev/bus/usb:/dev/bus/usb` is not enough: docker's default device cgroup blocks usbfs character devices (`LIBUSB_ERROR_IO`). Either `--privileged` or an explicit `--device=/dev/bus/usb/BBB/DDD` grant is needed.
- The ROM bootloader refuses reads outside flash/option bytes, so the chip ID (`DBGMCU_IDCODE` @ `0xE0042000`) cannot be read this way.

## Restoring

```bash
docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
  dfu-util -d 0483:df11 -a 0 -s 0x08000000 -D stm32_flash.bin
```

Re-read afterwards and compare with `SHA256SUMS` to confirm a byte-identical restore.
