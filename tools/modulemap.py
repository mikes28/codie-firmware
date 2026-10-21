#!/usr/bin/env python3
"""Map every decompiled function to the module it belongs to by looking at the
string literals it references and the functions it calls."""
import os, re, glob, collections, json

DEC = 'analysis/decomp'
MOD_WORDS = ['appDrive', 'appLights', 'appSys', 'appLightSense', 'drvBattery',
             'hwiAdc', 'hwiBuzz', 'hwiDrive', 'hwiEnc', 'hwiLed', 'hwiLightSense',
             'hwiLine', 'hwiMic', 'hwiPower', 'hwiSonar', 'bleCom', 'hwInit',
             'dutyA', 'dutyB', 'nFault', 'pwrOff', 'echo', 'Vsens', 'ttpms',
             'chargeStat', 'remainingCapacity', 'voltage', 'current', 'temp',
             'greeting', ':proxy', 'message', 'buf', 'ver', 'mat', 'date',
             'BLE-boot', 'PPRT', 'PPRE', '+ack', 'Invalid command', 'No interface',
             'possible cmdBuf', 'BUFOVF', 'USART framing', 'BLE USART', 'BLE framing',
             'ERR:BLE', 'on', 'true']

files = sorted(set(glob.glob(os.path.join(DEC, 'FUN_*.c')) +
                   glob.glob(os.path.join(DEC, '*Handler_0x*.c'))))
funcs = []
for f in files:
    m = re.search(r'0x([0-9a-fA-F]{8})\.c$', f)
    if not m:
        continue
    addr = int(m.group(1), 16)
    body = open(f, errors='replace').read()
    lits = re.findall(r'"((?:[^"\\]|\\.)*)"', body)
    calls = set(re.findall(r'\b(FUN_[0-9A-Fa-f]+)\s*\(', body))
    funcs.append((addr, f, lits, calls, body))

by_addr = {a: (f, lits, calls, body) for a, f, lits, calls, body in funcs}
call_addr = {}
for a, f, lits, calls, body in funcs:
    for c in calls:
        mm = re.search(r'0x([0-9A-Fa-f]{8})', c)
        if mm:
            call_addr[c] = int(mm.group(1), 16)

def score(lits):
    s = collections.Counter()
    for lit in lits:
        for w in MOD_WORDS:
            if lit == w:
                s[w] += 3
            elif lit.startswith(w):
                s[w] += 1
    return s

inherit = {}
for a, f, lits, calls, body in funcs:
    s = score(lits)
    inherit[a] = s.most_common(1)[0][0] if s else None

for _ in range(8):
    changed = False
    for a, f, lits, calls, body in funcs:
        if inherit.get(a):
            continue
        agg = collections.Counter()
        for c in calls:
            ca = call_addr.get(c)
            if ca is not None and inherit.get(ca):
                agg[inherit[ca]] += 1
        if agg:
            inherit[a] = agg.most_common(1)[0][0]
            changed = True
    if not changed:
        break

groups = collections.defaultdict(list)
for a, f, lits, calls, body in funcs:
    groups[inherit.get(a) or 'UNCLASSIFIED'].append((a, lits, calls, f))

with open('analysis/MODULE_MAP.txt', 'w') as out:
    for mod in sorted(groups, key=lambda m: (m == 'UNCLASSIFIED', m)):
        items = sorted(groups[mod])
        out.write(f"\n===== {mod}  ({len(items)} functions) =====\n")
        for a, lits, calls, f in items:
            sz = os.path.getsize(f)
            rs = ' | '.join(lits[:6])
            out.write(f"  0x{a:08X}  {os.path.basename(f):34s} {sz:6d}B  {rs}\n")
for mod in sorted(groups, key=lambda m: (m == 'UNCLASSIFIED', m)):
    print(f"{mod:16s} {len(groups[mod]):4d} functions")
json.dump({k: sorted(a for a, _, _, _ in v) for k, v in groups.items()},
          open('analysis/modules.json', 'w'), indent=1)
