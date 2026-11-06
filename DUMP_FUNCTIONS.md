# DUMP_FUNCTIONS.md — Complete function reference from `stm32_flash.bin`

All 378 functions recovered by Ghidra decompilation of the original firmware
dump. Addresses, sizes, and behavior are taken from the individual decompiled
`.c` files in `analysis/decomp/` and `analysis/WORKLIST.txt`.

## Function Groups

The WORKLIST.txt groups functions into three categories:

| Group | Function count | Description |
|---|---|---|
| `drvBattery` | 27 | Battery monitor (voltage, current, remainingCapacity, chargeStat, hwInit) |
| `hwiDrive` | 10 | Motor drive (dutyA, dutyB, nFault, max duty, fault flags) |
| `?` | 334 | Everything else: startup, clock, protocol, HAL, all other modules |

---

## 1. Startup & System (0x08000000 - 0x08000FFF)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x080000C8 | FUN_080000C8 | 20 | |
| 0x080000DE | FUN_080000DE | 4 | |
| 0x080000E2 | FUN_080000E2 | 2 | |
| 0x080000F0 | FUN_080000F0 | 32 | |
| 0x08000128 | FUN_08000128 | 24 | drvBattery - ADC reading helper |
| 0x0800012c | FUN_0800012c | 106 | drvBattery - battery value conversion |
| 0x080001b0 | FUN_080001b0 | 4 | drvBattery |
| 0x080001b4 | FUN_080001B4 | 14 | |
| 0x080001c4 | FUN_080001c4 | 132 | drvBattery - format battery reply |
| 0x080001c8 | FUN_080001C8 | 132 | |
| 0x08000270 | FUN_08000270 | 4 | |
| 0x08000274 | FUN_08000274 | 14 | |
| 0x08000290 | FUN_08000290 | 16 | |
| 0x080002A0 | FUN_080002A0 | 12 | |
| 0x080002AC | FUN_080002AC | 18 | |
| 0x080002C0 | FUN_080002C0 | 18 | |
| 0x080002D4 | FUN_080002D4 | 18 | |
| 0x080002E8 | FUN_080002E8 | 18 | |
| 0x080002fc | FUN_080002fc | 38 | |
| 0x08000328 | FUN_08000328 | 18 | |
| 0x0800033c | FUN_0800033c | 4 | hwiDrive - small helper |
| 0x08000340 | FUN_08000340 | 64 | hwiDrive - motor duty clamp |
| 0x08000380 | FUN_08000380 | 2 | |
| 0x08000382 | FUN_08000382 | 44 | |
| 0x080003b0 | FUN_080003b0 | 2 | |
| 0x080003B2 | FUN_080003B2 | 60 | |
| 0x08000400 | FUN_08000400 | 58 | GPIO read helper |
| 0x0800043C | FUN_0800043C | 54 | GPIO read helper |
| 0x08000474 | FUN_08000474 | 922 | Large function - string/format processing |
| 0x08000818 | FUN_08000818 | 614 | Large function - BLE/protocol processing |
| 0x08000a8c | FUN_08000a8c | 4 | |
| 0x08000A90 | FUN_08000A90 | 76 | |
| 0x08000ADC | FUN_08000ADC | 144 | |
| 0x08000B6C | FUN_08000B6C | 144 | |
| 0x08000BFC | FUN_08000BFC | 596 | Large function - memory copy/fill |
| 0x08000E5C | FUN_08000E5C | 932 | Large function - ADC/DMA processing |

---

## 2. Clock, Reset, and Watchdog (0x08002900 - 0x08002AFF)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08002904 | Reset_Handler | 56 | Set up C runtime, copy .data, zero .bss, call system_init, call main |
| 0x08002954 | FUN_08002954 | 2 | Default_Handler - endless loop |
| 0x08002958 | FUN_08002958 | 232 | **system_init** - clock tree: HSI16 → PLL×6 = 96 MHz, APB=96 MHz |
| 0x08002A60 | FUN_08002A60 | 88 | |
| 0x08002ac0 | FUN_08002ac0 | 2 | BLE AT formatter entry |
| 0x08002AC2 | FUN_08002AC2 | 274 | BLE AT formatter - nibble switch serialization |
| 0x08002DB8 | FUN_08002DB8 | 16 | IRQ 7 handler entry |
| 0x08002DC8 | FUN_08002DC8 | 12 | IRQ 29 handler entry |
| 0x08002DD4 | FUN_08002DD4 | 60 | Delay / SysTick-based wait |
| 0x08002E1C | FUN_08002E1C | 40 | Ring buffer - init |
| 0x08002E44 | FUN_08002E44 | 50 | Ring buffer - put byte |
| 0x08002E78 | FUN_08002E78 | 192 | Ring buffer - read bytes |
| 0x08002f38 | FUN_08002f38 | 16 | Ring buffer - flush |
| 0x08002F48 | FUN_08002F48 | 226 | Ring buffer / ADC helper |

---

## 3. Number Formatting (0x080068D4 - 0x0800693C)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x080068D4 | FUN_080068D4 | 104 | **Unsigned itoa** - variable radix (2-36), digits ≥10 use 'W'+digit → 'a'..'z' |
| 0x0800693C | FUN_0800693C | 126 | **Signed itoa** - variable radix, wraps on overflow |

---

## 4. Protocol Layer (0x0800A0AC - 0x0800A350)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x0800A0AC | FUN_0800A0AC | 138 | **Request parser** - tokenize `<get\|set\|call> <iface> <op> [args]` |
| 0x0800A160 | FUN_0800a160 | 8 | Op word table: "???","get","set","call" |
| 0x0800A16C | FUN_0800A16C | 84 | **Value reply** - `"<word> <iface> <op> [value]\n"` |
| 0x0800A1C8 | FUN_0800A1C8 | 226 | **Greeting** - `"call :proxy greeting "k":"v" ...\n"` |
| 0x0800A2C0 | FUN_0800a2c0 | 10 | Log level table: "???","inf","dbg","wrn","err" |
| 0x0800A2D0 | FUN_0800A2D0 | 110 | **System message** - `"call :proxy message <level> <text>[extra]\n"` |
| 0x0800A350 | FUN_0800A350 | 218 | **Interface dispatcher** - searches registry backwards, calls handler |

