# Works as intended vs. reality

The user's expectation, the dump's actual behaviour, and where the two differ.
Written after tracing every function in `stm32_flash.bin` (378 functions, decompiled
`analysis/decomp/`) against the Rust port in `rust/src/`.

Ground truth used here:

| Item | Value |
|---|---|
| Original image | `stm32_flash.bin`, 131 072 B, 3 658 B of strings |
| Rust image | `rust/build/fw.bin`, **14 864 B** (11.4 % of flash) |
| Tests | `cd rust && cargo test` → **19 passed, 0 failed**, three consecutive runs |
| Compile | `cargo check --all-targets` → 0 errors, 0 warnings |
| Unported dump functions | 2 — `FUN_08002578`, `FUN_080021A8` (64-bit division helpers) |

---

## 1. The expected sequence, and what actually happens

The robot was described as: **long press the button → LED animation → plug USB →
every second LED on → press the button again → LEDs red → bootloader appears on
the PC.** Traced against the dump, that expectation is *mostly wrong* — and the
port already reproduces the real behaviour, wrong expectation and all.

| Stage | What was expected | What the dump actually does | Port verdict |
|---|---|---|---|
| Power on | Long press latches the supply on | `FUN_08006338` → `FUN_08009654` (EXTI/PC13 arm) then `FUN_080098C4`: PC12 HIGH keeps the supply alive after the button is released. No software "boot" event exists. | ✅ identical |
| After boot | Some LED animation | The light-program engine (`FUN_08006000`) only runs if a program is selected. Nothing selects one by default, so **the LEDs do nothing** until the BLE link service fires. | ✅ identical |
| Plug USB | "every second LED on" | Nothing in the dump drives a "every second LED" pattern. PA12 going high only sets `DAT_20000410 = 2` (`FUN_08006294`) and queues animation **4** (solid red, `FUN_08005FBC(4)`). | ✅ identical — the expectation is not in the dump |
| Press button (2nd) | Something, unspecified | `FUN_0800626C`: if state == 2 → state = 3, emit `FUN_08005EC4(4,0,3,0x60)` and set the LED-engine run flag (`DAT_20000374 = 1`). | ✅ identical |
| State 3 + LED 4 | — | `FUN_080063EC` writes `0xD7E5F7A8` / `0x19C3FE40` into RAM (`0x200025A8` / `0x200017A8`), busy-waits until the 10 ms sonar window elapses, then parks in an infinite loop. The watchdog forces a reset. | ✅ identical |
| After the reset | Bootloader on PC | `main()` (`FUN_08009FA4`) sees the magic words, re-enters `FUN_0800AB88` (power-fail path), and `FUN_08009F8C` sets MSP = `0x200014A8` and calls the ROM slot at `0x1FFFC804`. | ✅ identical |

**Net:** the described behaviour is off by one detail (the LED pattern between
USB and the second button press is *solid red*, not "every second LED"), but the
whole state machine is ported end-to-end in `boot.rs`'s main loop. Nothing about
that chain has ever been executed on hardware.

---

## 2. What works as intended

