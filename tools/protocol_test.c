/* protocol_test.c - host side test of the reconstructed protocol layer.
 *
 * Compiles the real proxy.c for the host and checks the bytes that go on the
 * wire against the behaviour recovered from the original firmware:
 *
 *   FUN_0800A1C8  greeting
 *   FUN_0800A16C  value reply
 *   FUN_0800A2D0  system message
 *   FUN_0800A350  dispatcher (unknown interface / invalid command)
 *
 * build:  gcc -I../fw/include tools/protocol_test.c fw/modules/proxy.c -o /tmp/pt
 */
#include <stdio.h>
#include <string.h>
#include "codie.h"

static char out_buf[1024];
static int  out_len;

void out_str(const char *s)
{
    int n = (int)strlen(s);
    memcpy(out_buf + out_len, s, (size_t)n);
    out_len += n;
}

void out_char(char c)
{
    out_buf[out_len++] = c;
}

extern int proxy_dispatch(char *cmd);

static int fails;

static void expect(const char *what, const char *want)
{
    out_buf[out_len] = '\0';
    if (strcmp(out_buf, want) != 0) {
        printf("FAIL %-22s\n  got  %s\n  want %s\n", what, out_buf, want);
        fails++;
    } else {
        printf("ok   %-22s %s\n", what, out_buf);
    }
    out_len = 0;
}

static void demo_handler(uint8_t op, const char *opname, const char *args)
{
    char val[16];
    (void)args;
    if (strcmp(opname, "voltage") == 0) {
        /* the original prints the value in hex and always uses op code 2 */
        fmt_num(val, 16, 0x2084u);
        resp_value(2, "drvBattery", opname, val);
    }
}

static const iface_t demo = { "drvBattery", demo_handler };

int main(void)
{
    char cmd[64];

    iface_register(&demo);

    /* 1) the greeting of the original (RAM array at 0x20000A14) */
    {
        static const char *const kv[] = {
            "ver", "0.5.0hw2.1",
            "mat", "4",
            "date", "1602121419",
            "ttpms", "1000",
            "+ack", "0",
            0
        };
        resp_greeting(kv, 0);
        expect("greeting",
               "call :proxy greeting \"ver\":\"0.5.0hw2.1\" \"mat\":\"4\" "
               "\"date\":\"1602121419\" \"ttpms\":\"1000\" \"+ack\":\"0\"\n");
    }

    /* 2) a value reply: the original hard codes op code 2 ("set") */
    strcpy(cmd, "get drvBattery voltage");
    proxy_dispatch(cmd);
    expect("value reply", "set drvBattery voltage 2084\n");

    /* 3) an unknown interface: the name is appended without a separator */
    strcpy(cmd, "get nosuchiface op");
    proxy_dispatch(cmd);
    expect("unknown interface", "call :proxy message err No interface:nosuchiface\n");

    /* 4) a malformed request */
    strcpy(cmd, "hello world");
    proxy_dispatch(cmd);
    expect("invalid command", "call :proxy message err Invalid command!\n");

    /* 5) the command buffer overflow message */
    resp_message(LOG_ERR, "possible cmdBuf overflow", 0);
    expect("cmdBuf overflow", "call :proxy message err possible cmdBuf overflow\n");

    /* 6) the system message with an extra field (no separator, as in the
     *    original) */
    resp_message(LOG_ERR, "No interface:", "appFoo");
    expect("message + extra", "call :proxy message err No interface:appFoo\n");

    /* 7) the log level words */
    resp_message(LOG_WRN, "text", 0);
    expect("warn level", "call :proxy message wrn text\n");

    printf(fails ? "\n%d FAILURES\n" : "\nall protocol checks passed\n", fails);
    return fails ? 1 : 0;
}
