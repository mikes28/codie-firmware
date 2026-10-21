/* bleCom.c - UART link to the external BLE module + the byte level
 * output primitives of the whole protocol layer (out_str / out_char).
 *
 * Original firmware (the BLE module link):
 *   - FUN_08007F7C  init of the BLE USART (0x40004800 = "USART3" of the
 *     original's address map):
 *         RCC AHBENR |= 0xE0000   (GPIOA + GPIOB + GPIOC)
 *         RCC APB1ENR |= 0x40000  (the BLE USART)
 *         GPIOB  PB1  and PB13  -> alternate function AF4
 *         GPIOC  PC4  and PC5   -> alternate function AF1
 *         GPIOA  PA2            -> input (module boot / state line)
 *         USART: 14400 bit/s (the divisor 0x3840 is the *baud* value
 *                 that FUN_0800B3FC divides the peripheral clock by),
 *                 8 data bits, 2 stop bits (CR2 = 0x2000), TE|RE,
 *                 CR3 = 0x0C
 *         receive ring buffer: 128 bytes
 *   - FUN_0800806C  enable: CR1.RXFNEIE + CR1.UE, flush the ring
 *   - FUN_08007EC8  the receive interrupt: fills the ring buffer and
 *                   caches the USART error flags
 *   - FUN_08008130  the main loop tick: reports the receive errors
 *                   ("BLE framing error", "BLE USART noise",
 *                   "BLE USART overrun"), and - if nothing was received
 *                   for 2000 ms - re-sends the GAP "set mode" command
 *   - FUN_08007D88  the transmit routine ("serout"): it writes the
 *                   bytes to TDR (0x40004828) while the transmit
 *                   register is free and gives up after
 *                   (len * 0x528) microseconds, which prints
 *                   "ERR:BLE_serout_TO\n"
 *   - FUN_08006608  the boot message "BLE-boot\n"
 *   - FUN_0800663C  the module's answer "BLE-RSP HELLO\n"
 *   - FUN_080067F4  the module's answer "BLE-RSP GAP SET MODE: "
 *   - FUN_08009CA0 / FUN_08009CCC  out_char() / out_str() of the original.
 *                   NOTE: in the original these two write to the *other*
 *                   USART (0x40004C00, the console/command link) and not
 *                   to the BLE link.  The reconstruction sends them over
 *                   the BLE link as required by the module contract.
 *
 * The console USART (0x40004C00, 460800 bit/s, "BUFOVF!!!!!\n" on a
 * receive overflow, FUN_08009968 / FUN_08009A04 / FUN_08009B14) belongs
 * to the command line side and is not implemented here.
 */
#include "codie.h"
#include "hwi_b_hal.h"
#include "bleCom.h"
#include <string.h>

/* The original's two links (see MEMORY.md 4.2):
 *   USART3 0x40004800 @14400 - the BLE module: the robot reads the module's
 *            answers here and pushes raw data to it (the "serout" routine).
 *   USART4 0x40004C00 @460800 - the command + reply link: the commands come
 *            in here and EVERYTHING the robot prints goes out here (the
 *            original's out_char at 0x08009CA0 waits for TX-ready on
 *            0x40004C00 and writes the byte to its TDR).
 */
#define BLE_USART        HWI_USART3
/* baud rate recovered from FUN_08007F7C (cfg[0] = 0x3840 = 14400) */
#define BLE_BAUD         14400u
/* 2 stop bits (the original writes CR2 = 0x2000) */
#define BLE_STOP_BITS    2u

/* receive ring buffer: 128 bytes like the original */
#define BLE_RX_SIZE      128u
/* transmit ring buffer */
#define BLE_TX_SIZE      64u

/* The console / command link of the original: USART 0x40004C00.  The
 * original reads the incoming commands from THIS link (FUN_08009B14 checks
 * its error flags) and writes the replies to the BLE link above, which is
 * why the replies are prefixed ":proxy". */
#define CONSOLE_USART      HWI_USART4
#define CONSOLE_BAUD       460800u
#define CONSOLE_STOP_BITS  1u
#define CONSOLE_RX_SIZE    200u

/* the original's transmit timeout: 0x528 = 1320 us per byte */
#define BLE_TX_TIMEOUT_US_PER_BYTE 1320u
/* the original re-sends the GAP command after 2000 ms without traffic */
#define BLE_IDLE_TIMEOUT_MS        2000u

/* "BLE-boot\n" */
static const char ble_boot_msg[] = "BLE-boot\n";
/* the answers of the BLE module that the original recognises */
static const char ble_rsp_hello[] = "BLE-RSP HELLO";
static const char ble_rsp_gap[]   = "BLE-RSP GAP SET MODE: ";

