# HAL parity review - Rust port vs. the C reference

Read-only review of the shared layers of the Rust port against the C reconstruction
and, where the C is itself a reconstruction, against the ground truth
(`analysis/decomp/ALL.c` and the machine code in `stm32_flash.bin`).

Reviewed:

| Rust | C |
| --- | --- |
| `rust/src/hal/*.rs` | `fw/modules/hwi_a_hal.c`, `fw/modules/hwi_b_hal.c` (+ both headers) |
| `rust/src/proto/proxy.rs`, `rust/src/proto/fmt.rs` | `fw/modules/proxy.c` |
| `rust/src/system.rs` | `fw/src/system_stm32g0.c` |
| `rust/src/mmio.rs` | the `REG32` / `REG16` macros of the C |
| `rust/src/state.rs` | the file scope variables of the C |

Nothing was modified. Line numbers refer to the files as they are now.

## Method

Every C function was read next to its Rust counterpart and compared on
(a) the register offsets and bit masks, (b) the order of the register accesses,
(c) the arithmetic (rounding, masks, shifts, wrap around) and
(d) the behaviour at the edges (empty argument, zero value, out of range value).

Where the C is a reconstruction, the expectation was taken from the decompilation
in `analysis/decomp/ALL.c` and, when the decompilation and the machine code
disagreed, from the machine code itself:

```
tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin/arm-none-eabi-objdump -D -b binary \
    -m arm -M force-thumb --adjust-vma=0x08000000 \
    --start-address=0x0800a964 --stop-address=0x0800aa96 stm32_flash.bin
```

**Note on the decompilation:** the Ghidra output in `analysis/decomp/ALL.c` is
wrong for `FUN_0800A964` (it uses the wrong shift variable for the `PUPDR` store of
the odd pin block and it mis-transcribes the `OTYPER` shifts). The machine code is
the truth, and it is used below. Where this document says "the original", it means
the machine code of `stm32_flash.bin`.

## Findings that look like real bugs, most important first

1. **`resp_value()` always emits the separator and the value** -
   `rust/src/proto/proxy.rs:115`. The C omits `" "` + value when the value is
   `NULL`; the Rust has no way to say "no value" and always writes
   `" <value>"`. Byte level difference on the command link. (F-1)
2. **`gpio::set_afr()` masks the alternate function to 4 bits** -
   `rust/src/hal/gpio.rs:142`. The C `gpio_set_afr()` of `hwi_a_hal.c` does not
   mask, and neither does the original (`FUN_0800AAC8`); the Rust therefore
   behaves like the *other* C helper (`hwi_gpio_set_af()`), which does mask.
   (F-2)
3. **Debug builds panic where the C wraps around** - `rust/src/system.rs:114`
   (`reload - 1` with `reload == 0`), `rust/src/hal/util.rs:78` (`-v` for
   `i32::MIN`), `rust/src/hal/timer.rs:122` and `:197` (shift/overflow with
   `ch == 0`). The C wraps (or is undefined) and never aborts. (F-3)
4. **`resp_greeting()` always writes the closing quote** -
   `rust/src/proto/proxy.rs:161`. The C skips the closing `"` when the value
   pointer is `NULL`. (F-4)
5. **Inherited from the C, not a Rust bug: the `OTYPER` handling of
   `gpio_config()` does not match the original for even pins** - the original
   shifts the output type by `2 * pin` for even pins and by `pin` for odd pins.
   The C (and therefore the Rust) uses `pin` for every pin. (F-5)
6. **Inherited from the C, not a Rust bug: `gpio_config()` writes the four
   registers once at the end, the original writes them per pin.** The number and
   the order of the register accesses therefore differ from the original (the
   final register values are the same). (F-6)
7. **Inherited from the C, not a Rust bug: the special USART mode of
   `FUN_0800B3FC` is missing** (the original switches to a 2x clock and an
   oversampling-16 divisor when `CR1` bit 15 is set). (F-7)

Details, with both code snippets, are in the "Findings" section below.

---

# Parity table

## `fw/modules/hwi_a_hal.c` -> `rust/src/hal/`

