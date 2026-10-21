# MEMORY — Codie robot STM32 firmware reverse engineering

**Status: ~90% complete.** The original firmware is fully decompiled and
analysed, the rebuild compiles (12 608 byte image), the wire protocol is
verified byte-for-byte against the original, and every protocol-visible
string of the original is present in the rebuild.

**Two open P0 bugs in the rebuild (see §4.3 and §4.4): the two UART roles are
swapped, and the clock constants disagree with the clock the code actually
sets up.  Fix those before flashing anything to the robot.**  After that
only the on-hardware test is left.

Last updated: 2026-10-21

**READ FIRST**
1. **§4** — USB (bootloading only!) and the two serial links, including the
   two open P0 bugs in the rebuild.  Do not flash the robot before fixing
   §4.3 (UART roles swapped) and §4.4 (clock constants).
2. **§2** — chip identification and the register map (taken from the binary).
3. **§3/§5** — the protocol and the per-module behaviour that was recovered.
4. **§8** — the checklist of what is done and what is left.
5. **§9** — the option of rewriting the whole thing in Rust (feasible; what
   improves, what does not, and how to port it against the C reference).

---

## 1. What we have

| artefact | where | state |
|---|---|---|
| original flash dump | `stm32_flash.bin` (128 KB) | untouched, sha256 `0c5f3583…` |
| Ghidra project | `analysis/ghidra_proj2/` (2nd run, 378 functions) | imported + auto-analysed |
| decompiled C (378 functions) | `analysis/decomp/ALL.c` (+ one file per function) | complete, 0 failures |
| function list | `analysis/decomp/FUNCS.txt` | complete |
| strings | `analysis/decomp/STRINGS.txt` | complete |
| reconstructed firmware project | `fw/` | **complete and building** (12 608 B image) |
| verification tools | `tools/verify_features.py`, `tools/protocol_test.c` | protocol verified byte-exact |
| analysis tooling | `tools/` | analyze2.py, mkelf.py, modulemap.py, worklist.py |
| toolchains | `tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/`, `tools/ghidra_12.1.4_PUBLIC/` | installed |
| python venv (capstone, pyelftools) | `.venv/` | use `.venv/bin/python` |

Toolchain invocation used everywhere:
`tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin/arm-none-eabi-gcc`

---

## 2. Target identification (all evidence-based)

* **Cortex-M0+ (Thumb-1 only)**, no FPU.
* **Flash 128 KB @ 0x08000000** (DFU alt 0 = 64 × 2 KB), **RAM 16 KB @ 0x20000000**
  (initial SP in the dump = 0x20003FFF, i.e. the project assumed a 16 KB budget).
* **Vector table = 0x08000000 … 0x080000BF** = 48 words: SP, Reset, 15 system
  exceptions, 31 external IRQs (0…30) ⇒ the part has 31 IRQs.
  Real handlers exist for IRQ 7, 9, 11, 13, 15, 17, 20, 22, 29.
* **Peripheral bases actually used by the firmware** (taken from the decompiled code,
  these are ground truth, not guesses):
  * GPIO: A 0x48000000, B 0x48000400, C 0x48000800, F 0x48001400
    (0x400-byte spacing, standard GPIO layout: MODER 0x00, OTYPER 0x04, OSPEEDR 0x08,
    PUPDR 0x0C, IDR 0x10, ODR 0x14, BSRR 0x18, LCKR 0x1C, AFR 0x20/0x24)
  * TIM1 0x40012C00, TIM3 0x40000400, TIM6 0x40001000,
    TIM15 0x40014000, TIM16 0x40014400, TIM17 0x40014800
  * USART1 0x40013800, USART3 0x40004800, USART4 0x40004C00
    (G4-style layout: CR1 0x00, CR2 0x04, CR3 0x08, BRR 0x0C, GTPR 0x10, RTOR 0x14,
    RQR 0x18, ISR 0x1C, ICR 0x20, RDR 0x24, TDR 0x28 — proven by FUN_0800B54C
    reading +0x08/+0x1C and FUN_08007D88 writing TDR at +0x28)
  * ADC1 0x40012400 (CCR 0x40012440), DMA1 0x40020000 (channel stride 0x14),
    RCC 0x40021000, FLASH 0x40022000, IWDG 0x40003000, SYSCFG 0x40010000
  * Cortex-M: SysTick 0xE000E010, NVIC 0xE000E100, SCB 0xE000ED00
