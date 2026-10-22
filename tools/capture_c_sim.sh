#!/usr/bin/env bash
# Capture the observable behaviour of the C firmware as a golden fixture.
#
#   tools/capture_c_sim.sh [output]
#
# The C firmware of fw/ is compiled for the host against the fake memory
# mapped peripheral map of tools/host_sim.c and driven with the scenario of
# that harness (the boot, then the seven commands).  The simulator writes
# the observable facts of the run - the bytes that came out of the command
# link, the bytes that went into the AT link and the values of the
# registers the firmware touched - into the file given by C_SIM_FACTS.
#
# The output is the fixture rust/tests/data/c_sim.txt; the Rust port is
# compared with it by rust/tests/c_equivalence.rs.
#
# NOTE: the transcript of tools/run_sim.sh models the baud divisor as
# 96000000/baud (truncated).  That is a simplification of the *harness*:
# the firmware itself (fw/modules/hwi_b_hal.c hwi_usart_init, and the
# original FUN_0800B3FC) rounds the divisor up, so the facts mode lets the
# firmware do the register writes and reads back what it wrote.
set -euo pipefail
cd "$(dirname "$0")/.."

OUT=${1:-rust/tests/data/c_sim.txt}
mkdir -p "$(dirname "$OUT")"

OBJ=$(mktemp -d)
trap 'rm -rf "$OBJ"' EXIT

for f in fw/src/main.c fw/src/system_stm32g0.c fw/modules/*.c; do
    o="$OBJ/$(echo "$f" | tr '/' '_').o"
    gcc -std=gnu99 -O1 -I fw/include -I fw/modules -Dmain=fw_main -c "$f" -o "$o" 2>/dev/null
done

gcc -std=gnu99 -O1 -I fw/include -I fw/modules tools/host_sim.c "$OBJ"/*.o -o "$OBJ/sim" \
    -lpthread \
    -Wl,--wrap=hwi_usart_init,--wrap=hwi_usart_enable,--wrap=hwi_usart_putc,\
--wrap=hwi_usart_rx_ready,--wrap=hwi_usart_getc,--wrap=hwi_usart_status,\
--wrap=hwi_usart_clear,--wrap=hwi_usart_tx_ready

FACTS=$(mktemp)
trap 'rm -rf "$OBJ" "$FACTS"' EXIT

C_SIM_FACTS="$FACTS" "$OBJ/sim" > /dev/null

{
    echo "# c_sim.txt - the observable behaviour of the C firmware (fw/)"
    echo "#"
    echo "# Captured with: tools/capture_c_sim.sh"
    echo "# C source:      fw/ (the C reconstruction of the original firmware)"
    echo "# Harness:       tools/host_sim.c (the C firmware on the host against"
    echo "#                a fake memory mapped peripheral map)"
    echo "# Date:          $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
    echo "#"
    echo "# The facts below are what the C firmware did during the scenario of"
    echo "# tools/host_sim.c: the boot, then the commands"
    echo "#"
    echo "#   get drvBattery voltage"
    echo "#   get drvBattery chargeStat"
    echo "#   get nosuchiface op"
    echo "#   garbage"
    echo "#   set hwiDrive dutyA 200"
    echo "#   set hwiDrive dutyB 100"
    echo "#   call drvBattery hwInit"
    echo "#"
    echo "# Format: \"key: value\", one fact per line."
    echo "#   <n>.cmdlink.tx  the bytes the firmware sent on USART4 (the command"
    echo "#                   and reply link), hex encoded"
    echo "#   <n>.atlink.tx   the bytes the firmware sent on USART3 (the AT link"
    echo "#                   to the BLE module), hex encoded"
    echo "#   reg.*           the value of a register the firmware touched"
    echo ""
    cat "$FACTS"
} > "$OUT"

echo "wrote $OUT"
