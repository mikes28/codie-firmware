//! Small helpers of the original firmware: the millisecond tick, the busy
//! wait, the watchdog kick and the tiny string/number parsers.

use super::addr::IWDG;
use crate::system;

/// The shared millisecond tick.
pub fn millis() -> u32 {
    system::tick_ms()
}

/// Busy wait.
///
/// The original's delay helper (`FUN_08002DD4`) re-programs the SysTick and
/// then spins; the reconstruction waits on the shared millisecond tick
/// instead so that it does not disturb the system tick.  If the tick does
/// not advance the wait is aborted (bounded spin).
pub fn delay_ms(ms: u32) {
    let start = system::tick_ms();
    let mut spins = 0u32;
    while (system::tick_ms().wrapping_sub(start)) < ms {
        spins += 1;
        if spins > 200_000 {
            // the tick is not running - give up
            break;
        }
    }
}

/// `FUN_0800AB10` - independent watchdog kick (IWDG_KR = 0xAAAA).
pub fn iwdg_kick() {
    crate::mmio::w32(IWDG + 0x00, 0xAAAA);
}

/// `FUN_0800A084` - the original accepts "on", "1" and "true".
pub fn is_on(s: &str) -> bool {
    let b = s.as_bytes();
    if b.len() == 2 && (b[0] == b'o' || b[0] == b'O') && (b[1] == b'n' || b[1] == b'N') {
        return true;
    }
    if b.len() == 1 && b[0] == b'1' {
        return true;
    }
    if b.len() == 4
        && (b[0] | 0x20) == b't'
        && (b[1] | 0x20) == b'r'
        && (b[2] | 0x20) == b'u'
        && (b[3] | 0x20) == b'e'
    {
        return true;
    }
    false
}

/// A small decimal parser (`hwi_atoi` of `hwi_b_hal.c`).
///
/// The C version wraps around on overflow; the Rust version does the same.
pub fn atoi(s: &str) -> i32 {
    let b = s.as_bytes();
    let mut i = 0usize;
    let mut v: i32 = 0;
    let mut neg = false;

    while i < b.len() && (b[i] == b' ' || b[i] == b'\t') {
        i += 1;
    }
    if i < b.len() && b[i] == b'-' {
        neg = true;
        i += 1;
    } else if i < b.len() && b[i] == b'+' {
        i += 1;
    }
    while i < b.len() && b[i].is_ascii_digit() {
        v = v.wrapping_mul(10).wrapping_add((b[i] - b'0') as i32);
        i += 1;
    }
    if neg {
        // the C wraps around on the minimum value, the Rust would panic
        v.wrapping_neg()
    } else {
        v
    }
}

/// Parse a whitespace separated list of decimal numbers.
///
/// Returns the number of values found.  The separators are the ones of
/// `hwi_parse_args`: space, tab and comma.
pub fn parse_args(args: &str, vals: &mut [i32]) -> usize {
    let b = args.as_bytes();
    let mut i = 0usize;
    let mut n = 0usize;

    while i < b.len() && n < vals.len() {
        while i < b.len() && (b[i] == b' ' || b[i] == b'\t' || b[i] == b',') {
            i += 1;
        }
        if i >= b.len() {
            break;
        }
        let start = i;
        while i < b.len() && b[i] != b' ' && b[i] != b'\t' && b[i] != b',' {
            i += 1;
        }
        vals[n] = atoi(&args[start..i]);
        n += 1;
    }
    n
}