---

## 5. System Initialization & Watchdog (0x0800A43C - 0x0800A6FF)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x0800A43C | FUN_0800A43C | 32 | **TIM6 update interrupt** (IRQ 17) - increments dump_tick counter |
| 0x0800A464 | FUN_0800A464 | 84 | **TIM6 init** - PSC=48, ARR=0xFFFF, ~65.5ms period |
| 0x0800A4C0 | FUN_0800A4C0 | 24 | |
| 0x0800A4DC | FUN_0800A4DC | 28 | Copy 64-bit value |
| 0x0800A500 | FUN_0800A500 | 74 | Timer compare / capture read |
| 0x0800A558 | FUN_0800A558 | 90 | Read timer counter |
| 0x0800A5C8 | FUN_0800A5C8 | 40 | |
| 0x0800A5F0 | FUN_0800A5F0 | 62 | GPIO bank enable for watchdog |
| 0x0800A640 | FUN_0800A640 | 50 | **Watchdog init** - IWDG: PR=0x5555, RLR=2, WLR=0xFFE, KR=0xAAAA, KR=0xCCCC |
| 0x0800A67C | FUN_0800A67C | 139 | |
| 0x0800A6A0 | FUN_0800A6A0 | 38 | ADC CFGR1 config: `(v & mask) \| set \| (exten << 13)` |
| 0x0800a6cc | FUN_0800a6cc | 16 | |
| 0x0800A6DC | FUN_0800A6DC | 26 | ADC CR: bit 0 = ADEN, bit 1 = ADDIS |
| 0x0800a6f8 | FUN_0800a6f8 | 10 | ADC channel select + sample time |
| 0x0800A704 | FUN_0800A704 | 74 | **ADC start** - bounded wait (counter 0xEFFF, returns 0 on timeout) |
| 0x0800a754 | FUN_0800a754 | 10 | ADC CR bit 2 (stop) |
| 0x0800A760 | FUN_0800A760 | 26 | ADC DMA mode bit |
| 0x0800a77c | FUN_0800a77c | 236 | ADC CFGR1 DMA bits |
| 0x0800A78C | FUN_0800A78C | 214 | ADC status flag test |
| 0x0800a7ac | FUN_0800a7ac | 99 | ADC ISR flag clear |
| 0x0800A7B0 | FUN_0800A7B0 | 687 | DMA channel reset + clear flags |
| 0x0800A840 | FUN_0800A840 | 299 | DMA channel config (CCR, CNDTR, CPAR, CMAR) |
| 0x0800A87C | FUN_0800A87C | 166 | DMA channel enable |
| 0x0800a89c | FUN_0800A89C | 111 | DMA CNDTR set |
| 0x0800A8A0 | FUN_0800A8A0 | 187 | DMA direction bit |
| 0x0800a8b8 | FUN_0800a8b8 | 165 | DMA flag test |
| 0x0800a8c8 | FUN_0800a8c8 | 167 | DMA interrupt clear |
| 0x0800A8D4 | FUN_0800A8D4 | 923 | EXTI arming helper (IMR/RTSR/FTSR) |
| 0x0800a948 | FUN_0800a948 | 165 | EXTI pending flag test |
| 0x0800a958 | FUN_0800a958 | 167 | EXTI pending flag clear |
| 0x0800A964 | FUN_0800A964 | 1793 | **GPIO config** - two flavours: hwi_a_hal (write once) and hwi_b_hal (per-pin) |
| 0x0800aaa4 | FUN_0800aaa4 | 238 | |
| 0x0800aabc | FUN_0800aabc | 10 | GPIO read (IDR offset 0x10, halfword) |
| 0x0800AAC8 | FUN_0800AAC8 | 34 | **GPIO set AFR** - 4-bit field, value not masked |
| 0x0800aaec | FUN_0800AAEC | 167 | IWDG PR write (0x5555 → base+0x00) |
| 0x0800aaf8 | FUN_0800aaf8 | 167 | IWDG RLR write (2 → base+0x04) |
| 0x0800ab04 | FUN_0800ab04 | 167 | IWDG WLR write (0xFFE → base+0x08) |
| 0x0800ab10 | FUN_0800ab10 | 8 | **IWDG kick** - KR = 0xAAAA |
| 0x0800ab20 | FUN_0800ab20 | 152 | **IWDG enable** - KR = 0xCCCC |
| 0x0800AB30 | FUN_0800AB30 | 505 | **NVIC enable + priority** - IPR word RMW, byte = prio<<6 |
| 0x0800ab88 | FUN_0800ab88 | 380 | **Power fail mode** - clock back to HSI, GPIOC setup, PC14 high |
| 0x0800ABE8 | FUN_0800ABE8 | 259 | GPIO bank release |
| 0x0800ac0c | FUN_0800ac0c | 263 | |
| 0x0800AC40 | FUN_0800AC40 | 1647 | Clock tree helper - returns 96 MHz |
| 0x0800AD5C | FUN_0800AD5C | 276 | RCC AHBENR enable |
| 0x0800AD7C | FUN_0800AD7C | 276 | RCC APB2ENR enable |
| 0x0800AD9C | FUN_0800AD9C | 276 | RCC APB1ENR enable |
| 0x0800ADBC | FUN_0800ADBC | 276 | RCC APB1ENR enable (duplicate) |
| 0x0800ADDC | FUN_0800ADDC | 379 | Reusable flag test (RCC+0/0x20/0x24/0x34, bit select) |
| 0x0800AE14 | FUN_0800AE14 | 276 | |
| 0x0800AE34 | FUN_0800AE34 | 337 | **EXTICR offset compute** - `((line>>2)+2)<<2`, field `(line&3)<<2` |
| 0x0800AE5C | FUN_0800AE5C | 343 | |
| 0x0800AE88 | FUN_0800AE88 | 355 | |
| 0x0800AEC4 | FUN_0800AEC4 | 911 | **Timer base config** - CR1 → ARR → PSC, TIM6 special case |
| 0x0800af4c | FUN_0800af4c | 178 | Timer prescaler + EGR |
| 0x0800af58 | FUN_0800AF58 | 168 | Timer ARPE |
| 0x0800af74 | FUN_0800AF74 | 171 | Timer ARPE off |
| 0x0800AFDC | FUN_0800AFDC | 820 | **Timer channel cfg ch1** - CCMR low byte mask 0x0073 |
| 0x0800AFBC | FUN_0800AFBC | 245 | Timer BDTR MOE |
| 0x0800AFA0 | FUN_0800AFA0 | 168 | Timer CR1 CEN |
| 0x0800b1fc | FUN_0800b1fc | 145 | Timer OCPE ch1 |
| 0x0800b208 | FUN_0800b208 | 149 | Timer OCPE ch2 |
| 0x0800b220 | FUN_0800b220 | 145 | Timer OCPE ch3 |
| 0x0800b22c | FUN_0800b22c | 149 | Timer OCPE ch4 |
| 0x0800B244 | FUN_0800B244 | 272 | Timer CCER channel enable |
| 0x0800b260 | FUN_0800b260 | 186 | |
| 0x0800b270 | FUN_0800b270 | 206 | |
| 0x0800b280 | FUN_0800b280 | 210 | |
| 0x0800b298 | FUN_0800b298 | 206 | |
| 0x0800b2a8 | FUN_0800b2a8 | 210 | |
| 0x0800B2C0 | FUN_0800B2C0 | 1301 | Large function |
| 0x0800B38C | FUN_0800B38C | 259 | Timer DIER |
| 0x0800b3a4 | FUN_0800b3a4 | 114 | Timer EGR |
| 0x0800B3A8 | FUN_0800B3A8 | 160 | |
| 0x0800b3c0 | FUN_0800b3c0 | 139 | Timer SR clear (inverted mask) |
| 0x0800B3C8 | FUN_0800B3C8 | 259 | Timer DIER ch enable |
| 0x0800b3e0 | FUN_0800b3e0 | 139 | |
| 0x0800b3ec | FUN_0800b3ec | 194 | |
| 0x0800B3FC | FUN_0800B3FC | 1063 | **USART init** - CR1/CR2/CR3 config, baud divisor with rounding |
| 0x0800B4C0 | FUN_0800B4C0 | 170 | USART CR1 UE |
| 0x0800b4dc | FUN_0800b4dc | 114 | USART putc (TDR write) |
| 0x0800b4e4 | FUN_0800b4e4 | 88 | USART getc (RDR read) |
| 0x0800B4EC | FUN_0800B4EC | 389 | USART enable with IRQ bits |
| 0x0800B524 | FUN_0800B524 | 263 | |
| 0x0800b53c | FUN_0800b53c | 106 | USART tx_ready (ISR bit 7) |
| 0x0800b548 | FUN_0800b548 | 114 | USART status |
| 0x0800B54C | FUN_0800B54C | 448 | USART rx_ready (ISR bit 5) |
| 0x0800B58C | FUN_0800B58C | 1073 | |
| 0x0800B650 | FUN_0800B650 | 248 | |
| 0x0800B6B0 | FUN_0800B6B0 | 378 | __libc_init_array |
| 0x0800b740 | FUN_0800b740 | 276 | strcmp |
| 0x0800b754 | FUN_0800b754 | 194 | strlen |
| 0x0800B764 | FUN_0800B764 | 261 | |
| 0x0800B778 | FUN_0800B778 | 1262 | **strtok** - space delimiter tokenizer |
| 0x0800B7E0 | FUN_0800B7E0 | 100 | |
| 0x0800B7F8 | FUN_0800B7F8 | 153 | |
| 0x0800B808 | FUN_0800B808 | 43 | |
| 0x0800B814 | FUN_0800B814 | 43 | |
| 0x0800bfd4 | FUN_0800bfd4 | 718 | |

