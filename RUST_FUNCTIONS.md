# RUST_FUNCTIONS.md — Complete function reference for Rust port (`rust/src/`)

Every public and private function in the Rust firmware port, organized by module.
Each entry lists: function name, file:line, dump equivalent (if any), and behavior.

---

## 1. Crate Root (`src/lib.rs`)

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `panic_handler` | lib.rs:49 | None | Panic → spin loop |

---

## 2. Startup (`src/startup.rs`)

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `Reset_Handler` | startup.rs:44 | `Reset_Handler` (0x08002904) | Set SP, copy .data, zero .bss, call system_init, call main |
| `Default_Handler` | startup.rs:76 | `FUN_08002954` (0x08002954) | Endless loop for unused IRQs |
| `VECTORS` (static) | startup.rs:89 | Vector table (0x08000000) | 48-word vector table |

---

## 3. System (`src/system.rs`)

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `tick_ms()` | system.rs:72 | None | Returns millisecond tick |
| `dump_tick()` | system.rs:82 | None | Returns TIM6 dump counter |
| `tim6_tick()` | system.rs:88 | `FUN_0800A43C` (0x0800A43C) | TIM6 update IRQ (IRQ 17), increments dump_tick |
| `tim6_tick_init()` | system.rs:121 | `FUN_0800A464` (0x0800A464) | TIM6 init: PSC=48, ARR=0xFFFF, DIER update IE |
| `watchdog_init()` | system.rs:170 | `FUN_0800A640` (0x0800A640) | IWDG: PR=0x5555, RLR=2, WLR=0xFFE, KR=0xAAAA, KR=0xCCCC |
| `system_init()` | system.rs:200 | `FUN_08002958` (0x08002958) | Clock tree: HSI16→PLL×6=96 MHz |
| `sys_tick_init(reload)` | system.rs:287 | None | Program SysTick reload |
| `SysTick_Handler()` | system.rs:299 | None | SysTick IRQ, increments tick_ms |

---

## 4. HAL (`src/hal/`)

### addr.rs

| Item | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `GPIOA/B/C/F` | addr.rs:10-13 | None | GPIO base addresses (0x48000000, 0x48000400, 0x48000800, 0x48001400) |
| `GPIO_MODER/OTYPER/...` | addr.rs:16-25 | None | GPIO register offsets |
| `TIM1/2/3/6/15/16/17` | addr.rs:28-41 | None | Timer base addresses |
| `TIM_CR1/CR2/...` | addr.rs:44-61 | None | Timer register offsets |
| `ADC1/DMA1` | addr.rs:64-65 | None | ADC1 and DMA1 bases |
| `DMA1_CH1..CH7` | addr.rs:68-74 | None | DMA1 channel bases |
| `USART3/USART4` | addr.rs:78-80 | None | USART bases (0x40004800, 0x40004C00) |
| `USART_CR1/CR2/...` | addr.rs:83-91 | None | USART register offsets |
| `RCC/FLASH/SYSCFG/EXTI/IWDG` | addr.rs:94-107 | None | Core peripheral bases |
| `NVIC_ISER/NVIC_IPR/SYSTICK` | addr.rs:107-109 | None | Cortex-M core bases |
| `APB1_HZ` | addr.rs:116 | None | 96,000,000 |
| `TIMCLK_HZ` | addr.rs:125 | None | 48,000,000 (APB1/2) |

### gpio.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `config(port, &Cfg)` | gpio.rs:76 | `FUN_0800A964` (0x0800A964) | hwi_a_hal GPIO config (write once, whole-byte otype) |
| `config_per_pin(port, &PerPinCfg)` | gpio.rs:110 | `FUN_0800A964` (0x0800A964) | hwi_b_hal GPIO config (per-pin RMW, 1-bit otype) |
| `set_afr(port, pin, afr)` | gpio.rs:165 | `FUN_0800AAC8` (0x0800AAC8) | Set 4-bit AFR field (value not masked) |
| `read(port, mask)` | gpio.rs:182 | `FUN_0800aabc` (0x0800aabc) | Read IDR (offset 0x10, halfword) |
| `set(port, mask)` | gpio.rs:191 | None | Set BSRR (offset 0x18, word) |
| `reset(port, mask)` | gpio.rs:195 | `FUN_080097E8` (0x080097E8) | Clear at offset 0x28 (halfword, NOT BRR) |
| `write(port, mask, level)` | gpio.rs:205 | `FUN_08009E0C` (0x08009E0C) | Set via BSRR, clear at 0x28 (halfword) |

