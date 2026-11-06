# Parent-verified findings (from the independent dump cross-check)

Every item below was verified against `stm32_flash.bin` machine code (function
address + instruction address given). Status: `OPEN` = not yet fixed.

## Verified CORRECT (no action)

| what | evidence |
|---|---|
| GPIO bases A/B/C/F | literals in flash; 11 decompiled call sites use 0x48000000 |
| TIM2 = 0x40000000 | `FUN_08008C20` / `FUN_0800AEC4` take it; 0 literal hits but 64 decompiled refs |
| TIM6 = 0x40001000 | 7 literal hits, all in the timer-helper region (0x800A45C..0x800AF38) |
| GPIO register offsets (MODER..AFR1) | match the dump's helpers |
| RCC offsets CR/CFGR/CIR/AHBENR/APB2ENR/APB1ENR (0x00/04/08/14/18/1C) | `FUN_0800AD5C/AD7C/AD9C` write exactly those |
| ADC channel masks [0x02,0x04,0x20,0x40] | `FUN_080071DC`-ish init: `FUN_0800a6f8(0x40012400, DAT_200004b1..b4, 4/6/6/6)`, and the .data copy yields exactly 02/04/20/40 |
| ADC sample times 4,6,6,6 | same four calls |
| ADC CFGR1 mask/clear | `FUN_0800A6A0`: `(v & 0xffffd203) \| set \| exten<<13` |
| ADC CR bits | `FUN_0800A6DC`: bit 0 ADEN, bit 1 ADDIS |
| BLE USART config | `FUN_08007F7C`: cfg baud=0x3840 (14400), CR2=0x2000, CR1=0x000C, CR3=0x0300 - port matches |
| Console USART config | `FUN_08009A04`: baud=0x70800 (460800), CR2=0, CR1=0x000C, CR3=0, PC10/PC11 AF0, mask 0xC00 |
| BLE TX timeout = len*0x528 us | `FUN_08007D88`: `_DAT_20000f88 = (len1+len2) * 0x528` |
| Sonar TIM17 base | `FUN_08008F14`: psc=0, arr=0x3B, GPIOB pin 9 AF2, IRQ22 prio3, IRQ9 prio2 - port matches |
| Encoder TIM1/TIM2 | `FUN_08008C48`: CCMR1 0x21, CCER 0xB, CR2 7, SMCR 0x50, IRQ13/15 prio 2 - port matches |
| Encoder pins | GPIOA pin 0 and 8 AF2, pin 15 is the supply enable |
| Motor drive TIM3 | `FUN_080087D8`: GPIOC 6..9 AF0 (0x3c0), PC2 enable, PC1 input pull-up, TIM3 PSC 0 ARR 1000, `ccer5=0x0008` |
| Interface table order | dump @0x0800D164: hwiAdc, hwiLed, hwiDrive, drvBattery, hwiSonar, bleCom, hwiBuzz, hwiMic, hwiEnc, hwiLine, hwiPower, hwiLightSense, appLights, appDrive, appSys, appLightSense - port matches |
| Command reader overflow | `FUN_08009B14` (0x8009b60): `*param_1 = 0; return param_2;` - port matches |
| Reset magic words | `_DAT_200025a8 == -0x281a0858` (= 0xD7E5F7A8) and `_DAT_200017a8 == 0x19c3fe40` |
| Charge-state probe | GPIOF PF6 pull-up/pull-down 10 ms delays - port's `read_charge_state` matches |
| Light program gate | `FUN_08006000` sets `_DAT_2000035c = 0xa0` (160 ms) when zero - port matches |
| `TIMCLK_HZ = APB1_HZ/2` | `FUN_0800849C`: `ARR = (SystemCoreClock >> 1)/freq - 1` |
| IRQ 9 = TIM17 CC / DMA1 ch1 handler | the port's `DMA1_Channel1_IRQHandler` body equals the dump's 0x08008EE8 |

## OPEN bugs

### FIXED (2026-10-26, agent ThoroughWhitefish) - IWDG register offsets

