#!/usr/bin/env python3
"""Check the Rust firmware image against the dump of the original firmware.

This is the counterpart of `tools/verify_features.py` (the check of the C
rebuild).  The list of the protocol visible strings is the one of the C tool -
it is imported, so that the two cannot drift apart.

The difference to the C tool is the way the strings are looked for: the C
tool extracts the *maximal printable runs* of the image and compares them
with the list.  The Rust image stores its `&str` literals merged (the Rust
compiler puts them into one `.rodata` blob without a NUL between them), so
the maximal runs of the Rust image are long concatenations
("hwiDrivehwiPowergreeting...").  The strings are therefore looked for as
*substrings* of the image - they are there, byte for byte, they are just not
separated by a NUL.

    python3 rust/verify_features.py rust/build/fw.bin
"""
import importlib.util
import os
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ORIG = os.path.join(REPO, "stm32_flash.bin")


def load_must_list():
    """The protocol visible strings of the original (the list of the C tool)."""
    path = os.path.join(REPO, "tools", "verify_features.py")
    spec = importlib.util.spec_from_file_location("c_verify_features", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.MUST


def strings(path):
    """The maximal printable runs of a file (the method of the C tool)."""
    d = open(path, "rb").read()
    out = set()
    cur, start = bytearray(), None
    for i, b in enumerate(d):
        if 0x20 <= b < 0x7F:
            if start is None:
                start = i
            cur.append(b)
        else:
            if len(cur) >= 2 and start is not None:
                out.add(bytes(cur).decode("ascii"))
            cur, start = bytearray(), None
    return out, d


def main():
    new = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO, "rust", "build", "fw.bin")
    if not os.path.exists(ORIG):
        print(f"FAIL: the dump {ORIG} is not there")
        return 1
    if not os.path.exists(new):
        print(f"FAIL: the image {new} is not there")
        return 1

    so, do = strings(ORIG)
    sn, dn = strings(new)

    missing = []
    for s in load_must_list():
        if s in so and s.encode() not in dn:
            missing.append(s)

    print(f"original: {len(so)} strings, {len(do)} bytes")
    print(f"rebuilt : {len(sn)} strings, {len(dn)} bytes")
    if missing:
        print(f"\nMISSING from the rebuild ({len(missing)}):")
        for m in missing:
            print(f"  - {m!r}")
        return 1
    print("\nOK: every protocol-visible string of the original is present.")

    # the structural checks of the C tool
    if len(dn) > 0x20000:
        print(f"FAIL: image is {len(dn)} bytes, flash is 128 KB")
        return 1
    sp, reset = struct.unpack_from("<II", dn, 0)
    print(f"vector table: SP=0x{sp:08X} (orig 0x20003FFF)  reset=0x{reset:08X}")
    if not (0x08000000 < reset < 0x08020000):
        print("FAIL: reset vector not in flash")
        return 1
    if sp not in (0x20003FFF, 0x20004000):
        print(f"WARN: initial SP 0x{sp:08X} differs from the original 0x20003FFF")
    print(f"OK: image size {len(dn)} bytes fits in 128 KB")
    return 0


if __name__ == "__main__":
    sys.exit(main())