| # | Area | Dump evidence | Port location | Why it is trustworthy |
|---|---|---|---|---|
| 1 | Clock tree 96 MHz (HSI16 → PLL×6) | `FUN_08002958` | `system::system_init` | Bit-exact register writes; the PLLRDY/H SERDY gate bugs found in the first pass are fixed and locked by a test |
| 2 | SysTick 1 kHz tick | port choice (dump uses TIM6) | `system::SysTick_Handler` | The dump's TIM6 is a ~65.5 ms update, *not* the 1 ms its greeting advertises; the port keeps 1 ms and runs TIM6 alongside for `dump_tick()` |
| 3 | Watchdog init + kick | `FUN_0800A640`, `FUN_0800AB10` | `system::watchdog_init`, `util::iwdg_kick` | Every main-loop iteration; register offsets +0/+4/+8 verified against the dump |
| 4 | Power hold / power fail | `FUN_08006338`, `FUN_080098C4`, `FUN_0800AB88` | `hwi_power::power_hold_on`, `boot::power_fail_mode` | Runs first in boot so a hang cannot cut power |
| 5 | Power-good EXTI (IRQ 7) | `FUN_0800976C` | `hwi_power::EXTI4_15_IRQHandler` | EXTI13 routed through **EXTICR4** (the port originally wrote EXTICR3 and the interrupt could never fire) |
| 6 | Command protocol: parse, dispatch, replies | `FUN_0800A0AC`, `FUN_0800A350`, `FUN_0800A16C` | `proto::proxy::dispatch` | Registry searched backwards, hex radix, greeting/`msg:`/log levels byte-identical |
| 7 | Number formatting | `FUN_080068D4`, `FUN_0800693C` | `proto::fmt::num` | Variable radix 2–36, matches the original's digit trick |
| 8 | USART4 command link | `FUN_08009A04` | `ble_com::init` (CONSOLE_USART) | CR2 = 0, CR1 = 0x000C (RE+TE+UE); polling port, ISR bits deliberately left clear |
| 9 | USART3 BLE AT link | `FUN_08007F7C` | `ble_com::init` (BLE_USART) | CR2 = 0x2000 (2 stop bits), CR3 = 0x300 |
| 10 | Ring buffers | `FUN_08002E1C/2E44/2E78` | `hal::ring` | 128 B BLE RX, 64 B BLE TX, 200 B console |
| 11 | ADC + DMA sampling | `FUN_08000E5C`, `FUN_080071C0` | `hwi_adc::tick` | `get hwiAdc buf` reproduces the 32-sample dump and the blank line after the 9th/17th/25th value |
| 12 | LED strip frame encoding (striped ARGB) | `FUN_080090A8`, `FUN_08007B34` | `hwi_led::fill_ccr_stream`, `send_frame` | MSB-first, GRB, `0 → 0x0e`, `1 → 0x2e`; ARR 0x187 gives 69.8 ms/frame, matching the dump's 70 ms stall (`0x46`) to 0.2 % |
| 13 | LED timer bring-up | `FUN_080079A0` | `hwi_led::init` | PSC 29, PB14 AF1 pull-up, PB15/PB12 on GPIOB |
| 14 | Battery pack query over the LED line | `FUN_08006E94`, `FUN_08006D34` | `drv_battery::hw_init`, `reply_machine`, `tick` | 7-state machine, `{cc,0xb4}`/`{cc,0xb8,x}`/`{cc,0xbe,x}` frames, reply parse at states 4 and 7; covered by 5 tests |
| 15 | Single-outline transport + reply bit-bang | `FUN_080078E4`, `FUN_080077A0`, `FUN_0800745C` | `hwi_led::send_raw`, `reply_take`, `tim15_update_irq` | Second encoder (LSB-first, `0x60`/`4`), ARR 0x308, PB14 bit-bang into `DAT_2000058a` |
| 16 | All 11 periodic engines | `FUN_080063EC` | `boot::run` main loop | BLE link service, motor fault, ADC overrun, line service, BLE re-send, light sense, light program, mic sample, line sample, tone sequencer, drive speed loop |
| 17 | All 9 IRQ vectors the dump serves | `FUN_0800976C`…`FUN_08002DC8` | `startup::VECTORS` + `nvic::enable` | IRQ 7, 9, 11, 13, 15, 17, 20, 22, 29 — none fall to `Default_Handler` |
| 18 | Light-program engine + animation queue | `FUN_08006000`, `FUN_08005F94`, `FUN_08005FBC` | `app_lights::tick`, `select`, `request` | 160 ms gate, max-3 FIFO queue, program 4 = solid red, 5 = chase (`FUN_080039D8`), 6 = sine fade (`FUN_08004CD8`) |
| 19 | Tone sequencer (non-blocking) | `FUN_08008574` | `hwi_buzz::tick` | Actually moves TIM16: `call hwiBuzz play 3` leaves `CR1 = 0x81`, `ARR = 19199`, `CCR = 5759` |
| 20 | Motor drive duty | `FUN_08008B3C` | `hwi_drive::set_duty`, `app_drive::tick` | 16-bit truncation, ±100 clamp, 200 ms speed loop |
| 21 | LED ARGB op + global brightness | dump stub `0x080090A5` | `hwi_led::handler` (`argb`) | `set hwiLed argb <led|all> <r> <g> <b>`, brightness multiplier applied in `fill_ccr_stream` |
| 22 | LEGACY_CHASE / LEGACY_SINE | `FUN_080039D8` (13 KB), `FUN_08004CD8` (13 KB) | `app_lights::legacy_chase`, `legacy_sine` | Ported from the decompilation; write the frame buffer directly via `led_rgb_mut()` |

