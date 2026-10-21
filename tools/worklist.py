#!/usr/bin/env python3
"""Produce a per-module work list: which decompiled functions belong to which
interface module (by string reference + call graph closure)."""
import re, os, json, collections

ALL = open('analysis/decomp/ALL.c', errors='replace').read()

# split into functions
parts = re.split(r'/\* ==== (\S+) @ (0x[0-9a-fA-F]+) ==== \*/', ALL)
funcs = {}
order = []
for i in range(1, len(parts), 3):
    name, addr, body = parts[i], parts[i+1], parts[i+2]
    funcs[addr] = (name, body)
    order.append(addr)

callmap = {}
for addr, (name, body) in funcs.items():
    calls = set()
    for c in re.findall(r'\b(FUN_[0-9A-Fa-f]+)\s*\(', body):
        for a2, (n2, b2) in funcs.items():
            if n2 == c:
                calls.add(a2)
    callmap[addr] = calls

MODULES = {
 'hwiDrive':      ['dutyA', 'dutyB', 'nFault'],
 'hwiSonar':      ['echo'],
 'hwiLed':        ['hwiLed'],
 'hwiBuzz':       ['hwiBuzz'],
 'hwiMic':        ['hwiMic'],
 'hwiEnc':        ['hwiEnc'],
 'hwiLine':       ['hwiLine'],
 'hwiPower':      ['pwrOff'],
 'hwiLightSense': ['hwiLightSense'],
 'hwiAdc':        ['hwiAdc'],
 'bleCom':        ['bleCom', 'buf', 'BLE-boot', 'BLE-RSP', 'PPRT', 'PPRE'],
 'drvBattery':    ['drvBattery', 'voltage', 'chargeStat', 'remainingCapacity'],
 'appSys':        ['appSys', 'ver', 'mat', 'date', 'ttpms', 'hwInit'],
 'appDrive':      ['appDrive'],
 'appLights':     ['appLights'],
 'appLightSense': ['appLightSense'],
}

def refs(body, word):
    return re.search(r'"%s"' % re.escape(word), body) is not None

assigned = {}
for mod, words in MODULES.items():
    seeds = [a for a, (n, b) in funcs.items() if any(refs(b, w) for w in words)]
    # closure: functions called by seeds that don't reference other modules
    closure = set(seeds)
    frontier = list(seeds)
    for _ in range(4):
        nxt = []
        for a in frontier:
            for c in callmap.get(a, ()):
                if c not in closure:
                    closure.add(c); nxt.append(c)
        frontier = nxt
    for a in closure:
        assigned.setdefault(a, []).append(mod)

# report
out = {}
for a in order:
    mods = assigned.get(a, [])
    name, body = funcs[a]
    if mods:
        out.setdefault(','.join(sorted(set(mods))), []).append((a, name, len(body)))

with open('analysis/WORKLIST.txt', 'w') as f:
    for mod in sorted(out):
        f.write(f"\n===== {mod} =====\n")
        for a, n, l in sorted(out[mod]):
            f.write(f"  {a}  {n:16s} {l:6d} bytes   (file analysis/decomp/{n}_0x{a[2:].upper()}.c)\n")

for mod in sorted(out):
    print(f"{mod:28s} {len(out[mod]):3d} functions")
json.dump({k: [a for a, _, _ in v] for k, v in out.items()},
          open('analysis/worklist.json', 'w'), indent=1)
