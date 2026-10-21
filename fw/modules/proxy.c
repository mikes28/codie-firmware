/* proxy.c -- the console command protocol layer of the original Codie
 * firmware (the "proxy" side of the BLE app link).
 *
 * Recovered from (Ghidra decompilation of the original flash dump):
 *   FUN_0800A0AC  request parser          (0x0800A0AC)
 *   FUN_0800A350  interface dispatcher     (0x0800A350)
 *   FUN_0800A16C  value reply             (0x0800A16C)
 *   FUN_0800A2D0  system message          (0x0800A2D0)
 *   FUN_0800A1C8  greeting                (0x0800A1C8)
 *   FUN_0800A160  op word table           (0x0800B820 "???","get","set","call")
 *   FUN_0800A2C0  log level table         (0x0800B834 "???","inf","dbg","wrn","err")
 *   FUN_0800B740  strcmp, FUN_0800B754 strlen, FUN_0800B778 strtok
 *   FUN_080068D4  unsigned itoa, FUN_0800693C signed itoa
 *
 * Wire format (byte exact with the original):
 *   request          "<get|set|call> <interface> <op> [args]"   (one line)
 *   value reply      "<word> <interface> <op> [value]\n"
 *   system message   "call :proxy message <level> <text>[<extra>]\n"
 *                     (the original appends "extra" WITHOUT a separator)
 *   greeting         "call :proxy greeting \"k\":\"v\" ... \"msg:<text>\"\n"
 */
#include "codie.h"
#include <string.h>

/* ------------------------------------------------------------------ *
 * word tables (flash 0x0800B820 / 0x0800B834 in the original)
 * ------------------------------------------------------------------ */
static const char *const op_word_tbl[4] = { "???", "get", "set", "call" };
static const char *const log_word_tbl[5] = { "???", "inf", "dbg", "wrn", "err" };

static const char *op_word(uint8_t op)
{
    return (op < 4u) ? op_word_tbl[op] : op_word_tbl[0];
}

static const char *log_word(uint8_t level)
{
    return (level < 5u) ? log_word_tbl[level] : log_word_tbl[0];
}

/* ------------------------------------------------------------------ *
 * interface registry
 *
 * The original keeps a 16 entry {const char *name, void (*handler)(op,
 * opname, args)} table in RAM (a copy of the table in flash at 0x0800D164,
 * copied to 0x20000A44 by FUN_0800A150) and the dispatcher searches it
 * BACKWARDS (FUN_0800A350).  In the rebuild the table is filled by
 * iface_register(), which every module calls from its init function; the
 * modules are initialised in the order of the original table so that the
 * resulting order is the same:
 *   hwiAdc hwiLed hwiDrive drvBattery hwiSonar bleCom hwiBuzz hwiMic
 *   hwiEnc hwiLine hwiPower hwiLightSense appLights appDrive appSys
 *   appLightSense
 * ------------------------------------------------------------------ */
#define IFACE_MAX 16

static iface_t g_iface[IFACE_MAX];
static int     g_iface_count;

void iface_register(const iface_t *iface)
{
    int i;

    if ((iface == 0) || (iface->name == 0) || (iface->handler == 0)) {
        return;
    }
    for (i = 0; i < g_iface_count; i++) {
        if (strcmp(g_iface[i].name, iface->name) == 0) {
            g_iface[i] = *iface;   /* re-registration replaces the entry */
            return;
        }
    }
    if (g_iface_count < IFACE_MAX) {
        g_iface[g_iface_count] = *iface;
        g_iface_count++;
    }
}

/* ------------------------------------------------------------------ *
 * replies
 * ------------------------------------------------------------------ */

/* from FUN_0800A16C */
void resp_value(uint8_t op, const char *iface, const char *opname, const char *value)
{
    out_str(op_word(op));
    out_char(' ');
    out_str(iface);
    out_char(' ');
    out_str(opname);
    if (value != 0) {
        out_char(' ');
        out_str(value);
    }
    out_char('\n');
}

/* from FUN_0800A2D0 - note: the original prints "extra" directly behind the
 * text, there is NO separator between them. */
void resp_message(uint8_t level, const char *text, const char *extra)
{
    out_str(op_word_tbl[3]);       /* "call" */
    out_char(' ');
    out_str(":proxy");
    out_char(' ');
    out_str("message");
    out_char(' ');
    out_str(log_word(level));
    out_char(' ');
    out_str(text);
    if (extra != 0) {
        out_str(extra);
    }
    out_char('\n');
}

/* from FUN_0800A1C8 */
void resp_greeting(const char *const *kv, const char *msg)
{
    int i;

    out_str(op_word_tbl[3]);       /* "call" */
    out_char(' ');
    out_str(":proxy");
    out_char(' ');
    out_str("greeting");

    if (kv != 0) {
        for (i = 0; kv[i] != 0; i += 2) {
            out_char(' ');
            out_char('"');
            out_str(kv[i]);
            out_char('"');
            out_char(':');
            out_char('"');
            if (kv[i + 1] != 0) {
                out_str(kv[i + 1]);
                out_char('"');
            } else {
                break;
            }
        }
    }
    if (msg != 0) {
        out_char(' ');
        out_char('"');
        out_str("msg:");
        out_str(msg);
        out_char('"');
    }
    out_char('\n');
}

/* ------------------------------------------------------------------ *
 * number -> text
 *
 * The original formats the values with FUN_080068D4 (unsigned) and
 * FUN_0800693C (signed); both are itoa() with a variable radix.  Every call
 * site that produces a reply uses radix 16, so the original prints its
 * numbers in HEXADECIMAL (the only exception is the hwiAdc handler, which
 * uses radix 10).
 * ------------------------------------------------------------------ */

