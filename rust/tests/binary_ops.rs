//! The ops of the **original** firmware against the Rust port.
//!
//! `analysis/INTERFACE_OPS.txt` was extracted from the dump
//! (`stm32_flash.bin`): for every entry of the interface table of the
//! original (flash `0x0800D164`) it lists the op names the handler of that
//! interface really uses.  Twelve of the sixteen handlers are the 4 byte
//! stub of the original ("movs r0,#0; bx lr") and answer nothing at all.
//!
//! This test drives the Rust port with exactly those ops and compares the
//! answers with the behaviour of the original, which was recovered from the
//! decompilation (`analysis/decomp/ALL.c`):
//!   `FUN_080073DC` (hwiAdc), `FUN_08008B3C` (hwiDrive), `FUN_08006F2C`
//!   (drvBattery), `FUN_0800981C` (hwiPower) and the reply helpers
//!   `FUN_0800A16C` (the value reply), `FUN_0800A2D0` (the system message),
//!   `FUN_0800A1C8` (the greeting).

use std::collections::BTreeMap;
use std::fs;

use codie::hal::addr::USART4;
use codie::modules::{
    app_drive, app_lights, app_lightsense, app_sys, ble_com, drv_battery, hwi_adc, hwi_buzz,
    hwi_drive, hwi_enc, hwi_led, hwi_lightsense, hwi_line, hwi_mic, hwi_power, hwi_sonar,
};
use codie::proto::proxy;

/// The op list of the original, as it was extracted from the dump.
fn original_ops() -> BTreeMap<String, Vec<String>> {
    let path = concat!(env!("CARGO_MANIFEST_DIR"), "/../analysis/INTERFACE_OPS.txt");
    let text = match fs::read_to_string(path) {
        Ok(t) => t,
        Err(e) => {
            panic!("cannot read {}: {}", path, e);
        }
    };
    let mut map = BTreeMap::new();
    for line in text.lines() {
        let line = line.trim();
        if line.is_empty() {
            continue;
        }
        // "hwiAdc         FUN_080073DC @0x080073DC: ['buf']"
        let (head, rest) = match line.split_once(':') {
            Some((h, r)) => (h, r.trim()),
            None => continue,
        };
        let name = match head.split_whitespace().next() {
            Some(n) => n.to_string(),
            None => continue,
        };
        let ops: Vec<String> = rest
            .trim_start_matches('[')
            .trim_end_matches(']')
            .split(',')
            .map(|s| s.trim().trim_matches('\'').to_string())
            .filter(|s| !s.is_empty())
            .collect();
        map.insert(name, ops);
    }
    assert_eq!(map.len(), 16, "the original has 16 interfaces");
    map
}

/// Run a request and return the bytes that went out on the command link.
fn request(cmd: &str) -> String {
    let _ = codie::sim::usart_take_tx(USART4);
    proxy::dispatch(cmd);
    String::from_utf8_lossy(&codie::sim::usart_take_tx(USART4)).into_owned()
}