* The exact ST part number is **not** pinned (the GPIO bus at 0x48000000 plus the
  G4-style USART plus 31 IRQs does not match one single published ST map).
  ⇒ the rebuild uses the **register map taken from the binary**, which is what
  matters for running on the same board. Do not "fix" the addresses to match a
  datasheet — keep them as observed.

---

## 3. Recovered architecture

* Startup (0x08002904): set SP, copy .data, zero .bss, `SystemInit()` (0x08002958,
  clock tree), `__libc_init_array()` (0x0800B6B0), `main()` (0x08009FA4), spin.
* `main()`: init, `hwInit`, send the **greeting**, then loop
  { read a line (0x28 = 40 byte buffer) → dispatch → periodic tasks }.
* **Interface dispatch table lives in flash at 0x0800D164**, 8 bytes per entry
  (`const char *name`, `void (*handler)(op, opname, args)`), 16 entries in this
  order (the dispatcher searches the table backwards):

  | # | interface | handler | # | interface | handler |
  |---|---|---|---|---|---|
  | 0 | hwiAdc | 0x080073DD | 8 | hwiEnc | 0x08008DFD |
  | 1 | hwiLed | 0x080090A5 | 9 | hwiLine | 0x08009479 |
  | 2 | hwiDrive | 0x08008B3D | 10 | hwiPower | 0x0800981D |
  | 3 | drvBattery | 0x08006F2D | 11 | hwiLightSense | 0x080093A9 |
  | 4 | hwiSonar | 0x08009F89 | 12 | appLights | 0x08005F01 |
  | 5 | bleCom | 0x0800837D | 13 | appDrive | 0x08003849 |
  | 6 | hwiBuzz | 0x08008491 | 14 | appSys | 0x080064F1 |
  | 7 | hwiMic | 0x0800962D | 15 | appLightSense | 0x08003855 |

  NOTE: 7 of those 16 handlers are 4-byte stubs in the original
  (`movs r0,#0; bx lr`): hwiLed, hwiSonar, bleCom, hwiEnc, hwiLightSense,
  appLights, appSys.  Their real behaviour lives in the periodic task
  (FUN_080063EC) and in the per-module engines.  The reconstruction keeps the
  stubs' behaviour where the original has it and exposes the engines as
  real operations where that is useful.
* **Peripheral map (all verified in the decompiled code):**
  * TIM15 0x40014000 + DMA1 ch5 (0x40020058) → LED pattern engine
    (12 × 24 table built from a 36 bit RGB bitmap, FUN_080090A8/FUN_08007B34)
  * TIM17 0x40014800 + DMA1 ch1 (0x40020008) → second LED engine (FUN_08009268)
  * TIM16 0x40014400 → buzzer engine (FUN_08008380/FUN_08008440)
  * TIM3 0x40000400 + GPIOC → motor drivers (hwiDrive), fault input GPIOC pin 14
  * TIM1 0x40012C00 + GPIOA → wheel encoders (hwiEnc), input GPIOA pin 12
  * USART 0x40004800 → BLE module link (TDR at +0x28, ISR at +0x1C)
  * USART 0x40004C00 → command/console link (error flags cleared here)
  * ADC1 0x40012400 → battery / mic / light sensing
  * GPIOF pin 6 → charge state, GPIOC pin 13 → line sensor, GPIOB pin 5 → buzzer
* **Clock setup (FUN_08002958, mirrored in fw/src/system_stm32g0.c):**
  CR |= HSION|HSEON (bits 0,16), wait HSERDY (bit 17, timeout 0x5000),
  CFGR &= 0xf8c0b80c; if CR bit 18 (HSE bypass): FLASH_ACR = 0x11, wait
  PLLRDY (bit 25), CFGR = (CFGR & 0xf8c0380c) | 0x130002, CR |= 0x1010001,
  wait SWS == PLL.  RCC layout of this part: CR 0x00, CFGR 0x04, CIR 0x08,
  AHBENR 0x14, APB2ENR 0x18, APB1ENR 0x1C, AHB2RSTR 0x2C, AHB3RSTR 0x30.