Dump, base 0x40003000:
```
FUN_0800AAEC (0x800aaec): str r0,[r3,#0]   -> 0x5555 to base+0x00
FUN_0800AAF8 (0x800aaf8): str r0,[r3,#4]   -> 2      to base+0x04
FUN_0800AB04 (0x800ab04): str r0,[r3,#8]   -> 0xffe  to base+0x08
FUN_0800AB10 (0x800ab10): str 0xAAAA,[r3,#0]
FUN_0800AB20 (0x800ab20): str 0xCCCC,[r3,#0]
```
Port writes 0x5555->+0x04, 2->+0x08, 0x0FFE->+0x0C.
Fix: match the dump; the 0x0FFE write is at +0x08, not +0x0C.

### FIXED (2026-10-26, parent) - buzzer TIM16 prescaler

Dump `FUN_08008380` @ 0x8008380, base-config struct built at `sp+20`:
```
0x80083de: strh r7,[r1,#0]  ; psc = 2     (r7 = 2, set at 0x80083aa)
0x80083dc: strh r4,[r1,#2]  ; cr1_hi = 0  (r4 = 0, set at 0x8008392)
0x80083e2: str  0x00005DC0,[r1,#4]  ; arr = 24000   (pool @0x8008438)
0x80083e6: strh r4,[r1,#8]  ; ckd = 0
0x80083e4: strb r6,[r1,#10] ; rcr = 0     (r6 = 0)
0x80083e8: ldr  r6 @0x800843c -> 0x40014400 (TIM16)
```
Port has `BUZZ_PSC = 96` and ARR 0. Fix: PSC 2, base ARR 24000.

### PARTLY FIXED (2026-10-26, agent) - LED engine timer + pins

Dump `FUN_080079A0` @ 0x80079a0. All four GPIO calls target **GPIOB** (pool
@0x8007af0 = 0x48000400):
```
0x80079d6: lsls r0,#128,#7   -> mask 0x8000 (PB15), mode=2, ospeed=3
0x80079ec: lsls r3,#128,#5   -> mask 0x1000 (PB12)  [an earlier reading of the
       trace said PB13 (0x2000); the instruction is 0x80<<5 = 0x1000 = PB12]
0x80079fc: lsls r1,#128,#8   -> mask 0x4000 (PB14), pull=1
0x8007a16: FUN_0800AAC8(0x48000400, 14, 1)      -> PB14 with AF1 (not AF2!)
Timer base config, struct at sp+52, passed to FUN_0800AEC4:
0x8007a24: strh 29,[r1,#0]   ; psc = 29
0x8007a20: strh 0,[r1,#2]    ; cr1_hi = 0
0x8007a26: str  0,[r1,#4]    ; arr = 0
0x8007a2a: strb 0,[r1,#10]   ; rcr = 0
0x8007a2c: strh 0,[r1,#8]    ; ckd = 0
0x8007a2e: ldr @0x8007af4 -> 0x40014000 (TIM15)
NVIC at 0x8007a36: irq 20, prio 3
```
Port has `LED_PSC=96`, `LED_ARR=255`, `LED_PIN=[4,5,1,2]` on
`[GPIOB,GPIOB,GPIOA,GPIOA]`, and arms **no** LED IRQ.
Fix: PSC 29, ARR 0, GPIOB PB14 (AF1)/PB12/PB15, arm IRQ 20 (prio 3) and IRQ 11 (prio 4).
STATUS 2026-10-26: hwi_led.rs now has LED_PSC=29, LED_ARR=0, LED_FULL_DUTY=1,
LED_COUNT=3, ports [GPIOB,GPIOB,GPIOB], pins [14,12,15]. The IRQ arms are not yet there.

### OPEN-4 - the port runs 3 of the dump's 11 periodic engines

Dump `FUN_080063EC` @ 0x80063ec, called from the main loop body, runs in order:

| # | function | what | in the port? |
|---|---|---|---|
| 1 | FUN_080096EC | BLE link service (800 ms) | no |
| 2 | FUN_08008784 | motor fault poll (GPIOA 0x1000) | no |
| 3 | FUN_080071C0 | ADC/DMA poll (`FUN_0800A78C(0x40012400,0x10,...)`) | no |
| 4 | FUN_08009EA0 | line sensor service (0x37 ms; GPIOA BSRR 0x80) | no |
| 5 | FUN_08008130 | BLE AT re-send (2000 ms; `FUN_08002ac0(0x39,2,2)`, `FUN_08002ac0(0x32,1)`) | no |
| 6 | FUN_08009340 | light-sense sample | yes (`app_lightsense::tick`) |
| 7 | FUN_08006000 | light program step | yes (`app_lights::tick`) |
| 8 | FUN_080030D0 | mic sample (500 ms) | no |
| 9 | FUN_080094F8 | line sensor sample | no |
| 10 | FUN_08008574 | buzzer tone sequencer | no |
| 11 | FUN_08003330 | drive speed loop | yes (`app_drive::tick`) |

Port `rust/src/boot.rs` main loop calls only the three `::tick()`.

### OPEN-5 - vector table has 5 missing IRQ handlers

Dump vector table (first 48 words, thumb bit stripped) has real handlers at:

| IRQ | vector | dump handler | what |
|---|---|---|---|
| 7 | 23 | 0x08002DB8 | FUN_0800976C, FUN_08009D30, FUN_080086C4 |
| 9 | 25 | 0x08008EE8 | TIM17 CC / DMA ch1 - port has it (named DMA1_Channel1) |
| 11 | 27 | 0x08007734 | TIM15 + DMA ch5 (LED engine) |
| 13 | 29 | 0x08008BF8 | TIM1 CC - port has it |
| 15 | 31 | 0x08008C20 | TIM2 CC - port has it |
| 17 | 33 | 0x0800A43C | TIM6 update - counter increment, DAT_20001018 |
| 20 | 36 | 0x0800745C | TIM15 update - LED pattern sequencer state machine |
| 22 | 38 | 0x08008E9C | TIM17 update - port has it |
| 29 | 45 | 0x08002DC8 | FUN_08009968, FUN_08007EC8 (USART/line error paths) |

Port `rust/src/startup.rs` defines handlers at IRQ 9, 13, 15, 22 only. The
missing ones fall into `Default_Handler`'s `spin_loop()`.

### OPEN-6 - boot order: the watchdog runs after the module inits

Dump `FUN_08009FA4` @ 0x8009fa4: `FUN_0800A464()` (TIM6 tick) -> `FUN_0800A4C0()` ->
`FUN_08006338()` (module inits) -> greeting -> loop.
`FUN_0800A640()` (the watchdog) is called **before** the inits.
Port calls `system::watchdog_init()` after all 16 module inits. Also the port's
comment claims "before the module inits", which is false.

### OPEN-7 - tick source: the dump uses TIM6, the port uses SysTick

Dump `FUN_0800A464` @ 0x800a464: APB1ENR |= 0x10; TIM6 (0x40001000) with psc=48,
arr=0xFFFF, cr1_hi=0, ckd=0; NVIC irq 17 prio 2; DIER update IE on.
IRQ 17 handler = FUN_0800A43C (counter increment at DAT_20001018).
With psc=48 and arr=0xFFFF on a 48 MHz timer clock the update period is
~65.5 ms, **not** 1 ms - so the dump's TIM6 is not what its greeting's
`"ttpms":"1000"` claims. Note the dump *does* also touch the SysTick registers
(0xE000E010/14/18) in `FUN_08002DD4`, but only as a 1 MHz-derived delay counter
and a "spin forever" exit for the charge-state probe.

### OPEN-8 - `call drvBattery hwInit` does not start the BLE BMS query

Dump: the `drvBattery` handler `FUN_08006F2C` (0x8006f2c), on op 3 + opname
`hwInit`, runs
```
while (FUN_08006C44() != 0) FUN_0800ab10();   // wait for the BLE link to be idle
FUN_08006E94(0x73a);                          // start the BMS state machine
```
`FUN_08006E94` (0x8006e94) builds an 8-byte frame
(`0xcc,0x4e,1,0,0,0,0, FUN_080001c4((0x73a+1000)<<5, 0x271)`), sets
`DAT_200004ab = 1` and the continuation `_DAT_200004ac = 0x8006ca1`, then
queues it on the BLE link (`FUN_080078E4(...,8,0)`).