#[test]
fn the_port_answers_the_ops_of_the_original() {
    codie::sim::reset();

    // the module inits, in the order of the interface table of the original
    // (this is what publishes the interfaces in the registry)
    hwi_adc::init();
    hwi_led::init();
    hwi_drive::init();
    drv_battery::init();
    hwi_sonar::init();
    ble_com::init();
    hwi_buzz::init();
    hwi_mic::init();
    hwi_enc::init();
    hwi_line::init();
    hwi_power::init();
    hwi_lightsense::init();
    app_lights::init();
    app_drive::init();
    app_sys::init();
    app_lightsense::init();

    let _ = codie::sim::usart_take_tx(USART4);

    let ops = original_ops();

    for (iface, iface_ops) in &ops {
        // 1) the interface has to exist in the port: the "No interface:"
        //    message is the only answer of an unknown interface
        let answer = request(&format!("get {} nosuchop", iface));
        assert!(
            !answer.contains("No interface:"),
            "the port does not know the interface '{}' of the original",
            iface
        );

        for op in iface_ops {
            // the op list of the analysis contains the name of the interface
            // itself for two interfaces ("hwiDrive", "drvBattery"): those
            // strings are the interface name of the reply
            // (`resp_value(op, "drvBattery", opname, value)`), not ops
            if *op == *iface {
                continue;
            }
            match (iface.as_str(), op.as_str()) {
                // --- hwiAdc: the 32 raw samples, one per line, with an extra
                //     empty line after the 9th, the 17th and the 25th value
                //     (the original: `(bVar3 & 7) == 0`, see
                //     analysis/decomp/FUN_080073DC_0x080073DC.c).  The reply
                //     carries no prefix at all.
                ("hwiAdc", "buf") => {
                    let out = request("get hwiAdc buf");
                    let lines: Vec<&str> = out.split('\n').collect();
                    // 32 values, each with its newline, plus the 3 extra empty
                    // lines and the empty tail of the last newline -> 36
                    // elements when split
                    assert_eq!(lines.len(), 36, "hwiAdc buf: {:?}", out);
                    // the empty lines sit behind the 9th, the 17th and the
                    // 25th value (the original: `(bVar3 & 7) == 0`); each value
                    // ends with its own newline, so the empty line is the
                    // element behind the value
                    for i in [9usize, 18, 27] {
                        assert_eq!(lines[i], "", "hwiAdc buf: no empty line at {}", i);
                    }
                    // every other line is a number
                    for (i, l) in lines.iter().enumerate() {
                        if i == 9 || i == 18 || i == 27 || i == 35 {
                            continue;
                        }
                        assert!(
                            !l.is_empty() && l.chars().all(|c| c.is_ascii_digit()),
                            "hwiAdc buf: line {} is not a number: {:?}",
                            i,
                            l
                        );
                    }
                }
                // --- hwiDrive: the duty is set without any answer, the fault
                //     flags are answered with the constant "0" - and with the
                //     op word "set", because the original hard codes op code 2
                //     in FUN_0800A16C
                ("hwiDrive", "dutyA") | ("hwiDrive", "dutyB") => {
                    let out = request(&format!("set hwiDrive {} 200", op));
                    assert_eq!(out, "", "hwiDrive {} must not be answered", op);
                }
                ("hwiDrive", "nFault") => {
                    let out = request("get hwiDrive nFault");
                    assert_eq!(out, "set hwiDrive nFault 0\n");
                }
                // --- drvBattery: the values are printed in hexadecimal and
                //     answered with the op word "set"; the pack init is not
                //     answered
                ("drvBattery", "voltage") => {
                    assert_eq!(request("get drvBattery voltage"), "set drvBattery voltage 0\n");
                }
                ("drvBattery", "temp") => {
                    assert_eq!(request("get drvBattery temp"), "set drvBattery temp 0\n");
                }
                ("drvBattery", "current") => {
                    assert_eq!(request("get drvBattery current"), "set drvBattery current 0\n");
                }
                ("drvBattery", "remainingCapacity") => {
                    assert_eq!(
                        request("get drvBattery remainingCapacity"),
                        "set drvBattery remainingCapacity 0\n"
                    );
                }
                ("drvBattery", "chargeStat") => {
                    // the charge pin of the pack (PF6) is low in the model ->
                    // the state is 1
                    assert_eq!(
                        request("get drvBattery chargeStat"),
                        "set drvBattery chargeStat 1\n"
                    );
                }
                ("drvBattery", "hwInit") => {
                    let out = request("call drvBattery hwInit");
                    assert_eq!(out, "", "call drvBattery hwInit must not be answered");
                }
                // --- hwiPower: "Vsens" is a *set* op, "pwrOff" cuts the power
                //     and never returns
                ("hwiPower", "Vsens") => {
                    let out = request("set hwiPower Vsens on");
                    assert_eq!(out, "set hwiPower Vsens 1\n");
                    let out = request("set hwiPower Vsens off");
                    assert_eq!(out, "set hwiPower Vsens 0\n");
                }
                ("hwiPower", "pwrOff") => {
                    // the op cuts the power and never returns, so it is not
                    // called here; the port has to know it
                    let src = fs::read_to_string(concat!(
                        env!("CARGO_MANIFEST_DIR"),
                        "/src/modules/hwi_power.rs"
                    ))
                    .expect("the hwiPower module");
                    assert!(
                        src.contains("\"pwrOff\""),
                        "the port does not know the op pwrOff"
                    );
                }
                (iface, op) => {
                    panic!("the op {}/{} of the original is not covered", iface, op);
                }
            }
        }
    }
}
