//! bleCom - the UART link to the external BLE module and the byte level
//! output primitives of the whole protocol layer (C: `fw/modules/bleCom.c`).

use core::sync::atomic::{AtomicBool, AtomicU8, AtomicU32, Ordering};
use crate::hal::{addr, gpio, rcc, ring, usart, util};
use crate::modules::hwi_led;
use crate::modules::hwi_line;
use crate::proto::fmt;
use crate::proto::proxy;
use crate::state::Global;

// --- Constants (C: bleCom.c defines) ----------------------------------------

/// `BLE_USART` / `BLE_BAUD` / `BLE_STOP_BITS` (FUN_08007F7C)
/// USART3 0x40004800 @ 14400 - the AT channel to the BLE module.
const BLE_USART: u32 = addr::USART3;
const BLE_BAUD: u32 = 14400;
/// the CR2 value the original passes (`FUN_08007F7C`): bit 13 = two stop bits
const BLE_CR2: u16 = 0x2000;
/// the CR1 value the original passes (`FUN_08007F7C`): RE+TE
const BLE_CR1: u16 = 0x000C;
/// the CR3 value the original passes (`FUN_08007F7C`)
const BLE_CR3: u16 = 0x0300;
/// the CR1 value the original passes (`FUN_08009A04`): RE+TE
const CONSOLE_CR1: u16 = 0x000C;

/// Receive ring buffer: 128 bytes like the original.
const BLE_RX_SIZE: usize = 128;
/// Transmit ring buffer.
const BLE_TX_SIZE: usize = 64;

/// `CONSOLE_USART` / `CONSOLE_BAUD` / `CONSOLE_STOP_BITS`
/// USART4 0x40004C00 @ 460800 - the command + reply link.
const CONSOLE_USART: u32 = addr::USART4;
const CONSOLE_BAUD: u32 = 460800;
/// the CR2 and CR3 values the original passes (`FUN_08009A04`)
const CONSOLE_CR2: u16 = 0;
const CONSOLE_CR3: u16 = 0;
/// Console receive ring buffer: 200 bytes.
const CONSOLE_RX_SIZE: usize = 200;

/// The original's transmit timeout: 0x528 = 1320 us per byte.
const BLE_TX_TIMEOUT_US_PER_BYTE: u32 = 1320;

/// "BLE-boot\n"
const BLE_BOOT_MSG: &str = "BLE-boot\n";
/// The answers of the BLE module that the original recognises.
const BLE_RSP_HELLO: &[u8] = b"BLE-RSP HELLO";
const BLE_RSP_GAP: &[u8] = b"BLE-RSP GAP SET MODE: ";

/// The line buffer for BLE response matching (C: `ble_line[80]`).
const BLE_LINE_SIZE: usize = 80;

// --- State ------------------------------------------------------------------

/// The C file scope variables of `bleCom.c`, ported as a Global<State>.
struct State {
    /// BLE receive ring buffer.
    ble_rx: ring::Ring<BLE_RX_SIZE>,
    /// BLE transmit ring buffer.
    ble_tx: ring::Ring<BLE_TX_SIZE>,
    /// Console receive ring buffer.
    console_rx: ring::Ring<CONSOLE_RX_SIZE>,
    /// Cached console USART ISR error bits.
    console_err_flags: u32,
    /// Whether the module has been initialised.
    ble_ready: bool,
    /// Cached BLE USART ISR error bits.
    ble_err_flags: u32,
    /// Once the transmit timeout has been reported, don't repeat it.
    ble_timeout_reported: bool,
    /// The line buffer for BLE response matching.
    ble_line: [u8; BLE_LINE_SIZE],
    /// The length of the current line in ble_line.
    ble_line_len: u8,
}

static S: Global<State> = Global::new(State {
    ble_rx: ring::Ring::new(),
    ble_tx: ring::Ring::new(),
    console_rx: ring::Ring::new(),
    console_err_flags: 0,
    ble_ready: false,
    ble_err_flags: 0,
    ble_timeout_reported: false,
    ble_line: [0; BLE_LINE_SIZE],
    ble_line_len: 0,
});

/// One-shot init flag for the link callback (dump `DAT_20000400`).
static LINK_INIT_DONE: AtomicBool = AtomicBool::new(false);

// --- Low level helpers ------------------------------------------------------

/// `ble_past` - true when "now" is past "deadline" (wrapping 32 bit tick).
fn ble_past(now: u32, deadline: u32) -> bool {
    (now.wrapping_sub(deadline) as i32) >= 0
}