### timer.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `base_cfg(tim, &cfg)` | timer.rs:117 | `FUN_0800AEC4` (0x0800AEC4) | Timer base: CR1→ARR→PSC, TIM6 special case |
| `prescaler_egr(tim, psc, egr)` | timer.rs:148 | `FUN_0800af4c` | Timer prescaler + EGR |
| `arpe(tim, on)` | timer.rs:154 | `FUN_0800AF74` | Timer ARPE (CR1 bit 7) |
| `ocpe(tim, ch, on)` | timer.rs:160 | `FUN_0800b1fc/208/220/22c` | Timer compare preload enable |
| `channel_cfg(tim, ch, &cfg)` | timer.rs:172 | `FUN_0800AFDC/B054/B0F4/B174` | Timer channel config (CCMR low byte mask 0x0073) |
| `pwm_set(tim, ch, ccr)` | timer.rs:245 | `FUN_0800b1ec/b1f0/b1f4/b1f8` | Timer compare (32-bit, clears next CCR) |
| `ccer(tim, ch, on)` | timer.rs:254 | `FUN_0800B244` | Timer CCER channel enable |
| `enable(tim, on)` | timer.rs:261 | `FUN_0800AFA0` | Timer CR1 CEN (bit 0) |
| `moe(tim, on)` | timer.rs:266 | `FUN_0800AFBC` | Timer BDTR MOE (bit 15) |
| `dier(tim, mask, on)` | timer.rs:271 | `FUN_0800B38C/B3C8` | Timer DIER |
| `sr_clear(tim, mask)` | timer.rs:277 | `FUN_0800b3c0` | Timer SR clear (inverted mask) |
| `egr(tim, val)` | timer.rs:282 | `FUN_0800b3a4` | Timer EGR |
| `get16(tim, off)` | timer.rs:287 | None | Read 16-bit timer register |
| `set16(tim, off, v)` | timer.rs:292 | None | Write 16-bit timer register |
| `get32(tim, off)` | timer.rs:297 | None | Read 32-bit timer register |
| `set32(tim, off, v)` | timer.rs:302 | None | Write 32-bit timer register |
| `pwm_init(tim, psc, arr, channel)` | timer.rs:310 | `FUN_0800AEC4`+`FUN_0800AFDC` | hwi_b_hal PWM init (mode 1, OCxPE) |
| `pwm_duty(tim, channel, ccr)` | timer.rs:327 | None | Set PWM compare |
| `ccr(tim, channel)` | timer.rs:333 | None | Read PWM compare |
| `set_arr(tim, arr)` | timer.rs:338 | None | Set ARR + EGR |
| `start(tim)` | timer.rs:344 | `FUN_0800AFA0` | Timer CR1 CEN |
| `stop(tim)` | timer.rs:349 | `FUN_0800AFA0` | Timer CR1 CEN off |

### usart.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init(usart, &cfg)` | usart.rs:29 | `FUN_0800B3FC` (0x0800B3FC) | USART config (CR1/CR2/CR3, baud with rounding) |
| `tx_ready(usart)` | usart.rs:71 | `FUN_0800B53C` | USART ISR bit 7 (TXFNF) |
| `putc(usart, c)` | usart.rs:76 | `FUN_0800B4DC` | USART TDR write |
| `rx_ready(usart)` | usart.rs:81 | `FUN_0800B54C` | USART ISR bit 5 (RXNE) |
| `getc(usart)` | usart.rs:86 | `FUN_0800B4E4` | USART RDR read (& 0x1FF) |
| `status(usart)` | usart.rs:92 | None | USART ISR read |
| `clear(usart, mask)` | usart.rs:97 | `FUN_0800B548` | USART ICR write |
| `enable(usart)` | usart.rs:111 | `FUN_0800B4C0` | USART CR1 UE (bit 0) |
| `disable(usart)` | usart.rs:116 | `FUN_0800B4C0` | USART CR1 UE off |

