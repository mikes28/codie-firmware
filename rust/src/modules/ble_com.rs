//! bleCom - the UART link to the external BLE module and the byte level
//! output primitives of the whole protocol layer (C: `fw/modules/bleCom.c`).

use crate::hal::{addr, gpio, rcc, ring, usart, util};
use crate::proto::{fmt, proxy};
use crate::state::Global;

// --- Constants (C: bleCom.c defines) ----------------------------------------

/// `BLE_USART` / `BLE_BAUD` / `BLE_STOP_BITS` (FUN_08007F7C)
/// USART3 0x40004800 @ 14400 - the AT channel to the BLE module.
const BLE_USART: u32 = addr::USART3;
const BLE_BAUD: u32 = 14400;
/// 2 stop bits (the original writes CR2 = 0x2000).
const BLE_STOP_BITS: u8 = 2;

/// Receive ring buffer: 128 bytes like the original.
const BLE_RX_SIZE: usize = 128;
/// Transmit ring buffer.
const BLE_TX_SIZE: usize = 64;

/// `CONSOLE_USART` / `CONSOLE_BAUD` / `CONSOLE_STOP_BITS`
/// USART4 0x40004C00 @ 460800 - the command + reply link.
const CONSOLE_USART: u32 = addr::USART4;
const CONSOLE_BAUD: u32 = 460800;
const CONSOLE_STOP_BITS: u8 = 1;
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
    /// Last time a byte was received on the BLE link (ms tick).
    #[allow(dead_code)]
    ble_last_rx_ms: u32,
    /// Cached BLE USART ISR error bits.
    ble_err_flags: u32,
    /// Once the transmit timeout has been reported, don't repeat it.
    ble_timeout_reported: bool,
    /// Set when the "BLE-RSP HELLO" answer has been seen.
    #[allow(dead_code)]
    ble_hello_seen: bool,
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
    ble_last_rx_ms: 0,
    ble_err_flags: 0,
    ble_timeout_reported: false,
    ble_hello_seen: false,
    ble_line: [0; BLE_LINE_SIZE],
    ble_line_len: 0,
});

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

/// `FUN_08007EC8` / `FUN_08008130` - collect the received bytes and report the
/// USART errors exactly like the original does.
fn ble_poll_rx() {
    while usart::rx_ready(BLE_USART) {
        let c = usart::getc(BLE_USART);
        if !S.get().ble_rx.put(c) {
            // The original stops the receive interrupt here; the reconstruction
            // reports the overflow like the console link does.
            out_str("BUFOVF!!!!!\n");
            break;
        }
        S.get().ble_last_rx_ms = util::millis();
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
        S.get().ble_hello_seen = true;
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
    gpio::set_af(addr::GPIOB, 1, 4);
    gpio::set_af(addr::GPIOB, 13, 4);

    // GPIOC: PC4 and PC5 as alternate function (AF1)
    let cfg = gpio::PerPinCfg {
        mask: 0x30,
        mode: gpio::MODE_ALT,
        speed: 3,
        otype: 2,
        pull: 0,
    };
    gpio::config_per_pin(addr::GPIOC, &cfg);
    gpio::set_af(addr::GPIOC, 4, 1);
    gpio::set_af(addr::GPIOC, 5, 1);

    // GPIOA: PA2 is an input (the module's state line)
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

    usart::init(BLE_USART, BLE_BAUD, BLE_STOP_BITS);
    usart::enable(BLE_USART); // CR1.UE, FUN_0800B4C0

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
    gpio::set_af(addr::GPIOC, 10, 0);
    gpio::set_af(addr::GPIOC, 11, 0);

    usart::init(CONSOLE_USART, CONSOLE_BAUD, CONSOLE_STOP_BITS);
    usart::enable(CONSOLE_USART);

    {
        let s = S.get();
        s.console_err_flags = 0;
        s.ble_last_rx_ms = util::millis();
        s.ble_err_flags = 0;
        s.ble_timeout_reported = false;
        s.ble_hello_seen = false;
        s.ble_line_len = 0;
        s.ble_ready = true;
    }

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