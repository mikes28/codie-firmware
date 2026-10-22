//! A model of the peripherals the firmware uses, so that the *same* code can
//! be run on the host by `cargo test`.
//!
//! The C original is tested the same way: `tools/host_sim.c` maps a fake
//! memory and interposes the USART primitives.  In Rust the switch is in
//! [`crate::mmio`]: on the target the accesses are volatile loads and
//! stores, on the host they end up here.
//!
//! Modelled:
//!   * the two USARTs (transmit capture, receive injection, the error bits
//!     of the ISR, the baud divisor),
//!   * the ADC1 (ADRDY/EOC in the ISR, the conversion start flag of CR
//!     clears itself, the data register),
//!   * the counters of the seven timers,
//!   * every other register is plain memory.

use std::collections::{HashMap, VecDeque};
use std::sync::{LazyLock, Mutex};

use crate::hal::addr::*;

const USART_FIRST: u32 = 0x4000_4800;
const USART_LAST: u32 = 0x4000_5000;
const ADC_FIRST: u32 = 0x4001_2400;
const ADC_LAST: u32 = 0x4001_2800;

/// The timers whose counter the model advances.
pub const TIMER_BASES: [u32; 7] = [TIM1, TIM2, TIM3, TIM6, TIM15, TIM16, TIM17];

/// The state of one USART.
#[derive(Default)]
struct Usart {
    /// the bytes the firmware has sent
    tx: Vec<u8>,
    /// the bytes the test has injected
    rx: VecDeque<u8>,
    /// CR1.UE
    enabled: bool,
    /// the baud divisor the firmware has written
    brr: u16,
    /// the error bits the test has forced into the ISR
    err: u32,
}

#[derive(Default)]
struct State {
    /// the plain registers
    mem: HashMap<u32, u32>,
    /// the two USARTs, keyed by their base address
    usarts: HashMap<u32, Usart>,
    /// the counter of every modelled timer, keyed by its base address
    timers: HashMap<u32, u32>,
}

static STATE: LazyLock<Mutex<State>> = LazyLock::new(|| Mutex::new(State::default()));

fn with_state<R>(f: impl FnOnce(&mut State) -> R) -> R {
    let mut guard = STATE.lock().unwrap_or_else(|e| e.into_inner());
    f(&mut guard)
}

/// Forget everything (the model is back to its reset state).
pub fn reset() {
    with_state(|s| {
        s.mem.clear();
        s.usarts.clear();
        s.timers.clear();
    });
}

// ---------------------------------------------------------------------------
// the test API
// ---------------------------------------------------------------------------

/// Inject bytes into the receive register of a USART.
pub fn usart_push_rx(base: u32, bytes: &[u8]) {
    with_state(|s| {
        let u = s.usarts.entry(base).or_default();
        for &b in bytes {
            u.rx.push_back(b);
        }
    });
}

/// Take the bytes the firmware has sent on a USART (and forget them).
pub fn usart_take_tx(base: u32) -> Vec<u8> {
    with_state(|s| {
        s.usarts
            .entry(base)
            .or_default()
            .tx
            .drain(..)
            .collect::<Vec<u8>>()
    })
}

/// The number of bytes the firmware has sent on a USART.
pub fn usart_tx_len(base: u32) -> usize {
    with_state(|s| s.usarts.get(&base).map(|u| u.tx.len()).unwrap_or(0))
}

/// The baud divisor the firmware has written into a USART.
pub fn usart_brr(base: u32) -> u16 {
    with_state(|s| s.usarts.get(&base).map(|u| u.brr).unwrap_or(0))
}

/// Force error bits into the status register of a USART (the firmware
/// reports them as "BLE framing error" and friends).
pub fn usart_set_error(base: u32, mask: u32) {
    with_state(|s| {
        s.usarts.entry(base).or_default().err |= mask;
    });
}

/// Advance the counter of every modelled timer.
pub fn advance_timers(ticks: u32) {
    with_state(|s| {
        for base in TIMER_BASES {
            let cnt = s.timers.entry(base).or_insert(0);
            *cnt = cnt.wrapping_add(ticks);
        }
    });
}

/// Read a register of the model (the tests use it to check the side
/// effects of the firmware).
pub fn read32(addr: u32) -> u32 {
    bus_read32(addr)
}

/// Write a register of the model.
pub fn write32(addr: u32, value: u32) {
    bus_write32(addr, value)
}