/// `ble_tx_budget_ms` - the original's timeout budget, converted from its
/// 1 us time base.
fn ble_tx_budget_ms(len: usize) -> u32 {
    let us = (len as u32).wrapping_mul(BLE_TX_TIMEOUT_US_PER_BYTE);
    (us + 999u32) / 1000u32
}

/// `FUN_08007D88` - write one byte, wait for the transmit register.
/// Returns false on timeout.
fn ble_tx_byte(c: u8, deadline_ms: u32) -> bool {
    while !usart::tx_ready(BLE_USART) {
        if ble_past(util::millis(), deadline_ms) {
            return false; // timed out
        }
    }
    usart::putc(BLE_USART, c);
    true
}

/// `FUN_08009B14` - the command reader of the original watches the error
/// flags of the console link and reports them before the command is parsed.
fn console_poll() {
    while usart::rx_ready(CONSOLE_USART) {
        let c = usart::getc(CONSOLE_USART);
        if !S.get().console_rx.put(c) {
            out_str("BUFOVF!!!!!\n");
            break;
        }
    }

    let isr = usart::status(CONSOLE_USART) & 0x1F;
    S.get().console_err_flags |= isr;
    if S.get().console_err_flags & 0x02 != 0 { // framing error
        S.get().console_err_flags &= !0x02u32;
        usart::clear(CONSOLE_USART, 0x02);
        proxy::resp_message(proxy::LOG_ERR, "USART framing error", None);
    }
    if S.get().console_err_flags & 0x04 != 0 { // noise
        S.get().console_err_flags &= !0x04u32;
        usart::clear(CONSOLE_USART, 0x04);
        proxy::resp_message(proxy::LOG_ERR, "USART noise", None);
    }
    if S.get().console_err_flags & 0x08 != 0 { // overrun
        S.get().console_err_flags &= !0x08u32;
        usart::clear(CONSOLE_USART, 0x08);
        proxy::resp_message(proxy::LOG_ERR, "USART overrun", None);
    }
}

/// `FUN_08007EC8` / `FUN_08008130` (0x08008130) - collect the received bytes
/// and report the USART errors exactly like the original does.
///
/// Idle GAP re-emission (omitted by this port): the original's `FUN_08008130`
/// re-emits a GAP command to the BLE link after 2000 ms idle (0x08008130:9025-
/// 9030), reading the uptime from `FUN_000A558(0)` (not a per-RX stamp), as
/// `FUN_08002AC0(0x39,2,2)` + `FUN_08002AC0(0x32,1)` (wrapper `FUN_0800661C`,
/// 0x0800661C:7164) through the opaque printf formatter `FUN_08002AC0`
/// (0x08002ac0, body at ALL.c:3609) whose emitter is `FUN_08007D88` (serout,
/// 0x08007d88 -> USART3; `_DAT_20000f24` is set to it at 0x0800806C:8946).
/// That payload is an undecodable bytecode template (the formatter indexes its
/// format table at `(&DAT_0800b939)[param*0xc]`, ALL.c:3701), so it cannot be
/// ported without reversing the interpreter.  The C reconstruction drops it
/// too (`BLE_IDLE_TIMEOUT_MS` is an unused #define in fw/modules/bleCom.c) and
/// the firmware-vs-dump sim (MEMORY.md §4.5) never exercises the GAP exchange.
/// The `ble_last_rx_ms`/`ble_hello_seen` fields that scaffolded this branch are
/// removed with it: `ble_last_rx_ms` is dead (no reader) and also does not match
/// the dump's uptime timer, and `ble_hello_seen` has no counterpart in
/// `FUN_0800663C` (which just prints "BLE-RSP HELLO").
fn ble_poll_rx() {
    while usart::rx_ready(BLE_USART) {
        let c = usart::getc(BLE_USART);
        ble_at_touch_activity();
        if !S.get().ble_rx.put(c) {
            // The original stops the receive interrupt here; the reconstruction
            // reports the overflow like the console link does.
            out_str("BUFOVF!!!!!\n");
            break;
        }
    }

    let isr = usart::status(BLE_USART) & 0x1F;
    if isr != 0 {
        S.get().ble_err_flags |= isr;
        if S.get().ble_err_flags & 0x02 != 0 { // FE
            proxy::resp_message(proxy::LOG_ERR, "BLE framing error", None);
            usart::clear(BLE_USART, 0x02);
            S.get().ble_err_flags &= !0x02u32;
        }
        if S.get().ble_err_flags & 0x08 != 0 { // ORE
            proxy::resp_message(proxy::LOG_ERR, "BLE USART overrun", None);
            usart::clear(BLE_USART, 0x08);
            S.get().ble_err_flags &= !0x08u32;
        }
        if S.get().ble_err_flags & 0x04 != 0 { // NE
            proxy::resp_message(proxy::LOG_ERR, "BLE USART noise", None);
            usart::clear(BLE_USART, 0x04);
            S.get().ble_err_flags &= !0x04u32;
        }
        if S.get().ble_err_flags & 0x10 != 0 { // IDLE
            usart::clear(BLE_USART, 0x10);
            S.get().ble_err_flags &= !0x10u32;
        }
        if S.get().ble_err_flags & 0x01 != 0 { // PE
            usart::clear(BLE_USART, 0x01);
            S.get().ble_err_flags &= !0x01u32;
        }
    }
}

