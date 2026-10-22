//! The command protocol layer of the original firmware (the "proxy" side of
//! the BLE app link).
//!
//! Recovered from (Ghidra decompilation of the original flash dump):
//!   FUN_0800A0AC  request parser          (0x0800A0AC)
//!   FUN_0800A350  interface dispatcher     (0x0800A350)
//!   FUN_0800A16C  value reply             (0x0800A16C)
//!   FUN_0800A2D0  system message          (0x0800A2D0)
//!   FUN_0800A1C8  greeting                (0x0800A1C8)
//!   FUN_0800A160  op word table           (0x0800B820 "???","get","set","call")
//!   FUN_0800A2C0  log level table         (0x0800B834 "???","inf","dbg","wrn","err")
//!   FUN_0800B740  strcmp, FUN_0800B754 strlen, FUN_0800B778 strtok
//!   FUN_080068D4  unsigned itoa, FUN_0800693C signed itoa
//!
//! Wire format (byte exact with the original):
//!   request          "<get|set|call> <interface> <op> [args]"   (one line)
//!   value reply      "<word> <interface> <op> [value]\n"
//!   system message   "call :proxy message <level> <text>[<extra>]\n"
//!                     (the original appends "extra" WITHOUT a separator)
//!   greeting         "call :proxy greeting \"k\":\"v\" ... \"msg:<text>\"\n"

use core::sync::atomic::{AtomicUsize, Ordering};

use crate::modules::ble_com::{out_char, out_str};
use crate::state::Global;

// --- op codes (FUN_0800A160) ---------------------------------------------
pub const OP_GET: u8 = 1;
pub const OP_SET: u8 = 2;
pub const OP_CALL: u8 = 3;

// --- log levels (FUN_0800A2C0) -------------------------------------------
pub const LOG_UNK: u8 = 0;
pub const LOG_INF: u8 = 1;
pub const LOG_DBG: u8 = 2;
pub const LOG_WRN: u8 = 3;
pub const LOG_ERR: u8 = 4;

/// The word of an op code ("???", "get", "set", "call").
pub fn op_word(op: u8) -> &'static str {
    match op {
        OP_GET => "get",
        OP_SET => "set",
        OP_CALL => "call",
        _ => "???",
    }
}

/// The words of the log levels (the table of the original, `FUN_0800A2C0`).
///
/// The table is `#[used]`: the firmware only ever emits "err", so the
/// optimiser would drop the other three words from the image - the original
/// carries all of them in its flash, and the protocol surface of the port
/// has to stay the same.
#[used]
pub static LOG_WORDS: [&str; 5] = ["???", "inf", "dbg", "wrn", "err"];

/// The prefix of the message of a greeting (`FUN_0800A1C8`).
///
/// `#[used]` for the same reason: the boot greeting has no message, so the
/// optimiser would drop the prefix from the image.
#[used]
pub static GREETING_MSG_PREFIX: &str = "msg:";

/// The word of a log level ("???", "inf", "dbg", "wrn", "err").
pub fn log_word(level: u8) -> &'static str {
    match level {
        LOG_INF => LOG_WORDS[1],
        LOG_DBG => LOG_WORDS[2],
        LOG_WRN => LOG_WORDS[3],
        LOG_ERR => LOG_WORDS[4],
        _ => LOG_WORDS[0],
    }
}

// ---------------------------------------------------------------------------
// interface registry
// ---------------------------------------------------------------------------

/// The signature of an interface handler.
pub type Handler = fn(u8, &str, &str);

/// One entry of the interface table of the original firmware.
#[derive(Clone, Copy)]
pub struct Iface {
    /// the name of the interface ("hwiAdc", "drvBattery", ...)
    pub name: &'static str,
    /// the handler of the interface
    pub handler: Handler,
}

const IFACE_MAX: usize = 16;

static IFACES: Global<[Option<Iface>; IFACE_MAX]> = Global::new([None; IFACE_MAX]);
static IFACE_COUNT: AtomicUsize = AtomicUsize::new(0);

/// Publish an interface.
///
/// The original keeps a 16 entry table in RAM (a copy of the table in flash
/// at 0x0800D164) and the dispatcher searches it BACKWARDS.  The rebuild
/// fills the table from the init functions of the modules, which run in the
/// order of the original table, so that the resulting order is the same:
///   hwiAdc hwiLed hwiDrive drvBattery hwiSonar bleCom hwiBuzz hwiMic
///   hwiEnc hwiLine hwiPower hwiLightSense appLights appDrive appSys
///   appLightSense
///
/// A second registration of the same name replaces the entry.
pub fn register(iface: Iface) {
    let list = IFACES.get();
    let count = IFACE_COUNT.load(Ordering::Relaxed);

    for i in 0..count {
        if let Some(entry) = list[i] {
            if entry.name == iface.name {
                list[i] = Some(iface);
                return;
            }
        }
    }
    if count < IFACE_MAX {
        list[count] = Some(iface);
        IFACE_COUNT.store(count + 1, Ordering::Relaxed);
    }
}

// ---------------------------------------------------------------------------
// replies
// ---------------------------------------------------------------------------