| C function | C file | Rust counterpart | Rust file | state |
| --- | --- | --- | --- | --- |
| `gpio_config` | `hwi_a_hal.c:30` | `gpio::config` | `hal/gpio.rs:80` | same (F-5, F-6) |
| `gpio_set_afr` | `hwi_a_hal.c:61` | `gpio::set_afr` | `hal/gpio.rs:139` | **different** (F-2) |
| `gpio_read` | `hwi_a_hal.c:70` | `gpio::read` | `hal/gpio.rs:148` | same |
| `gpio_write` | `hwi_a_hal.c:76` | `gpio::write` | `hal/gpio.rs:168` | same (0x28 quirk kept) |
| `timer_is_advanced` (static) | `hwi_a_hal.c:86` | `timer::is_advanced` | `hal/timer.rs:79` | same |
| `timer_base_cfg` | `hwi_a_hal.c:93` | `timer::base_cfg` | `hal/timer.rs:84` | same |
| `timer_prescaler_egr` | `hwi_a_hal.c:115` | `timer::prescaler_egr` | `hal/timer.rs:107` | same |
| `timer_arpe` | `hwi_a_hal.c:122` | `timer::arpe` | `hal/timer.rs:113` | same |
| `timer_ocpe` | `hwi_a_hal.c:132` | `timer::ocpe` | `hal/timer.rs:119` | same (edge F-3) |
| `timer_channel_cfg` | `hwi_a_hal.c:146` | `timer::channel_cfg` | `hal/timer.rs:131` | same |
| `timer_pwm_set` | `hwi_a_hal.c:212` | `timer::pwm_set` | `hal/timer.rs:196` | same (edge F-3) |
| `timer_ccer` | `hwi_a_hal.c:218` | `timer::ccer` | `hal/timer.rs:201` | same |
| `timer_enable` | `hwi_a_hal.c:229` | `timer::enable` | `hal/timer.rs:207` | same |
| `timer_moe` | `hwi_a_hal.c:238` | `timer::moe` | `hal/timer.rs:212` | same |
| `timer_dier` | `hwi_a_hal.c:247` | `timer::dier` | `hal/timer.rs:217` | same |
| `timer_sr_clear` | `hwi_a_hal.c:257` | `timer::sr_clear` | `hal/timer.rs:223` | same |
| `timer_egr` | `hwi_a_hal.c:263` | `timer::egr` | `hal/timer.rs:228` | same |
| `timer_get16` | `hwi_a_hal.c:268` | `timer::get16` | `hal/timer.rs:233` | same |
| `timer_set16` | `hwi_a_hal.c:269` | `timer::set16` | `hal/timer.rs:238` | same |
| `timer_get32` | `hwi_a_hal.c:270` | `timer::get32` | `hal/timer.rs:243` | same |
| `timer_set32` | `hwi_a_hal.c:271` | `timer::set32` | `hal/timer.rs:248` | same |
| `adc_cfg1` | `hwi_a_hal.c:276` | `adc::cfg1` | `hal/adc.rs:14` | same |
| `adc_channel_add` | `hwi_a_hal.c:285` | `adc::channel_add` | `hal/adc.rs:21` | same |
| `adc_dmngt` | `hwi_a_hal.c:292` | `adc::dmngt` | `hal/adc.rs:27` | same |
| `adc_dma_mode` | `hwi_a_hal.c:300` | `adc::dma_mode` | `hal/adc.rs:33` | same |
| `adc_enable` | `hwi_a_hal.c:311` | `adc::enable` | `hal/adc.rs:43` | same |
| `adc_stop` | `hwi_a_hal.c:322` | `adc::stop` | `hal/adc.rs:53` | same |
| `adc_start` | `hwi_a_hal.c:329` | `adc::start` | `hal/adc.rs:60` | same |
| `adc_ready` | `hwi_a_hal.c:341` | `adc::ready` | `hal/adc.rs:72` | same |
| `dma_channel_cfg` | `hwi_a_hal.c:357` | `dma::channel_cfg` | `hal/dma.rs:10` | same |
| `dma_channel_enable` | `hwi_a_hal.c:367` | `dma::channel_enable` | `hal/dma.rs:18` | same |
| `dma_channel_reset` | `hwi_a_hal.c:376` | `dma::channel_reset` | `hal/dma.rs:23` | same |
| `dma_set_cntdr` | `hwi_a_hal.c:396` | `dma::set_cntdr` | `hal/dma.rs:48` | same |
| `dma_set_dir` | `hwi_a_hal.c:402` | `dma::set_dir` | `hal/dma.rs:53` | same |
| `dma_flag` | `hwi_a_hal.c:408` | `dma::flag` | `hal/dma.rs:58` | same |
| `dma_clear_flags` | `hwi_a_hal.c:414` | `dma::clear_flags` | `hal/dma.rs:63` | same |
| `rcc_ahb_enable` | `hwi_a_hal.c:422` | `rcc::ahb_en` | `hal/rcc.rs:17` | same |
| `rcc_apb2_enable` | `hwi_a_hal.c:431` | `rcc::apb2_en` | `hal/rcc.rs:22` | same |
| `rcc_apb1_enable` | `hwi_a_hal.c:440` | `rcc::apb1_en` | `hal/rcc.rs:27` | same |
| `nvic_enable` | `hwi_a_hal.c:450` | `nvic::enable` | `hal/nvic.rs:9` | same |

## `fw/modules/hwi_b_hal.c` -> `rust/src/hal/`