### adc.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `cfg1(adc, clear_mask, set_mask, exten)` | adc.rs:14 | `FUN_0800A6A0` | ADC CFGR1: `(v & mask) \| set \| (exten << 13)` |
| `channel_add(adc, mask, smpr)` | adc.rs:21 | `FUN_0800a6f8` | ADC CHSELR + SMPR |
| `dmngt(adc, v)` | adc.rs:27 | `FUN_0800A77C` | ADC CFGR1 DMA bits |
| `dma_mode(adc, on)` | adc.rs:33 | `FUN_0800A760` | ADC DMA mode bit |
| `isr_clear(adc, mask)` | adc.rs:48 | `FUN_0800a7ac` | ADC ISR flag clear |
| `enable(adc, on)` | adc.rs:53 | `FUN_0800A6DC` | ADC CR bit 0 (ADEN) / bit 1 (ADDIS) |
| `stop(adc)` | adc.rs:63 | `FUN_0800a754` | ADC CR bit 2 |
| `start(adc)` | adc.rs:68 | `FUN_0800A704` | ADC start (bounded wait, returns 0 on timeout) |
| `ready(adc, mask)` | adc.rs:95 | `FUN_0800A78C` | ADC status flag test |

### dma.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `channel_cfg(ch, cpar, cmar, cntdr, ccr)` | dma.rs:10 | `FUN_0800A840` | DMA channel config (CCR, CNDTR, CPAR, CMAR) |
| `channel_enable(ch, on)` | dma.rs:18 | `FUN_0800A87C` | DMA channel enable |
| `channel_reset(ch)` | dma.rs:23 | `FUN_0800A7B0` | DMA channel reset + clear flags |
| `set_cntdr(ch, n)` | dma.rs:48 | `FUN_0800a89c` | DMA CNDTR set |
| `set_dir(ch, mask)` | dma.rs:53 | `FUN_0800A8A0` | DMA direction bit |
| `flag(mask)` | dma.rs:58 | `FUN_0800a8b8` | DMA ISR flag test |
| `clear_flags(mask)` | dma.rs:63 | `FUN_0800a8c8` | DMA IFCR write |

### rcc.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `ahb_en(mask, enable)` | rcc.rs:17 | `FUN_0800AD5C` | RCC AHBENR enable/disable |
| `apb2_en(mask, enable)` | rcc.rs:22 | `FUN_0800AD7C` | RCC APB2ENR enable/disable |
| `apb1_en(mask, enable)` | rcc.rs:27 | `FUN_0800AD9C` | RCC APB1ENR enable/disable |

### nvic.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `enable(irq, prio)` | nvic.rs:47 | `FUN_0800AB30` (0x0800AB30) | NVIC IPR RMW + ISER enable |

### ring.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `Ring::new()` | ring.rs:18 | `FUN_08002E1C` | Ring buffer init |
| `Ring::put(b)` | ring.rs:38 | `FUN_08002E44` | Put byte (returns false if full) |
| `Ring::get()` | ring.rs:49 | `FUN_08002E78` | Get byte (Option) |
| `Ring::read(dst)` | ring.rs:60 | `FUN_08002E78` | Read up to len bytes |
| `Ring::count()` | ring.rs:72 | None | Ring byte count |
| `Ring::flush()` | ring.rs:77 | `FUN_08002f38` | Flush ring |

### util.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `millis()` | util.rs:8 | None | Returns tick_ms() |
| `delay_ms(ms)` | util.rs:18 | `FUN_08002DD4` | Bounded busy wait on millis |
| `iwdg_kick()` | util.rs:31 | `FUN_0800ab10` (0x0800ab10) | IWDG KR = 0xAAAA |
| `is_on(s)` | util.rs:36 | `FUN_0800A084` (0x0800A084) | "on"/"1"/"true" test |
| `atoi(s)` | util.rs:58 | None | Decimal parser (wraps on overflow) |
| `parse_args(args, vals)` | util.rs:89 | None | Parse space/tab/comma-separated ints |