---

## 6. Module: hwiAdc (0x08007068 - 0x080073DC)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08007068 | FUN_08007068 | 234 | ADC1 hardware init |
| 0x08007168 | FUN_08007168 | 74 | ADC1 start continuous scan |
| 0x080071C0 | FUN_080071C0 | 24 | ADC overrun flag service (periodic) |
| 0x080071DC | FUN_080071DC | 508 | ADC channel average (8 samples, interleaved) |
| 0x080073DC | FUN_080073DC | 116 | **hwiAdc handler** - "get hwiAdc buf" → 32 samples, empty line after 9th/17th/25th |

---

## 7. Module: hwiLed (0x080079A0 - 0x08007B34)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x080079A0 | FUN_080079A0 | 330 | **LED bring-up** - GPIOB, PB14 AF1, TIM15 PSC=29 ARR=0, DMA1 ch5 |
| 0x08007B00 | FUN_08007B00 | 44 | |
| 0x08007B34 | FUN_08007B34 | 392 | **LED DMA transfer** - ARGB frame streaming via TIM15_CCR1 DMA |

---

## 8. Module: hwiDrive (0x080087D8 - 0x08008BD4)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x080087D8 | FUN_080087D8 | 298 | **Motor drive init** - TIM3, PSC=0, ARR=1000, GPIOC PWM |
| 0x08008910 | FUN_08008910 | 98 | Motor drive start |
| 0x08008980 | FUN_08008980 | 36 | Motor drive stop (unused) |
| 0x080089B0 | FUN_080089B0 | 26 | |
| 0x080089D0 | FUN_080089D0 | 114 | Motor A duty set |
| 0x08008A50 | FUN_08008A50 | 114 | Motor B duty set |
| 0x08008AD0 | FUN_08008AD0 | 102 | Motor fault set/clear |
| 0x08008B3C | FUN_08008B3C | 130 | **hwiDrive handler** - dutyA/dutyB (hex), nFault (always "0") |
| 0x08008BD4 | FUN_08008BD4 | 30 | Max duty set |

---

