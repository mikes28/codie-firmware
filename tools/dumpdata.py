#!/usr/bin/env python3
"""Dump the data region of the flash as words, annotating pointers to strings."""
import struct, sys, re
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN

FLASH = open('stm32_flash.bin', 'rb').read()
BASE = 0x08000000

def u32(o): return struct.unpack_from('<I', FLASH, o)[0]
def u16(o): return struct.unpack_from('<H', FLASH, o)[0]

def cstr_at(a):
    o = a - BASE
    if o < 0 or o >= len(FLASH): return None
    end = FLASH.find(b'\0', o)
    if end < 0: return None
    s = FLASH[o:end]
    if len(s) == 0 or len(s) > 64: return None
    try: t = s.decode('ascii')
    except: return None
    if all(0x20 <= ord(c) < 0x7f for c in t): return t
    return None

lo = int(sys.argv[1], 0) if len(sys.argv) > 1 else 0x0800C400
hi = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0x0800D624
for a in range(lo, hi, 4):
    v = u32(a - BASE)
    s = cstr_at(v) if v != 0 else None
    ann = ''
    if s is not None:
        ann = f'  -> "{s}"'
    elif 0x08000000 <= v < 0x08020000:
        ann = '  (flash ptr)'
    elif 0x20000000 <= v < 0x20004000:
        ann = '  (ram ptr)'
    elif 0x40000000 <= v < 0x50000000:
        ann = '  (periph)'
    elif 0x08000000 <= v < 0x0800C660:
        ann = '  (code ptr)'
    print(f'0x{a:08X}: {v:08X}{ann}')