---

## 5. Protocol (`src/proto/`)

### proxy.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `op_word(op)` | proxy.rs:44 | `FUN_0800a160` | Op code → word ("???"/"get"/"set"/"call") |
| `log_word(level)` | proxy.rs:71 | `FUN_0800a2c0` | Log level → word |
| `register(iface)` | proxy.rs:108 | None | Register interface in table (backwards search) |
| `resp_value(op, iface, opname, value)` | proxy.rs:136 | `FUN_0800A16C` | Value reply: `"<word> <iface> <op> [value]\n"` |
| `resp_value_opt(op, iface, opname, value)` | proxy.rs:145 | `FUN_0800A16C` | Value reply with optional value (NULL → no separator) |
| `resp_message(level, text, extra)` | proxy.rs:158 | `FUN_0800A2D0` | System message: `"call :proxy message <level> <text>[extra]\n"` |
| `resp_greeting(kv, msg)` | proxy.rs:184 | `FUN_0800A1C8` | Greeting: `"call :proxy greeting "k":"v" ...\n"` |
| `dispatch(cmd)` | proxy.rs:273 | `FUN_0800A0AC`+`FUN_0800A350` | Parse + dispatch request |

### fmt.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `num(radix, v)` | fmt.rs:90 | `FUN_080068D4` | Unsigned itoa, variable radix |
| `num_s(radix, v)` | fmt.rs:100 | `FUN_0800693C` | Signed itoa, variable radix |
| `int(v)` | fmt.rs:114 | None | Decimal itoa |
| `fixed(v, frac)` | fmt.rs:128 | None | Fixed-point decimal itoa |

---

## 6. Modules (`src/modules/`)

### hwi_adc.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_adc.rs:49 | `FUN_08007068` | ADC1 hardware init |
| `read(channel)` | hwi_adc.rs:64 | None | Read ADC sample from DMA buffer |
| `read_mv(channel)` | hwi_adc.rs:76 | None | Convert sample to mV |
| `handler(op, opname, args)` | hwi_adc.rs:85 | `FUN_080073DC` | hwiAdc handler - "buf" op |
| `tick()` | hwi_adc.rs:246 | `FUN_080071C0` | ADC overrun flag service |

### hwi_led.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_led.rs:144 | `FUN_080079A0` | LED ARGB bring-up (TIM15, DMA1 ch5, PB14 AF1) |
| `set(led, on)` | hwi_led.rs:264 | None | Set LED on/off |
| `set_pwm(led, duty)` | hwi_led.rs:279 | None | Set LED PWM (0..255) |
| `set_rgb(led, r, g, b)` | hwi_led.rs:293 | None | Set LED RGB (GRB order) |
| `get(led)` | hwi_led.rs:336 | None | Get LED duty |
| `pattern_request()` | hwi_led.rs:317 | None | Print "PPRT\n" |
| `pattern_done()` | hwi_led.rs:322 | None | Print "PPRET\n" |

### hwi_drive.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_drive.rs:104 | `FUN_080087D8` | Motor drive init (TIM3, GPIOC PWM) |
| `set_duty(motor, duty)` | hwi_drive.rs:120 | `FUN_080089D0/8A50` | Set motor duty |
| `get_fault(motor)` | hwi_drive.rs:129 | `FUN_08008AD0` | Get motor fault |
| `get_duty(motor)` | hwi_drive.rs:139 | None | Get motor duty |
| `set_max_duty(motor, percent)` | hwi_drive.rs:150 | `FUN_08008BD4` | Set max duty percent |
| `set_fault(motor, value)` | hwi_drive.rs:168 | `FUN_08008AD0` | Set/clear motor fault |
| `handler(op, opname, args)` | hwi_drive.rs:192 | `FUN_08008B3C` | hwiDrive handler (dutyA/dutyB/nFault) |
| `tick()` | hwi_drive.rs:88 | `FUN_08008784` | Motor fault poll (PA12) |

