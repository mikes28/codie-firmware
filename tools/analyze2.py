#!/usr/bin/env python3
"""Deep analysis of the Codie STM32 flash dump.

Produces:
  * function map (recovered from branch targets + prologues)
  * string table with xrefs (literal-pool pointer scan)
  * peripheral/IO register address usage
  * interface dispatch table reconstruction
"""
import struct, sys, json, re
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN

PATH = sys.argv[1] if len(sys.argv) > 1 else 'stm32_flash.bin'
FLASH = open(PATH, 'rb').read()
BASE = 0x08000000
END = BASE + len(FLASH)

def u32(o): return struct.unpack_from('<I', FLASH, o)[0]
def u16(o): return struct.unpack_from('<H', FLASH, o)[0]

# ---------------------------------------------------------------- strings
def extract_strings(minlen=4):
    out, cur, start = [], bytearray(), None
    for i, b in enumerate(FLASH):
        if 0x20 <= b < 0x7f:
            if start is None: start = i
            cur.append(b)
        else:
            if len(cur) >= minlen and start is not None:
                out.append((BASE + start, bytes(cur).decode('ascii')))
            cur, start = bytearray(), None
    return out

STRINGS = extract_strings(4)
STR_BY_ADDR = {a: s for a, s in STRINGS}

# ---------------------------------------------------------------- disasm
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
md.detail = False

CODE_END = 0x0800C660  # strings start here; refine later

# Disassemble the code region linearly
instrs = []
for ins in md.disasm(FLASH[:CODE_END - BASE], BASE):
    instrs.append((ins.address, ins.mnemonic, ins.op_str, ins.bytes))

# function starts: branch targets
funcs = set()
for addr, mn, ops, bs in instrs:
    if mn in ('bl', 'b', 'bx', 'blx', 'beq', 'bne', 'bgt', 'bge', 'blt', 'ble',
              'bcs', 'bcc', 'bhi', 'bls', 'bpl', 'bmi', 'bvc', 'bvs'):
        for t in re.findall(r'0x[0-9a-fA-F]+', ops):
            t = int(t, 16) & ~1
            if BASE <= t < CODE_END:
                funcs.add(t)

# plus prologue scan
def is_push_lr(off):
    if off + 2 > len(FLASH): return False
    h = u16(off)
    return (h & 0xFF00) == 0xB500 or (h & 0xFE00) == 0xB400

for off in range(0, CODE_END - BASE, 2):
    if is_push_lr(off):
        funcs.add(BASE + off)

funcs = sorted(f for f in funcs if f >= BASE)
print(f"code region: 0x{BASE:08X}..0x{CODE_END:08X} ({CODE_END-BASE} bytes)")
print(f"candidate function starts: {len(funcs)}")

# ---------------------------------------------------------------- string xrefs
# find literal pools: 32-bit words in flash that point at a string start
xref = {}
for off in range(0, len(FLASH) - 3, 4):
    v = u32(off)
    if v in STR_BY_ADDR:
        xref.setdefault(v, []).append(BASE + off)

def s_at(a):
    return STR_BY_ADDR.get(a)

print("\n== string xrefs (pool entry -> string) ==")
for addr in sorted(STR_BY_ADDR):
    s = STR_BY_ADDR[addr]
    if len(s) < 5: continue
    # printable-word check
    if not re.match(r'^[\x20-\x7e]+$', s): continue
    if not re.search(r'[a-zA-Z]{3,}', s): continue
    locs = xref.get(addr, [])
    locs_s = ', '.join(f'0x{l:08X}' for l in locs[:4])
    print(f"  0x{addr:08X} {s!r:<44} <- {locs_s}")

# ---------------------------------------------------------------- peripheral map
print("\n== peripheral addresses referenced ==")
periph = {}
for addr, mn, ops, bs in instrs:
    for t in re.findall(r'#0x[0-9a-fA-F]+', ops):
        v = int(t[1:], 16)
        if 0x40000000 <= v < 0x50000000:
            periph[v] = periph.get(v, 0) + 1
for v in sorted(periph):
    print(f"  0x{v:08X}  x{periph[v]}")

# ---------------------------------------------------------------- RAM usage
ram = {}
for off in range(0, len(FLASH) - 3, 4):
    v = u32(off)
    if 0x20000000 <= v < 0x20004000:
        ram[v] = ram.get(v, 0) + 1
print(f"\n== distinct RAM addresses referenced: {len(ram)} ==")
for v in sorted(ram)[:40]:
    print(f"  0x{v:08X} x{ram[v]}")
print("  ...")

json.dump({'funcs': funcs}, open('tools/funcs.json', 'w'))
