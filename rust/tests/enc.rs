//! Encoder (rotation sensor) protocol tests.
//!
//! The hwiEnc module tracks two motor rotation sensors (encoders) via TIM1
//! and TIM2 capture inputs.  The original handler is a stub that answers no
//! op; the reconstruction answers `get hwiEnc count <n>` and
//! `call hwiEnc count <n>` / `call hwiEnc reset <n>`.

use codie::hal::addr::USART4;
use codie::modules::hwi_enc;
use codie::proto::proxy;
use codie::sim;

/// Run one check and compare the bytes that went out on the command link.
fn expect(what: &str, want: &str, f: impl FnOnce()) {
    // clear any leftover bytes from parallel tests
    let _ = sim::usart_take_tx(USART4);
    f();
    let got = sim::usart_take_tx(USART4);
    let got = String::from_utf8_lossy(&got);
    // Strip leading "BLE-boot\n" if the sim didn't fully reset the link
    let got = got.strip_prefix("BLE-boot\n").unwrap_or(&got);
    assert_eq!(got, want, "check '{}' failed", what);
}

#[test]
fn hwi_enc_count_get() {
    sim::reset();
    let _ = sim::usart_take_tx(USART4);

    hwi_enc::init();
    hwi_enc::reset(1);
    hwi_enc::reset(2);

    let _ = sim::usart_take_tx(USART4);

    // get hwiEnc count 1 -> returns the count (0 after reset)
    expect(
        "get hwiEnc count 1",
        "set hwiEnc count 0\n",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("get hwiEnc count 1");
        },
    );

    // get hwiEnc count 2 -> returns the count (0 after reset)
    expect(
        "get hwiEnc count 2",
        "set hwiEnc count 0\n",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("get hwiEnc count 2");
        },
    );
}

#[test]
fn hwi_enc_count_call() {
    sim::reset();
    let _ = sim::usart_take_tx(USART4);

    hwi_enc::init();
    hwi_enc::reset(1);

    let _ = sim::usart_take_tx(USART4);

    // call hwiEnc count 1 -> returns the count (op code 2 "set")
    expect(
        "call hwiEnc count 1",
        "set hwiEnc count 0\n",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("call hwiEnc count 1");
        },
    );
}

#[test]
fn hwi_enc_reset() {
    sim::reset();
    let _ = sim::usart_take_tx(USART4);

    hwi_enc::init();
    hwi_enc::reset(1);

    let _ = sim::usart_take_tx(USART4);

    // call hwiEnc reset 1 -> no reply
    expect(
        "call hwiEnc reset 1",
        "",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("call hwiEnc reset 1");
        },
    );

    // call hwiEnc reset (no argument) -> reset both, no reply
    expect(
        "call hwiEnc reset",
        "",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("call hwiEnc reset");
        },
    );
}

#[test]
fn hwi_enc_unknown_op() {
    sim::reset();
    let _ = sim::usart_take_tx(USART4);

    hwi_enc::init();

    let _ = sim::usart_take_tx(USART4);

    // unknown op -> ignored, no reply
    expect(
        "call hwiEnc bogus 1",
        "",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("call hwiEnc bogus 1");
        },
    );

    // get hwiEnc bogus -> ignored, no reply
    expect(
        "get hwiEnc bogus",
        "",
        || {
            let _ = sim::usart_take_tx(USART4);
            proxy::dispatch("get hwiEnc bogus");
        },
    );
}