The state machine `FUN_08006D34` (0x8006d34) steps 1..7 and fills
`_DAT_200004a2` (temp), `_DAT_200004a4` (voltage), `_DAT_200004a6`
(current), `_DAT_200004a8` (remaining) from the 5 reply bytes:
```
a2 = (short)*(char *)(r+2)
a4 = ((bVar1 & 3) * 0x100 + (ushort)*(byte *)(r+3)) * 10      bVar1 = *(byte*)(r+4)
a6 = ((short)((ushort)*(byte *)(r+5) + (ushort)*(byte *)(r+6)*0x100) * 0x271) >> 6
```
The `voltage` op answers from `_DAT_200004a4` (the BMS value), the `current`
op from `_DAT_200004a6`. `FUN_08006F0C` (0x8006f0c) drains the callback each
periodic step. `FUN_08006C60`/`FUN_08006CDC` build the 3-byte CC/BE frames.

Port: `drv_battery::hw_init()` only zeroes four fields and never queues the
frame, so `voltage`/`current`/`remainingCapacity` answer from stale local
state instead of the pack's values. Note this is reachable on the robot only
if the BLE module answers - the robot's BLE link is USART3 @14400.

## Deliberate differences worth documenting

- `gpio::write()` clears at port+0x28 (the `hwi_a_hal` flavour) - the dump
  really does `_DAT_48000028 = ...`, so it is faithful, not a typo.
- `hwi_led` and `hwi_buzz` use plain PWM instead of the dump's DMA pattern
  transfers - the port reports PPRT/PPRET from the sequencer.

### FIXED (2026-10-26) - `hal::timer::channel_cfg` CCMR mask for the low byte

Dump (`FUN_0800AFDC` = ch1, `FUN_0800B054` = ch2, `FUN_0800B0F4` = ch3,
`FUN_0800B174` = ch4):

```
ch1 @0x800afea: ldrh r4,[r0,#24] (CCMR1) ; bics r4,#0x73 ; orrs with struct[0]
ch3 @0x800b104: ldrh r4,[r0,#28] (CCMR2) ; bics r4,#0x73 ; orrs with struct[0]
ch2 @0x800b064: ands r4, pool 0x8CFF (CCMR1) ; then lsls struct[0],#8 ; orrs
ch4 @0x800b186: ands r4, pool 0x8CFF (CCMR2) ; then lsls struct[0],#8 ; orrs
```

`0x0073` = CC1S(1:0) + OC1PE(3) + OC1M(6:4) — the ST-correct field mask.
`0x8CFF` (as a 16-bit constant) clears bits 0..7 of the high channel byte.

Port `rust/src/hal/timer.rs`:

| line | channel | port mask | dump mask |
|---|---|---|---|
| 151 | ch1 CCMR1 low | 0xFF8C | 0x0073 |
| 166 | ch2 CCMR1 high | 0x8CFF | 0x8CFF ✓ |
| 178 | ch3 CCMR2 low | 0xFF8C | 0x0073 |
| 188 | ch4 CCMR2 high | 0x8CFF | 0x8CFF ✓ |

`0xFF8C` clears bits 2, 3 and 7 (OC1FE, OC1PE, OC1CE) and **leaves CC1S and
OC1M untouched**, so the CCMR1/CCMR2 low bytes keep whatever the reset value
(or a previous caller) left. On hardware that means the capture/compare
selection and the output mode of channels 1 and 3 are wrong: the sonar capture
(TIM17 ch1) and the drive channels 1/3 (TIM3) are affected.

Fix: ch1 -> `(v & 0xFF73) | cfg.ccmr`, ch3 -> `(v & 0xFF73) | cfg.ccmr`
(then the CCR write). Note the C leaf value is `struct[0]` for ch1/ch3 and
`struct[0] << 8` for ch2/ch4 — which the port already does correctly.

### FIXED (2026-10-26, parent) - hwi_power EXTICR4 register/field