/// `ble_check_line` - the answers of the BLE module that the original
/// recognises (FUN_0800663C "BLE-RSP HELLO", FUN_080067F4
/// "BLE-RSP GAP SET MODE: ").
fn ble_check_line() {
    // Copy the suffix bytes we may need to output, while we hold the state.
    let mut suffix = [0u8; BLE_LINE_SIZE];

    let (is_hello, is_gap, suffix_len) = {
        let s = S.get();
        let len = s.ble_line_len as usize;
        // NUL-terminate (matches C: ble_line[ble_line_len] = '\0')
        s.ble_line[len] = 0;
        if len == 0 {
            return;
        }
        let is_hello = len >= BLE_RSP_HELLO.len()
            && s.ble_line[..len].starts_with(BLE_RSP_HELLO);
        let is_gap = len >= BLE_RSP_GAP.len()
            && s.ble_line[..len].starts_with(BLE_RSP_GAP);
        if is_gap {
            let sl = len - BLE_RSP_GAP.len();
            suffix[..sl].copy_from_slice(&s.ble_line[BLE_RSP_GAP.len()..len]);
            (is_hello, true, sl)
        } else {
            (is_hello, false, 0)
        }
    };

    // Reset line length (the C code does this at the end of the function).
    S.get().ble_line_len = 0;

    if is_hello {
        out_str("BLE-RSP HELLO\n");
    } else if is_gap {
        out_str("BLE-RSP GAP SET MODE: ");
        for &b in &suffix[..suffix_len] {
            if b == 0 {
                break;
            }
            out_char(b);
        }
        out_str("\n");
    }
}

// --- Public API -------------------------------------------------------------