static uint8_t ble_rx_buf[BLE_RX_SIZE];
static uint8_t ble_tx_buf[BLE_TX_SIZE];
static hwi_ring_t ble_rx;
static hwi_ring_t ble_tx;

static uint8_t  console_rx_buf[CONSOLE_RX_SIZE];
static hwi_ring_t console_rx;
static uint32_t console_err_flags;

static uint8_t  ble_ready;
static uint32_t ble_last_rx_ms;
static uint32_t ble_err_flags;      /* cached USART ISR error bits */
static uint8_t  ble_timeout_reported;
static uint8_t  ble_hello_seen;
static char     ble_line[80];
static uint8_t  ble_line_len;

/* ------------------------------------------------------------------ *
 * low level helpers
 * ------------------------------------------------------------------ */
/* true when "now" is past "deadline" (wrapping 32 bit tick) */
static int ble_past(uint32_t now, uint32_t deadline)
{
    return ((int32_t)(now - deadline) >= 0);
}

/* from FUN_08007D88 - write one byte, wait for the transmit register */
static int ble_tx_byte(uint8_t c, uint32_t deadline_ms)
{
    while (!hwi_usart_tx_ready(BLE_USART)) {
        if (ble_past(hwi_millis(), deadline_ms)) {
            return 0;                       /* timed out */
        }
    }
    hwi_usart_putc(BLE_USART, c);
    return 1;
}

/* the original's timeout budget, converted from its 1 us time base */
static uint32_t ble_tx_budget_ms(int len)
{
    uint32_t us = (uint32_t)len * BLE_TX_TIMEOUT_US_PER_BYTE;
    return (us + 999u) / 1000u;
}

/* ------------------------------------------------------------------ *
 * public API
 * ------------------------------------------------------------------ */
static const iface_t iface_bleCom;   /* the interface record, defined below */

void bleCom_init(void)
{
    hwi_gpio_cfg_t cfg;

    if (ble_ready) {
        return;
    }
    iface_register(&iface_bleCom);

    /* RCC: GPIOA + GPIOB + GPIOC (AHBENR 0xE0000) and the BLE USART
     * (APB1ENR 0x40000) - the original's bit values. */
    hwi_rcc_ahb_en(0xE0000u, 1);
    hwi_rcc_apb1_en(0x40000u, 1);

    /* GPIOB: PB1 and PB13 as alternate function (AF4) */
    cfg.mask  = 0x2002u;
    cfg.mode  = HWI_MODE_ALT;
    cfg.speed = 3u;
    cfg.otype = 2u;
    cfg.pull  = 0u;
    hwi_gpio_config(HWI_GPIOB, &cfg);
    hwi_gpio_set_af(HWI_GPIOB, 1u, 4u);
    hwi_gpio_set_af(HWI_GPIOB, 13u, 4u);

    /* GPIOC: PC4 and PC5 as alternate function (AF1) */
    cfg.mask  = 0x30u;
    cfg.mode  = HWI_MODE_ALT;
    cfg.speed = 3u;
    cfg.otype = 2u;
    cfg.pull  = 0u;
    hwi_gpio_config(HWI_GPIOC, &cfg);
    hwi_gpio_set_af(HWI_GPIOC, 4u, 1u);
    hwi_gpio_set_af(HWI_GPIOC, 5u, 1u);

    /* GPIOA: PA2 is an input (the module's state line) */
    cfg.mask  = 0x400u;
    cfg.mode  = HWI_MODE_INPUT;
    cfg.speed = 0u;
    cfg.otype = 0u;
    cfg.pull  = 0u;
    hwi_gpio_config(HWI_GPIOA, &cfg);

    hwi_ring_init(&ble_rx, ble_rx_buf, BLE_RX_SIZE);
    hwi_ring_init(&ble_tx, ble_tx_buf, BLE_TX_SIZE);
    hwi_ring_init(&console_rx, console_rx_buf, CONSOLE_RX_SIZE);

    hwi_usart_init(BLE_USART, BLE_BAUD, BLE_STOP_BITS);
    hwi_usart_enable(BLE_USART);           /* CR1.UE, FUN_0800B4C0 */

    /* The console / command link (FUN_08009A04): the console USART is
     * on APB1ENR bit 19, GPIOC PC10 (TX) and PC11 (RX) are alternate
     * function AF0 with a pull up. */
    hwi_rcc_apb1_en(0x80000u, 1);
    cfg.mask  = 0xC00u;
    cfg.mode  = HWI_MODE_ALT;
    cfg.speed = 3u;
    cfg.otype = 0u;
    cfg.pull  = HWI_PULL_UP;
    hwi_gpio_config(HWI_GPIOC, &cfg);
    hwi_gpio_set_af(HWI_GPIOC, 10u, 0u);
    hwi_gpio_set_af(HWI_GPIOC, 11u, 0u);

    hwi_usart_init(CONSOLE_USART, CONSOLE_BAUD, CONSOLE_STOP_BITS);
    hwi_usart_enable(CONSOLE_USART);
    console_err_flags = 0u;

    ble_last_rx_ms       = hwi_millis();
    ble_err_flags        = 0u;
    ble_timeout_reported = 0u;
    ble_hello_seen       = 0u;
    ble_line_len         = 0u;
    ble_ready            = 1u;

    /* the boot message of the original (FUN_08006608) */
    out_str(ble_boot_msg);
}