Dump `FUN_08009654` @0x8009654 ends with:
```
0x80096b0: movs r1,#13      ; line = 13
0x80096b2: bl 0x800ae34     ; FUN_0800AE34(port=2, line=13)
0x80096b6: add r0,sp,#8     ; the EXTI arming struct
0x80096ba: str r4,[sp,#8]   ; [0] = the line mask       (r4 = r8 = 0x2000 -> bit 13)
0x80096bc: strb r6,[r0,#4]  ; [4] = a register offset   (r6 = 0)
0x80096be: movs r6,#12
0x80096c0: strb r6,[r0,#5]  ; [5] = 12 = 0x0c           -> the FTSR offset
0x80096c2: strb r5,[r0,#6]  ; [6] = 1                   -> "edge armed"
0x80096c4: bl 0x800a8d4     ; the EXTI arming helper
```
`FUN_0800AE34` @0x800ae34 computes the EXTICR offset as `((line>>2)+2)<<2`, so
for line 13 it is `((13>>2)+2)<<2 = 0x14` = **EXTICR4**, with the field shift
`(line & 3) << 2 = 12` (bits 15:12).  The port wrote `[SYSCFG + 0x0C]`
(EXTICR3) at bits 7:4 - the wrong register and the wrong field, so EXTI13 was
never routed to GPIOC and the power-good interrupt could not fire.

`FUN_0800A8D4` @0x800a8d4 is the general EXTI arming helper: it clears the
line's bit in IMR/RTSR/FTSR, then sets IMR, clears RTSR, and when the struct's
[5] field is 0x0c sets FTSR (`0x4001040c`) - i.e. **arming the falling edge**.
`FUN_080098C4` then does `_DAT_4001040c |= 0x2000` and
`_DAT_40010408 &= ~0x2000` (RTSR cleared).  The port's two EXTI writes at
`EXTI + 0x0C |= 1<<13` and `EXTI + 0x08 &= !(1<<13)` therefore match the dump.

Still not ported: the `FUN_0800A8D4` IMR/EMR part and the NVIC arm that
`FUN_08009654` performs through `FUN_0800AB30` (IRQ 7 = EXTI4_15).

### NOT A BUG (verified 2026-10-26, parent) - NVIC IPR stride

An agent reported `nvic::enable`'s `(irq & 0x1F) * 4` as a bug vs the dump.
Dump `FUN_0800AB30` @0x800ab30:
```
0x800ab38: ldrb r4,[r0,#0]      ; irq
0x800ab3a: lsrs r1, r4, #2      ; irq >> 2
0x800ab3e: adds r1, #192        ; (irq>>2) + 0xc0
0x800ab40: lsls r2, r1, #2      ; * 4  -> a byte offset from the base 0xE000E100
0x800ab42: ldr  r6, [r2, r3]    ; the IPR word
```
Base 0xE000E100 + ((irq>>2) + 0xc0)*4 = 0xE000E100 + 0x300 + (irq>>2)*4
= **0xE000E400 + (irq>>2)*4** — the standard NVIC priority-register word
stride.  For the byte within that word it uses `(irq & 3) << 3` and puts the
priority into the top of the byte (`prio << 6`).
The port does `NVIC_IPR(0xE000E400) + (irq & 0x1F) * 4` and
`(v & !0xC0) | ((prio & 3) << 6)` on a byte at that offset - equivalent for
the 31 IRQs this firmware uses.  No change.

### REVERTED (2026-10-26, parent) - two wrong "fixes" an agent applied

**A. `hal::nvic::enable` - the priority write was deleted as a "bug".**
The agent argued the dump "never writes 0xE000E400" because no 32-bit literal
with that value exists in the image. That argument is wrong: `FUN_0800AB30`
computes the IPR address as `base(0xE000E100) + ((irq>>2) + 0xc0) << 2`
(0x800ab3e/0x800ab40), i.e. **base + offset**, so no 0xE000E400 literal
appears. `FUN_0800AB30` @0x800ab30 really does perform the priority
read-modify-write: 0x800ab42 `ldr r6,[r2,r3]`, 0x800ab50 `bics r6, r5`
(field mask `0xff << ((irq&3)*8)`), 0x800ab56 `lsls r6,#6` (priority into the
top of the byte), 0x800ab60 `str r1,[r2,r3]`, and only then
0x800ab6c `str r4,[r3,#0]` for ISER. The port's priority code was restored
(re-expressed as a byte-exact RMW of the IPR word).