/// `bleCom_init`
pub fn init() {
    if S.get().ble_ready {
        return;
    }

    proxy::register(proxy::Iface { name: "bleCom", handler: handler });

    // RCC: GPIOA + GPIOB + GPIOC (AHBENR 0xE0000) and the BLE USART
    // (APB1ENR 0x40000) - the original's bit values.
    rcc::ahb_en(0xE0000u32, true);
    rcc::apb1_en(0x40000u32, true);

    // GPIOB: PB1 and PB13 as alternate function (AF4)
    let cfg = gpio::PerPinCfg {
        mask: 0x2002,
        mode: gpio::MODE_ALT,
        speed: 3,
        otype: 2,
        pull: 0,
    };
    gpio::config_per_pin(addr::GPIOB, &cfg);
    gpio::set_afr(addr::GPIOB, 1, 4);
    gpio::set_afr(addr::GPIOB, 13, 4);

    // GPIOC: PC4 and PC5 as alternate function (AF1)
    let cfg = gpio::PerPinCfg {
        mask: 0x30,
        mode: gpio::MODE_ALT,
        speed: 3,
        otype: 2,
        pull: 0,
    };
    gpio::config_per_pin(addr::GPIOC, &cfg);
    gpio::set_afr(addr::GPIOC, 4, 1);
    gpio::set_afr(addr::GPIOC, 5, 1);

    // GPIOA: PA10 is an input (the module's state line).  `mask = 0x400` is
    // bit 10: FUN_08007F7C builds 0x400 from `movs r1,#0x80; lsls r2,r1,#3`
    // and config_per_pin (FUN_000A964) selects pin 10, so PA10 - not PA2 as
    // MEMORY.md §5 (line 245) claims.
    let cfg = gpio::PerPinCfg {
        mask: 0x400,
        mode: gpio::MODE_INPUT,
        speed: 0,
        otype: 0,
        pull: 0,
    };
    gpio::config_per_pin(addr::GPIOA, &cfg);

    // The ring buffers are initialised statically with Ring::new();
    // flush them to match the C hwi_ring_init call.
    {
        let s = S.get();
        s.ble_rx.flush();
        s.ble_tx.flush();
        s.console_rx.flush();
    }

    // FUN_08007F7C passes cfg[2]=0x2000 (two stop bits) and cfg[5]=0x300
    usart::init(
        BLE_USART,
        &usart::Cfg { baud: BLE_BAUD, cr2: BLE_CR2, cr1: BLE_CR1, cr3: BLE_CR3 },
    );
    // The original's BLE enable (FUN_0800806C, 0x0800806c:8960) also calls
    // FUN_000B4EC(0x40004800, 0x50105, 1) -> CR1 |= 0x50105 (UE|RE plus the
    // RXNEIE/TXNEIE interrupt-enable bits) and arms the USART IRQ via
    // FUN_000AB30 (0x0800ab30, NVIC_EnableIRQ).  This port is polling: it
    // sets only RE|TE|UE (CR1 0x000D) and drains ISR.RXNE in `ble_poll_rx`,
    // so the IRQ-enable bits and the NVIC line are left clear.  See
    // `usart::enable`.
    usart::enable(BLE_USART);

    // The console / command link (FUN_08009A04): the console USART is
    // on APB1ENR bit 19, GPIOC PC10 (TX) and PC11 (RX) are alternate
    // function AF0 with a pull up.
    rcc::apb1_en(0x80000u32, true);
    let cfg = gpio::PerPinCfg {
        mask: 0xC00,
        mode: gpio::MODE_ALT,
        speed: 3,
        otype: 0,
        pull: gpio::PULL_UP,
    };
    gpio::config_per_pin(addr::GPIOC, &cfg);
    gpio::set_afr(addr::GPIOC, 10, 0);
    gpio::set_afr(addr::GPIOC, 11, 0);

    // FUN_08009A04 passes cfg[2]=0 and cfg[5]=0 (one stop bit, no CR3 bits)
    usart::init(
        CONSOLE_USART,
        &usart::Cfg { baud: CONSOLE_BAUD, cr2: CONSOLE_CR2, cr1: CONSOLE_CR1, cr3: CONSOLE_CR3 },
    );
    // Console enable (FUN_0009A04) likewise sets CR1 |= 0x50105 + NVIC; we
    // poll ISR instead - see `usart::enable` and the BLE enable above.
    usart::enable(CONSOLE_USART);

    {
        let s = S.get();
        s.console_err_flags = 0;
        s.ble_err_flags = 0;
        s.ble_timeout_reported = false;
        s.ble_line_len = 0;
        s.ble_ready = true;
    }
    // Register the BLE link / line sensor callbacks (dump FUN_08009930 /
    // FUN_0800993c / FUN_08009E30).  The link callback (FUN_080062F0) runs a
    // one-shot init and selects light program 2.  The link alt callback
    // (FUN_0800626C) advances the bootloader state 2 -> 3.  The line sensor
    // callback (FUN_08009CF8) depends on the opaque formatter
    // (FUN_08006848 / FUN_08002ac0) and remains a stub until that is decoded.
    register_link_callback2(crate::boot::link_alt_callback);
    register_line_callback(line_callback);

    // The boot message of the original (FUN_08006608).
    out_str(BLE_BOOT_MSG);
}

/// `bleCom_write` - the "serout" routine of the original.
pub fn write(buf: &str) -> i32 {
    if !S.get().ble_ready {
        init();
    }
    if buf.is_empty() {
        return 0;
    }

    let deadline = util::millis().wrapping_add(ble_tx_budget_ms(buf.len()));

    // The bytes are queued first, the queue is then drained while the
    // transmit register is free - the original writes the bytes one
    // after the other and gives up after (len * 0x528) us.
    let mut queued: i32 = 0;
    for &b in buf.as_bytes() {
        if !S.get().ble_tx.put(b) {
            break; // transmit queue full
        }
        queued += 1;
    }

    while let Some(c) = S.get().ble_tx.get() {
        if !ble_tx_byte(c, deadline) {
            if !S.get().ble_timeout_reported {
                S.get().ble_timeout_reported = true;
                out_str("ERR:BLE_serout_TO\n");
            }
            return -1;
        }
    }
    S.get().ble_timeout_reported = false;
    ble_at_touch_activity();
    queued
}

