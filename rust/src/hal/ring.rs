//! The ring buffer of the original firmware
//! (`FUN_08002E1C` / `FUN_08002E44` / `FUN_08002E78` / `FUN_08002F38`).
//!
//! The C version keeps the buffer in a separate array and the indices in a
//! struct; in Rust the buffer is part of the ring, so a ring can never be
//! used with a foreign buffer.

/// A byte ring buffer of a fixed capacity.
pub struct Ring<const N: usize> {
    buf: [u8; N],
    head: u16,
    tail: u16,
    count: u16,
}

impl<const N: usize> Ring<N> {
    /// An empty ring.
    pub const fn new() -> Self {
        Ring { buf: [0; N], head: 0, tail: 0, count: 0 }
    }

    /// Put one byte into the ring, returns false when the ring is full.
    pub fn put(&mut self, b: u8) -> bool {
        if self.count as usize >= N {
            return false; // buffer full
        }
        self.buf[self.head as usize] = b;
        self.head = ((self.head + 1) % N as u16) as u16;
        self.count += 1;
        true
    }

    /// Take one byte out of the ring.
    pub fn get(&mut self) -> Option<u8> {
        if self.count == 0 {
            return None;
        }
        let b = self.buf[self.tail as usize];
        self.tail = ((self.tail + 1) % N as u16) as u16;
        self.count -= 1;
        Some(b)
    }

    /// Move up to `dst.len()` bytes out of the ring, returns their number.
    pub fn read(&mut self, dst: &mut [u8]) -> u16 {
        let mut n = 0u16;
        while (n as usize) < dst.len() && self.count != 0 {
            dst[n as usize] = self.buf[self.tail as usize];
            self.tail = ((self.tail + 1) % N as u16) as u16;
            self.count -= 1;
            n += 1;
        }
        n
    }

    /// The number of bytes in the ring.
    pub fn count(&self) -> u16 {
        self.count
    }

    /// Throw the content away.
    pub fn flush(&mut self) {
        self.head = 0;
        self.tail = 0;
        self.count = 0;
    }
}

impl<const N: usize> Default for Ring<N> {
    fn default() -> Self {
        Ring::new()
    }
}