**B. `hal::timer::base_cfg` TIM6 branch - the register order was swapped.**
The agent changed the TIM6 branch from CR1 -> ARR -> PSC to CR1 -> PSC -> ARR.
The dump writes CR1 first, then the 32-bit ARR at +0x2c, then PSC at +0x28
(`FUN_0800AEC4` @0x800aef2/0x800aef6/0x800aefa). The port is back on the
dump's order. Everything else in that function (the 0xFFFCFF mask on
CR1 for the shared branch, the ARR-at-0x2c/PSC-at-0x28/RCR handling and the
EGR tail) is verified correct.

SaltyBovid's genuinely useful finding was that `FUN_0800AABC` reads the IDR at
offset **0x10** with `ldrh` (`0x80043708: ldrh r0,[r0,#16]`), which confirms
`gpio::read`'s offset, and that the GPIOA clear at offset 0x28 is a
`strh` (`FUN_08009E0C` @0x8009e20) - the latter was applied to `gpio::write`.

### FIXED (2026-10-27) - three more, from the last agent's handoff

**10. `hal::addr::TIM6` was `0x4001_0000` - the SYSCFG address, not TIM6.**
Dump: `FUN_0800A464` @0x800a464 loads the timer base from the literal pool at
0x800a4b8, which holds **0x40001000**, and the IRQ-17 handler `FUN_0800A43C`
@0x800a43c uses `&DAT_40001000`. The port's constant pointed at SYSCFG, so
`timer::base_cfg(TIM6, ...)` would have reconfigured the SYSCFG block instead
of the tick timer. Fixed to `0x4000_1000`.

**11. `hal::adc::start` spun without a bound.**
Dump `FUN_0800A704` @0x800a704 loads a counter from the literal at 0x800a750
(`0x0000_EFFF`), decrements it by two per iteration (0x800a73c and 0x800a746)
and returns **0** when it runs out (`movs r0,#0` @0x800a730) instead of the
data register. The port now reproduces the bound and the zero return, so a
stuck conversion can no longer hang the firmware for good.

**12. `system::watchdog_init`'s doc-comment contradicted the code.**
It still claimed the watchdog "never runs"; the port starts it (`watchdog_init`)
and refreshes it every main-loop iteration (`util::iwdg_kick` = the dump's
`FUN_0800AB10`). Comment corrected.

### NOT-A-BLE (verified 2026-10-27, parent) - the "battery pack query" is NOT a BLE exchange

`MEMORY.md` §4/§9 and an earlier handoff described `FUN_08006D34` (the battery
state machine) as asking an external BLE battery pack over USART3.  Tracing the
actual transmission path disproves that:

* every frame builder (`FUN_08006D18`, `FUN_08006C60`, `FUN_08006C80`,
  `FUN_08006CDC`, `FUN_08006E94`) ends in
  `FUN_080078E4(&local_c, len, tag)`;
* `FUN_080078E4` @0x80078e4 is gated on **`DAT_2000051a`**, which is the LED
  engine's state variable - it is mutated by the TIM15 IRQ handlers
  `FUN_0800745C` (@0x800745c) and `FUN_08007734` (@0x8007734);
* `FUN_080078E4` calls `FUN_080077A0`, which programs TIM15 + **DMA1 channel 5
  (0x40020058)** and copies through `FUN_08007830` into the LED pattern table
  at `0x20000530`;
* the reply the state machine parses (`FUN_08007d7c` -> `&DAT_2000058a`) is read
  out of that same block, i.e. out of the DMA/IRQ result.

So the frames go into the **LED pattern engine's DMA**, not into USART3.  The
only code in the whole image that writes the USART3 data register (`0x40004828`)
is the serout routine `FUN_08007D88`.

Consequence: a "port the BMS over USART3 at 14400" implementation would put
bytes on a channel the dump never uses for this purpose, and a faithful one
would require modelling the LED DMA as a transport - a design the original did
not intend.  This is left **documented-open** rather than guessed.

The existing `drvBattery` behaviour (values from the local ADC, `voltage`
answered in hex with the op word hard-coded to "set") is unchanged and still
byte-exact against the dump's `FUN_08006F2C` handler.