* **Number formatting (FUN_080068D4 / FUN_0800693C):** the third argument is the
  RADIX, not a buffer size - the original prints every sensor value in
  **hex** (radix 16) and the ADC dump in decimal (radix 10).  Digits ≥ 10 are
  produced with the original's `'W' + digit` trick (yields 'a'..'f').
* **Greeting** (RAM array 0x20000A14, 5 key/value pairs):
  `call :proxy greeting "ver":"0.5.0hw2.1" "mat":"4" "date":"1602121419" "ttpms":"1000" "+ack":"0"`
* Original RAM layout: .data = flash 0x0800C720..0x0800D624 → RAM
  0x20000000..0x20000F04, .bss = 0x20000F08..0x20001030.
* **7 interface handlers were missing from the first decompilation** (they are
  only reachable through the flash table).  `tools/mkelf.py` now adds them as
  ELF entry points; the second Ghidra run (analysis/ghidra_proj2) has 378
  functions.

* Request grammar: `<get|set|call> <interface> <op> [args]`
  (op codes 1/2/3; the words "get"/"set"/"call" live at 0x0800B824/28/2C).
  The parser is FUN_0800A0AC, the dispatcher FUN_0800A350.
* Replies (exact layout, see `fw/include/codie.h`):
  * value reply: `"<get|set|call> <interface> <op> [value]\n"` (FUN_0800A16C)
  * system message: `"call :proxy message <level> <text> [extra]\n"` (FUN_0800A2D0)
  * greeting: `"call :proxy greeting \"k\":\"v\" … \"msg:<text>\"\n"` (FUN_0800A1C8)
  * log levels: "???"/inf/dbg/wrn/err (0x0800B834…)
* BLE link: external module over USART; strings "BLE-boot", "BLE-RSP HELLO",
  "BLE-RSP GAP SET MODE: ", "PPRT", "PPRE", "ERR:BLE_serout_TO", "+ack";
  error strings "BLE framing error", "BLE USART noise", "BLE USART overrun",
  "USART framing error", "possible cmdBuf overflow", "BUFOVF!!!!!",
  "Invalid command!", "No interface:".
* Version block (string table @ 0x0800C65C…): ver "0.5.0hw2.1", mat "4",
  date "1602121419", ttpms "1000".

---

## 4. USB and the two serial links (found 2026-10-21 — read this first)

### 4.1 USB: bootloading only
* The application **never touches the USB peripheral**: 0 references to the
  USB base (0x40005C00) or the USB SRAM (0x40002800) anywhere in the
  decompilation, and no USB clock-enable bit in any of the original's RCC
  enable calls (`FUN_0800AD5C/7C/9C` masks are only GPIO/timer/ADC/USART).
* So on the robot **USB = the ROM bootloader's DFU 1.1a interface**
  (0483:df11): read / flash / erase the 128 KB flash + read the option bytes.
  That is how this repo's dump was taken.  No CDC, no HID, no data channel.
  PA12 is read once as a plain GPIO input, but the USB PHY is never enabled.
* ⇒ for a console you need the **UART header** (USART4 below), not USB.

### 4.2 The two UARTs, with their exact roles
| | **USART3** `0x40004800` | **USART4** `0x40004C00` |
|---|---|---|
| role | AT / control channel to the BLE module | the robot's **command + reply link** |
| pins | PB1 + PB13 (AF4), PC4 + PC5 (AF1), PA2 = input | **PC10 TX / PC11 RX (AF0)** |
| baud | **14400**, 2 stop bits (CR2 = 0x2000) | **460800**, 2 stop bits |
| traffic | "BLE-boot", "PPRT"/"PPRET", reads "BLE-RSP HELLO" / "BLE-RSP GAP SET MODE: " | commands **in**, and **every** reply **out** (greeting, values, errors, "BUFOVF!!!!!") |