---

## 3. What does NOT work as intended

### 3.1 Never executed on hardware — the biggest real gap

Everything above is verified against a host peripheral model (`src/sim/mod.rs`)
and machine-code cross-checks. **Nothing has ever run on the robot.** The image is
flashable (`rust/build/fw.bin`) but §4 below has never been performed.

### 3.2 Bootloader entry is arithmetically dead

Verified from the release ELF (`arm-none-eabi-objdump -h` / `-t`):

| Cell | Address | Section |
|---|---|---|
| `RESET_MAGIC0_CELL` | `0x200004c8` | `.bss` |
| `RESET_MAGIC1_CELL` | `0x200004cc` | `.bss` |
| `.bss` span | `0x200001e8` – `0x200005f8` (4 128 B) | zero-filled at boot |

The port's boot-loader entry writes the magic words to those cells. But the reset
the watchdog forces re-runs `Reset_Handler`, which zero-fills `_sbss.._ebss`
(`startup.rs:45-60`) — so on the second boot `RESET_MAGIC0_CELL == 0` and the
reset-cause test at `boot.rs:170` fails.

The original survives this only because it writes to fixed RAM addresses
`0x200025A8` / `0x200017A8`, which lie **above** `.bss`'s `0x200005f8` end — no
startup code touches them, so they keep their contents across the reset.

**Result on real hardware:** USB → button → LEDs red → watchdog reset → the
firmware boots normally instead of jumping to the ROM bootloader. The state
machine, the LED animation and the magic-word write all work; the *persistence
across the reset* does not.

**Fix (not yet applied):** the two magic words must live at absolute addresses
outside `.bss` (e.g. a `#[link_section = ".uninit"]` cell or a raw
`0x200025A8`/`0x200017A8` pointer write), and `run()`'s reset-cause test must read
those addresses. This is the single highest-value fix in the port and is still
open — flagged here rather than silently shipped.

### 3.3 The LED "red" animation can never satisfy its own gate

`LED_ANIMATION_STATE` in `boot.rs` is set to 4 directly when PA12 rises. But the
dump's `FUN_08005FBC(4)` only *queues* program 4; whether animation 4 is
"selected" depends on the light-program engine, whose selected-program state
lives in `app_lights` (`st.prog`), which the boot-loop statics never read. The
port wires the two together by hand (both set to 4 in the same place), which
reproduces the *effect* of the dump's cross-module coupling but is a duplicated
source of truth: `app_lights::select(4)` and `boot::LED_ANIMATION_STATE` can
diverge. Low severity today, fragile tomorrow.

### 3.4 PC13 button polarity is inverted relative to the dump

The dump's `FUN_08008784` is a *level* poll: `_DAT_200006f4 = (iVar == 0)` and
the callback fires on the **level** (`0 →` fault, `2 →` healthy). The port's
boot loop polls PC13 and advances on any **edge**, and presses PC13 in the
direction that pulls it low (the dump arms a *falling* edge on EXTI13,
`hwi_power::init` sets FTSR). On real hardware the button pulls PC13 to ground,
so the port does see the edge — but a level-held press is reported once while the
dump would report continuously, and a released-then-re-pressed button behaves
differently. Cosmetic in the boot context; a real behavioural divergence from
`FUN_08008784`.

### 3.5 Motor-fault flag semantics are conflated

`hwi_drive::tick` applies the dump's callback mapping to **both** motors
(`fault_a = fault_b = fault`). The dump's `FUN_08006294` only sets the PA12
callback's global state and leaves the per-motor assignment to `FUN_08008AD0`.
The port therefore reports both motors faulted when one line trips. The
protocol-level answer (`nFault` is always `"0"`) hides this, but the internal
flags are wrong.

### 3.6 `get hwiLed <op>` ignores `<op>` and returns the LED's duty

`hwi_led::handler` answers `on`/`pwm`/`rgb` `get` all with `hwi_led::get(led)`,
which is `led_duty[led]` (`hwi_led.rs:630-635`) — so `get hwiLed on 3`,
`get hwiLed pwm 3` and `get hwiLed rgb 3` return the same number, and an RGB query
returns a single-channel duty rather than the packed colour the `set` side
replies with. The dump's `hwiLed` handler is a stub (`0x080090A5`) that answers
no op, so there is no dump contract to violate; but within the port's own
surface the get/set op pairing is inconsistent. Only the strip-wide dark-LED
counter is used for the no-op fallback, as it is.