## 9. Module: drvBattery (0x08006F2C - 0x08006E94)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08006F2C | FUN_08006F2C | 268 | **drvBattery handler** - voltage/current/remainingCapacity/chargeStat/temp/hwInit |
| 0x08006F0C | FUN_08006F0C | 28 | Battery callback drain |
| 0x08006E94 | FUN_08006E94 | 104 | **BMS query start** - 8-byte frame {0xcc,0x4e,1,0,0,0,0,parity} |
| 0x08006E24 | FUN_08006E24 | 50 | |
| 0x08006D34 | FUN_08006D34 | 228 | **BMS state machine** - steps 1..7, parses 5 reply bytes into voltage/current/remaining |
| 0x08006D18 | FUN_08006D18 | 28 | Frame builder |
| 0x08006CFC | FUN_08006CFC | 28 | |
| 0x08006CDC | FUN_08006CDC | 213 | 3-byte CC/BE frame builder |
| 0x08006CA0 | FUN_08006CA0 | 429 | |
| 0x08006C80 | FUN_08006C80 | 262 | |
| 0x08006C60 | FUN_08006C60 | 213 | |
| 0x08006C44 | FUN_08006C44 | 201 | BLE link idle test |
| 0x08006c40 | FUN_08006C40 | 88 | |
| 0x08006AB4 | FUN_08006AB4 | 2248 | Large function |
| 0x08006A84 | FUN_08006A84 | 403 | |

---

## 10. Module: hwiSonar (0x08008F14 - 0x080090A4)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08008F14 | FUN_08008F14 | 310 | Sonar hardware init - TIM17, GPIOB pin 9 AF2, IRQ 22/9 |
| 0x08009060 | FUN_08009060 | 42 | |
| 0x08009094 | FUN_08009094 | 146 | **Sonar busy** - measurement running flag |
| 0x080090A4 | FUN_080090A4 | 4 | |

---

## 11. Module: bleCom (0x08007F7C - 0x08008380)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08007F7C | FUN_08007F7C | 210 | BLE USART init - USART3, 14400, 2 stop bits |
| 0x0800806C | FUN_0800806C | 237 | BLE USART enable with IRQ |
| 0x080080AC | FUN_080080AC | 1020 | |
| 0x08008130 | FUN_08008130 | 556 | **BLE AT re-send** - 2000ms, GAP command via FUN_08002AC0 |
| 0x0800837C | FUN_0800837C | 4 | **bleCom handler** - stub |
| 0x08008380 | FUN_08008380 | 174 | **Buzzer init** - TIM16, PSC=2 (NOT 96!), ARR=24000 |

---

## 12. Module: hwiBuzz (0x08008440 - 0x08008574)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08008440 | FUN_08008440 | 70 | Buzzer tone set |
| 0x08008490 | FUN_08008490 | 10 | **hwiBuzz handler** - stub |
| 0x0800849C | FUN_0800849C | 392 | Tone frequency → ARR computation (SystemCoreClock/2) |
| 0x080084C0 | FUN_080084C0 | 759 | |
| 0x08008514 | FUN_08008514 | 313 | |
| 0x08008538 | FUN_08008538 | 320 | |
| 0x08008574 | FUN_08008574 | 230 | **Buzzer tone sequencer** (periodic) |

---

## 13. Module: hwiEnc (0x08008C20 - 0x08008DFc)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08008C20 | FUN_08008C20 | 36 | **TIM2 IRQ handler** (IRQ 15) |
| 0x08008C48 | FUN_08008C48 | 352 | Encoder hardware init - TIM1/TIM2, GPIOA pins 0/8 AF2 |
| 0x08008DBC | FUN_08008DBC | 377 | |
| 0x08008DFC | FUN_08008DFC | 4 | |
| 0x08008E00 | FUN_08008E00 | 540 | |
| 0x08008E70 | FUN_08008E70 | 42 | |
| 0x08008E9C | FUN_08008E9C | 357 | **TIM17 update IRQ handler** (IRQ 22) |
| 0x08008EE8 | FUN_08008EE8 | 40 | **DMA1 ch1 IRQ handler** (IRQ 9) - sonar capture |
| 0x08008DFc | FUN_08008DFc | 4 | **hwiEnc handler** - stub |

---

## 14. Module: hwiMic (0x0800962C - 0x08009654)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x0800962C | FUN_0800962C | 10 | **hwiMic handler** - stub |
| 0x08009638 | FUN_08009638 | 124 | |
| 0x08009654 | FUN_08009654 | 604 | **EXTI13 configuration** - SYSCFG EXTICR4, falling edge, IRQ 7 |

---

## 15. Module: hwiLine (0x0800930C - 0x080094F8)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x0800930C | FUN_0800930C | 117 | |
| 0x08009324 | FUN_08009324 | 117 | |
| 0x08009340 | FUN_08009340 | 94 | **hwiLightSense handler** - stub |
| 0x080093A8 | FUN_080093A8 | 4 | |
| 0x080093AC | FUN_080093AC | 124 | |
| 0x080093C8 | FUN_080093C8 | 546 | |
| 0x0800940C | FUN_0800940C | 241 | |
| 0x0800942c | FUN_0800942C | 188 | |
| 0x08009444 | FUN_08009444 | 401 | |
| 0x08009478 | FUN_08009478 | 10 | **hwiLine handler** - stub |
| 0x08009484 | FUN_08009484 | 376 | |
| 0x080094C4 | FUN_080094C4 | 117 | |
| 0x080094DC | FUN_080094DC | 117 | |
| 0x080094F8 | FUN_080094F8 | 298 | **Line sensor sample** (periodic) |

---

## 16. Module: hwiPower (0x08009758 - 0x08009968)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08009758 | FUN_08009758 | 14 | |
| 0x0800976C | FUN_0800976C | 110 | **EXTI4_15 IRQ handler** (IRQ 7) - power-good edge switching |
| 0x080097E8 | FUN_080097E8 | 46 | **GPIO clear** - offset 0x28 halfword store (NOT BRR 0x1C) |
| 0x0800981C | FUN_0800981C | 70 | **hwiPower handler** - pwrOff / Vsens |
| 0x08009874 | FUN_08009874 | 430 | |
| 0x080098C4 | FUN_080098C4 | 622 | **Power hold on** - PC12 HIGH, EXTI arm |
| 0x08009930 | FUN_08009930 | 167 | |
| 0x0800993c | FUN_0800993c | 167 | |
| 0x08009948 | FUN_08009948 | 152 | |
| 0x08009958 | FUN_08009958 | 152 | |
| 0x08009968 | FUN_08009968 | 933 | Serial error path (IRQ 29) - "BUFOVF!!!!!\n" |

