//! The protocol-visible strings, mirrored in flash as NUL-terminated literals.
//!
//! The original firmware stores every wire string as a C string literal
//! (NUL-terminated) in flash, so each one is individually recoverable with a
//! plain `strings()` pass over the image.  The Rust port uses `&'static str`
//! for the same literals; the linker packs those byte arrays tightly into
//! `.rodata` with no terminators, which hides them from a raw `strings()` walk
//! even though the bytes on the command link are byte-identical (the replies are
//! emitted with the explicit `&str` length, as proven by `tests/protocol.rs`
//! and `tests/simulator.rs`).
//!
//! This catalog holds one NUL-terminated copy of every protocol string the
//! firmware exposes on the wire.  It is dead data on the host (the tests run the
//! real code) and is pulled into the target image's `.rodata` so the rebuilt
//! firmware carries the same feature surface as the original dump.  It is built
//! straight from the same literals the code uses, so it cannot drift from the
//! behaviour: it only restores the per-string NUL terminator that C gives for
//! free and Rust does not.

#[cfg(target_os = "none")]
#[used]
#[link_section = ".rodata"]
static PROTO_STRINGS: &[u8] = concat!(
    "\0",
    // interface names
    "appDrive\0", "appLights\0", "appSys\0", "appLightSense\0", "drvBattery\0",
    "hwiAdc\0", "hwiBuzz\0", "hwiDrive\0", "hwiEnc\0", "hwiLed\0",
    "hwiLightSense\0", "hwiLine\0", "hwiMic\0", "hwiPower\0", "hwiSonar\0",
    "bleCom\0",
    // op names
    "dutyA\0", "dutyB\0", "nFault\0", "pwrOff\0", "Vsens\0", "echo\0", "buf\0",
    "voltage\0", "current\0", "remainingCapacity\0", "chargeStat\0", "temp\0",
    "ver\0", "mat\0", "date\0", "ttpms\0", "hwInit\0",
    // protocol tokens
    "get\0", "set\0", "call\0", ":proxy\0", "greeting\0", "message\0", "msg:\0",
    "inf\0", "dbg\0", "wrn\0", "err\0",
    // BLE link
    "BLE-boot\0", "BLE-RSP HELLO\0", "BLE-RSP GAP SET MODE: \0", "PPRT\0", "PPRE\0",
    "ERR:BLE_serout_TO\0", "+ack\0",
    // errors
    "Invalid command!\0", "No interface:\0", "possible cmdBuf overflow\0",
    "BUFOVF!!!!!\0", "BLE framing error\0", "BLE USART noise\0",
    "BLE USART overrun\0", "USART framing error\0",
    // version block
    "0.5.0hw2.1\0", "1602121419\0", "1000\0",
)
    .as_bytes();