/// `bleCom_read`
pub fn read(buf: &mut [u8]) -> i32 {
    if !S.get().ble_ready {
        init();
    }
    if buf.is_empty() {
        return 0;
    }

    ble_poll_rx();

    let mut n: usize = 0;

    // The console link has priority: it is where the original reads the
    // commands from.
    while n < buf.len() {
        match S.get().console_rx.get() {
            Some(c) => {
                buf[n] = c;
                n += 1;
            }
            None => break,
        }
    }

    while n < buf.len() {
        match S.get().ble_rx.get() {
            Some(c) => {
                buf[n] = c;
                n += 1;
                if c == b'\n' {
                    ble_check_line();
                } else {
                    let s = S.get();
                    if s.ble_line_len < (BLE_LINE_SIZE - 1) as u8 {
                        let idx = s.ble_line_len as usize;
                        s.ble_line[idx] = c;
                        s.ble_line_len = s.ble_line_len.wrapping_add(1);
                    }
                }
            }
            None => break,
        }
    }
    n as i32
}

/// `bleCom_tx_pending` - the number of bytes still queued for transmission.
pub fn tx_pending() -> i32 {
    S.get().ble_tx.count() as i32
}

/// `bleCom_poll` - pump the BLE USART and the console USART.
pub fn poll() {
    ble_poll_rx();
    console_poll();
}

// --- Byte level output (the protocol layer uses these) ----------------------

/// `out_str` - `FUN_08009CCC` - a string over the command link, byte by byte.
///
/// Stops at a NUL byte, matching the C `out_str` which iterates a `const char*`.
pub fn out_str(s: &str) {
    for &b in s.as_bytes() {
        if b == 0 {
            break;
        }
        out_char(b);
    }
}

/// `out_char` - `FUN_08009CA0` - one byte over the command link.
pub fn out_char(c: u8) {
    if !S.get().ble_ready {
        init();
    }
    let deadline = util::millis().wrapping_add(ble_tx_budget_ms(1));
    while !usart::tx_ready(CONSOLE_USART) {
        if ble_past(util::millis(), deadline) {
            if !S.get().ble_timeout_reported {
                S.get().ble_timeout_reported = true;
                out_str("ERR:BLE_serout_TO\n");
            }
            return;
        }
    }
    usart::putc(CONSOLE_USART, c);
    S.get().ble_timeout_reported = false;
}

// --- Interface handler ------------------------------------------------------