/* from FUN_08007D88 - the "serout" routine of the original */
int bleCom_write(const char *buf, int len)
{
    uint32_t deadline;
    uint8_t c;
    int i;
    int queued = 0;

    if (!ble_ready) {
        bleCom_init();
    }
    if ((buf == 0) || (len <= 0)) {
        return 0;
    }
    deadline = hwi_millis() + ble_tx_budget_ms(len);

    /* the bytes are queued first, the queue is then drained while the
     * transmit register is free - the original writes the bytes one
     * after the other and gives up after (len * 0x528) us. */
    for (i = 0; i < len; i++) {
        if (hwi_ring_put(&ble_tx, (uint8_t)buf[i]) == 0) {
            break;                      /* transmit queue full */
        }
        queued++;
    }

    while (hwi_ring_get(&ble_tx, &c) != 0) {
        if (!ble_tx_byte(c, deadline)) {
            /* the original prints this when the transmit does not drain */
            if (!ble_timeout_reported) {
                ble_timeout_reported = 1u;
                out_str("ERR:BLE_serout_TO\n");
            }
            return -1;
        }
    }
    ble_timeout_reported = 0u;
    return queued;
}

/* from FUN_08007EC8 / FUN_08008130 - collect the received bytes and
 * report the USART errors exactly like the original does. */
static void ble_poll_rx(void);

/* from FUN_08009B14 - the command reader of the original watches the error
 * flags of the console link and reports them before the command is parsed. */
static void console_poll(void)
{
    uint32_t isr;
    uint8_t c;

    while (hwi_usart_rx_ready(CONSOLE_USART)) {
        c = hwi_usart_getc(CONSOLE_USART);
        if (hwi_ring_put(&console_rx, c) == 0) {
            out_str("BUFOVF!!!!!\n");
            break;
        }
    }

    isr = hwi_usart_status(CONSOLE_USART) & 0x1Fu;
    console_err_flags |= isr;
    if ((console_err_flags & 0x02u) != 0u) {          /* framing error */
        console_err_flags &= ~0x02u;
        hwi_usart_clear(CONSOLE_USART, 0x02u);
        resp_message(LOG_ERR, "USART framing error", 0);
    }
    if ((console_err_flags & 0x04u) != 0u) {          /* noise */
        console_err_flags &= ~0x04u;
        hwi_usart_clear(CONSOLE_USART, 0x04u);
        resp_message(LOG_ERR, "USART noise", 0);
    }
    if ((console_err_flags & 0x08u) != 0u) {          /* overrun */
        console_err_flags &= ~0x08u;
        hwi_usart_clear(CONSOLE_USART, 0x08u);
        resp_message(LOG_ERR, "USART overrun", 0);
    }
}

/* public entry point used by the main loop */
void bleCom_poll(void)
{
    ble_poll_rx();
    console_poll();
}

static void ble_poll_rx(void)
{
    uint32_t isr;

    while (hwi_usart_rx_ready(BLE_USART)) {
        uint8_t c = hwi_usart_getc(BLE_USART);
        if (!hwi_ring_put(&ble_rx, c)) {
            /* the original stops the receive interrupt here; the
             * reconstruction reports the overflow like the console
             * link does. */
            out_str("BUFOVF!!!!!\n");
            break;
        }
        ble_last_rx_ms = hwi_millis();
    }

    isr = hwi_usart_status(BLE_USART) & 0x1Fu;
    if (isr != 0u) {
        ble_err_flags |= isr;
        if ((ble_err_flags & 0x02u) != 0u) {          /* FE */
            resp_message(LOG_ERR, "BLE framing error", 0);
            hwi_usart_clear(BLE_USART, 0x02u);
            ble_err_flags &= ~0x02u;
        }
        if ((ble_err_flags & 0x08u) != 0u) {          /* ORE */
            resp_message(LOG_ERR, "BLE USART overrun", 0);
            hwi_usart_clear(BLE_USART, 0x08u);
            ble_err_flags &= ~0x08u;
        }
        if ((ble_err_flags & 0x04u) != 0u) {          /* NE */
            resp_message(LOG_ERR, "BLE USART noise", 0);
            hwi_usart_clear(BLE_USART, 0x04u);
            ble_err_flags &= ~0x04u;
        }
        if ((ble_err_flags & 0x10u) != 0u) {          /* IDLE */
            hwi_usart_clear(BLE_USART, 0x10u);
            ble_err_flags &= ~0x10u;
        }
        if ((ble_err_flags & 0x01u) != 0u) {          /* PE */
            hwi_usart_clear(BLE_USART, 0x01u);
            ble_err_flags &= ~0x01u;
        }
    }
}