### drv_battery.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | drv_battery.rs:88 | None | drvBattery init |
| `hw_init()` | drv_battery.rs:98 | `FUN_08006E94` | BMS query start |
| `voltage_mv()` | drv_battery.rs:107 | None | Battery voltage |
| `current_ma()` | drv_battery.rs:112 | None | Battery current |
| `remaining_pct()` | drv_battery.rs:117 | None | Battery remaining % |
| `charge_state()` | drv_battery.rs:122 | None | Charge state (PF6 probe) |
| `temp_c10()` | drv_battery.rs:127 | None | Battery temp |
| `handler(op, opname, args)` | drv_battery.rs:132 | `FUN_08006F2C` | drvBattery handler |

### hwi_sonar.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_sonar.rs:58 | None | Sonar init |
| `busy()` | hwi_sonar.rs:77 | `FUN_08009094` | Sonar measurement running flag |
| `echo_us()` | hwi_sonar.rs:83 | None | Get echo duration |
| `handler(op, opname, args)` | hwi_sonar.rs:90 | None | hwiSonar handler stub |

### ble_com.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | ble_com.rs:249 | `FUN_08007F7C` | BLE USART + console USART init |
| `write(buf)` | ble_com.rs:359 | None | Write to BLE link |
| `read(buf)` | ble_com.rs:394 | None | Read from console link |
| `tx_pending()` | ble_com.rs:441 | None | BLE TX pending count |
| `poll()` | ble_com.rs:446 | None | Poll BLE + console USART |
| `out_str(s)` | ble_com.rs:454 | `FUN_08009CCC` | Output string (all robot output goes here) |
| `out_char(c)` | ble_com.rs:465 | `FUN_08009CA0` | Output char |
| `handler(op, opname, args)` | ble_com.rs:487 | `FUN_0800837C` | bleCom handler stub |

### hwi_buzz.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_buzz.rs:66 | `FUN_08008380` | Buzzer init (TIM16, PSC=2) |
| `set_frequency(freq)` | hwi_buzz.rs:99 | None | Set buzzer frequency |
| `set_duty(duty)` | hwi_buzz.rs:113 | None | Set buzzer duty |
| `play(pattern)` | hwi_buzz.rs:129 | None | Play tone pattern |
| `handler(op, opname, args)` | hwi_buzz.rs:158 | `FUN_08008490` | hwiBuzz handler stub |
| `tick()` | hwi_buzz.rs:83 | `FUN_08008574` | Buzzer tone sequencer (periodic) |

### hwi_enc.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_enc.rs:49 | None | Encoder init (TIM1/TIM2, GPIOA pins 0/8 AF2) |
| `count()` | hwi_enc.rs:67 | None | Get encoder count |
| `reset()` | hwi_enc.rs:95 | None | Reset encoder |
| `enc_update_isr()` | hwi_enc.rs:208 | None | Encoder update ISR |
| `TIM1_BRK_UP_TRG_COM_IRQHandler()` | hwi_enc.rs:116 | `FUN_08008BF8` | TIM1 CC IRQ (IRQ 13) |
| `TIM2_IRQHandler()` | hwi_enc.rs:123 | `FUN_08008C20` | TIM2 CC IRQ (IRQ 15) |
| `handler(op, opname, args)` | hwi_enc.rs:110 | `FUN_08008DFc` | hwiEnc handler stub |

### hwi_line.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_line.rs:40 | None | Line sensor init |
| `read()` | hwi_line.rs:65 | None | Read line sensor |
| `mask()` | hwi_line.rs:95 | None | Line sensor mask (one bit per sensor) |
| `handler(op, opname, args)` | hwi_line.rs:119 | `FUN_08009478` | hwiLine handler stub |
| `tick()` | hwi_line.rs:75 | `FUN_080094F8` | Line sensor sample (periodic) |

### hwi_mic.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_mic.rs:26 | None | Mic init |
| `level()` | hwi_mic.rs:39 | None | Mic level (tracking filter) |
| `handler(op, opname, args)` | hwi_mic.rs:70 | `FUN_0800962C` | hwiMic handler stub |