| C function | C file | Rust counterpart | Rust file | state |
| --- | --- | --- | --- | --- |
| `hwi_gpio_config` | `hwi_b_hal.c:66` | `gpio::config_per_pin` | `hal/gpio.rs:112` | same (F-5, F-6) |
| `hwi_gpio_set_af` | `hwi_b_hal.c:94` | `gpio::set_afr` | `hal/gpio.rs:139` | same |
| `hwi_gpio_read` | `hwi_b_hal.c:104` | `gpio::read` | `hal/gpio.rs:148` | same |
| `hwi_gpio_set` | `hwi_b_hal.c:110` | `gpio::set` | `hal/gpio.rs:153` | same |
| `hwi_gpio_reset` | `hwi_b_hal.c:115` | `gpio::reset` | `hal/gpio.rs:158` | same |
| `hwi_rcc_ahb_en` | `hwi_b_hal.c:123` | `rcc::ahb_en` | `hal/rcc.rs:17` | same |
| `hwi_rcc_apb2_en` | `hwi_b_hal.c:132` | `rcc::apb2_en` | `hal/rcc.rs:22` | same |
| `hwi_rcc_apb1_en` | `hwi_b_hal.c:141` | `rcc::apb1_en` | `hal/rcc.rs:27` | same |
| `hwi_tim_pwm_init` | `hwi_b_hal.c:154` | `timer::pwm_init` | `hal/timer.rs:257` | same |
| `hwi_tim_pwm_duty` | `hwi_b_hal.c:175` | `timer::pwm_duty` | `hal/timer.rs:271` | same |
| `hwi_tim_ccr` | `hwi_b_hal.c:180` | `timer::ccr` | `hal/timer.rs:276` | same |
| `hwi_tim_set_arr` | `hwi_b_hal.c:185` | `timer::set_arr` | `hal/timer.rs:281` | same |
| `hwi_tim_start` | `hwi_b_hal.c:192` | `timer::start` | `hal/timer.rs:287` | same |
| `hwi_tim_stop` | `hwi_b_hal.c:197` | `timer::stop` | `hal/timer.rs:292` | same |
| `hwi_usart_init` | `hwi_b_hal.c:209` | `usart::init` | `hal/usart.rs:11` | same (F-7) |
| `hwi_usart_tx_ready` | `hwi_b_hal.c:232` | `usart::tx_ready` | `hal/usart.rs:28` | same |
| `hwi_usart_putc` | `hwi_b_hal.c:238` | `usart::putc` | `hal/usart.rs:33` | same |
| `hwi_usart_rx_ready` | `hwi_b_hal.c:244` | `usart::rx_ready` | `hal/usart.rs:38` | same |
| `hwi_usart_getc` | `hwi_b_hal.c:250` | `usart::getc` | `hal/usart.rs:43` | same |
| `hwi_usart_status` | `hwi_b_hal.c:255` | `usart::status` | `hal/usart.rs:48` | same |
| `hwi_usart_clear` | `hwi_b_hal.c:261` | `usart::clear` | `hal/usart.rs:53` | same |
| `hwi_usart_enable` | `hwi_b_hal.c:267` | `usart::enable` | `hal/usart.rs:58` | same |
| `hwi_usart_disable` | `hwi_b_hal.c:272` | `usart::disable` | `hal/usart.rs:63` | same |
| `hwi_ring_init` | `hwi_b_hal.c:280` | `ring::Ring::new` | `hal/ring.rs:18` | different (structure, see F-8) |
| `hwi_ring_put` | `hwi_b_hal.c:289` | `ring::Ring::put` | `hal/ring.rs:23` | same |
| `hwi_ring_get` | `hwi_b_hal.c:300` | `ring::Ring::get` | `hal/ring.rs:34` | same (return convention) |
| `hwi_ring_read` | `hwi_b_hal.c:311` | `ring::Ring::read` | `hal/ring.rs:45` | same |
| `hwi_ring_count` | `hwi_b_hal.c:323` | `ring::Ring::count` | `hal/ring.rs:57` | same |
| `hwi_ring_flush` | `hwi_b_hal.c:328` | `ring::Ring::flush` | `hal/ring.rs:62` | same |
| `hwi_millis` | `hwi_b_hal.c:338` | `util::millis` | `hal/util.rs:8` | same |
| `hwi_delay_ms` | `hwi_b_hal.c:347` | `util::delay_ms` | `hal/util.rs:18` | same |
| `hwi_iwdg_kick` | `hwi_b_hal.c:361` | `util::iwdg_kick` | `hal/util.rs:31` | same |
| `hwi_is_on` | `hwi_b_hal.c:367` | `util::is_on` | `hal/util.rs:36` | same |
| `hwi_atoi` | `hwi_b_hal.c:385` | `util::atoi` | `hal/util.rs:58` | same (edge F-3) |
| `hwi_parse_args` | `hwi_b_hal.c:411` | `util::parse_args` | `hal/util.rs:88` | same |

## `fw/modules/proxy.c` -> `rust/src/proto/`

| C function | C file | Rust counterpart | Rust file | state |
| --- | --- | --- | --- | --- |
| `op_word` (static) | `proxy.c:31` | `proxy::op_word` | `proto/proxy.rs:40` | same |
| `log_word` (static) | `proxy.c:36` | `proxy::log_word` | `proto/proxy.rs:50` | same |
| `iface_register` | `proxy.c:60` | `proxy::register` | `proto/proxy.rs:92` | same |
| `resp_value` | `proxy.c:84` | `proxy::resp_value` | `proto/proxy.rs:115` | **different** (F-1) |
| `resp_message` | `proxy.c:100` | `proxy::resp_message` | `proto/proxy.rs:130` | same |
| `resp_greeting` | `proxy.c:118` | `proxy::resp_greeting` | `proto/proxy.rs:147` | **different** (F-4) |
| `uitoa` (static) | `proxy.c:165` | `fmt::uitoa` | `proto/fmt.rs:62` | same |
| `fmt_num` | `proxy.c:187` | `fmt::num` | `proto/fmt.rs:82` | same |
| `fmt_num_s` | `proxy.c:194` | `fmt::num_s` | `proto/fmt.rs:92` | same |
| `fmt_int` | `proxy.c:215` | `fmt::int` | `proto/fmt.rs:106` | same |
| `fmt_fixed` | `proxy.c:238` | `fmt::fixed` | `proto/fmt.rs:120` | same |
| `tok_next` (static) | `proxy.c:284` | `proxy::Lexer::next_token` | `proto/proxy.rs:192` | same |
| `proxy_dispatch` | `proxy.c:314` | `proxy::dispatch` | `proto/proxy.rs:221` | same |

## `fw/src/system_stm32g0.c` -> `rust/src/system.rs`