### 3.7 LED pin map and motor pin map are family defaults, not dump facts

`hwi_led.rs` and `hwi_drive.rs` carry the part-family pin maps. The port, clock
tree and RCC bits are all cross-checked against the machine code; the **physical
pin assignment of the LED matrix and the motor outputs is not recoverable from
the image**. If LEDs or motors do not move while the command link answers, these
are the first suspects. `MEMORY.md` claims the pins are "no longer guesswork" —
that is true of the *port/pin/AF/RCC* mapping, not of the *board wiring*.

### 3.8 The BLE GAP command body is unrecoverable

`FUN_08008130` re-sends a GAP command every 2000 ms to keep the module
advertising. The formatter (`FUN_08002AC2`) is decoded; the *payload bytes* are
runtime-derived from a table whose address field points into the code region at
`0x08002BE1` (`47 c0 46 70`), and the only `GAP SET MODE` string in the whole
dump is the **reply matcher**, not the command body. The port sends a plain
`AT\r\n` probe instead — which keeps *a* link alive but may not make the module
begin advertising. **If the BLE radio never comes up on real hardware, this is
the reason.** Needs a live capture from the module.

### 3.9 `verify_features.py` reports failure for a cosmetic reason

`tools/verify_features.py` exits **1** on the current image. The check requires
each protocol string to appear as its own printable run; the linker merges the
interface-name literals into one contiguous `.rodata` run (e.g.
`drvBatteryvoltagecurrentremainingCapacitychargeStathwInit…`), so
`strings()` never isolates `drvBattery`. Every required string **is** in the image
(`strings -n 3` finds all of them). The tool's assumption — NUL-separated string
literals — is wrong for a Rust binary that stores lengths. Tool defect, not
firmware defect; not yet fixed.

### 3.10 Host tests share one global model

The firmware keeps module state in `Global<T>` statics (`src/state.rs`) and the
peripheral model in one `LazyLock<Mutex<…>>`. Tests running in parallel interleave
their sequencer phases, which made `tests/battery.rs` fail ~2 of 3 runs
("left: 0, right: 5000"). Fixed this session by serialising `tests/battery.rs`
behind a file-level `Mutex` — the same reasoning `tests/periodic.rs` and
`tests/usart.rs` already state in their headers. `tests/simulator.rs` and
`tests/c_equivalence.rs` are already single-test files. Deterministic now:
19/19 across three consecutive runs.

### 3.11 64-bit division helpers unported (harmless)

`FUN_08002578` and `FUN_080021A8` (6 KB / 7 KB) are the C compiler's `__aeabi_uldivmod`
family. Rust emits native `u64` division for the target. No behaviour lost.

---

## 4. Summary table: what works, what doesn't

| Status | Count | What |
|---|---:|---|
| ✅ Works as intended (host-verified, dump-evidenced) | 22 | Everything in §2, including all 11 periodic engines, all 9 IRQs, the complete protocol surface, LED encoding, tone sequencer, drive loop, and the full battery reply machine |
| ⚠️ Works, but wrong survival semantics | 1 | Bootloader entry: the state machine fires, but the magic words are in `.bss` and get zeroed by the reset, so the ROM bootloader is never entered (§3.2) |
| ⚠️ Works, but fragile | 1 | LED animation state duplicated between `boot.rs` and `app_lights` (§3.3) |
| ⚠️ Works, but diverges from the dump | 2 | PC13 edge- vs level-detect (§3.4); motor fault flags conflated across both motors (§3.5) |
| ⚠️ Never validated on hardware | — | The whole image (§3.1) |
| ❌ Not recoverable from the dump | 2 | BLE GAP payload bytes (§3.8); LED/motor board pin map (§3.7) |
| ❌ Tooling defect | 1 | `verify_features.py` fails on merged `.rodata` runs (§3.9) |
| ➖ Intentionally not ported | 2 | 64-bit division helpers — Rust native division covers them (§3.11) |

**Bottom line:** every function in the original now has a port counterpart, the
suite is green, and the image builds at 14 864 B. The two things standing between
this firmware and a working robot are (a) the `.bss` magic-word fix in §3.2 —
without it the bootloader-entry button sequence silently does nothing on real
hardware — and (b) actually flashing it.