### hwi_power.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `power_hold_on()` | hwi_power.rs:37 | `FUN_080098C4` | Set PC12 HIGH (power hold latch) |
| `init()` | hwi_power.rs:59 | None | hwiPower init (PC12/13/14/15 setup, EXTI13) |
| `off()` | hwi_power.rs:171 | `FUN_080097E8` | Power off (clear PC12, spin with watchdog) |
| `vsens()` | hwi_power.rs:179 | None | Read Vsens (PC15) |
| `handler(op, opname, args)` | hwi_power.rs:197 | `FUN_0800981C` | hwiPower handler (pwrOff/Vsens) |
| `EXTI4_15_IRQHandler()` | hwi_power.rs:142 | `FUN_0800976C` | Power-good IRQ (IRQ 7), edge-switching |

### hwi_lightsense.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | hwi_lightsense.rs:36 | None | Light sense init |
| `read()` | hwi_lightsense.rs:48 | None | Light sense read |
| `handler(op, opname, args)` | hwi_lightsense.rs:74 | `FUN_08009340` | hwiLightSense handler stub |

### app_lights.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | app_lights.rs:98 | None | appLights init |
| `tick()` | app_lights.rs:62 | `FUN_08006000` | Light program step (periodic) |
| `handler(op, opname, args)` | app_lights.rs:56 | `FUN_08005F00` | appLights handler stub |

### app_drive.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | app_drive.rs:79 | None | appDrive init |
| `tick()` | app_drive.rs:46 | `FUN_08003330` | Drive speed loop (periodic) |
| `handler(op, opname, args)` | app_drive.rs:40 | `FUN_08003849` | appDrive handler stub |

### app_sys.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `hw_init()` | app_sys.rs:58 | `FUN_08006F2C` | drvBattery hwInit (BMS query start) |
| `init()` | app_sys.rs:65 | `FUN_080064F0` | appSys init |
| `handler(op, opname, args)` | app_sys.rs:30 | `FUN_080064F1` | appSys handler (version strings) |

### app_lightsense.rs

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `init()` | app_lightsense.rs:63 | None | appLightSense init |
| `tick()` | app_lightsense.rs:44 | `FUN_08009340` | Light sense sample (periodic) |
| `handler(op, opname, args)` | app_lightsense.rs:36 | None | appLightSense handler stub |

---

## 7. Boot (`src/boot.rs`)

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `main_mark_reset_cause()` | boot.rs:34 | None | Write magic words to RAM |
| `power_fail_mode()` | boot.rs:42 | `FUN_0800ab88` | Clock back to HSI, GPIOC setup, PC14 high |
| `read_line()` | boot.rs:111 | `FUN_08009B14` | Read line from command link (40 byte buffer) |
| `run()` | boot.rs:145 | `FUN_08009FA4` | **main()** - boot sequence + main loop |

**NOTE**: `boot::run()` is MISSING the bootloader entry state machine:
- No PA12/USB VBUS detection
- No button press detection (PC13)
- No bootloader state machine (0→2→3)
- No magic word writing for bootloader entry
- No LED animation state integration

---

## 8. MMIO (`src/mmio.rs`)

| Function | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `r32(addr)` | mmio.rs:12 | None | Read 32-bit register (volatile/simulated) |
| `w32(addr, val)` | mmio.rs:25 | None | Write 32-bit register |
| `r16(addr)` | mmio.rs:38 | None | Read 16-bit register |
| `w16(addr, val)` | mmio.rs:51 | None | Write 16-bit register |
| `r8(addr)` | mmio.rs:64 | None | Read 8-bit register |
| `w8(addr, val)` | mmio.rs:77 | None | Write 8-bit register |
| `rmw32(addr, f)` | mmio.rs:90 | None | Read-modify-write 32-bit |
| `rmw16(addr, f)` | mmio.rs:97 | None | Read-modify-write 16-bit |
| `rmw8(addr, f)` | mmio.rs:104 | None | Read-modify-write 8-bit |

---

## 9. State (`src/state.rs`)

| Item | File:Line | Dump Equiv | Description |
|---|---|---|---|
| `Global<T>` | state.rs:11 | None | Global cell with unsafe Sync |
| `Global::new()` | state.rs:22 | None | Create global cell |
| `Global::get()` | state.rs:28 | None | Get mutable reference |
