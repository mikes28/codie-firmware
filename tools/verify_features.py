#!/usr/bin/env python3
"""Verify that the rebuilt firmware still exposes the original feature surface.

Compares the protocol-visible string literals of the rebuilt image against the
ones recovered from the original dump.  Every string the original uses on the
wire must be present in the rebuild.
"""
import re, sys, struct

ORIG = 'stm32_flash.bin'
NEW = sys.argv[1] if len(sys.argv) > 1 else 'fw/build/fw.bin'

# strings the original puts on the wire (interface names, op names, protocol
# tokens, error texts).  Order irrelevant, presence required.
MUST = [
    # interface names
    'appDrive', 'appLights', 'appSys', 'appLightSense', 'drvBattery',
    'hwiAdc', 'hwiBuzz', 'hwiDrive', 'hwiEnc', 'hwiLed', 'hwiLightSense',
    'hwiLine', 'hwiMic', 'hwiPower', 'hwiSonar', 'bleCom',
    # op names
    'dutyA', 'dutyB', 'nFault', 'pwrOff', 'Vsens', 'echo', 'buf',
    'voltage', 'current', 'remainingCapacity', 'chargeStat', 'temp',
    'ver', 'mat', 'date', 'ttpms', 'hwInit',
    # protocol tokens
    'get', 'set', 'call', ':proxy', 'greeting', 'message', 'msg:',
    'inf', 'dbg', 'wrn', 'err',
    # BLE link
    'BLE-boot', 'BLE-RSP HELLO', 'BLE-RSP GAP SET MODE: ', 'PPRT', 'PPRE',
    'ERR:BLE_serout_TO', '+ack',
    # errors
    'Invalid command!', 'No interface:', 'possible cmdBuf overflow',
    'BUFOVF!!!!!', 'BLE framing error', 'BLE USART noise',
    'BLE USART overrun', 'USART framing error',
    # version block
    '0.5.0hw2.1', '1602121419', '1000',
]


def strings(path):
    d = open(path, 'rb').read()
    out = set()
    cur, start = bytearray(), None
    for i, b in enumerate(d):
        if 0x20 <= b < 0x7f:
            if start is None:
                start = i
            cur.append(b)
        else:
            if len(cur) >= 2 and start is not None:
                out.add(bytes(cur).decode('ascii'))
            cur, start = bytearray(), None
    return out, d


def main():
    so, do = strings(ORIG)
    sn, dn = strings(NEW)
    missing = [s for s in MUST if s in so and s not in sn]
    print(f"original: {len(so)} strings, {len(do)} bytes")
    print(f"rebuilt : {len(sn)} strings, {len(dn)} bytes")
    if missing:
        print(f"\nMISSING from the rebuild ({len(missing)}):")
        for m in missing:
            print(f"  - {m!r}")
        return 1
    print("\nOK: every protocol-visible string of the original is present.")
    # structural checks
    if len(dn) > 0x20000:
        print(f"FAIL: image is {len(dn)} bytes, flash is 128 KB")
        return 1
    sp, reset = struct.unpack_from('<II', dn, 0)
    print(f"vector table: SP=0x{sp:08X} (orig 0x20003FFF)  reset=0x{reset:08X}")
    if not (0x08000000 < reset < 0x08020000):
        print("FAIL: reset vector not in flash")
        return 1
    if sp not in (0x20003FFF, 0x20004000):
        print(f"WARN: initial SP 0x{sp:08X} differs from the original 0x20003FFF")
    print(f"OK: image size {len(dn)} bytes fits in 128 KB")
    return 0


if __name__ == '__main__':
    sys.exit(main())