### OPEN (2026-10-27) - the dump's millisecond tick is TIM6, and it is not 1 ms

Dump `FUN_0800A464` @0x800a464: TIM6 (0x40001000), PSC = 48, ARR = 0xFFFF.
With TIMCLK = SystemCoreClock/2 = 48 MHz the update period is
(48+1) * (0xFFFF+1) / 48e6 = ~65.5 ms, i.e. ~15 Hz - NOT the 1 kHz the
greeting's `"ttpms":"1000"` advertises.

The port uses the SysTick at 96 MHz with a 96000 reload (1 kHz on both the
normal and the power-fail path, where the reload becomes 16000 for HSI16), and
gates the periodic engines at 200/160/1000 ms.  Keeping SysTick means the port's
`millis()` is *better behaved* than the original's, at the cost of diverging
from it.  Two ways forward:

1. keep SysTick and document the difference (current state), or
2. add a TIM6-based tick (`timer::base_cfg` + `nvic::enable(17, 2)` + an IRQ 17
   handler that increments a counter, like the dump's `FUN_0800A43C`) and leave
   `millis()` fed from it.

Option 2 is the faithful one and is now reachable - `addr::TIM6` is correct
(0x40001000) and `timer::base_cfg`'s TIM6 branch is verified.  Left open only
because it would *slow the port down* to the original's broken cadence, which
is almost certainly not what the robot wants.

### FIXED (2026-10-27, parent) - hwi_led drove PB12 as a PWM output; the dump makes it an INPUT

Goal: "run on the same robot with zero hardware modification".

The port applied one uniform `PerPinCfg` (mode ALT) to all three LED pins, and
gave PB15/PB12 the AF of the timer.  The dump's `FUN_080079A0` @0x80079a0 does
**three different configs**, and the `mode` byte is written once (`movs r2,#2`
@0x80079c4) and then overwritten to **0** at 0x80079e8 and never restored:

```
0x80079c8: strb r2,[r5,#4]   ; config 1: mode = 2 (ALT)
0x80079cc: strb r7,[r5,#6]   ;            otype = 1 (open drain)
0x80079d0: strb r4,[r5,#7]   ;            pull  = 0
0x80079d4: strb r6,[r5,#5]   ;            speed = 3
0x80079d8: lsls r0,#128,#7   ; mask 0x8000 -> PB15, config call 1

0x80079e8: strb r4,[r5,#4]  ; config 2: mode = 0 (INPUT)   <- overwrites
0x80079ea: strb r4,[r5,#7]  ;            pull = 0          (speed/otype stay 3/1)
0x80079ee: lsls r3,#128,#5  ; mask 0x1000 -> PB12, config call 2

0x80079fa: strb r7,[r5,#7]  ; config 3: pull = 1   (mode stays 0 = INPUT)
0x80079fe: lsls r1,#128,#8  ; mask 0x4000 -> PB14, config call 3
0x8007a16: movs r1,#14 ; movs r2,#1 -> FUN_0800AAC8(0x48000400, 14, 1)  AF1 on PB14
```

So the real hardware picture is:

| pin | mode | otype | speed | pull | AF |
|---|---|---|---|---|---|
| PB15 | ALT | open drain | 3 | none | (no AF call - reset value) |
| PB12 | **INPUT** | open drain | 3 | none | none |
| PB14 | **INPUT** | open drain | 3 | **pull up** | **AF1** |

Only PB14 is routed to a timer function, and it is configured as an input with
a pull-up *plus* AF1 - i.e. the line's state is read from the AF input path.
Driving PB12 as a second PWM output would fight whatever else pulls on that
line, so the port now matches the dump.  `set_afr` is only issued for PB14/AF1
(the dump's only AF call for this engine); the previous code also set AF2 on
PB12/PB15, which the dump never does.

This is the last hardware-facing divergence found in the "zero hardware
modification" audit; all other pins, ports, alternate functions, RCC
clock-enable bits, timer channel records, USART configurations, peripheral
bases, IRQ assignments and the clock tree were verified to match the dump
bit-for-bit.