---

## 17. Module: hwiLightSense (0x08009268 - 0x080093C8)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08009268 | FUN_08009268 | 517 | |
| 0x080092D4 | FUN_080092D4 | 239 | |
| 0x0800930C | FUN_0800930C | 117 | |
| 0x08009324 | FUN_08009324 | 117 | |
| 0x08009340 | FUN_08009340 | 696 | Light-sense sample (periodic) |
| 0x080093A8 | FUN_080093A8 | 4 | |
| 0x080093AC | FUN_080093AC | 124 | |
| 0x080093C8 | FUN_080093C8 | 506 | |

---

## 18. Module: appLights / appDrive / appSys / appLightSense (0x08005F00 - 0x080039D8)

| Address | Function | Size | Description |
|---|---|---|---|
| 0x08005F00 | FUN_08005F00 | 4 | **appLights handler** - stub |
| 0x08005F04 | FUN_08005F04 | 697 | LED animation loader (state → function pointer) |
| 0x08005F94 | FUN_08005F94 | 241 | LED animation step queue (max 3) |
| 0x08005FBC | FUN_08005FBC | 544 | **LED animation state setter** - _DAT_2000035e = state |
| 0x08006000 | FUN_08006000 | 210 | **Light program step** (periodic) - drives LED strip |
| 0x080060DC | FUN_080060DC | 384 | LED animation timer / state manager |
| 0x08006114 | FUN_08006114 | 10 | |
| 0x08006124 | FUN_08006124 | 6 | **LED animation state getter** - returns _DAT_2000035e |
| 0x08006130 | FUN_08006130 | 8 | Set _DAT_20000374 = 1 (LED engine flag) |
| 0x0800613c | FUN_0800613c | 22 | |
| 0x08006158 | FUN_08006158 | 541 | LED animation timer callback |
| 0x080061C4 | FUN_080061C4 | 790 | |
| 0x0800626C | FUN_0800626C | 34 | **Button press → bootloader state 2→3** - sets LED state 4 |
| 0x08006294 | FUN_08006294 | 293 | **USB callback** - PA12 HIGH → state=2, LED state=4 |
| 0x080062CC | FUN_080062CC | 10 | |
| 0x080062DC | FUN_080062DC | 14 | |
| 0x080062F0 | FUN_080062F0 | 66 | **Idle / deep sleep** - sets power off state, LED state 2 |
| 0x08006338 | FUN_08006338 | 806 | **Module inits** - power_hold_on first, then rest |
| 0x080063EC | FUN_080063EC | 222 | **Periodic task** - 11 engines + bootloader entry check |
| 0x080064F0 | FUN_080064F0 | 4 | **appSys handler** - stub |
| 0x080064f4 | FUN_080064f4 | 6 | |
| 0x08006500 | FUN_08006500 | 104 | |
| 0x0800656C | FUN_0800656C | 106 | |
| 0x080065DC | FUN_080065DC | 143 | |
| 0x080065F4 | FUN_080065F4 | 179 | |
| 0x08006608 | FUN_08006608 | 12 | "BLE-boot\n" printer |
| 0x0800661C | FUN_0800661C | 134 | |
| 0x0800663C | FUN_0800663C | 165 | "BLE-RSP HELLO\n" printer |
| 0x08006650 | FUN_08006650 | 95 | |
| 0x08006668 | FUN_08006668 | 1879 | Large function |
| 0x080067F4 | FUN_080067F4 | 172 | "BLE-RSP GAP SET MODE: " matcher |
| 0x08006808 | FUN_08006808 | 170 | |
| 0x08006828 | FUN_08006828 | 307 | |
| 0x08006848 | FUN_08006848 | 1014 | |
| 0x080069BC | FUN_080069BC | 169 | |
| 0x080069D4 | FUN_080069D4 | 174 | Hex parser (base 16) for drive duty |
| 0x08006A84 | FUN_08006A84 | 403 | |
| 0x08006AB4 | FUN_08006AB4 | 2248 | Large function |
| 0x08006c40 | FUN_08006c40 | 88 | |
| 0x08006C60 | FUN_08006C60 | 213 | |
| 0x08006C80 | FUN_08006C80 | 222 | |
| 0x08006CA0 | FUN_08006CA0 | 389 | |
| 0x08006CDC | FUN_08006CDC | 213 | |
| 0x08006CFC | FUN_08006CFC | 165 | |
| 0x08006D18 | FUN_08006D18 | 28 | |
| 0x08006D34 | FUN_08006D34 | 1636 | BMS state machine |
| 0x08006E24 | FUN_08006E24 | 337 | |
| 0x08006E5C | FUN_08006E5C | 388 | |
| 0x08006E94 | FUN_08006E94 | 892 | BMS frame builder |
| 0x08006F0C | FUN_08006F0C | 281 | |
| 0x08006F2C | FUN_08006F2C | 1862 | drvBattery handler |
| 0x0800705c | FUN_0800705c | 65 | |
| 0x08007068 | FUN_08007068 | 1520 | ADC init |
| 0x08007168 | FUN_08007168 | 381 | ADC start |
| 0x080071C0 | FUN_080071C0 | 254 | ADC overrun service |
| 0x080071DC | FUN_080071DC | 3381 | ADC average |
| 0x080073DC | FUN_080073DC | 743 | hwiAdc handler |
| 0x0800745C | FUN_0800745C | 3243 | TIM15 update IRQ - LED pattern sequencer state machine |
| 0x08007734 | FUN_08007734 | 409 | DMA1 ch5 IRQ - LED transfer complete |
| 0x080077A0 | FUN_080077A0 | 832 | Single LED refresh (ARR 0x308) |
| 0x08007830 | FUN_08007830 | 1190 | Second LED encoder (LSB first, 0x60/0x04) |
| 0x080078E4 | FUN_080078E4 | 304 | LED DMA transfer gate |
| 0x0800791C | FUN_0800791C | 544 | |
| 0x08007980 | FUN_08007980 | 60 | |
| 0x080079A0 | FUN_080079A0 | 2063 | LED bring-up (PSC=29, ARR=0, PB14 AF1) |
| 0x08007B00 | FUN_08007B00 | 323 | |
| 0x08007B34 | FUN_08007B34 | 1869 | LED DMA transfer (ARR 0x187) |
| 0x08007CE4 | FUN_08007CE4 | 208 | |
| 0x08007CFC | FUN_08007CFC | 731 | GPIOF read (charge state PF6) |
| 0x08007d7c | FUN_08007d7c | 65 | |
| 0x08007D88 | FUN_08007D88 | 1460 | **USART3 serout** - BLE serout routine, writes TDR |
| 0x08007EC8 | FUN_08007EC8 | 1073 | USART error path |
| 0x08007F7C | FUN_08007F7C | 1208 | BLE USART init |
| 0x0800806C | FUN_0800806C | 237 | BLE USART enable + IRQ |
| 0x080080AC | FUN_080080AC | 1020 | |
| 0x08008130 | FUN_08008130 | 2663 | BLE AT re-send (2000ms, GAP command) |
| 0x0800837C | FUN_0800837C | 4 | bleCom handler stub |
| 0x08008380 | FUN_08008380 | 1277 | Buzzer init (PSC=2) |
| 0x08008440 | FUN_08008440 | 279 | Buzzer tone |
| 0x08008490 | FUN_08008490 | 69 | hwiBuzz handler stub |
| 0x0800849C | FUN_0800849C | 392 | Tone freq computation |
| 0x080084C0 | FUN_080084C0 | 759 | |
| 0x08008514 | FUN_08008514 | 313 | |
| 0x08008538 | FUN_08008538 | 320 | |
| 0x08008574 | FUN_08008574 | 1203 | Buzzer tone sequencer (periodic) |
| 0x08008668 | FUN_08008668 | 514 | |
| 0x080086C4 | FUN_080086C4 | 309 | |
| 0x08008700 | FUN_08008700 | 704 | |
| 0x08008774 | FUN_08008774 | 188 | Register PA12 callback |
| 0x08008784 | FUN_08008784 | 511 | **PA12 polling** - USB VBUS detect, edge-triggered |
| 0x080087D8 | FUN_080087D8 | 1692 | Motor drive init |
| 0x08008910 | FUN_08008910 | 457 | Motor drive start |
| 0x08008980 | FUN_08008980 | 245 | Motor drive stop |
| 0x080089B0 | FUN_080089B0 | 202 | |
| 0x080089D0 | FUN_080089D0 | 759 | Motor A duty |
| 0x08008A50 | FUN_08008A50 | 759 | Motor B duty |
| 0x08008AD0 | FUN_08008AD0 | 692 | Motor fault set/clear |
| 0x08008B3C | FUN_08008B3C | 761 | hwiDrive handler |
| 0x08008BD4 | FUN_08008BD4 | 231 | Max duty |
| 0x08008BF8 | FUN_08008BF8 | 370 | TIM1 CC IRQ (IRQ 13) |
| 0x08008C20 | FUN_08008C20 | 370 | TIM2 CC IRQ (IRQ 15) |
| 0x08008C48 | FUN_08008C48 | 1704 | Encoder init |
| 0x08008DBC | FUN_08008DBC | 337 | |
| 0x08008DFC | FUN_08008DFC | 4 | hwiEnc handler stub |
| 0x08008E00 | FUN_08008E00 | 540 | |
| 0x08008E70 | FUN_08008E70 | 367 | |
| 0x08008E9C | FUN_08008E9C | 357 | TIM17 update IRQ (IRQ 22) |
| 0x08008EE8 | FUN_08008EE8 | 197 | DMA1 ch1 IRQ (IRQ 9) |
| 0x08008F14 | FUN_08008F14 | 1923 | Sonar init |
| 0x08009060 | FUN_08009060 | 198 | |
| 0x08009094 | FUN_08009094 | 146 | Sonar busy flag |
| 0x080090A4 | FUN_080090A4 | 4 | hwiSonar handler stub |
| 0x080090A8 | FUN_080090A8 | 3017 | LED frame encoder (MSB first, GRB) |
| 0x08009268 | FUN_08009268 | 517 | |
| 0x080092D4 | FUN_080092D4 | 239 | |
| 0x0800930C | FUN_0800930C | 117 | |
| 0x08009324 | FUN_08009324 | 117 | |
| 0x08009340 | FUN_08009340 | 696 | Light-sense sample |
| 0x080093A8 | FUN_080093A8 | 4 | hwiLightSense handler stub |
| 0x080093AC | FUN_080093AC | 124 | |
| 0x080093C8 | FUN_080093C8 | 506 | |
| 0x0800940C | FUN_0800940C | 241 | |
| 0x0800942c | FUN_0800942C | 188 | |
| 0x08009444 | FUN_08009444 | 401 | |
| 0x08009478 | FUN_08009478 | 10 | hwiLine handler stub |
| 0x08009484 | FUN_08009484 | 376 | |
| 0x080094C4 | FUN_080094C4 | 117 | |
| 0x080094DC | FUN_080094DC | 117 | |
| 0x080094F8 | FUN_080094F8 | 2301 | Line sensor sample (periodic) |
| 0x0800962C | FUN_0800962C | 10 | hwiMic handler stub |
| 0x08009638 | FUN_08009638 | 124 | |
| 0x08009654 | FUN_08009654 | 604 | EXTI13 configuration |
| 0x080096EC | FUN_080096EC | 567 | BLE link service (periodic, 800ms) |
| 0x08009758 | FUN_08009758 | 14 | |
| 0x0800976C | FUN_0800976C | 641 | EXTI4_15 IRQ handler (IRQ 7) |
| 0x080097E8 | FUN_080097E8 | 331 | GPIO clear (offset 0x28) |
| 0x0800981C | FUN_0800981C | 649 | hwiPower handler (pwrOff/Vsens) |
| 0x08009874 | FUN_08009874 | 430 | |
| 0x080098C4 | FUN_080098C4 | 622 | Power hold on |
| 0x08009930 | FUN_08009930 | 167 | |
| 0x0800993c | FUN_0800993c | 167 | |
| 0x08009948 | FUN_08009948 | 152 | |
| 0x08009958 | FUN_08009958 | 152 | |
| 0x08009968 | FUN_08009968 | 933 | Serial error path (IRQ 29) |
| 0x08009a04 | FUN_08009A04 | 850 | Console USART init |
| 0x08009aac | FUN_08009AAC | 204 | |
| 0x08009ae4 | FUN_08009AE4 | 149 | |
| 0x08009B14 | FUN_08009B14 | 2236 | **Command line reader** - USART4, 40 byte buffer, overflow → "Invalid command!" |
| 0x08009CA0 | FUN_08009CA0 | 242 | out_char |
| 0x08009CCC | FUN_08009CCC | 307 | out_str |
| 0x08009CF8 | FUN_08009CF8 | 277 | |
| 0x08009D30 | FUN_08009D30 | 547 | |
| 0x08009D88 | FUN_08009D88 | 739 | |
| 0x08009E0C | FUN_08009E0C | 226 | GPIO clear (offset 0x28) |
| 0x08009E30 | FUN_08009E30 | 622 | |
| 0x08009EA0 | FUN_08009EA0 | 1047 | Line sensor service (periodic, 0x37ms) |
| 0x08009F4C | FUN_08009F4C | 314 | |
| 0x08009F88 | FUN_08009F88 | 4 | |
| 0x08009F8C | FUN_08009F8C | 264 | Power fail tail - set MSP, call ROM at 0x1FFFC804 |
| 0x08009FA4 | FUN_08009FA4 | 1077 | **main()** - magic word check, bootloader entry, main loop |
| 0x0800A084 | FUN_0800A084 | 282 | **is_on()** - "on"/"1"/"true" test |
| 0x0800A0AC | FUN_0800A0AC | 962 | Request parser |
| 0x0800A140 | FUN_0800A140 | 150 | |
| 0x0800A150 | FUN_0800A150 | 221 | Greeting KV pair writer |
| 0x0800a160 | FUN_0800a160 | 85 | Op word table |
| 0x0800A16C | FUN_0800A16C | 495 | Value reply |
| 0x0800A1C8 | FUN_0800A1C8 | 1161 | Greeting |
| 0x0800a2c0 | FUN_0800a2c0 | 85 | Log level table |
| 0x0800A2D0 | FUN_0800A2D0 | 591 | System message |
| 0x0800A350 | FUN_0800A350 | 1652 | Interface dispatcher |
| 0x0800A43C | FUN_0800A43C | 376 | TIM6 update IRQ |
| 0x0800A464 | FUN_0800A464 | 509 | TIM6 init |
| 0x0800A4C0 | FUN_0800A4C0 | 111 | |
| 0x0800A4DC | FUN_0800A4DC | 339 | Copy 64-bit |
| 0x0800A500 | FUN_0800A500 | 750 | Timer compare read |
| 0x0800A558 | FUN_0800A558 | 773 | Timer counter read |
| 0x0800A5C8 | FUN_0800A5C8 | 420 | |
| 0x0800A5F0 | FUN_0800A5F0 | 363 | GPIO bank enable |
| 0x0800A640 | FUN_0800A640 | 261 | Watchdog init |
| 0x0800A67C | FUN_0800A67C | 139 | |
| 0x0800A6A0 | FUN_0800A6A0 | 241 | ADC CFGR1 |
| 0x0800a6cc | FUN_0800a6cc | 182 | |
| 0x0800A6DC | FUN_0800A6DC | 215 | ADC CR |
| 0x0800a6f8 | FUN_0800a6f8 | 194 | ADC channel select |
| 0x0800A704 | FUN_0800A704 | 560 | ADC start (bounded) |
| 0x0800a754 | FUN_0800a754 | 105 | ADC stop |
| 0x0800A760 | FUN_0800A760 | 232 | ADC DMA mode |
| 0x0800a77c | FUN_0800a77c | 196 | ADC DMA bits |
| 0x0800A78C | FUN_0800A78C | 214 | ADC status test |
| 0x0800a7ac | FUN_0800a7ac | 99 | ADC ISR clear |
| 0x0800A7B0 | FUN_0800A7B0 | 687 | DMA channel reset |
| 0x0800A840 | FUN_0800A840 | 299 | DMA channel config |
| 0x0800A87C | FUN_0800A87C | 166 | DMA channel enable |
| 0x0800a89c | FUN_0800a89c | 111 | DMA CNDTR |
| 0x0800A8A0 | FUN_0800A8A0 | 187 | DMA direction |
| 0x0800a8b8 | FUN_0800a8b8 | 165 | DMA flag |
| 0x0800a8c8 | FUN_0800a8c8 | 167 | DMA clear flags |
| 0x0800A8D4 | FUN_0800A8D4 | 923 | EXTI arming |
| 0x0800a948 | FUN_0800a948 | 165 | EXTI pending test |
| 0x0800a958 | FUN_0800a958 | 167 | EXTI pending clear |
| 0x0800A964 | FUN_0800A964 | 1793 | GPIO config |