/* the answers of the BLE module that the original recognises
 * (FUN_0800663C "BLE-RSP HELLO", FUN_080067F4 "BLE-RSP GAP SET MODE: ") */
static void ble_check_line(void)
{
    ble_line[ble_line_len] = '\0';
    if (ble_line_len == 0u) {
        return;
    }
    if (strncmp(ble_line, ble_rsp_hello, sizeof(ble_rsp_hello) - 1u) == 0) {
        ble_hello_seen = 1u;
        out_str("BLE-RSP HELLO\n");
    } else if (strncmp(ble_line, ble_rsp_gap, sizeof(ble_rsp_gap) - 1u) == 0) {
        out_str(ble_rsp_gap);
        out_str(&ble_line[sizeof(ble_rsp_gap) - 1u]);
        out_str("\n");
    }
    ble_line_len = 0u;
}

int bleCom_read(char *buf, int maxlen)
{
    uint8_t c;
    int n = 0;

    if (!ble_ready) {
        bleCom_init();
    }
    if ((buf == 0) || (maxlen <= 0)) {
        return 0;
    }
    ble_poll_rx();

    /* the console link has priority: it is where the original reads the
     * commands from */
    while ((n < maxlen) && (hwi_ring_get(&console_rx, &c) != 0)) {
        buf[n++] = (char)c;
    }

    while ((n < maxlen) && (hwi_ring_get(&ble_rx, &c) != 0)) {
        buf[n++] = (char)c;
        if (c == (uint8_t)'\n') {
            ble_check_line();
        } else if (ble_line_len < (sizeof(ble_line) - 1u)) {
            ble_line[ble_line_len++] = (char)c;
        }
    }
    return n;
}

/* the number of bytes that are still queued for transmission */
int bleCom_tx_pending(void)
{
    return (int)hwi_ring_count(&ble_tx);
}

/* ------------------------------------------------------------------ *
 * byte level output (the protocol layer uses these)
 * ------------------------------------------------------------------ */
/* from FUN_08009CA0 - one byte over the BLE link */
void out_char(char c)
{
    uint32_t deadline;

    if (!ble_ready) {
        bleCom_init();
    }
    deadline = hwi_millis() + ble_tx_budget_ms(1);
    while (!hwi_usart_tx_ready(BLE_USART)) {
        if (ble_past(hwi_millis(), deadline)) {
            if (!ble_timeout_reported) {
                ble_timeout_reported = 1u;
                out_str("ERR:BLE_serout_TO\n");
            }
            return;
        }
    }
    hwi_usart_putc(BLE_USART, (uint8_t)c);
    ble_timeout_reported = 0u;
}

/* from FUN_08009CCC - a string over the BLE link, byte by byte */
void out_str(const char *s)
{
    if (s == 0) {
        return;
    }
    while (*s != '\0') {
        out_char(*s);
        s++;
    }
}

/* ------------------------------------------------------------------ *
 * interface handler
 * ------------------------------------------------------------------ */
static void bleCom_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];

    if (opname == 0) {
        return;
    }

    if (strcmp(opname, "buf") == 0) {
        if (op == OP_GET) {
            ble_poll_rx();
            fmt_int(val, (int)sizeof(val), (int32_t)hwi_ring_count(&ble_rx));
            resp_value(op, "bleCom", opname, val);
        }
        return;
    }
    if (strcmp(opname, "write") == 0) {
        int n = bleCom_write((args != 0) ? args : "",
                             (args != 0) ? (int)strlen(args) : 0);
        fmt_int(val, (int)sizeof(val), (int32_t)n);
        resp_value(op, "bleCom", opname, val);
        return;
    }
    if (strcmp(opname, "echo") == 0) {
        if (args != 0) {
            bleCom_write(args, (int)strlen(args));
            out_str(args);
        }
        fmt_int(val, (int)sizeof(val), (int32_t)((args != 0) ? strlen(args) : 0));
        resp_value(op, "bleCom", opname, val);
        return;
    }
    if (op == OP_GET) {
        ble_poll_rx();
        fmt_int(val, (int)sizeof(val), (int32_t)hwi_ring_count(&ble_rx));
        resp_value(op, "bleCom", opname, val);
    }
}

static const iface_t iface_bleCom = { "bleCom", bleCom_handler };
