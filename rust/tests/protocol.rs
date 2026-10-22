//! The protocol layer against the exact bytes of the original firmware.
//!
//! This is the Rust counterpart of `tools/protocol_test.c`: the same checks,
//! the same expected bytes.  The replies of the protocol layer go out on the
//! command link (USART4), so the bytes are taken from the simulated
//! peripheral model.

use codie::hal::addr::USART4;
use codie::modules::ble_com;
use codie::proto::{fmt, proxy};

/// A demo interface, the Rust counterpart of the `demo_handler` of
/// `tools/protocol_test.c`.
fn demo_handler(_op: u8, opname: &str, _args: &str) {
    if opname == "voltage" {
        // the original prints the value in hex and always uses op code 2
        let val = fmt::num(16, 0x2084);
        proxy::resp_value(2, "drvBattery", opname, val.as_str());
    }
}

/// Run one check and compare the bytes that went out on the command link.
fn expect(what: &str, want: &str, f: impl FnOnce()) {
    f();
    let got = codie::sim::usart_take_tx(USART4);
    let got = String::from_utf8_lossy(&got);
    assert_eq!(got, want, "check '{}' failed", what);
}

#[test]
fn protocol_bytes_match_the_original() {
    codie::sim::reset();

    // the link has to be up before the protocol layer can write to it
    ble_com::init();
    // ... and its boot message is not part of the checks
    let _ = codie::sim::usart_take_tx(USART4);

    proxy::register(proxy::Iface { name: "drvBattery", handler: demo_handler });

    // 1) the greeting of the original (the RAM array at 0x20000A14)
    expect(
        "greeting",
        "call :proxy greeting \"ver\":\"0.5.0hw2.1\" \"mat\":\"4\" \
         \"date\":\"1602121419\" \"ttpms\":\"1000\" \"+ack\":\"0\"\n",
        || {
            proxy::resp_greeting(
                &[
                    ("ver", Some("0.5.0hw2.1")),
                    ("mat", Some("4")),
                    ("date", Some("1602121419")),
                    ("ttpms", Some("1000")),
                    ("+ack", Some("0")),
                ],
                None,
            )
        },
    );

    // 2) a value reply: the original hard codes op code 2 ("set")
    expect("value reply", "set drvBattery voltage 2084\n", || {
        proxy::dispatch("get drvBattery voltage");
    });

    // 3) an unknown interface: the name is appended without a separator
    expect(
        "unknown interface",
        "call :proxy message err No interface:nosuchiface\n",
        || {
            proxy::dispatch("get nosuchiface op");
        },
    );

    // 4) a malformed request
    expect(
        "invalid command",
        "call :proxy message err Invalid command!\n",
        || {
            proxy::dispatch("hello world");
        },
    );

    // 5) the command buffer overflow message
    expect(
        "cmdBuf overflow",
        "call :proxy message err possible cmdBuf overflow\n",
        || {
            proxy::resp_message(proxy::LOG_ERR, "possible cmdBuf overflow", None);
        },
    );

    // 6) the system message with an extra field (no separator, as in the
    //    original)
    expect(
        "message + extra",
        "call :proxy message err No interface:appFoo\n",
        || {
            proxy::resp_message(proxy::LOG_ERR, "No interface:", Some("appFoo"));
        },
    );

    // 7) the log level words
    expect("warn level", "call :proxy message wrn text\n", || {
        proxy::resp_message(proxy::LOG_WRN, "text", None);
    });

    // 8) the value reply without a value: the C skips the separator *and*
    //    the value when the value is NULL (`fw/modules/proxy.c`)
    expect("value reply without a value", "get drvBattery voltage\n", || {
        proxy::resp_value_opt(proxy::OP_GET, "drvBattery", "voltage", None);
    });

    // 9) the greeting with a message: the C writes the "msg:" prefix behind
    //    the key/value block (`FUN_0800A1C8`)
    expect(
        "greeting with a message",
        "call :proxy greeting \"ver\":\"0.5.0hw2.1\" \"msg:hello\"\n",
        || {
            proxy::resp_greeting(&[("ver", Some("0.5.0hw2.1"))], Some("hello"));
        },
    );

    // 10) the greeting with a NULL value: the C writes the colon and the
    //     opening quote and then breaks out of the loop - the value and the
    //     closing quote are missing (`fw/modules/proxy.c`)
    expect(
        "greeting with a NULL value",
        "call :proxy greeting \"ver\":\"0.5.0hw2.1\" \"mat\":\"\n",
        || {
            proxy::resp_greeting(&[("ver", Some("0.5.0hw2.1")), ("mat", None)], None);
        },
    );
}

#[test]
fn the_numbers_are_formatted_like_the_original() {
    // the original prints every sensor value in hexadecimal
    assert_eq!(fmt::num(16, 0x2084).as_str(), "2084");
    assert_eq!(fmt::num(16, 0).as_str(), "0");
    assert_eq!(fmt::num(16, 0xDEAD_BEEF).as_str(), "deadbeef");
    assert_eq!(fmt::num(10, 12345).as_str(), "12345");
    // the signed variant prints a leading minus
    assert_eq!(fmt::num_s(16, -2).as_str(), "-2");
    assert_eq!(fmt::num_s(16, i32::MIN).as_str(), "-80000000");
    // the decimal convenience wrapper
    assert_eq!(fmt::int(0).as_str(), "0");
    assert_eq!(fmt::int(-42).as_str(), "-42");
    assert_eq!(fmt::int(2147483647).as_str(), "2147483647");
    // the fixed point variant
    assert_eq!(fmt::fixed(1234, 2).as_str(), "12.34");
    assert_eq!(fmt::fixed(-1234, 2).as_str(), "-12.34");
    assert_eq!(fmt::fixed(1234, 0).as_str(), "1234");
}