| C function | C file | Rust counterpart | Rust file | state |
| --- | --- | --- | --- | --- |
| `SystemInit` | `system_stm32g0.c:63` | `system::system_init` | `system.rs:72` | same |
| `sys_tick_init` | `system_stm32g0.c:104` | `system::sys_tick_init` | `system.rs:112` | same (edge F-3) |
| `SysTick_Handler` | `system_stm32g0.c:112` | `system::SysTick_Handler` | `system.rs:125` | same |
| `g_tick_ms` (file scope) | `system_stm32g0.c:61` | `system::TICK_MS` | `system.rs:64` | same |
| `SYSC_RCC_BASE` / `SYSC_FLASH_BASE` | `system_stm32g0.c:44` | `addr::RCC` / `addr::FLASH` | `hal/addr.rs:87` | same |
| `SYSTICK_*` | `system_stm32g0.c:55` | `addr::SYSTICK` | `hal/addr.rs:94` | same |

## `rust/src/mmio.rs` vs. the `REG32` / `REG16` macros

| C | Rust | state |
| --- | --- | --- |
| `REG32(a)` (`hwi_a_hal.c:13`, `hwi_b_hal.c:14`, `system_stm32g0.c:42`) | `mmio::r32` / `mmio::w32` | same (volatile 32 bit access) |
| `REG16(a)` (`hwi_a_hal.c:14`, `hwi_b_hal.c:15`) | `mmio::r16` / `mmio::w16` | same (volatile 16 bit access) |
| - | `mmio::r8` / `mmio::w8` | extra (needed by `nvic_enable`) |
| `REG32(x) = REG32(x) \| m` | `mmio::rmw32` | same (read + write, two accesses) |
| - | `mmio::rmw16`, `mmio::rmw8` | extra |
| - | host build routes to `crate::sim` | by design, documented in the module header |

The only structural difference: the C macros are usable with any lvalue
(`*(volatile uint32_t *)(a)`), the Rust offers a fixed set of accessors. The
access width is the same everywhere: the C writes `REG16(port + 0x04)` for
`OTYPER` and the Rust writes `mmio::w16(port + GPIO_OTYPER, ..)`; the C writes
`REG32(ch + 0x04) = cntdr` (a 16 bit value stored as 32 bit) and the Rust writes
`mmio::w32(ch + 0x04, cntdr as u32)`.

## `rust/src/state.rs` vs. the file scope variables of the C

| C variable | C file | Rust | Rust file | state |
| --- | --- | --- | --- | --- |
| `g_iface[16]`, `g_iface_count` | `proxy.c:57` | `IFACES`, `IFACE_COUNT` | `proto/proxy.rs:78` | same |
| `g_tick_ms` | `system_stm32g0.c:61` | `TICK_MS` | `system.rs:64` | same |
| - | `hwi_a_hal.c` | no file scope variable | - | same |
| - | `hwi_b_hal.c` | no file scope variable | - | same |

`state::Global<T>` is the Rust equivalent of a C file scope static: it hands out
`&mut T` from a `&self`, so the aliasing rules are as relaxed as in the C (the
firmware is single threaded, the interrupts only touch their own state).
`Global` is not used for `TICK_MS` and `IFACE_COUNT` (they are atomics), which is
the Rust equivalent of the C's `volatile` on `g_tick_ms`.

---

# Findings

## F-1 - `resp_value()` always writes the separator and the value (real bug, wire format)

`fw/modules/proxy.c:84`

```c
void resp_value(uint8_t op, const char *iface, const char *opname, const char *value)
{
    out_str(op_word(op));
    out_char(' ');
    out_str(iface);
    out_char(' ');
    out_str(opname);
    if (value != 0) {
        out_char(' ');
        out_str(value);
    }
    out_char('\n');
}
```

`rust/src/proto/proxy.rs:115`

```rust
pub fn resp_value(op: u8, iface: &str, opname: &str, value: &str) {
    out_str(op_word(op));
    out_char(b' ');
    out_str(iface);
    out_char(b' ');
    out_str(opname);
    out_char(b' ');
    out_str(value);
    out_char(b'\n');
}
```

**Difference:** the C skips `" "` *and* the value when `value == NULL`; the Rust
has no "no value" state and always writes `" " + value`. A value-less reply is
`"get hwiLed state\n"` in the C and `"get hwiLed state \n"` in the Rust (a
trailing blank).

**Impact:** latent. No call site in `fw/modules/*.c` passes `NULL` (all 30 call
sites pass a buffer), and the Rust call sites pass `buf.as_str()` /
`val.as_str()` / a literal, so the bytes on the link are identical today. The
difference is in the API: the Rust cannot express a value-less reply.

**Fix:** change the parameter to `Option<&str>` and guard the two writes, e.g.

```rust
pub fn resp_value(op: u8, iface: &str, opname: &str, value: Option<&str>) {
    ...
    if let Some(value) = value {
        out_char(b' ');
        out_str(value);
    }
    out_char(b'\n');
}
```

## F-2 - `gpio::set_afr()` masks the alternate function to 4 bits (real bug, differs from the C)

`fw/modules/hwi_a_hal.c:61` (the `hwi_a_hal` flavour)

```c
void gpio_set_afr(uint32_t port, uint8_t pin, uint8_t afr)
{
    uint32_t shift = (uint32_t)(pin & 7u) * 4u;
    uint32_t reg   = port + 0x20u + (uint32_t)(pin >> 3) * 4u;

    REG32(reg) = (REG32(reg) & ~(0xFu << shift)) | ((uint32_t)afr << shift);
}
```

`rust/src/hal/gpio.rs:139` (one function for both C flavours)

