#!/usr/bin/env bash
# Flash a firmware image to the Codie robot over USB DFU (STM32 ROM bootloader).
#
#   ./flash.sh [image.bin]
#
# The host needs the DFU device: either native dfu-util with the usbfs node
# accessible, or docker (the script falls back to docker automatically).
set -euo pipefail

IMG="${1:-build/fw.bin}"
[ -f "$IMG" ] || { echo "no such image: $IMG" >&2; exit 1; }

VID=0483:df11
ALT=0
ADDR=0x08000000
SIZE=$(stat -c%s "$IMG")

# The ROM bootloader only exposes the flash in 2 KiB pages; pad to a whole
# number of pages so dfu-util does not complain about a partial last transfer.
PAD=$(( (SIZE + 2047) / 2048 * 2048 ))
TMP="$(mktemp -d)/$(basename "$IMG")"
trap 'rm -rf "$(dirname "$TMP")"' EXIT
cp "$IMG" "$TMP"
truncate -s "$PAD" "$TMP"

run_dfu() {
  dfu-util -d "$VID" -a "$ALT" -s "$ADDR" -D "$TMP" "$@"
}

if command -v dfu-util >/dev/null 2>&1 && [ -w /dev/bus/usb ] 2>/dev/null; then
  echo ">> flashing with native dfu-util"
  run_dfu
else
  echo ">> flashing with docker (dfu-util image)"
  docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
    dfu-util -d "$VID" -a "$ALT" -s "$ADDR" -D "$TMP" "$@"
fi

echo
echo ">> verifying: reading the flash back"
VERIFY="$(mktemp -d)/verify.bin"
if command -v dfu-util >/dev/null 2>&1 && [ -w /dev/bus/usb ] 2>/dev/null; then
  dfu-util -d "$VID" -a "$ALT" -s "$ADDR" -Z "$PAD" -U "$VERIFY" || true
else
  docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
    dfu-util -d "$VID" -a "$ALT" -s "$ADDR" -Z "$PAD" -U "$VERIFY" || true
fi

if [ -s "$VERIFY" ]; then
  if cmp -s "$IMG" <(head -c "$SIZE" "$VERIFY"); then
    echo "OK: flash contents match $IMG"
  else
    echo "MISMATCH: flash differs from $IMG" >&2
    cmp "$IMG" <(head -c "$SIZE" "$VERIFY") || true
    exit 1
  fi
  rm -f "$VERIFY"
else
  echo "note: could not read the flash back for verification (device may have left DFU mode)"
fi

# Leave the DFU mode LAST: the ROM bootloader resets the MCU out of the
# bootloader only when it is told to leave.  Without this the chip keeps
# running the bootloader and the application never starts (the robot then
# advertises over BLE but drops every connection).
# ':leave' resets the MCU into the application.
echo
echo ">> leaving DFU mode (the robot reboots into the application)"
if command -v dfu-util >/dev/null 2>&1 && [ -w /dev/bus/usb ] 2>/dev/null; then
  dfu-util -d "$VID" -a "$ALT" -s "$ADDR:leave" || \
    echo "warning: leave failed — power-cycle the robot (unplug USB + battery)" >&2
else
  docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
    dfu-util -d "$VID" -a "$ALT" -s "$ADDR:leave" || \
    echo "warning: leave failed — power-cycle the robot (unplug USB + battery)" >&2
fi

echo "done. The robot reboots into the application now."
