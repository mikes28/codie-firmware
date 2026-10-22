#!/bin/sh
# Build the Rust firmware image and (optionally) flash it.
#
#   ./build.sh            build target/.../release/codie-fw and build/fw.bin
#   ./build.sh flash      build and flash the robot (USB DFU)
#
# The toolchain:
#   cargo        a Rust toolchain with the target thumbv6m-none-eabi
#                (nightly + `-Z build-std=core` if the target is not installed)
#   objcopy      arm-none-eabi-objcopy, auto detected in ../tools/ or on $PATH
set -e

cd "$(dirname "$0")"

TARGET=thumbv6m-none-eabi
BUILD=build

TOOLCHAIN_PATH=${TOOLCHAIN_PATH:-$(ls -d ../tools/xpack-arm-none-eabi-gcc-*/bin 2>/dev/null | head -1)}
if [ -z "$TOOLCHAIN_PATH" ]; then
    # not in ../tools: look for the objcopy on $PATH
    _oc=$(command -v arm-none-eabi-objcopy 2>/dev/null || true)
    [ -n "$_oc" ] && TOOLCHAIN_PATH=$(dirname "$_oc")
fi
OBJCOPY="$TOOLCHAIN_PATH/arm-none-eabi-objcopy"
SIZE="$TOOLCHAIN_PATH/arm-none-eabi-size"

mkdir -p "$BUILD"

echo "== building the firmware =="
# RUSTFLAGS replaces the rustflags of .cargo/config.toml, so the whole list
# has to be repeated here.  The link map (which symbol is how big) is
# written next to the image.
RUSTFLAGS="-C link-arg=-Tlinker/stm32g071rb.ld -C link-arg=--nmagic -C link-arg=--gc-sections -C link-arg=-Map=$BUILD/fw.map" \
    cargo build --release --target "$TARGET"

"$OBJCOPY" -O binary "target/$TARGET/release/codie-fw" "$BUILD/fw.bin"

# The size of the image: this is the number that has to stay below the
# 128 KB of the flash (the C reference rebuild is 12 608 bytes).
if [ -x "$SIZE" ]; then
    echo "== sections =="
    "$SIZE" -A "target/$TARGET/release/codie-fw"
    echo "== symbols (the 12 biggest) =="
    "$SIZE" "target/$TARGET/release/codie-fw"
fi
echo "built $BUILD/fw.bin: $(stat -c%s "$BUILD/fw.bin") bytes"
echo "link map: $BUILD/fw.map"

if [ "$1" = "flash" ]; then
    echo "== flashing =="
    ../fw/flash.sh "$BUILD/fw.bin"
fi