```rust
pub fn set_afr(port: u32, pin: u8, afr: u8) {
    let shift = (pin & 7) as u32 * 4;
    let reg = port + GPIO_AFR0 + (pin as u32 >> 3) * 4;
    mmio::rmw32(reg, |v| (v & !(0xFu32 << shift)) | ((afr as u32 & 0xF) << shift));
}
```

**Difference:** the Rust masks `afr` with `0xF`, the C `gpio_set_afr()` does not.
The Rust therefore follows the *other* C helper, `hwi_gpio_set_af()`
(`fw/modules/hwi_b_hal.c:94`), which does mask:

```c
    REG32(reg) = (REG32(reg) & ~(0xFu << shift)) | ((uint32_t)(af & 0xFu) << shift);
```

The original does **not** mask either (`FUN_0800AAC8`, machine code at
0x0800AAC8): `*(uint *)(iVar1 + param_1) = param_3 << iVar2 | *(uint *)(iVar1 + param_1);`
- `param_3` (the alternate function) is shifted in unmasked, so a value > 15
overwrites the next pin's field as well.

**Impact:** latent. Every call site passes `afr <= 4`
(`hwi_drive` 0, `hwi_enc` 2, `hwi_sonar` 2, `hwi_buzz` 2, `hwi_led` 2,
`bleCom` 0/1/4), so the produced bytes are identical today.

**Fix:** drop the `& 0xF` (to match `hwi_a_hal.c`) or add a second function
`set_afr_raw()` for the `hwi_a_hal` call sites and keep the masked one for the
`hwi_b_hal` call sites.

## F-3 - Debug builds panic where the C wraps around (real bug at the edges)

### F-3a - `sys_tick_init(0)`

`fw/src/system_stm32g0.c:104`

```c
void sys_tick_init(uint32_t reload)
{
    SYSTICK_CTRL = 0u;
    SYSTICK_LOAD = reload - 1u;      /* reload == 0 -> 0xFFFFFFFF */
    SYSTICK_VAL  = 0u;
    SYSTICK_CTRL = 7u;
}
```

`rust/src/system.rs:112`

```rust
pub fn sys_tick_init(reload: u32) {
    mmio::w32(SYSTICK + SYSTICK_CTRL, 0);
    mmio::w32(SYSTICK + SYSTICK_LOAD, reload - 1);   // panics in a debug build
    mmio::w32(SYSTICK + SYSTICK_VAL, 0);
    mmio::w32(SYSTICK + SYSTICK_CTRL, 7);
}
```

`reload - 1` is a *checked* subtraction: with `reload == 0` a debug build
aborts with "attempt to subtract with overflow", the C wraps to `0xFFFFFFFF`.
A release build wraps as well, so the firmware is unaffected, but a host test
(`cargo test` builds with overflow checks on) that calls `sys_tick_init(0)`
panics. Fix: `reload.wrapping_sub(1)`.

### F-3b - `atoi("-2147483648")`

`fw/modules/hwi_b_hal.c:385`

```c
int hwi_atoi(const char *s)
{
    int v = 0;
    int neg = 0;
    ...
    while ((*s >= '0') && (*s <= '9')) {
        v = (v * 10) + (int)(*s - '0');
        s++;
    }
    return neg ? -v : v;
}
```

`rust/src/hal/util.rs:58`

```rust
pub fn atoi(s: &str) -> i32 {
    ...
    while i < b.len() && b[i].is_ascii_digit() {
        v = v.wrapping_mul(10).wrapping_add((b[i] - b'0') as i32);
        i += 1;
    }
    if neg {
        -v                    // panics in a debug build for i32::MIN
    } else {
        v
    }
}
```

The accumulation wraps (as documented), but the final negation is checked: for
the input `"-2147483648"` the value is `i32::MIN` and `-v` panics in a debug
build. The C is undefined behaviour there (it never aborts). Fix:
`-v.wrapping_neg()`.

### F-3c - `ch == 0` in the timer helpers

`fw/modules/hwi_a_hal.c:132` / `:212`

```c
void timer_ocpe(uint32_t tim, uint8_t ch, int on)
{
    uint32_t reg = (ch <= 2u) ? (tim + 0x18) : (tim + 0x1C);
    uint16_t bit = (ch <= 2u) ? (uint16_t)(1u << ((ch - 1u) * 8u + 3u))
                              : (uint16_t)(1u << ((ch - 3u) * 8u + 3u));
    ...
}

void timer_pwm_set(uint32_t tim, uint8_t ch, uint32_t ccr)
{
    REG32(tim + 0x34u + 4u * (uint32_t)(ch - 1u)) = ccr;
}
```

`rust/src/hal/timer.rs:119` / `:196`

```rust
pub fn ocpe(tim: u32, ch: u8, on: bool) {
    let reg = if ch <= 2 { tim + TIM_CCMR1 } else { tim + TIM_CCMR2 };
    let bit: u16 = if ch <= 2 {
        1u16 << ((ch as u32 - 1) * 8 + 3)      // panics for ch == 0
    } else {
        1u16 << ((ch as u32 - 3) * 8 + 3)
    };
    ...
}

pub fn pwm_set(tim: u32, ch: u8, ccr: u32) {
    mmio::w32(tim + TIM_CCR1 + 4 * (ch as u32 - 1), ccr);   // panics for ch == 0
}
```