---

## 19. Remaining Functions (0x08000000 - 0x08003FFF range not covered above)

All functions from FUNCS.txt lines 1-300 not listed above are covered in the tables above. Functions 301-378 are covered in sections 5-9 above.

---

## String Table (from STRINGS.txt)

| Address | String | Usage |
|---|---|---|
| 0x0800c468 | "BLE-boot" | BLE boot announcement |
| 0x0800c474 | "BLE-RSP HELLO" | BLE response matcher |
| 0x0800c484 | "BLE-RSP GAP SET MODE: " | BLE GAP response matcher |
| 0x0800c49c | "voltage" | drvBattery op |
| 0x0800c4a4 | "drvBattery" | Interface name |
| 0x0800c4b8 | "current" | drvBattery op |
| 0x0800c4c0 | "remainingCapacity" | drvBattery op |
| 0x0800c4d4 | "chargeStat" | drvBattery op |
| 0x0800c4e0 | "hwInit" | drvBattery op |
| 0x0800c4ec | "PPRT" | LED pattern start |
| 0x0800c4f4 | "PPRET" | LED pattern end |
| 0x0800c4fc | "ERR:BLE_serout_TO" | BLE TX timeout |
| 0x0800c510 | "BLE framing error" | USART error |
| 0x0800c524 | "BLE USART noise" | USART error |
| 0x0800c534 | "BLE USART overrun" | USART error |
| 0x0800c548 | "dutyA" | hwiDrive op |
| 0x0800c550 | "dutyB" | hwiDrive op |
| 0x0800c558 | "nFault" | hwiDrive op |
| 0x0800c560 | "hwiDrive" | Interface name |
| 0x0800c56c | "pwrOff" | hwiPower op |
| 0x0800c574 | "Vsens" | hwiPower op |
| 0x0800c57c | "BUFOVF!!!!!" | Command buffer overflow |
| 0x0800c594 | ":proxy" | Protocol prefix |
| 0x0800c59c | "USART framing error" | USART error |
| 0x0800c5b0 | "hwiSonar" | Interface name |
| 0x0800c5bc | "possible cmdBuf overflow" | Command buffer overflow |
| 0x0800c5d8 | "hwiAdc" | Interface name |
| 0x0800c5e0 | "hwiLed" | Interface name |
| 0x0800c5e8 | "bleCom" | Interface name |
| 0x0800c5f0 | "hwiBuzz" | Interface name |
| 0x0800c5f8 | "hwiMic" | Interface name |
| 0x0800c600 | "hwiEnc" | Interface name |
| 0x0800c608 | "hwiLine" | Interface name |
| 0x0800c610 | "hwiPower" | Interface name |
| 0x0800c61c | "hwiLightSense" | Interface name |
| 0x0800c62c | "appLights" | Interface name |
| 0x0800c638 | "appDrive" | Interface name |
| 0x0800c644 | "appSys" | Interface name |
| 0x0800c64c | "appLightSense" | Interface name |
| 0x0800c660 | "0.5.0hw2.1" | Firmware version |
| 0x0800c67c | "1602121419" | Build date |
| 0x0800c688 | "ttpms" | Tick period ms |
| 0x0800c6ac | "greeting" | Greeting key |
| 0x0800c6c0 | "message" | Message key |
| 0x0800c6c8 | "Invalid command!" | Protocol error |
| 0x0800c6dc | "No interface:" | Protocol error |

