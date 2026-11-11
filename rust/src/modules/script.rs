//! script - a tiny on-robot script engine.
//!
//! A small script (max 128 bytes of bytecode) can be loaded over the command
//! link (USB or Bluetooth) and then runs purely on the robot - no wireless
//! connectivity needed while it plays.  Pressing the button on top twice
//! within 500 ms starts the script.
//!
//! Bytecode (all values are unsigned bytes):
//!   0x00                          STOP          - stop execution
//!   0x01 <led> <r> <g> <b>        SET_LED       - set one LED to RGB
//!   0x02 <motor> <duty>           SET_DUTY      - set motor A=0/B=1 duty
//!   0x03 <ms_lo> <ms_hi>          WAIT          - wait ms_lo + ms_hi*256 ms
//!   0x04                          LOOP          - jump back to byte 0
//!
//! The script runs one bytecode step per main-loop tick, so it never blocks
//! the watchdog or the command link.  A WAIT simply holds the program counter
//! until the millisecond tick passes the deadline.
//!
//! Protocol (see `crate::proto::proxy`):
//!   get script             -> "set script <b1> <b2> ..." (stored bytes)
//!   set script load <n> .. -> store the bytes as the script
//!   set script clear       -> erase the stored script
//!   call script run        -> start executing the stored script
//!   call script stop       -> stop a running script

use crate::hal::{addr, gpio, util};
use crate::modules::{hwi_drive, hwi_led};
use crate::proto::proxy;
use crate::state::Global;

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------

/// Maximum size of the stored script, in bytes.
const SCRIPT_MAX: usize = 128;

/// Two button presses within this many milliseconds count as a double press.
const DOUBLE_PRESS_MS: u32 = 500;

// ---------------------------------------------------------------------------
// Bytecode opcodes
// ---------------------------------------------------------------------------

const OP_STOP: u8 = 0x00;
const OP_SET_LED: u8 = 0x01;
const OP_SET_DUTY: u8 = 0x02;
const OP_WAIT: u8 = 0x03;
const OP_LOOP: u8 = 0x04;

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

struct State {
    /// The stored script bytes.
    bytes: [u8; SCRIPT_MAX],
    /// Number of valid bytes in `bytes`.
    len: usize,
    /// Program counter into `bytes`.
    pc: usize,
    /// Is the script currently running?
    active: bool,
    /// Millisecond deadline for the next step (used by WAIT).
    next_ms: u32,
    /// Millisecond timestamp of the last button press.
    last_press_ms: u32,
    /// Previous PC13 level for edge detection.
    last_pc13: u8,
}

static S: Global<State> = Global::new(State {
    bytes: [0; SCRIPT_MAX],
    len: 0,
    pc: 0,
    active: false,
    next_ms: 0,
    last_press_ms: 0,
    last_pc13: 0,
});

// ---------------------------------------------------------------------------
// Protocol handler
// ---------------------------------------------------------------------------

/// the interface handler of the module
pub fn handler(op: u8, opname: &str, args: &str) {
    use crate::proto::proxy;

    if op == proxy::OP_GET {
        if opname == "script" {
            script_get_reply();
        }
        return;
    }

    if op == proxy::OP_SET {
        if opname == "script" {
            let first = args.split_whitespace().next().unwrap_or("");
            if first == "load" {
                script_load(args);
            } else if first == "clear" {
                script_clear();
            }
        }
        return;
    }

    if op == proxy::OP_CALL {
        if opname == "script" {
            let first = args.split_whitespace().next().unwrap_or("");
            if first == "run" {
                script_run();
            } else if first == "stop" {
                script_stop();
            }
        }
    }
}

/// `get script` - reply with the stored bytes as decimal numbers.
fn script_get_reply() {
    use crate::proto::proxy;
    let s = S.get();
    if s.len == 0 {
        proxy::resp_value(proxy::OP_GET, "script", "script", "");
        return;
    }
    drop(s);

    // Build a space-separated decimal list in a fixed buffer.
    let mut buf: [u8; 256] = [0; 256];
    let mut pos = 0usize;
    let st = S.get();
    for i in 0..st.len {
        if i > 0 {
            if pos < buf.len() {
                buf[pos] = b' ';
                pos += 1;
            } else {
                break;
            }
        }
        let n = st.bytes[i] as i32;
        let digits = num_to_buf(n, &mut buf[pos..]);
        pos += digits;
        if pos >= buf.len() {
            break;
        }
    }
    drop(st);
    let s = core::str::from_utf8(&buf[..pos]).unwrap_or("");
    proxy::resp_value(proxy::OP_GET, "script", "script", s);
}

/// Convert a signed 32-bit integer to decimal bytes at `dst`.
/// Returns the number of bytes written.
fn num_to_buf(n: i32, dst: &mut [u8]) -> usize {
    if n == 0 {
        if dst.is_empty() {
            return 0;
        }
        dst[0] = b'0';
        return 1;
    }
    let neg = n < 0;
    let mut digits: [u8; 11] = [0; 11];
    let mut count = 0usize;
    let mut m = if neg { n.wrapping_neg() } else { n };
    while m > 0 {
        digits[count] = b'0' + (m % 10) as u8;
        m /= 10;
        count += 1;
    }
    let start = if neg { 1 } else { 0 };
    let total = count + start;
    if total > dst.len() {
        return 0;
    }
    if neg {
        dst[0] = b'-';
    }
    for i in 0..count {
        dst[start + i] = digits[count - 1 - i];
    }
    total
}