For `ch == 0` the C computes an out of range shift (undefined behaviour, in
practice a mask of the shift count) and the Rust aborts in a debug build
("attempt to subtract with overflow" / "attempt to shift left with overflow").
No caller passes `ch == 0`; the fix would be `ch.max(1)` or a `wrapping_sub`.

## F-4 - `resp_greeting()` always writes the closing quote (real bug, minor)

`fw/modules/proxy.c:118`

```c
void resp_greeting(const char *const *kv, const char *msg)
{
    int i;
    ...
    if (kv != 0) {
        for (i = 0; kv[i] != 0; i += 2) {
            out_char(' ');
            out_char('"');
            out_str(kv[i]);
            out_char('"');
            out_char(':');
            out_char('"');
            if (kv[i + 1] != 0) {
                out_str(kv[i + 1]);
                out_char('"');        /* the closing quote is only written here */
            } else {
                break;                /* no closing quote at all */
            }
        }
    }
    ...
}
```

`rust/src/proto/proxy.rs:147`

```rust
pub fn resp_greeting(kv: &[(&str, &str)], msg: Option<&str>) {
    ...
    for (key, value) in kv {
        out_char(b' ');
        out_char(b'"');
        out_str(key);
        out_char(b'"');
        out_char(b':');
        out_char(b'"');
        out_str(value);
        out_char(b'"');            /* always written */
    }
    ...
}
```

**Difference:** with an empty / missing value the C leaves the line without a
closing quote (`"k":"`), the Rust writes `"k":""`. The C's `kv` is a NULL
terminated array, the Rust's is a slice - for well formed data the output is
identical. Latent (no caller passes a missing value).

## F-5 - The `OTYPER` handling of `gpio_config()` does not match the original for even pins (inherited from the C)

The original (`FUN_0800A964`, machine code) writes the output type **twice as
far to the left for the even pins** as for the odd ones. Even pin block
(`r3` = the even pin, `r2` = `2 * pin`):

```
800a9a2: 8885  ldrh  r5, [r0, #4]      ; OTYPER
800a9a4: 041f  lsls  r7, r3, #16       ; r7 = pin << 16
800a9a6: 0c3c  lsrs  r4, r7, #15       ; r4 = 2 * pin
800a9aa: 40a7  lsls  r7, r4            ; r7 = 1 << (2 * pin)
800a9ac: 43bd  bics  r5, r7            ; OTYPER &= ~(1 << (2 * pin))
800a9ae: 8085  strh  r5, [r0, #4]
800a9b2: 798f  ldrb  r7, [r1, #6]      ; otype
800a9b4: 40a7  lsls  r7, r4            ; otype << (2 * pin)
800a9bc: 8085  strh  r5, [r0, #4]
```

Odd pin block (`r3` = the odd pin):

```
800aa6c: 8884  ldrh  r4, [r0, #4]      ; OTYPER
800aa70: 041f  lsls  r7, r3, #16
800aa72: 0c3c  lsrs  r4, r7, #16       ; r4 = pin
800aa78: 40a7  lsls  r7, r4            ; r7 = 1 << pin
800aa7c: 43bc  bics  r4, r7            ; OTYPER &= ~(1 << pin)
800aa84: 798f  ldrb  r7, [r1, #6]      ; otype
800aa8a: 40bc  lsls  r4, r7            ; otype << pin
800aa92: 8084  strh  r4, [r0, #4]
```

The C (`fw/modules/hwi_a_hal.c:45`) and the Rust (`rust/src/hal/gpio.rs:97`)
use `pin` for **every** pin:

```c
            otyper  = (uint16_t)((otyper & (uint16_t)~(1u << pin))
                    | (uint16_t)((uint16_t)cfg->otype << pin));
```

```rust
            otyper = ((otyper & !(1u16 << pin)) | ((cfg.otype as u16) << pin)) as u16;
```

**Difference:** for the even pins the original clears bit `2 * pin` and writes
`otype << (2 * pin)` (i.e. the 2 bit field of the pin is set to `otype`, the
"textbook" STM32 behaviour), the C and the Rust clear bit `pin` and write
`otype << pin` (the type byte spills into the next pin). For the odd pins
both agree.

**Impact:** the motor driver pins of `hwiDrive` use `otype = 3`. With the
original, an even pin gets `0b11` in its own 2 bit field; with the C and the
Rust the bits `pin` and `pin + 1` are set, which also changes the next pin.
This is a deviation of the C reconstruction that the Rust inherited - the Rust
is faithful to the C.

