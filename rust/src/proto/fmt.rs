//! Number -> text.
//!
//! The original formats the values with `FUN_080068D4` (unsigned) and
//! `FUN_0800693C` (signed); both are itoa() with a variable radix.  Every
//! call site that produces a reply uses radix 16, so the original prints its
//! numbers in HEXADECIMAL (the only exception is the hwiAdc handler, which
//! uses radix 10).
//!
//! The C version writes into a caller supplied buffer; the Rust version
//! returns a fixed capacity [`Buf`] instead - no allocation, and the length
//! can never exceed the buffer.

/// A fixed capacity text buffer.
///
/// One byte of the capacity is reserved for the terminating NUL of the C
/// original, so the usable length is `N - 1`.
pub struct Buf<const N: usize> {
    data: [u8; N],
    len: usize,
}

impl<const N: usize> Buf<N> {
    /// An empty buffer.
    pub const fn new() -> Self {
        Buf { data: [0; N], len: 0 }
    }

    /// The text.
    ///
    /// The buffer only ever holds the characters of the number formatting
    /// (ASCII), so the conversion is unchecked - like the C, which writes
    /// into a `char[]` without validating anything.  The check is kept for
    /// the debug build.
    pub fn as_str(&self) -> &str {
        debug_assert!(core::str::from_utf8(&self.data[..self.len]).is_ok());
        // SAFETY: the buffer holds the digits of a number and the characters
        // of the format strings, all of them ASCII
        unsafe { core::str::from_utf8_unchecked(&self.data[..self.len]) }
    }

    /// The number of characters.
    pub fn len(&self) -> usize {
        self.len
    }

    /// Is the buffer empty?
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// Append one character (silently dropped when the buffer is full).
    fn push(&mut self, c: u8) {
        if self.len + 1 < N {
            self.data[self.len] = c;
            self.len += 1;
        }
    }
}

impl<const N: usize> Default for Buf<N> {
    fn default() -> Self {
        Buf::new()
    }
}

/// `FUN_080068D4` - unsigned itoa with a variable radix.
///
/// Digits 10..35 are produced with the original's `'W' + digit` trick, which
/// yields 'a'..'z'.
fn uitoa(mut v: u32, radix: u32) -> ([u8; 36], usize) {
    let mut radix = radix;
    if !(2..=36).contains(&radix) {
        radix = 10;
    }
    let mut tmp = [0u8; 36];
    let mut n = 0usize;
    loop {
        let d = v % radix;
        v /= radix;
        tmp[n] = if d < 10 { b'0' + d as u8 } else { b'W' + d as u8 };
        n += 1;
        if v == 0 || n >= tmp.len() {
            break;
        }
    }
    (tmp, n)
}

/// `FUN_080068D4` - unsigned itoa.
pub fn num(radix: u32, v: u32) -> Buf<40> {
    let (tmp, n) = uitoa(v, radix);
    let mut out = Buf::<40>::new();
    for i in 0..n {
        out.push(tmp[n - 1 - i]);
    }
    out
}

/// `FUN_0800693C` - signed itoa.
pub fn num_s(radix: u32, v: i32) -> Buf<40> {
    let mag = if v < 0 { (v as u32).wrapping_neg() } else { v as u32 };
    let (tmp, n) = uitoa(mag, radix);
    let mut out = Buf::<40>::new();
    if v < 0 {
        out.push(b'-');
    }
    for i in 0..n {
        out.push(tmp[n - 1 - i]);
    }
    out
}

/// Decimal convenience wrapper (the C `fmt_int`).
pub fn int(v: i32) -> Buf<16> {
    let mag = if v < 0 { (v as u32).wrapping_neg() } else { v as u32 };
    let (tmp, n) = uitoa(mag, 10);
    let mut out = Buf::<16>::new();
    if v < 0 {
        out.push(b'-');
    }
    for i in 0..n {
        out.push(tmp[n - 1 - i]);
    }
    out
}

/// Decimal convenience wrapper, scaled by 10^frac (the C `fmt_fixed`).
pub fn fixed(v: i32, frac: u32) -> Buf<24> {
    let mag = if v < 0 { (v as u32).wrapping_neg() } else { v as u32 };
    let mut out = Buf::<24>::new();
    if v < 0 {
        out.push(b'-');
    }

    if frac > 0 {
        let mut div = 1u32;
        let mut f = 0u32;
        while f < frac && f < 9 {
            div *= 10;
            f += 1;
        }
        let (tmp, n) = uitoa(mag / div, 10);
        for i in 0..n {
            out.push(tmp[n - 1 - i]);
        }
        out.push(b'.');
        let mut f = frac as i32 - 1;
        while f >= 0 {
            let mut p = 1u32;
            for _ in 0..f {
                p *= 10;
            }
            out.push(b'0' + ((mag / p) % 10) as u8);
            f -= 1;
        }
    } else {
        let (tmp, n) = uitoa(mag, 10);
        for i in 0..n {
            out.push(tmp[n - 1 - i]);
        }
    }
    out
}