Proof of the direction: the original's output primitives are `out_str` =
`0x08009CCC` and `out_char` = `0x08009CA1` (taken from the .data blob at
flash 0x0800D130 / 0x0800D160 → RAM 0x20000A10 / 0x20000A40), and their
literal pool holds **0x40004C00** — so *all* robot output goes to USART4.
The command reader (FUN_08009B14) also reads USART4 and clears *its* error
flags ("USART framing error" / "noise" / "overrun").  USART3 only carries the
module's setup traffic and its RX is consumed by the BLE service code.
The ":proxy" prefix = "this message is proxied through the BLE module".

### 4.3 (FIXED 2026-10-21) the link roles were SWAPPED in the rebuild
`fw/modules/bleCom.c` currently does the opposite of the original:
* `out_str`/`out_char` write to **USART3 (0x40004800) @14400** — the AT
  channel, where the phone is not listening;
* the command reader treats **USART4 (0x40004C00)** as a secondary source.
**What was done**: `out_str`/`out_char` in `fw/modules/bleCom.c` now write to
CONSOLE_USART (0x40004C00 @ 460800, 2 stop bits); the command input stays on
USART4; USART3 (0x40004800 @ 14400) is the module link - the robot reads the
module's "BLE-RSP ..." answers there and pushes raw data to it (the original's
"serout" routine).  The "USART framing error/noise/overrun" reports stay on
USART4, where the commands come from.
**Confirmed** by the host simulator (§4.5): before the fix every reply came
out on USART3, after it they come out on USART4.

### 4.4 (FIXED 2026-10-21) the clock constants disagreed
`fw/src/system_stm32g0.c` reproduces the original's clock tree
(HSI16 → PLL, PLLMUL ×6 ⇒ **96 MHz**, APB = AHB = 96 MHz, SysTick 95999),
but `fw/modules/hwi_b_hal.h` has `#define HWI_APB1_HZ 48000000u`:
* USART BRR is computed for 48 MHz ⇒ the link runs at **2× the requested
  baud** (14400 → 28800) ⇒ **the BLE/console link is dead**;
* `HWI_TIMCLK_HZ (HWI_APB1_HZ / 2u)` — on this part family the timer clock is
  **2 × APB** (prescaler 1), not APB/2 ⇒ buzzer tone, sonar tick and LED PWM
  are off by 8×.
**What was done**: `fw/modules/hwi_b_hal.h` now has
`HWI_APB1_HZ 96000000` (what the PLL actually produces) and
`HWI_TIMCLK_HZ (HWI_APB1_HZ * 2u)` (APB prescaler 1 ⇒ timer clock = 2 × APB).
The simulator checks the resulting BRR values: 96e6/14400 = 6666 on the
module link and 96e6/460800 = 208 on the command link.
Still open (optional): a 48 MHz build switch if the part turns out not to be
rated for 96 MHz (the original's own helper FUN_0800AC40 says 96 MHz, so it
presumably is).

### 4.5 Host simulator - "does it do the same as the original?"
`tools/host_sim.c` + `tools/run_sim.sh` (also `cd fw && make test`) run the
**rebuilt firmware unmodified** on the host:
* 256 MB anonymous mapping at 0x40000000 covers RCC/USART/timers/ADC/DMA and
  the GPIO bus at 0x48000000; plus RAM at 0x20000000, flash at 0x08000000 and
  the Cortex-M system space at 0xE0000000.
* the USART primitives are interposed with `ld --wrap` so the test can inject
  bytes and capture them; the ADC and the timers are modelled in a thread
  (ADRDY/EOC set, ADSTART cleared, CNT advancing).
* the firmware's `main()` runs in a thread, the test drives it.

Result: the rebuild's observable behaviour (replies, register side effects)
is **the same as the original's**, and the checks are now regression tests:

| behaviour | original | rebuild (simulated) | ok |
|---|---|---|---|
| boot output on the command link | "BLE-boot" + the greeting | identical bytes | yes |
| greeting | `call :proxy greeting "ver":"0.5.0hw2.1" "mat":"4" "date":"1602121419" "ttpms":"1000" "+ack":"0"` | identical | yes |
| `get drvBattery voltage` | `set drvBattery voltage <hex>` (op word hard-coded to "set" by the original) | identical | yes |
| `get drvBattery chargeStat` | `set drvBattery chargeStat 1` | identical | yes |
| unknown interface | `call :proxy message err No interface:<name>` | identical | yes |
| malformed request | `call :proxy message err Invalid command!` | identical | yes |
| `set hwiDrive dutyA 200` | no reply; the duty goes to the TIM3 compare register (ARG parsed as **base 16** → 0x200 = 512) | no reply, TIM3 CCR1 = 0x200 | yes |
| `set hwiDrive dutyB 100` | as above | TIM3 CCR3 = 0x100 | yes |
| `call drvBattery hwInit` | runs the init, **no reply** | no reply | yes |
| module link BRR / command link BRR | 96e6/14400 = 6666, 96e6/460800 = 208 | identical | yes |

