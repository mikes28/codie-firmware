#!/usr/bin/env bash
# Build and run the host simulation of the reconstructed firmware.
#
#   tools/run_sim.sh
#
# The firmware is compiled for the host against a fake memory-mapped
# peripheral map (the HAL writes to absolute addresses, so a 256 MB mapping
# at 0x40000000 is enough).  The USART primitives are interposed with
# ld --wrap so the test can inject commands and capture the replies.
set -euo pipefail
cd "$(dirname "$0")/.."

OBJ=$(mktemp -d)
trap 'rm -rf "$OBJ"' EXIT

for f in fw/src/main.c fw/src/system_stm32g0.c fw/modules/*.c; do
    o="$OBJ/$(echo "$f" | tr '/' '_').o"
    gcc -std=gnu99 -O1 -I fw/include -I fw/modules -Dmain=fw_main -c "$f" -o "$o"
done

gcc -std=gnu99 -O1 -I fw/include -I fw/modules tools/host_sim.c "$OBJ"/*.o -o "$OBJ/sim" \
    -lpthread \
    -Wl,--wrap=hwi_usart_init,--wrap=hwi_usart_enable,--wrap=hwi_usart_putc,\
--wrap=hwi_usart_rx_ready,--wrap=hwi_usart_getc,--wrap=hwi_usart_status,\
--wrap=hwi_usart_clear,--wrap=hwi_usart_tx_ready

"$OBJ/sim"