/// `FUN_0800A16C` - the value reply.
pub fn resp_value(op: u8, iface: &str, opname: &str, value: &str) {
    resp_value_opt(op, iface, opname, Some(value))
}

/// `FUN_0800A16C` - the value reply, with the "no value" case of the C.
///
/// The C skips the separator **and** the value when the value is NULL
/// (`fw/modules/proxy.c`): `"<word> <interface> <op>\n"`.  No caller of the
/// C passes NULL, but the case is part of the contract.
pub fn resp_value_opt(op: u8, iface: &str, opname: &str, value: Option<&str>) {
    out_str(op_word(op));
    out_char(b' ');
    out_str(iface);
    out_char(b' ');
    out_str(opname);
    if let Some(value) = value {
        out_char(b' ');
        out_str(value);
    }
    out_char(b'\n');
}

/// `FUN_0800A2D0` - the system message.
///
/// NOTE: the original prints "extra" directly behind the text, there is NO
/// separator between them.
pub fn resp_message(level: u8, text: &str, extra: Option<&str>) {
    out_str("call");
    out_char(b' ');
    out_str(":proxy");
    out_char(b' ');
    out_str("message");
    out_char(b' ');
    out_str(log_word(level));
    out_char(b' ');
    out_str(text);
    if let Some(extra) = extra {
        out_str(extra);
    }
    out_char(b'\n');
}

/// `FUN_0800A1C8` - the greeting.
///
/// The key/value block of the C is a NULL terminated array of pairs; a pair
/// with a NULL value ends the greeting (the C writes the opening quote and
/// then breaks out of the loop, so the closing quote is missing).  `None` is
/// that NULL value.
pub fn resp_greeting(kv: &[(&str, Option<&str>)], msg: Option<&str>) {
    out_str("call");
    out_char(b' ');
    out_str(":proxy");
    out_char(b' ');
    out_str("greeting");

    for (key, value) in kv {
        out_char(b' ');
        out_char(b'"');
        out_str(key);
        out_char(b'"');
        out_char(b':');
        out_char(b'"');
        match value {
            Some(value) => {
                out_str(value);
                out_char(b'"');
            }
            None => break, // the C: the value is NULL, the loop ends
        }
    }
    if let Some(msg) = msg {
        out_char(b' ');
        out_char(b'"');
        out_str(GREETING_MSG_PREFIX);
        out_str(msg);
        out_char(b'"');
    }
    out_char(b'\n');
}

// ---------------------------------------------------------------------------
// request parser / dispatcher
// ---------------------------------------------------------------------------

/// The tokenizer of the original (`FUN_0800B778`, strtok with the delimiter
/// " "): leading delimiters are skipped, the token ends at the next space
/// and the rest of the line is the "args" of the request.
struct Lexer<'a> {
    s: &'a str,
    pos: usize,
}

impl<'a> Lexer<'a> {
    fn new(s: &'a str) -> Self {
        Lexer { s, pos: 0 }
    }

    /// The next token, `None` at the end of the line.
    fn next_token(&mut self) -> Option<&'a str> {
        let b = self.s.as_bytes();
        while self.pos < b.len() && b[self.pos] == b' ' {
            self.pos += 1;
        }
        if self.pos >= b.len() {
            return None;
        }
        let start = self.pos;
        while self.pos < b.len() && b[self.pos] != b' ' {
            self.pos += 1;
        }
        let end = self.pos;
        if self.pos < b.len() {
            // skip the delimiter
            self.pos += 1;
        }
        Some(&self.s[start..end])
    }

    /// The rest of the line behind the last token.
    fn rest(&self) -> &'a str {
        &self.s[self.pos..]
    }
}

/// `FUN_0800A0AC` + `FUN_0800A350` - parse a request and dispatch it.
///
/// Returns true when an interface handled the request.
pub fn dispatch(cmd: &str) -> bool {
    let mut lex = Lexer::new(cmd);

    let tok = match lex.next_token() {
        Some(t) => t,
        None => {
            resp_message(LOG_ERR, "Invalid command!", None);
            return false;
        }
    };
    let op = match tok {
        "get" => OP_GET,
        "set" => OP_SET,
        "call" => OP_CALL,
        _ => {
            resp_message(LOG_ERR, "Invalid command!", None);
            return false;
        }
    };

    let iface = match lex.next_token() {
        Some(t) => t,
        None => {
            resp_message(LOG_ERR, "Invalid command!", None);
            return false;
        }
    };

    let opname = match lex.next_token() {
        Some(t) => t,
        None => {
            resp_message(LOG_ERR, "Invalid command!", None);
            return false;
        }
    };
    let args = lex.rest();

    // the original searches the registry backwards
    let list = IFACES.get();
    let count = IFACE_COUNT.load(Ordering::Relaxed);
    for i in (0..count).rev() {
        if let Some(entry) = list[i] {
            if entry.name == iface {
                (entry.handler)(op, opname, args);
                return true;
            }
        }
    }

    resp_message(LOG_ERR, "No interface:", Some(iface));
    false
}