// ---------------------------------------------------------------------------
// the bus
// ---------------------------------------------------------------------------

fn usart_base(addr: u32) -> Option<u32> {
    if addr >= USART_FIRST && addr < USART_LAST {
        Some(addr & !0x3FF)
    } else {
        None
    }
}

fn timer_base(addr: u32) -> Option<u32> {
    for base in TIMER_BASES {
        if addr >= base && addr < base + 0x100 {
            return Some(base);
        }
    }
    None
}

fn bus_read32(addr: u32) -> u32 {
    if let Some(base) = usart_base(addr) {
        let off = addr - base;
        if off == USART_ISR {
            return with_state(|s| {
                let u = s.usarts.entry(base).or_default();
                let mut isr = u.err & 0x1F;
                if u.enabled {
                    isr |= 0x80; // TXFNF
                }
                if !u.rx.is_empty() {
                    isr |= 0x20; // RXNE
                }
                isr
            });
        }
        if off == USART_RDR {
            return with_state(|s| {
                let u = s.usarts.entry(base).or_default();
                u.rx.pop_front().unwrap_or(0) as u32
            });
        }
    }

    if addr >= ADC_FIRST && addr < ADC_LAST {
        let off = addr - ADC_FIRST;
        if off == 0x00 {
            return 0x0000_0003; // ADRDY | EOC
        }
        if off == 0x40 {
            return 0; // the data register of the model
        }
    }

    if let Some(base) = timer_base(addr) {
        if addr - base == TIM_CNT {
            return with_state(|s| s.timers.get(&base).copied().unwrap_or(0));
        }
    }

    with_state(|s| s.mem.get(&addr).copied().unwrap_or(0))
}

fn bus_write32(addr: u32, value: u32) {
    if let Some(base) = usart_base(addr) {
        let off = addr - base;
        match off {
            USART_TDR => {
                with_state(|s| {
                    let u = s.usarts.entry(base).or_default();
                    u.tx.push((value & 0xFF) as u8);
                    s.mem.insert(addr, value);
                });
                return;
            }
            USART_CR1 => {
                with_state(|s| {
                    let u = s.usarts.entry(base).or_default();
                    u.enabled = value & 1 != 0;
                    s.mem.insert(addr, value);
                });
                return;
            }
            USART_BRR => {
                with_state(|s| {
                    let u = s.usarts.entry(base).or_default();
                    u.brr = (value & 0xFFFF) as u16;
                    s.mem.insert(addr, value);
                });
                return;
            }
            USART_ICR => {
                with_state(|s| {
                    let u = s.usarts.entry(base).or_default();
                    u.err &= !value;
                    s.mem.insert(addr, value);
                });
                return;
            }
            _ => {}
        }
    }

    if addr >= ADC_FIRST && addr < ADC_LAST {
        let off = addr - ADC_FIRST;
        if off == 0x08 {
            // CR: the conversion start flag clears itself
            with_state(|s| {
                s.mem.insert(addr, value & 0x7FFF_FFFF);
            });
            return;
        }
    }

    if let Some(base) = timer_base(addr) {
        if addr - base == TIM_CNT {
            with_state(|s| {
                s.timers.insert(base, value);
            });
            return;
        }
    }

    with_state(|s| {
        s.mem.insert(addr, value);
    });
}

// --- the 16 and 8 bit views ------------------------------------------------

pub(crate) fn bus_read16(addr: u32) -> u16 {
    let word = bus_read32(addr & !3);
    ((word >> ((addr & 2) * 8)) & 0xFFFF) as u16
}

pub(crate) fn bus_write16(addr: u32, value: u16) {
    let aligned = addr & !3;
    let shift = (addr & 2) * 8;
    let word = bus_read32(aligned);
    let word = (word & !(0xFFFFu32 << shift)) | ((value as u32) << shift);
    bus_write32(aligned, word);
}

pub(crate) fn bus_read8(addr: u32) -> u8 {
    let word = bus_read32(addr & !3);
    ((word >> ((addr & 3) * 8)) & 0xFF) as u8
}

pub(crate) fn bus_write8(addr: u32, value: u8) {
    let aligned = addr & !3;
    let shift = (addr & 3) * 8;
    let word = bus_read32(aligned);
    let word = (word & !(0xFFu32 << shift)) | ((value as u32) << shift);
    bus_write32(aligned, word);
}
