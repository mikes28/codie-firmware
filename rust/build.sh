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

TOOLCHAIN_PATH=${TOOLCHAIN_PATH:-$(firstword $(ls -d ../tools/xpack-arm-none-eabi-gcc-*/bin 2>/dev/null) $(command -v arm-none-eabi-objcopy 2>/dev/null | xargs dirname 2>/dev/null))}
OBJCOPY="$TOOLCHAIN_PATH/arm-none-eabi-objcopy"
SIZE="$TOOLCHAIN_PATH/arm-none-eabi-size"

echo "== building the firmware =="
cargo build --release --target "$TARGET"

mkdir -p "$BUILD"
"$OBJCOPY" -O binary "target/$TARGET/release/codie-fw" "$BUILD/fw.bin"
[ -x "$SIZE" ] && "$SIZE" "target/$TARGET/release/codie-fw" || true
echo "built $BUILD/fw.bin: $(stat -c%s "$BUILD/fw.bin") bytes"

if [ "$1" = "flash" ]; then
    echo "== flashing =="
    ../fw/flash.sh "$BUILD/fw.bin"
fi