---

## RAM State Variables (from decompilation)

| Address | Size | Description |
|---|---|---|
| 0x20000358 | 2 bytes | LED animation current step |
| 0x2000035c | 2 bytes | LED animation duration |
| 0x2000035e | 2 bytes | **LED animation state** (0=off, 1..N=pattern, 4=red) |
| 0x20000360 | 4 bytes | LED animation function pointer |
| 0x20000374 | 1 byte | LED engine flag |
| 0x2000037e | 1 byte | LED animation queue count |
| 0x200003ac | 4 bytes | LED animation param |
| 0x20000400 | 1 byte | **Power off state** (0=off, 1=pending, 2=active) |
| 0x20000408 | 4 bytes | Power off timer |
| 0x20000410 | 1 byte | **Bootloader state** (0=normal, 2=USB detected, 3=ready for entry) |
| 0x200006f0 | 4 bytes | PA12 callback function pointer |
| 0x200006f4 | 1 byte | PA12 current level |
| 0x200006f8 | 4 bytes | PA12 callback |
| 0x20000fc0 | 1 byte | PA12 last level |
| 0x200008c8 | 4 bytes | Power sense state struct |
| 0x200008c9 | 1 byte | EXTI edge flag |
| 0x200008ca | 1 byte | EXTI edge counter flag |
| 0x200008d0 | 4 bytes | Power sense timestamp |
| 0x200008d8 | 4 bytes | Power sense debounce stamp |
| 0x200008ec | 4 bytes | Power sense callback |
| 0x200008f0 | 4 bytes | Power sense callback 2 |
| 0x20000f24 | 4 bytes | BLE serout function pointer |
| 0x20000fd8 | 4 bytes | BLE link timestamp |
| 0x20000fdc | 4 bytes | BLE link flag |
| 0x20000fe8 | 40 bytes | Command buffer |
| 0x20001018 | 4 bytes | **Dump tick** (TIM6 counter) |
| 0x200025a8 | 4 bytes | **Magic word 0** (0xD7E5F7A8 for bootloader entry) |
| 0x200017a8 | 4 bytes | **Magic word 1** (0x19C3FE40 for bootloader entry) |