/* from FUN_080068D4 (unsigned itoa).  The buffer must hold radix+2 bytes. */
static int uitoa(uint32_t v, char *out, int radix)
{
    char tmp[36];
    int n = 0, i;

    if ((radix < 2) || (radix > 36)) {
        radix = 10;
    }
    do {
        uint32_t d = v % (uint32_t)radix;
        v /= (uint32_t)radix;
        /* the original maps 10..35 to 'a'..'z' with the offset 'W' */
        tmp[n++] = (char)((d < 10u) ? ('0' + d) : ('W' + d));
    } while ((v != 0u) && (n < (int)sizeof(tmp)));

    for (i = 0; i < n; i++) {
        out[i] = tmp[n - 1 - i];
    }
    out[n] = '\0';
    return n;
}

char *fmt_num(char *buf, int radix, uint32_t v)
{
    uitoa(v, buf, radix);
    return buf;
}

/* from FUN_0800693C (signed itoa) */
char *fmt_num_s(char *buf, int radix, int32_t v)
{
    uint32_t mag = (uint32_t)v;
    char tmp[40];
    int n, i = 0, j = 0;

    if (v < 0) {
        mag = (~mag) + 1u;         /* two's complement, INT32_MIN safe */
    }
    n = uitoa(mag, tmp, radix);
    if (v < 0) {
        buf[i++] = '-';
    }
    while ((j < n) && (i < (int)sizeof(tmp) + 1)) {
        buf[i++] = tmp[j++];
    }
    buf[i] = '\0';
    return buf;
}

/* convenience wrapper (decimal) */
void fmt_int(char *buf, int buflen, int32_t v)
{
    uint32_t mag = (uint32_t)v;
    char tmp[12];
    int n, i = 0, j = 0;

    if ((buf == 0) || (buflen <= 0)) {
        return;
    }
    if (v < 0) {
        mag = (~mag) + 1u;
        if (buflen > 1) {
            buf[i++] = '-';
        }
    }
    n = uitoa(mag, tmp, 10);
    while ((j < n) && (i < buflen - 1)) {
        buf[i++] = tmp[j++];
    }
    buf[i] = '\0';
}

/* convenience wrapper: decimal, scaled by 10^frac */
void fmt_fixed(char *buf, int buflen, int32_t v, int frac)
{
    uint32_t mag = (uint32_t)v;
    char tmp[24];
    uint32_t div = 1u;
    int n, i = 0, j = 0, f;

    if ((buf == 0) || (buflen <= 0)) {
        return;
    }
    if (v < 0) {
        mag = (~mag) + 1u;
        if (buflen > 1) {
            buf[i++] = '-';
        }
    }
    if (frac > 0) {
        for (f = 0; (f < frac) && (f < 9); f++) {
            div *= 10u;
        }
        n = uitoa(mag / div, tmp, 10);
        tmp[n++] = '.';
        for (f = frac - 1; f >= 0; f--) {
            uint32_t p = 1u;
            int g;
            for (g = 0; g < f; g++) {
                p *= 10u;
            }
            tmp[n++] = (char)('0' + (mag / p) % 10u);
        }
    } else {
        n = uitoa(mag, tmp, 10);
    }
    while ((j < n) && (i < buflen - 1)) {
        buf[i++] = tmp[j++];
    }
    buf[i] = '\0';
}

/* ------------------------------------------------------------------ *
 * request parser / dispatcher
 * ------------------------------------------------------------------ */

/* The original uses strtok() (FUN_0800B778) with the delimiter " "
 * (0x0800C6A4): leading delimiters are skipped, the token is terminated in
 * place and the rest of the line is the "args" of the request. */
static char *tok_next(char **cur)
{
    char *s = *cur;
    char *start;

    if (s == 0) {
        return 0;
    }
    while (*s == ' ') {
        s++;
    }
    if (*s == '\0') {
        *cur = s;
        return 0;
    }
    start = s;
    while ((*s != '\0') && (*s != ' ')) {
        s++;
    }
    if (*s == ' ') {
        *s = '\0';
        s++;
    }
    *cur = s;
    return start;
}

/* from FUN_0800A0AC + FUN_0800A350.
 * Not part of codie.h (the contract does not declare a dispatcher), so
 * main.c declares it locally. */
int proxy_dispatch(char *cmd)
{
    char  *cur = cmd;
    char  *tok;
    const char *iface;
    char  *opname;
    const char *args;
    uint8_t op;
    int i;

    if (cmd == 0) {
        return 0;
    }

    tok = tok_next(&cur);
    if (tok == 0) {
        resp_message(LOG_ERR, "Invalid command!", 0);
        return 0;
    }
    if (strcmp(tok, "get") == 0) {
        op = OP_GET;
    } else if (strcmp(tok, "set") == 0) {
        op = OP_SET;
    } else if (strcmp(tok, "call") == 0) {
        op = OP_CALL;
    } else {
        resp_message(LOG_ERR, "Invalid command!", 0);
        return 0;
    }

    tok = tok_next(&cur);
    if (tok == 0) {
        resp_message(LOG_ERR, "Invalid command!", 0);
        return 0;
    }
    iface = tok;

    tok = tok_next(&cur);
    if (tok == 0) {
        resp_message(LOG_ERR, "Invalid command!", 0);
        return 0;
    }
    opname = tok;
    args = cur;            /* the rest of the line behind the op name */

    /* the original searches the registry backwards */
    for (i = g_iface_count - 1; i >= 0; i--) {
        if (strcmp(g_iface[i].name, iface) == 0) {
            g_iface[i].handler(op, opname, args);
            return 1;
        }
    }

    resp_message(LOG_ERR, "No interface:", iface);
    return 0;
}