**Note:** the decompilation of this function in `analysis/decomp/ALL.c` is
wrong (it transcribes the even block's `OTYPER` shifts as `pin` and uses the
wrong variable for the odd block's `PUPDR` store). The machine code above is
the ground truth. `PUPDR` and `OSPEEDR` and `MODER` of the original match the
C and the Rust exactly.

## F-6 - `gpio_config()` writes the registers once, the original writes them per pin (inherited from the C)

Original (`FUN_0800A964`): for every selected pin it does *read + write* for
each of the four registers, in the order `OSPEEDR`, `OTYPER`, `MODER`, `PUPDR`
(8 reads and 8 writes per selected pin).

C (`fw/modules/hwi_a_hal.c:30`): 4 reads before the loop, 4 writes after it.

```c
    uint32_t moder   = REG32(port + 0x00);
    uint16_t otyper  = REG16(port + 0x04);
    uint32_t ospeedr = REG32(port + 0x08);
    uint32_t pupdr   = REG32(port + 0x0C);
    ...loop...
    REG32(port + 0x00) = moder;
    REG16(port + 0x04) = otyper;
    REG32(port + 0x08) = ospeedr;
    REG32(port + 0x0C) = pupdr;
```

Rust (`rust/src/hal/gpio.rs:80`): identical to the C.

```rust
    let mut moder = mmio::r32(port + GPIO_MODER);
    let mut otyper = mmio::r16(port + GPIO_OTYPER);
    let mut ospeedr = mmio::r32(port + GPIO_OSPEEDR);
    let mut pupdr = mmio::r32(port + GPIO_PUPDR);
    ...loop...
    mmio::w32(port + GPIO_MODER, moder);
    mmio::w16(port + GPIO_OTYPER, otyper);
    mmio::w32(port + GPIO_OSPEEDR, ospeedr);
    mmio::w32(port + GPIO_PUPDR, pupdr);
```

**Difference:** the number and the order of the register accesses differ from
the original (the final register values are the same). The Rust is faithful to
the C, so this is a C level deviation. The `hwi_b_hal` flavour
(`hwi_gpio_config` / `rust/src/hal/gpio.rs:112`) writes per pin like the
original, but still with one read and one write per register instead of two.

## F-7 - The special USART mode of `FUN_0800B3FC` is missing (inherited from the C)

The original (`FUN_0800B3FC`):

```c
  if ((int)(*param_1 << 0x10) < 0) {          /* CR1 bit 15 */
    iVar1 = FUN_08000128(iVar2 * 2,uVar3);    /* (2 * clock) / baud */
    FUN_080001b0(iVar2 * 2,uVar3);
    ...
  }
  else {
    iVar1 = FUN_08000128(iVar2);              /* clock / baud */
    FUN_080001b0(iVar2,uVar3);
  }
  uVar3 = iVar1 + (uint)(*param_2 >> 1 <= uVar3);
  uVar4 = (ushort)uVar3;
  if ((int)(*param_1 << 0x10) < 0) {
    uVar4 = uVar4 & 0xfff0 | (ushort)((uVar3 & 0xf) >> 1);   /* oversampling 16 */
  }
  *(ushort *)(param_1 + 3) = uVar4;
```

C (`fw/modules/hwi_b_hal.c:209`) and Rust (`rust/src/hal/usart.rs:11`):

```c
    brr = HWI_APB1_HZ / baud;
    if ((baud / 2u) <= (HWI_APB1_HZ % baud)) {
        brr += 1u;
    }
```

```rust
    let mut brr = APB1_HZ / baud;
    if (baud / 2 <= APB1_HZ % baud {
        brr += 1;
    }
```

**The rounding itself is identical to the original** (`quotient + (baud/2 <=
remainder)`) - this is the important part and it is correct in both.
Missing: the branch for `CR1` bit 15 (2x clock and the oversampling-16
divisor). It is inert, because the same function clears `CR1` bit 15
(`CR1 & 0xE9F3 | 0x0300`).

**Also note:** the original computes the peripheral clock at run time
(`FUN_0800AC40`); the C and the Rust use the constant `96 000 000`. Same
behaviour as long as the clock tree of `system_stm32g0.c` is the one that runs
(96 MHz, no bus divider), which it is.

## F-8 - The ring buffer: the capacity is a compile time constant (structural difference)

`fw/modules/hwi_b_hal.c:280`

```c
typedef struct {
    uint8_t *buf;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} hwi_ring_t;

void hwi_ring_init(hwi_ring_t *r, uint8_t *buf, uint16_t size)
{
    r->buf   = buf;
    r->size  = size;
    r->head  = 0u;
    r->tail  = 0u;
    r->count = 0u;
}
```

`rust/src/hal/ring.rs:9`

```rust
pub struct Ring<const N: usize> {
    buf: [u8; N],
    head: u16,
    tail: u16,
    count: u16,
}

impl<const N: usize> Ring<N> {
    pub const fn new() -> Self {
        Ring { buf: [0; N], head: 0, tail: 0, count: 0 }
    }
```

**Differences:** (a) the capacity is a compile time constant (`N`) instead of a
run time `size`; (b) the ring owns the buffer instead of pointing at an
external one; (c) there is no separate `init` - the constructor is the init;
(d) the C divides by `r->size`, so a ring of size 0 is a division by zero on
the first `put`, while `Ring<0>` panics with an index out of bounds; (e)
`hwi_ring_get(r, b)` returns 0/1 and writes through a pointer, the Rust
returns `Option<u8>`; (f) `hwi_ring_read(r, dst, max)` takes an explicit
maximum, the Rust takes it from the length of the slice.

For `N > 0` the behaviour is identical: full -> `put` returns false without
overwriting, empty -> `get` returns nothing, `read` moves at most
`min(max, count)` bytes, the indices wrap with `% size`, `flush` resets the
three indices.

---

# Verified identical (the interesting details)

These are the places where a mistake would have been easy and where the Rust
matches the C exactly:

* **`timer_channel_cfg()`** (`rust/src/hal/timer.rs:131`): all four channel
  branches, the `0xFFFD` / `0xFFDF` / `0xFDFF` / `0xDFFF` masks, the
  `0xFFF7` / `0xFF7F` / `0xF7FF` bits of the advanced timers, the
  `0xFCFF` / `0xFBFF` / `0xCFFF` / `0xBFFF` masks of `CR2`, the `0xFF8C` /
  `0x8CFF` masks of `CCMR1` / `CCMR2` and the 32 bit write of `CCR1..CCR4`
  (which also clears the next compare register) are identical, including the
  order of the writes: `CR2`, `CCMRx`, `CCRx`, and finally `CCER`.
* **`timer_base_cfg()`** (`rust/src/hal/timer.rs:84`): the special case of
  `TIM6` (which writes `ARR`, `PSC`, `CR1` in that order and *not* `CR1`
  first) and of `TIM1/TIM2/TIM3` (`CR1 & 0xFF8F | cr1_hi`) is identical, as is
  the 32 bit write of `ARR` (which also writes `RCR`) and the write of `RCR`
  (0x30) for the advanced timers.
* **`adc_ready()`** (`rust/src/hal/adc.rs:72`): the selection between the `CR`
  and the `ISR` register uses `mask << 7` tested for the sign bit, i.e. bit 24
  of the mask, in the C and in the Rust; the `0xFEFFFFFF` mask of the `CR`
  read is the same.
* **`dma_channel_cfg()`** (`rust/src/hal/dma.rs:10`): the `0xFFFF800F` mask of
  the configuration register and the order `CCR`, `CNDTR`, `CPAR`, `CMAR`
  are identical.
* **`dma_channel_reset()`** (`rust/src/hal/dma.rs:23`): the double write of the
  configuration register (first the enable bit is cleared, then the register
  is set to 0) and the per channel flag masks (`0x0F`, `0xF0`, `0xF00`,
  `0xF000`, `0xF0000`, nothing for `CH6`/`CH7`) are identical.
* **`gpio_config()`** (`rust/src/hal/gpio.rs:80`): the output type is written
  as a whole byte shifted by the pin number (so `otype = 3` also touches the
  next pin), the `mode - 1 < 2` test for "output or alternate function" (with
  the `uint8_t` wrap around for `mode == 0`) and the order of the four final
  writes (`MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`) are identical.
* **`gpio_write()`** (`rust/src/hal/gpio.rs:168`): the C of `hwi_a_hal.c`
  clears the bits in the register at offset **0x28** (and not in `BRR` at
  0x1C) - the Rust keeps that quirk, while `gpio::reset()` (the
  `hwi_gpio_reset()` of the `hwi_b_hal` flavour) uses 0x1C, exactly like the
  corresponding C helpers. The two are *not* interchangeable, and the Rust
  keeps them apart.
* **`config_per_pin()`** (`rust/src/hal/gpio.rs:112`): the `hwi_b_hal` flavour
  clears the output type bit `(bit << 1)` but sets the bit `otype & 1 << pin`
  - the asymmetric mask of the original is kept.
* **`nvic_enable()`** (`rust/src/hal/nvic.rs:9`): the two priority bits go
  into the top of the byte (`(prio & 3) << 6`, mask `0xC0`), the interrupt
  number is masked with `0x1F` for both the priority byte address
  (`NVIC_IPR + irq * 4`) and the enable bit (`1 << irq`).
* **`usart::init()`** (`rust/src/hal/usart.rs:11`): the order of the five
  register writes (`CR1 & 0xFFFE`, `CR2`, `CR1`, `CR3`, `BRR`), the masks
  `0xFFFE` / `0xCFFF` / `0xE9F3` / `0xFCFF` and the values `0x0300` (TE|RE)
  and `0x000C` are identical, and the `baud == 0` guard returns without
  touching a register. The rounding of the divisor is identical to the
  original (see F-7).
* **`util::delay_ms()`** (`rust/src/hal/util.rs:18`): the unsigned wrap around
  of `g_tick_ms - start` and the 200000 spin limit are identical.
* **`util::is_on()`** (`rust/src/hal/util.rs:36`): "on"/"ON", "1" and "true"
  in any case are accepted, and nothing else (the C requires the terminating
  NUL, the Rust the exact length).
* **`util::parse_args()`** (`rust/src/hal/util.rs:88`): the separators are
  space, tab and comma, the empty argument list returns 0, and the values are
  parsed with `atoi`; the C scans the token and the Rust slices it - the
  result is the same for every input.
* **`fmt::uitoa()`** (`rust/src/proto/fmt.rs:62`): the radix is clamped to
  `2..36` (else 10), the digits `10..35` are produced with the original's
  `'W' + digit` trick (which yields `a`..`z`), the buffer holds 36 digits and
  the loop stops at 36.
* **`fmt::fixed()`** (`rust/src/proto/fmt.rs:120`): the divisor is capped at
  `10^9` but the number of fraction digits is not, the fraction is produced
  from the least significant digit upwards, and the sign is written before
  the digits - identical to `fmt_fixed()`.
* **`proxy::dispatch()`** (`rust/src/proto/proxy.rs:221`): the tokenizer skips
  *spaces only* (not tabs), the "args" is the rest of the line behind the
  third token with the delimiter consumed, the registry is searched
  **backwards**, and the three "Invalid command!" cases plus the
  "No interface:" message are byte identical.
* **`system::system_init()`** (`rust/src/system.rs:72`): the 16 register
  operations of `FUN_08002958` in the same order with the same masks
  (`0xF8FFB80C`, `0xFEF6FFFF`, `0xFFFBFFFF`, `0xFFC0FFFF`, `0xFFFFFFF0`,
  `0xFFFFFEAC`, `0xFFFFFFFE`, `CR = 0`, `| 0x00010000`, the `0x5000` spin
  limit, `FLASH->ACR = 0x11`, `0xFFC07FFF`, `| 0x00130000`, `| 0x01000000`,
  the wait for `0x02000000`, `CFGR = (CFGR & ~3) | 2` and the wait for
  `SWS == 0x8`) are identical.