---

## Peripheral Register Access Summary

| Peripheral | Base | Registers Used |
|---|---|---|
| GPIOA | 0x48000000 | MODER, OTYPER, OSPEEDR, PUPDR, IDR(0x10), BSRR, clear@0x28 |
| GPIOB | 0x48000400 | MODER, OTYPER, OSPEEDR, PUPDR, IDR, BSRR, AFR0/AFR1 |
| GPIOC | 0x48000800 | MODER, OTYPER, OSPEEDR, PUPDR, IDR, BSRR, clear@0x28 |
| GPIOF | 0x48001400 | IDR (PF6 charge state) |
| TIM1 | 0x40012C00 | Encoder |
| TIM2 | 0x40000000 | Encoder |
| TIM3 | 0x40000400 | Motor PWM |
| TIM6 | 0x40001000 | Millisecond tick (original) |
| TIM15 | 0x40014000 | LED ARGB + DMA ch5 |
| TIM16 | 0x40014400 | Buzzer |
| TIM17 | 0x40014800 | Sonar + DMA ch1 |
| USART3 | 0x40004800 | BLE AT (14400, 2 stop) |
| USART4 | 0x40004C00 | Command+Reply (460800, 2 stop) |
| ADC1 | 0x40012400 | Battery/mic/light |
| DMA1 | 0x40020000 | ch1=sonar, ch2=ADC, ch5=LED |
| RCC | 0x40021000 | Clock enables |
| IWDG | 0x40003000 | Watchdog (PR@0, RLR@4, WLR@8, KR@0) |
| SYSCFG | 0x40010000 | EXTICR (line→port mapping) |
| EXTI | 0x40010400 | RTSR@0x08, FTSR@0x0C, PR@0x14 |
| NVIC | 0xE000E100 | ISER enable |
| NVIC_IPR | 0xE000E400 | Priority (byte = prio<<6) |
| SysTick | 0xE000E010 | Millisecond tick (port) |
