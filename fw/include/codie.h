/* codie.h - shared contract for the reconstructed Codie robot firmware.
 *
 * Recovered from the original STM32 flash dump (firmware 0.5.0hw2.1, build
 * 2020-10-07, app version "0.5.0", hardware "2.1").  Every function declared
 * here corresponds to behaviour that exists in the original binary; the module
 * that owns it is noted.
 */
#ifndef CODIE_H
#define CODIE_H

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ *
 * Command protocol, recovered from FUN_0800A0AC / FUN_0800A350 /
 * FUN_0800A16C / FUN_0800A2D0 / FUN_0800A1C8
 *
 *   request   : "<get|set|call> <interface> <op> [args...]"  (one line)
 *   op codes  : 1 = get, 2 = set, 3 = call
 *   reply     : "get|set|call <interface> <op> [value]\n"
 *   system msg: "call :proxy message <level> <text> [extra]\n"
 *               level: 0="???" 1="inf" 2="dbg" 3="wrn" 4="err"
 *   greeting  : "call :proxy greeting \"k\":\"v\" ... \"msg:<text>\"\n"
 * ------------------------------------------------------------------ */
#define OP_GET  1
#define OP_SET  2
#define OP_CALL 3

#define LOG_UNK 0
#define LOG_INF 1
#define LOG_DBG 2
#define LOG_WRN 3
#define LOG_ERR 4

/* command buffer of the original (FUN_08009B14 is called with 0x28) */
#define CMD_BUF_SIZE 40

/* ---- byte level output (owned by ble_link.c) ------------------------- */
void out_str(const char *s);
void out_char(char c);

/* ---- protocol layer (owned by proxy.c) ------------------------------ */
void resp_value(uint8_t op, const char *iface, const char *opname, const char *value);
void resp_message(uint8_t level, const char *text, const char *extra);
void resp_greeting(const char *const *kv, const char *msg);

/* Number -> text, exactly like the original FUN_080068D4 (unsigned) and
 * FUN_0800693C (signed): the RADIX is the third argument (the original passes
 * 16, i.e. HEX, for all its sensor values, 10 for the ADC dump) and the
 * buffer must hold at least radix+2 bytes. */
char *fmt_num(char *buf, int radix, uint32_t v);
char *fmt_num_s(char *buf, int radix, int32_t v);

/* convenience wrappers (decimal) */
void fmt_int(char *buf, int buflen, int32_t v);
void fmt_fixed(char *buf, int buflen, int32_t v, int frac);

/* ---- interface registry (owned by proxy.c) ---------------------------- */
typedef void (*iface_handler_t)(uint8_t op, const char *opname, const char *args);
typedef struct {
    const char *name;
    iface_handler_t handler;
} iface_t;
void iface_register(const iface_t *iface);

/* GPIO pin modes (the original's FUN_0800A964 uses mode-1 < 2 for "special") */
#define GPIO_MODE_INPUT   0
#define GPIO_MODE_OUTPUT  1
#define GPIO_MODE_ALT     2
#define GPIO_MODE_ANALOG  3


void tim_pwm_init(uint32_t tim, uint32_t psc, uint32_t arr, uint8_t ch, uint8_t mode);
void tim_pwm_set(uint32_t tim, uint8_t ch, uint32_t ccr);
void tim_pwm_enable(uint32_t tim, uint8_t ch);
void tim_start(uint32_t tim);
void tim_set_prescaler(uint32_t tim, uint32_t psc);
void tim_set_arr(uint32_t tim, uint32_t arr);
uint32_t tim_get_cnt(uint32_t tim);
void tim_clear_update_flag(uint32_t tim);

void adc_init(void);
uint16_t adc_read(uint8_t channel);
uint16_t adc_read_mv(uint8_t channel);
void adc_dma_start(uint8_t channel, uint16_t *dst, uint16_t count);
void adc_dma_stop(void);

void usart_init(uint32_t usart, uint32_t baud);
int  usart_write(uint32_t usart, const char *buf, int len);
int  usart_read_byte(uint32_t usart, uint8_t *out);
uint32_t usart_status(uint32_t usart);
void usart_clear_status(uint32_t usart, uint32_t mask);


/* ==================================================================== *
 * Hardware interface layer (owned by hw_iface.c)
 * ==================================================================== */
/* hwiAdc - raw ADC channels */
void     hwiAdc_init(void);
uint16_t hwiAdc_read(uint8_t channel);
uint16_t hwiAdc_read_mv(uint8_t channel);

/* hwiDrive - two motor drivers (duty A/B) and their fault flags */
void     hwiDrive_init(void);
int      hwiDrive_set_duty(uint8_t motor, int32_t duty);
int32_t  hwiDrive_get_fault(uint8_t motor);
int32_t  hwiDrive_get_duty(uint8_t motor);

/* hwiEnc - wheel encoders */
void     hwiEnc_init(void);
int32_t  hwiEnc_count(uint8_t encoder);
void     hwiEnc_reset(uint8_t encoder);

/* hwiSonar - ultrasonic distance sensor */
void     hwiSonar_init(void);
int32_t  hwiSonar_echo_us(void);

/* hwiLed - status LEDs */
void     hwiLed_init(void);
int      hwiLed_set(uint8_t led, int on);
int      hwiLed_set_pwm(uint8_t led, uint8_t duty);
void     hwiLed_pattern_request(void);   /* "PPRT\n"  (original FUN_08007B34) */
void     hwiLed_pattern_done(void);      /* "PPRET\n" (original FUN_08007B34) */

/* hwiBuzz - buzzer */
void     hwiBuzz_init(void);
int      hwiBuzz_play(uint8_t pattern);

/* hwiMic - microphone level */
void     hwiMic_init(void);
uint16_t hwiMic_level(void);

/* hwiLine - line follower sensor array */
void     hwiLine_init(void);
uint16_t hwiLine_read(uint8_t sensor);
uint16_t hwiLine_mask(void);

/* hwiPower - power / battery switch */
void     hwiPower_init(void);
void     hwiPower_off(void);
uint16_t hwiPower_vsens(void);

/* hwiLightSense - ambient light sensors */
void     hwiLightSense_init(void);
uint16_t hwiLightSense_read(uint8_t sensor);

/* ==================================================================== *
 * Drivers / application layer
 * ==================================================================== */
/* bleCom - BLE module link (owned by ble_link.c) */
void bleCom_init(void);
int  bleCom_write(const char *buf, int len);
int  bleCom_read(char *buf, int maxlen);
int  bleCom_tx_pending(void);
void bleCom_poll(void);

/* drvBattery - battery pack */
void    drvBattery_init(void);
void    drvBattery_hwInit(void);
int32_t drvBattery_voltage_mv(void);
int32_t drvBattery_current_ma(void);
int32_t drvBattery_remaining_pct(void);
int     drvBattery_charge_state(void);
int32_t drvBattery_temp_c10(void);

/* app level behaviour */
void appSys_init(void);
void appDrive_init(void);
void appLights_init(void);
void appLightSense_init(void);
void appDrive_tick(void);
void appLights_tick(void);
void appLightSense_tick(void);
void hw_init(void);            /* the original's "hwInit" */

/* system */
void SystemInit(void);
void sys_tick_init(uint32_t reload);

/* peripheral bases (the module HAL headers define the same values) */
#ifndef RCC_BASE
#define RCC_BASE   0x40021000UL
#endif
#ifndef FLASH_BASE
#define FLASH_BASE 0x40022000UL
#endif

/* shared millisecond tick (defined by system_stm32g0.c) */
extern volatile uint32_t g_tick_ms;


#endif /* CODIE_H */
