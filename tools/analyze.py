#!/usr/bin/env python3
"""Raw-binary analysis of the Codie STM32 flash dump: vectors, thumb prologues,
literal pools, string xrefs.  No ELF needed - addresses are flash-relative."""
import struct, sys, re, json
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN

FLASH = open(sys.argv[1] if len(sys.argv) > 1 else 'stm32_flash.bin', 'rb').read()
BASE = 0x08000000

def u32(o): return struct.unpack_from('<I', FLASH, o)[0]
def u16(o): return struct.unpack_from('<H', FLASH, o)[0]

# ---- vector table -------------------------------------------------------
VT = 0
print("== vector table (0x08000000) ==")
names = ['SP','Reset','NMI','HardFault','MemManage','BusFault','UsageFault',
         'Reserved7','Reserved8','Reserved9','Reserved10','SVCall','DebugMon',
         'Reserved13','PendSV','SysTick']
vectors = []
for i in range(0, 16):
    v = u32(VT + 4*i)
    vectors.append(v)
    if i < len(names):
        print(f"  {i:2d} {names[i]:<10} 0x{v:08X}")

# ---- find code/data split ----------------------------------------------
# The reset handler is at 0x08002904 -> code starts around there? No: startup
# code lives between VT and 0x2904 typically.  Scan whole image.
print("\n== reset handler target ==", hex(vectors[1]))

# find literal pools: sequences of addresses in 0x08000000..0x08020000
def looks_addr(v): return 0x08000000 <= v < 0x08020000

# ---- strings with addresses --------------------------------------------
strings = []
cur = bytearray()
start = None
for i, b in enumerate(FLASH):
    if 0x20 <= b < 0x7f:
        if start is None: start = i
        cur.append(b)
    else:
        if len(cur) >= 4 and start is not None:
            strings.append((BASE+start, bytes(cur).decode()))
        cur = bytearray(); start = None
print(f"\n== {len(strings)} ascii strings ==")

# ---- capstone: find function starts via push {... lr} prologues ---------
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
md.detail = False

def is_thumb_func_start(off):
    """PUSH {r4-r7,lr} = 0xB5xx ; PUSH {r0-r7,lr}=0xB5FF"""
    if off + 2 > len(FLASH): return False
    h = u16(off)
    if (h & 0xFF00) == 0xB500: return True
    if (h & 0xFE00) == 0xB400:  # push r0-r3 (+lr variants)
        return True
    return False

# Disassemble whole image as thumb, collect call targets
call_targets = {}
xrefs = {}
for ins in md.disasm(FLASH, BASE):
    if ins.mnemonic in ('bl', 'blx', 'b', 'bx'):
        try:
            t = int(ins.op_str.lstrip('#'), 0) & ~1
        except ValueError:
            continue
        if 0x08000000 <= t < 0x08000000 + len(FLASH):
            call_targets.setdefault(t, 0)
            call_targets[t] += 1
            xrefs.setdefault(t, []).append(ins.address)

print(f"\n== distinct direct branch targets: {len(call_targets)} ==")

# ---- string table region -----------------------------------------------
data_region = None
print("\n== strings in address order (>=5 chars, printable) ==")
for addr, s in strings:
    if len(s) >= 5:
        print(f"  0x{addr:08X}  {s!r}")