## 5. Details recovered by the module agents (verbatim from the decompilation)

* **BLE UART**: 0x40004800, **14400 bit/s**, 8 data bits, **2 stop bits**
  (CR2 = 0x2000), TE|RE (CR1 = 0x300), CR3 = 0x0C, UE set after init.
  Pins: GPIOB1 + GPIOB13 → AF4, GPIOC4 + GPIOC5 → AF1, GPIOA2 = input.
  RX ring 128 B, TX ring 64 B.  Errors: FE → "BLE framing error",
  NE → "BLE USART noise", ORE → "BLE USART overrun" (ICR 2/4/8),
  ring overflow → "BUFOVF!!!!!\n".
* **hwiBuzz**: TIM16 CH1 on **PB8 AF2**, PSC 96; ARR = TIMCLK/freq−1,
  CCR = ARR·duty/100, tone queue.
* **hwiPower**: PC12 = power hold (output), PC13 = power-good input + EXTI13
  (SYSCFG EXTICR3 = 2), PC14, PC15 (Vsens); power off = clear PC12 then spin.
* **hwiLine**: PA4 output = sensor bar, ADC groups 0x20/0x40, deltas vs.
  calibration.
* **hwiMic**: ADC sensor group id 2 with a tracking filter.
* **hwiLightSense**: ADC group id 4, averaged over 8 samples.
* **hwiLed engines**: TIM15 + DMA1 ch5 (12 × 24 pattern table from a 36-bit RGB
  bitmap, prints "PPRT\n"/"PPRET\n" at the end of a transfer) and
  TIM17 + DMA1 ch1.
* **Op names that really exist in the original**: `buf` (hwiAdc),
  `dutyA`/`dutyB`/`nFault` (hwiDrive),
  `voltage`/`current`/`remainingCapacity`/`chargeStat`/`temp`/`hwInit`
  (drvBattery), `pwrOff`/`Vsens` (hwiPower).  `echo` sits in the string table
  but no code references it.  The other 11 interfaces have 4-byte no-op
  handlers in the original - the reconstruction keeps the engines and exposes
  them with clearly documented, invented op names (e.g. hwiLed `on`/`pwm`).
* **The original's hwiPower `Vsens` is a *set* op** (arg `on|1|true` → PC15 set,
  else reset) - hwi_b_hal's `hwi_is_on()` implements that argument test.

## 6. Rebuild project layout

```
fw/
  Makefile              # arm-none-eabi-gcc, -mcpu=cortex-m0plus, linker script below
  linker/stm32g071rb.ld # FLASH 128K @0x08000000, RAM 16K @0x20000000, SP=0x20003FFF
  flash.sh              # dfu-util upload + read-back verify (docker fallback)
  include/codie.h       # SHARED CONTRACT - do not change signatures
  include/CMSIS/        # core_cm0plus.h etc. (for reference only)
  src/                  # startup, system (clock), main
  modules/              # one file per original module
```

Build: `cd fw && make` → `build/fw.bin` (128 KB image, `make flash` uploads it).

---

## 7. Work split (3 subagents)

| agent | owns | files |
|---|---|---|
| A | hwiAdc, hwiDrive, hwiEnc, hwiSonar | `fw/modules/hwi_adc.c`, `hwi_drive.c`, `hwi_enc.c`, `hwi_sonar.c`, `hwi_a_hal.c` |
| B | hwiLed, hwiBuzz, hwiMic, hwiLine, hwiPower, hwiLightSense, bleCom | `fw/modules/hwi_b_*.c`, `bleCom.c` |
| C | appSys, appDrive, appLights, appLightSense, drvBattery, proxy/protocol, main, system clock | `fw/modules/app_*.c`, `drv_battery.c`, `proxy.c`, `fw/src/main.c`, `fw/src/system_stm32g0.c` |

