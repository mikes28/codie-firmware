#!/usr/bin/env python3
"""Build an ARM ELF32 from the raw STM32 flash dump so Ghidra can import it
with correct Thumb mapping symbols and function symbols."""
import struct, sys, re
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN

BIN = sys.argv[1] if len(sys.argv) > 1 else 'stm32_flash.bin'
OUT = sys.argv[2] if len(sys.argv) > 2 else 'tools/fw.elf'
d = open(BIN, 'rb').read()
BASE = 0x08000000
N = len(d)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)

# ---------------------------------------------------------------- functions
funcs = set()
insns = list(md.disasm(d, BASE))
by_addr = {i.address: (i.mnemonic, i.op_str) for i in insns}
for i in insns:
    if i.mnemonic in ('bl', 'b', 'bx', 'blx', 'beq', 'bne', 'bgt', 'bge', 'blt',
                      'ble', 'bcs', 'bcc', 'bhi', 'bls', 'bpl', 'bmi', 'bvc', 'bvs'):
        for t in re.findall(r'0x[0-9a-fA-F]+', i.op_str):
            t = int(t, 16) & ~1
            if BASE <= t < BASE + N:
                funcs.add(t)
for a in range(0, N - 1, 2):
    h = struct.unpack_from('<H', d, a)[0]
    if (h & 0xFF00) == 0xB500:          # push {..,lr}
        funcs.add(BASE + a)
# vector table entries
for i in range(1, 16):
    v = struct.unpack_from('<I', d, 4*i)[0]
    if v & 1:
        funcs.add(v & ~1)
funcs = sorted(f for f in funcs if BASE <= f < BASE + N)

# The 16 interface handlers are only reachable through the dispatch table at
# 0x0800D164 - without an explicit entry point Ghidra never creates a function
# for them, so add them here.
HANDLERS = [0x080073DC, 0x080090A4, 0x08008B3C, 0x08006F2C, 0x08009F88,
            0x0800837C, 0x08008490, 0x0800962C, 0x08008DFC, 0x08009478,
            0x0800981C, 0x080093A8, 0x08005F00, 0x08003848, 0x080064F0,
            0x08003854]
for h in HANDLERS:
    if BASE <= h < BASE + N and h not in funcs:
        funcs.append(h)
funcs = sorted(funcs)

# ---------------------------------------------------------------- data runs
# verified by hand: dispatch table + string table
DATA_RANGES = [(0x0800C400, 0x0800C700)]
# aligned runs of >=3 consecutive plausible flash/ram pointers
def ptrlike(v):
    return (BASE <= v < BASE + N) or (0x20000000 <= v < 0x20004000) or (0x40000000 <= v < 0x50000000)
run = []
runs = []
for off in range(0, N - 3, 4):
    v = struct.unpack_from('<I', d, off)[0]
    if ptrlike(v):
        run.append(off)
    else:
        if len(run) >= 3:
            runs.append((run[0], run[-1] + 4))
        run = []
if len(run) >= 3:
    runs.append((run[0], run[-1] + 4))

def in_data(a):
    for lo, hi in DATA_RANGES:
        if lo <= a < hi:
            return True
    for lo, hi in runs:
        if lo <= a < hi:
            return True
    return False

# ---------------------------------------------------------------- ELF build
def sym(name, value, shndx, info=0, size=0):
    return name, value, shndx, info, size

symbols = []
symtab = bytearray()
strtab = bytearray(b'\0')
sym_offsets = {}

def add_symbol(name, value, shndx, info, size=0):
    off = len(symtab)
    nm_off = len(strtab)
    strtab.extend(name.encode() + b'\0')
    symtab.extend(struct.pack('<IIIBBH', name and sym_offsets.get(name, 0) or 0, value, size, info, 0, shndx))
    return off

# build properly: symbol name offset must be in strtab
def add_sym(name, value, shndx, info, size=0):
    nm_off = len(strtab)
    strtab.extend(name.encode() + b'\0')
    symtab.extend(struct.pack('<IIIBBH', nm_off, value, size, info, 0, shndx))

add_sym('', 0, 0, 0)                       # null
add_sym('$d', BASE, 1, 0)                 # data at start (vector table)
add_sym('$t', BASE + 0x40, 1, 0)          # code from here
# data / code mapping symbols over the whole section
cur = None
for off in range(0, N, 2):
    a = BASE + off
    kind = 'd' if in_data(a) else 't'
    if kind != cur:
        add_sym('$' + kind, a, 1, 0)
        cur = kind
# function symbols
for i, f in enumerate(funcs):
    add_sym('FUN_%08X' % f, f, 1, (1 << 4) | 2, 0)
# reset + default handler
add_sym('Reset_Handler', struct.unpack_from('<I', d, 4)[0] & ~1, 1, (1 << 4) | 2, 0)

while len(symtab) % 4:
    symtab.extend(b'\0')

shstrtab = bytearray(b'\0')
def shstr(name):
    off = len(shstrtab)
    shstrtab.extend(name.encode() + b'\0')
    return off

TEXT_OFF = 0x1000
text = d

# ---- section headers
shstr_names = ['.text', '.symtab', '.strtab', '.shstrtab']
sh_name_off = {n: shstr(n) for n in shstr_names}

symtab_off = TEXT_OFF + len(text)
symtab_off = (symtab_off + 3) & ~3
strtab_off = symtab_off + len(symtab)
shstrtab_off = strtab_off + len(strtab)
shoff = (shstrtab_off + len(shstrtab) + 3) & ~3

def shdr(name, typ, flags, addr, offset, size, link=0, info=0, align=1, entsize=0):
    return struct.pack('<IIIIIIIIII', sh_name_off.get(name, 0), typ, flags, addr, offset, size, link, info, align, entsize)

sections = [
    shdr('', 0, 0, 0, 0, 0),
    shdr('.text', 1, 0x6, BASE, TEXT_OFF, len(text), 0, 0, 4),
    shdr('.symtab', 2, 0, 0, symtab_off, len(symtab), 3, 1, 4, 16),
    shdr('.strtab', 3, 0, 0, strtab_off, len(strtab), 0, 0, 1),
    shdr('.shstrtab', 3, 0, 0, shstrtab_off, len(shstrtab), 0, 0, 1),
]

ehsize = 52
phoff = 52
phentsize = 32
phnum = 1
e = bytearray()
e.extend(b'\x7fELF\x01\x01\x01\x00' + b'\0' * 8)
e.extend(struct.pack('<HHIIIIIHHHHHH',
    2, 40, 1, 0x08002905, phoff, shoff, 0x05000200,
    ehsize, phentsize, phnum, 40, len(sections), 4))
ph = struct.pack('<IIIIIIII', 1, TEXT_OFF, BASE, BASE, len(text), len(text), 5, 0x1000)

out = bytearray()
out.extend(e)
out.extend(ph)
out.extend(b'\0' * (TEXT_OFF - len(out)))
out.extend(text)
out.extend(b'\0' * (symtab_off - len(out)))
out.extend(symtab)
out.extend(strtab)
out.extend(shstrtab)
out.extend(b'\0' * (shoff - len(out)))
for s in sections:
    out.extend(s)

open(OUT, 'wb').write(out)
print(f"wrote {OUT}: {len(out)} bytes, {len(funcs)} function symbols, "
      f"{len(runs)} literal-pool runs, {len(DATA_RANGES)} manual data ranges")
