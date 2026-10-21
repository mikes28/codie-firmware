/* bleCom.h - module local interface of the BLE serial driver.
 *
 * The shared contract (fw/include/codie.h) owns bleCom_init/write/read/
 * tx_pending and out_str/out_char.  This header only adds the extra
 * cross module symbol that the reconstruction needs: bleCom_poll(),
 * which the main loop uses to pump the UART when bleCom_read() has
 * nothing to hand out.
 *
 * It belongs to module "bleCom" (fw/modules/bleCom.c).
 */
#ifndef BLECOM_H
#define BLECOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Pump the BLE USART: move the received bytes into the ring buffer and
 * report the USART errors ("BLE framing error", "BLE USART noise",
 * "BLE USART overrun").  The main loop calls this when bleCom_read()
 * has nothing to hand out.  (Original: FUN_08007EC8 + FUN_08008130.)
 *
 * NOTE: fw/include/codie.h declares the same symbol - this header only
 * exists so that the module local view of the driver is complete. */
void bleCom_poll(void);

#ifdef __cplusplus
}
#endif

#endif /* BLECOM_H */