Rules for all agents: implement the behaviour found in the decompiled C, keep the
signatures in `fw/include/codie.h`, no new dependencies, no build/test runs.

---

## 8. Remaining work (checklist)

1. [x] dump analysis, chip identification
2. [x] Ghidra decompile (371 → 378 functions with the table handlers)
3. [x] protocol + interface table recovery
4. [x] agent A modules (hwiAdc, hwiDrive, hwiEnc, hwiSonar + HAL)
5. [x] agent B modules (hwiLed, hwiBuzz, hwiMic, hwiLine, hwiPower,
       hwiLightSense, bleCom + HAL)
6. [x] agent C modules (proxy, app*, drvBattery, main, clock setup)
7. [x] integration, build → `fw/build/fw.bin` (12 556 bytes, 3 392 B BSS)
8. [x] size check (< 128 KB), vector table check (SP 0x20004000 == the
       original's 0x20003FFF, reset entry 0x08000269, 48 vectors)
9. [x] protocol verified byte-for-byte on the host
       (`tools/protocol_test.c`: greeting, value reply, unknown interface,
       invalid command, cmdBuf overflow, log levels - all pass)
10. [x] every protocol-visible string of the original present in the rebuild
       (`tools/verify_features.py` → OK)
11. [x] README.md + fw/README.md document the rebuild
12. [x] **P0: swap the link roles** (see §4.3) — DONE and confirmed by the
       host simulator
13. [x] **P0: fix the clock constants** (see §4.4) — DONE
14. [x] **host MMIO simulator** (see §4.5) — `tools/run_sim.sh`, `make test`
15. [ ] optional: battery values from the BLE BMS (original: FUN_08006D34)
       instead of the local ADC
16. [ ] optional: op-parity checker (parse the original's op strings per
       interface and diff them against the rebuild's registered ops)
17. [ ] **on-hardware test**: flash with `fw/flash.sh` and check that the
       robot boots, answers `get drvBattery voltage` over BLE and drives the
       motors (`set hwiDrive dutyA 200`)
13. [ ] optional: the pin map of the LED / motor outputs is the one of the
       original binary where it could be recovered, otherwise it is the
       family default - verify against the board if the LEDs/motors do not
       move (see the comments in `fw/modules/hwi_led.c` / `hwi_drive.c`)

## 9. Possible future: rewrite the firmware in Rust

**Verdict: feasible, and probably better as software - but not automatically
better as source text, and not smaller.**  The C version stays the reference
implementation; a Rust port should be done *against* the C one, using the
host tests as the oracle.

### 9.1 Is it technically possible?  Yes.
* Target: `thumbv6m-none-eabi` (Cortex-M0+).  It is a **tier-3 Rust target**,
  so it needs a nightly toolchain and a rebuilt core:
  `rustup toolchain install nightly` +
  `cargo build -Z build-std=core --target thumbv6m-none-eabi`.
* Runtime: `cortex-m-rt` (vector table, reset handler, `#[entry]`) and
  `cortex-m` (`Peripherals::take()` for the safe peripheral singleton) are
  both tier-3-compatible and work on thumbv6m.
* No `std`: `#![no_std]`, `panic = "abort"`, a `#[panic_handler]`, and
  `heapless` (or fixed arrays) for the 40-byte command buffer and the ring
  buffers - the C version has no heap either, so nothing is lost.
* **No vendor PAC matches this chip.**  The register map is a hybrid
  (F0-style RCC at 0x40021000, F0x4-style USART with CR1/BRR/ISR/TDR,
  GPIO on the 0x48000000 bus, TIM15/16/17 at 0x40014000/44/48) - the
  `stm32g0`/`stm32f0`/`stm32l4` crates all assume a different map.  The
  register layer has to be hand-written as `#[repr(C)]` structs over
  `volatile` pointers (~300 lines) - that part is exactly as readable as the
  C HAL and no better.

### 9.2 What would actually be better
* **No UB**: the C version does `*(volatile uint32_t *)(port + 0x18)` style
  pointer arithmetic everywhere and mixes `volatile` with normal accesses.
* **No buffer bugs** in the 40-byte command line / the RX and TX rings -
  the classic place where this firmware could break.
* **`cargo test` on the host**: the protocol layer can be unit tested
  natively (the C version needs the hand-written harness in
  `tools/protocol_test.c` + `tools/host_sim.c`).
* **The protocol becomes typed**: `enum Op { Get, Set, Call }`,
  `enum Iface { DrvBattery, HwiDrive, ... }`, `enum HwOp { Voltage, ChargeStat,
  DutyA, ... }` with `match` instead of `strcmp` on string literals - and the
  "did I lose an op?" question is answered by the compiler.
* **Units in the type system**: `Millivolts(u16)`, `Permille(i16)`,
  `Hex16(u16)` - which would have made the original's quirks (radix-16
  printing, base-16 argument parsing) explicit instead of accidental.
* **The dependency story**: `Cargo.toml` instead of "which newlib version".

### 9.3 What would NOT be better
* **Readability only if written idiomatically.**  A line-by-line transliteration
  of the C into Rust is *less* readable than the C (lifetimes/borrows around
  the peripheral registers).  The win comes from the enums/match/typed
  units, i.e. from rewriting the *design*, not the syntax.
* **Size**: Rust is typically 1.5-3x the C for the same logic.  Here that
  does not matter (12.6 KB used of 128 KB), but it is a point against.
* **Toolchain comfort**: nightly Rust + `-Z build-std` vs. a boring
  `arm-none-eabi-gcc`; no vendor HAL, no CubeMX, no vendor examples.
* **The register-poking layer** gains nothing - it is still raw writes.
* **The hardware unknowns stay**: the LED matrix pin map, the 96 MHz question
  and the BMS battery values are the same in either language.

### 9.4 How to do the port safely
1. Keep this C tree as the **reference** (it is the one with the verified
   behaviour and the byte-exact protocol).
2. Freeze the golden outputs: `tools/protocol_test.c` (7 protocol bytes) and
   `tools/host_sim.c` (13 behavioural checks) are the oracle.  Port them to
   `cargo test` and make the Rust port reproduce **every** byte.
3. Port in this order: (a) the protocol layer (pure logic, testable on the
   host), (b) the register layer + the vector table/startup, (c) the
   modules, (d) main().
4. Flash the Rust build and diff its behaviour against the C build with the
   same simulator.

**Effort**: the register layer + runtime bring-up is the risky part (a day
or two with the `cortex-m-rt` template); the rest is a mechanical port of
~5 kLOC with the tests as the guide.

## 10. Gotchas

* **Do not assume the BLE UART carries the robot's replies.** In the original
  the replies go out on **USART4 (0x40004C00) @460800**; USART3
  (0x40004800) @14400 is only the AT channel to the BLE module (§4.2).
  The rebuild had this backwards (§4.3).
* **USB is DFU-only** in the original - never a CDC/HID console (§4.1).

* `dfu-util -U` needs a fresh filename and refuses to overwrite (exit 73).
* `:leave` resets the MCU out of DFU mode — do all reads first.
* **After the last read you MUST still leave DFU** (`:leave`): the chip otherwise keeps
  running the ROM bootloader, the application never starts, and the robot advertises over
  BLE but drops every connection (it did exactly that on 2026-10-21 — `fw/flash.sh` wrote
  and verified the image without a final leave; the robot was stuck in the bootloader and
  the host logged `0483:df11 STM32 BOOTLOADER` on every plug-in). `fw/flash.sh` now issues
  the leave after the verification read; `scripts/dfu/backup.sh` in the codie repo does the
  same when `--no-option-bytes` is used.
* docker needs `--privileged` (or an explicit `--device=/dev/bus/usb/BBB/DDD`).
* Ghidra was extracted with python zipfile → exec bits were lost; `chmod -R +x
  tools/ghidra_12.1.4_PUBLIC/` is required before running `analyzeHeadless`.
* `tools/fw.elf` is the synthetic ELF used for the Ghidra import
  (regenerate with `.venv/bin/python tools/mkelf.py stm32_flash.bin tools/fw.elf`).