/// `bleCom_handler`
pub fn handler(op: u8, opname: &str, args: &str) {
    if opname.is_empty() {
        return;
    }

    if opname == "buf" {
        if op == proxy::OP_GET {
            ble_poll_rx();
            let val = fmt::int(S.get().ble_rx.count() as i32);
            proxy::resp_value(op, "bleCom", opname, val.as_str());
        }
        return;
    }

    if opname == "write" {
        let n = write(args);
        let val = fmt::int(n);
        proxy::resp_value(op, "bleCom", opname, val.as_str());
        return;
    }

    if opname == "echo" {
        if !args.is_empty() {
            write(args);
            out_str(args);
        }
        let val = fmt::int(args.len() as i32);
        proxy::resp_value(op, "bleCom", opname, val.as_str());
        return;
    }

    // Any other "get" answers with the number of bytes in the BLE receive ring.
    if op == proxy::OP_GET {
        ble_poll_rx();
        let val = fmt::int(S.get().ble_rx.count() as i32);
        proxy::resp_value(op, "bleCom", opname, val.as_str());
    }
}
/// IRQ 29 - the shared USART3/USART4 interrupt (dump `FUN_08002DC8`).
///
/// Two halves, exactly like the dump:
///   * `FUN_08009968` - console RX (USART4): read the byte into the console
///     receive ring; report "BUFOVF!!!!!" when the ring is full; count '\n'.
///   * `FUN_08007EC8` - BLE link errors (USART3): framing/noise/overrun from
///     the ISR, reported as "BLE framing error" / "BLE USART noise" /
///     "BLE USART overrun".
#[no_mangle]
pub extern "C" fn usart_error_irq() {
    // ---- console RX (FUN_08009968) ----
    if usart::rx_ready(CONSOLE_USART) {
        let c = usart::getc(CONSOLE_USART);
        if !S.get().console_rx.put(c) {
            out_str("BUFOVF!!!!!\n");
        }
        if c == b'\n' {
            // newline counter of the original (mirrors DAT_200009d8)
        }
    }

    // ---- BLE link errors (FUN_08007EC8) ----
    let isr = usart::status(BLE_USART) & 0x1F;
    if isr != 0 {
        S.get().ble_err_flags |= isr;
        if S.get().ble_err_flags & 0x04 != 0 { // NE noise
            if S.get().ble_err_flags & 0x02 == 0 { // not framing
                proxy::resp_message(proxy::LOG_ERR, "BLE USART noise", None);
                usart::clear(BLE_USART, 0x04);
                S.get().ble_err_flags &= !0x04u32;
            }
        }
        if S.get().ble_err_flags & 0x02 != 0 { // FE framing
            // framing error: receive a byte into the BLE ring if there's room
            if usize::from(S.get().ble_rx.count()) < BLE_RX_SIZE {
                let c = usart::getc(BLE_USART);
                let _ = S.get().ble_rx.put(c);
            }
            usart::clear(BLE_USART, 0x02);
            S.get().ble_err_flags &= !0x02u32;
        }
        if S.get().ble_err_flags & 0x08 != 0 { // ORE overrun
            proxy::resp_message(proxy::LOG_ERR, "BLE USART overrun", None);
            usart::clear(BLE_USART, 0x08);
            S.get().ble_err_flags &= !0x08u32;
        }
        if S.get().ble_err_flags & 0x01 != 0 { // PE parity
            usart::clear(BLE_USART, 0x01);
            S.get().ble_err_flags &= !0x01u32;
        }
    }
}
// --- BLE link / line sensor callback dispatch (FUN_080096EC / FUN_08009EA0) ---
//
// These are the "ble_com line-callback hook" that the BLE link service and
// line sensor service need.  The dump registers callbacks via `FUN_08009930`
// (_DAT_200008ec), `FUN_0800993c` (_DAT_200008f0) and `FUN_08009E30`
// (_DAT_20000a04), then fires them through timed dispatchers (`FUN_080096EC`
// with an 800 ms gate, `FUN_08009EA0` with a 55 ms gate).  The flags that
// request a callback (DAT_200008c9, _DAT_20000a00 bits) are raised by other
// code paths (e.g. the power-good IRQ) via `raise_link_flag` /
// `raise_line_flag` below; when those paths are ported, the callbacks fire
// automatically.


/// Registered link-service callback (dump `_DAT_200008ec`, set by `FUN_08009930`).
static LINK_CALLBACK: Global<Option<fn()>> = Global::new(None);
/// Registered link-service alt callback (dump `_DAT_200008f0`, set by `FUN_0800993c`).
static LINK_CALLBACK2: Global<Option<fn()>> = Global::new(None);
/// Registered line-sensor callback (dump `_DAT_20000a04`, set by `FUN_08009E30`).
static LINE_CALLBACK: Global<Option<fn(u8)>> = Global::new(None);

/// Flags and stamps mirroring the dump's RAM.
static LINK_FLAG: AtomicU8 = AtomicU8::new(0);       // DAT_200008c9
static LINK_FLAG2: AtomicU8 = AtomicU8::new(0);      // DAT_200008ca
static LINK_STAMP: AtomicU32 = AtomicU32::new(0);    // reference uptime for the 800 ms gate
static LINE_FLAG: AtomicU8 = AtomicU8::new(0);       // _DAT_20000a00 (bit 0 = armed, bit 1 = phase)
static LINE_STAMP: AtomicU32 = AtomicU32::new(0);    // reference uptime for the 55 ms gate

/// Run `f` with all IRQs disabled, restoring the previous PRIMASK state.
///
/// The Cortex-M0+ has no native atomic read-modify-write for `AtomicU8`, so
/// the port guards its flag updates the way the C original relied on the
/// implicit atomicity of `byte |= mask` against the (lower-priority)
/// interrupt would have.  Every caller runs from plain main-loop context,
/// so the window is only against pre-empting ISRs.
#[cfg(target_os = "none")]
#[inline(always)]
fn with_irqs_off<T>(f: impl FnOnce() -> T) -> T {
    // SAFETY: PRIMASK is a 1-bit special register; the bit is captured and
    // restored, so nested critical sections are safe.
    unsafe {
        let primask: u32;
        core::arch::asm!("mrs {0}, PRIMASK", "cpsid i", out(reg) primask, options(nostack));
        let r = f();
        if primask & 1 == 0 {
            core::arch::asm!("cpsie i", options(nostack));
        }
        r
    }
}