/// `set script load <n1> <n2> ...` - store the argument bytes as the script.
/// The first whitespace token must be "load"; everything after it is parsed
/// as decimal bytes and stored (truncated to SCRIPT_MAX).
fn script_load(args: &str) {
    let mut tokens = args.split_whitespace();
    tokens.next(); // skip "load"
    let s = S.get();
    let mut written = 0usize;
    for tok in tokens {
        if written >= SCRIPT_MAX {
            break;
        }
        let v: i32 = tok.parse().unwrap_or(0);
        // clamp to byte range
        let b = if v < 0 { 0u8 } else if v > 255 { 255u8 } else { v as u8 };
        s.bytes[written] = b;
        written += 1;
    }
    s.len = written;
}

/// `set script clear` - erase the stored script.
fn script_clear() {
    let s = S.get();
    s.bytes = [0; SCRIPT_MAX];
    s.len = 0;
    s.pc = 0;
    s.active = false;
}

/// `call script run` - start executing the stored script.
fn script_run() {
    let s = S.get();
    if s.len == 0 {
        return;
    }
    s.pc = 0;
    s.active = true;
    s.next_ms = util::millis();
}

/// `call script stop` - stop a running script.
fn script_stop() {
    let s = S.get();
    s.active = false;
    s.pc = 0;
}

// ---------------------------------------------------------------------------
// Tick - run one script step per main-loop iteration
// ---------------------------------------------------------------------------

/// `script_tick` - called once per main-loop iteration.
///
/// Runs one step of the script if it is active and the millisecond deadline
/// has passed.  Also watches PC13 for a double press, which starts the
/// script automatically.
pub fn tick() {
    let now = util::millis();

    // ---- double-press detection on PC13 (button on top) ----
    // PC13 goes low when the button is pressed (active low).  Two presses
    // within DOUBLE_PRESS_MS start the script automatically.
    let pc13 = gpio::read(addr::GPIOC, 0x2000) as u8;
    {
        let st = S.get();
        let changed = pc13 != st.last_pc13;
        let pressed = pc13 == 0;
        let elapsed = now.wrapping_sub(st.last_press_ms);
        let double_press = changed && pressed && elapsed != 0 && elapsed < DOUBLE_PRESS_MS;
        drop(st);
        if double_press {
            let st = S.get();
            st.pc = 0;
            st.active = true;
            st.next_ms = now;
            st.last_press_ms = 0; // avoid re-triggering on release
        } else if changed {
            let st = S.get();
            st.last_pc13 = pc13;
            if pressed {
                st.last_press_ms = now;
            }
        }
    }

    // ---- run one script step ----
    {
        let st = S.get();
        if !st.active || st.pc >= st.len || now < st.next_ms {
            return;
        }
        // Copy the opcode and advance pc under a single borrow.
        let op = st.bytes[st.pc];
        st.pc += 1;
        // st is still borrowed here; execute below via S.get() again after drop.
        drop(st);

        match op {
            OP_STOP => {
                let s = S.get();
                s.active = false;
                s.pc = 0;
            }
            OP_SET_LED => {
                let s = S.get();
                if s.pc + 4 > s.len {
                    s.active = false;
                    s.pc = 0;
                    return;
                }
                let led = s.bytes[s.pc];
                let r = s.bytes[s.pc + 1];
                let g = s.bytes[s.pc + 2];
                let b = s.bytes[s.pc + 3];
                s.pc += 4;
                s.next_ms = now;
                drop(s);
                let _ = hwi_led::set_rgb(led, r, g, b);
            }
            OP_SET_DUTY => {
                let s = S.get();
                if s.pc + 2 > s.len {
                    s.active = false;
                    s.pc = 0;
                    return;
                }
                let motor = s.bytes[s.pc];
                let duty = s.bytes[s.pc + 1] as u8 as i32;
                s.pc += 2;
                s.next_ms = now;
                drop(s);
                let _ = hwi_drive::set_duty(motor, duty);
            }
            OP_WAIT => {
                let s = S.get();
                if s.pc + 2 > s.len {
                    s.active = false;
                    s.pc = 0;
                    return;
                }
                let ms = (s.bytes[s.pc] as u32) | ((s.bytes[s.pc + 1] as u32) << 8);
                s.pc += 2;
                s.next_ms = now.wrapping_add(ms);
            }
            OP_LOOP => {
                let s = S.get();
                s.pc = 0;
                s.next_ms = now;
            }
            _ => {
                let s = S.get();
                s.active = false;
                s.pc = 0;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Init
// ---------------------------------------------------------------------------

/// `script_init` - register the "script" interface.
pub fn init() {
    let s = S.get();
    s.bytes = [0; SCRIPT_MAX];
    s.len = 0;
    s.pc = 0;
    s.active = false;
    s.next_ms = 0;
    s.last_press_ms = 0;
    s.last_pc13 = 1;
    drop(s);

    proxy::register(proxy::Iface { name: "script", handler: handler });
}