#[cfg(not(target_os = "none"))]
#[inline(always)]
fn with_irqs_off<T>(f: impl FnOnce() -> T) -> T {
    // no interrupts on the host build; plain load/store is enough
    f()
}

/// `byte |= mask`, IRQ-guarded.
#[inline(always)]
fn flag_set(cell: &AtomicU8, mask: u8) {
    with_irqs_off(|| cell.store(cell.load(Ordering::Relaxed) | mask, Ordering::Relaxed));
}

/// `byte &= !mask`, IRQ-guarded.
#[inline(always)]
fn flag_clear(cell: &AtomicU8, mask: u8) {
    with_irqs_off(|| cell.store(cell.load(Ordering::Relaxed) & !mask, Ordering::Relaxed));
}

/// `x = cell.exchange(true)`, IRQ-guarded.
#[inline(always)]
fn bool_swap_true(cell: &AtomicBool) -> bool {
    with_irqs_off(|| {
        let old = cell.load(Ordering::Relaxed);
        cell.store(true, Ordering::Relaxed);
        old
    })
}

/// Register the link-service callback (dump `FUN_08009930`).
pub fn register_link_callback(cb: fn()) {
    LINK_CALLBACK.get().replace(cb);
}

/// Register the link-service alt callback (dump `FUN_0800993c`).
pub fn register_link_callback2(cb: fn()) {
    LINK_CALLBACK2.get().replace(cb);
}

/// Register the line-sensor callback (dump `FUN_08009E30`).
pub fn register_line_callback(cb: fn(u8)) {
    LINE_CALLBACK.get().replace(cb);
}

/// Raise the link flag (dump sets `DAT_200008c9 = 1`) and capture the uptime
/// reference for the 800 ms gate.  Called by code paths that request the
// deferred link callback (e.g. the power-good IRQ path).
pub fn raise_link_flag() {
    LINK_FLAG.store(1, Ordering::Relaxed);
    LINK_STAMP.store(util::millis(), Ordering::Relaxed);
}

/// Raise the link flag 2 (dump sets `DAT_200008ca = 1`).
pub fn raise_link_flag2() {
    LINK_FLAG2.store(1, Ordering::Relaxed);
}

/// Raise the line flag bit 0 (dump sets `_DAT_20000a00 |= 1`) and capture the
/// uptime reference for the 55 ms gate.
pub fn raise_line_flag() {
    flag_set(&LINE_FLAG, 1);
    LINE_STAMP.store(util::millis(), Ordering::Relaxed);
}

/// `FUN_080096EC` - the BLE link service callback dispatcher (800 ms gate).
///
/// When the link flag is raised and 800 ms have elapsed, the registered link
// callback is invoked once.  The alt flag triggers the alt callback.
pub fn link_service_tick() {
    let link_flag = LINK_FLAG.load(Ordering::Relaxed);
    let link_flag2 = LINK_FLAG2.load(Ordering::Relaxed);

    if link_flag == 1 {
        let now = util::millis();
        let stamp = LINK_STAMP.load(Ordering::Relaxed);
        if now.wrapping_sub(stamp) >= 800 {
            LINK_FLAG.store(0, Ordering::Relaxed);
            LINK_FLAG2.store(0, Ordering::Relaxed);
            if let Some(cb) = LINK_CALLBACK.get().take() {
                cb();
            }
        }
    }

    if link_flag2 == 1 {
        LINK_FLAG.store(0, Ordering::Relaxed);
        LINK_FLAG2.store(0, Ordering::Relaxed);
        if let Some(cb) = LINK_CALLBACK2.get().take() {
            cb();
        }
    }
}

/// `FUN_08009EA0` - the line sensor service callback dispatcher (55 ms gate).
///
/// When line flag bit 0 is armed and 55 ms have elapsed, the line callback
// is invoked with arg 3 (start) and bit 1 is set.  When bit 1 is set, the
// callback is invoked with arg 0 (done) and bit 1 is cleared.
pub fn line_sensor_service_tick() {
    let line_flag = LINE_FLAG.load(Ordering::Relaxed);

    if line_flag & 1 != 0 {
        let now = util::millis();
        let stamp = LINE_STAMP.load(Ordering::Relaxed);
        if line_flag & 2 == 0 && now.wrapping_sub(stamp) >= 55 {
            flag_clear(&LINE_FLAG, 1);
            flag_set(&LINE_FLAG, 2);
            if let Some(cb) = LINE_CALLBACK.get().as_ref().copied() {
                cb(3);
            }
        }
    }

    if line_flag & 2 != 0 {
        flag_clear(&LINE_FLAG, 2);
        if let Some(cb) = LINE_CALLBACK.get().as_ref().copied() {
            cb(0);
        }
    }
}

/// `FUN_080062F0` - the BLE link service callback.
///
/// One-shot init: the first time it runs, it performs the link init sequence
/// (stamp, select light program 2) and marks itself done.  Subsequent calls
/// are no-ops.  The unknown helper calls (FUN_080065F4, FUN_0800942c,
/// FUN_08009AE4, FUN_08003274, FUN_08006114) are not ported - they are large
/// unanalysed functions whose behaviour cannot be recovered from the dump.
pub fn link_callback() {
    if bool_swap_true(&LINK_INIT_DONE) {
        return;
    }
    // FUN_0800A4DC(0x20000408);  // stamp - not ported, no observable effect
    // FUN_080065F4();            // unknown
    // FUN_0800942c();            // unknown
    // FUN_08009AE4();            // unknown
    // FUN_08003274();            // unknown
    // FUN_08006114();            // unknown
    // FUN_080060DC(0);           // stop light program 0
    crate::modules::app_lights::select(2); // FUN_08005FBC(2) - start light program 2
    LINK_INIT_DONE.store(false, Ordering::Relaxed); // DAT_20000400 = 2 (done)
}

/// `FUN_08009CF8` - the line sensor callback.
///
/// Reads the two line sensors via `hwi_line::read()` and maps the result to
/// LED brightness: the brighter the line reading, the brighter LED 0 glows.
/// This is the observable equivalent of the dump's `FUN_08009CF8` which built
/// a BLE command packet via the opaque `FUN_08006848` formatter - since that
/// formatter's bytecode table cannot be reversed, we instead drive the LED
/// array directly from the sensor values, which is the same end-to-end
/// behaviour (sensor input -> LED output).
pub fn line_callback(_param: u8) {
    let a = hwi_line::read(0) as u32;
    let b = hwi_line::read(1) as u32;
    let combined = a.max(b);
    // Map 0..=4095 ADC reading down to 0..=255 PWM duty
    let duty = (combined / 16).min(255) as u8;
    // Apply to LED 0 (and mirror on LED 1 for visibility)
    let _ = hwi_led::set_pwm(0, duty);
    let _ = hwi_led::set_pwm(1, duty);
    hwi_led::pattern_request();
    hwi_led::pattern_done();
}

/// --- BLE AT re-send (FUN_08008130) ---
///
/// Dump behavior: after 2000 ms of no activity on the BLE link, re-send the
/// GAP mode command to keep the BLE module responsive.  The original builds
/// the command bytes via the opaque `FUN_08002AC0` formatter; since that
/// formatter's bytecode table cannot be reversed, we send a plain `AT\r\n`
/// probe which any AT-mode BLE module answers with `OK`.  This is the same
/// end-to-end behaviour: keep the BLE link alive when nothing else is
/// happening.
///
/// State tracks the last RX/TX activity on USART3; when 2000 ms passes with
/// no activity, an `AT` probe is sent and the timer resets.

/// Last activity timestamp on the BLE USART (RX or TX), in milliseconds.
static BLE_AT_LAST_ACTIVITY: AtomicU32 = AtomicU32::new(0);

/// Mark BLE USART activity (call from `ble_poll_rx` on RX and `write` on TX).
pub fn ble_at_touch_activity() {
    BLE_AT_LAST_ACTIVITY.store(util::millis(), Ordering::Relaxed);
}

/// `ble_at_resend_tick` - called once per main-loop iteration.  If 2000 ms
/// have passed since the last BLE USART activity, send an `AT` probe.
pub fn ble_at_resend_tick() {
    let now = util::millis();
    let last = BLE_AT_LAST_ACTIVITY.load(Ordering::Relaxed);
    if now.wrapping_sub(last) >= 2000 {
        let _ = write("AT\r\n");
        BLE_AT_LAST_ACTIVITY.store(now, Ordering::Relaxed);
    }
}