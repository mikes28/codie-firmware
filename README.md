# Codie robot — STM32 firmware dump

Raw dump of the robot's STM32 bootloader flash, captured over USB DFU on 2026-10-21.

> **This dump has been reverse engineered.** The original firmware was
> decompiled (371/378 functions, Ghidra) and rebuilt as editable C in
> [`fw/`](fw/) — see [Rebuilding the firmware](#rebuilding-the-firmware) below.
> [`MEMORY.md`](MEMORY.md) holds the full analysis notes (chip identification,
> protocol, register map, recovered module behaviour) and is the file to read
> to continue the work.

## Rebuilding the firmware

```bash
cd fw
make            # -> build/fw.elf, build/fw.bin  (12.5 KB, fits 128 KB flash)
make flash      # upload over USB DFU + read back and verify
```

* toolchain: `arm-none-eabi-gcc` (auto-detected in `../tools/`, or on `$PATH`)
* target: Cortex-M0+, 128 KB flash @ 0x08000000, 16 KB RAM @ 0x20000000
* no HAL, no dynamic allocation, one file per original module in `fw/modules/`
* the wire protocol is byte-identical to the original (see `fw/README.md`)

Verification helpers:

```bash
python3 tools/verify_features.py fw/build/fw.bin   # protocol surface vs. the dump
gcc -Ifw/include tools/protocol_test.c fw/modules/proxy.c -o /tmp/pt && /tmp/pt
```

The host test prints the exact bytes of the greeting, the value reply, the
error messages and the unknown-interface reply and compares them with the
behaviour recovered from the original binary.

## Files

| file | size | sha256 (short) | what |
|---|---|---|---|
| `stm32_flash.bin` | 131072 (128 KB) | `0c5f3583ee1a96a7…` | internal flash @ `0x08000000`, the running application |
| `stm32_optionbytes.bin` | 16 | `aa039199b4ef7262…` | option bytes @ `0x1FFFF800` (read only, never written) |
| `stm32_flash_postwrite_verify.bin` | 131072 | `0c5f3583ee1a96a7…` | re-read after a same-image write test — byte-identical to the dump above |
| `strings.txt` | — | — | all printable strings ≥ 5 chars found in the flash |
| `SHA256SUMS` | — | — | checksums for every file here |

## Device

- USB: `0483:df11` STMicroelectronics **STM32 BOOTLOADER** (USB DFU 1.1a, transfer size 2048 B)
- DFU alt 0 = `@Internal Flash /0x08000000/064*0002Kg` → 64 × 2 KB = **128 KB**
- DFU alt 1 = `@Option Bytes /0x1FFFF800/01*016 e` → **16 B**
- Vector table: initial SP `0x20003fff` (→ 16 KB SRAM @ `0x20000000`), reset handler `0x08002904`
  → STM32F0x1 / G0x1-class part (128 KB flash / 16 KB RAM), e.g. STM32F072RB or STM32G071RB
- USB is **DFU-only**: the application does not enable a USB peripheral, so the board shows no
  CDC/serial device when running normally. There is no way to talk to the robot over USB
  without reflashing.

## Firmware identity (from the binary)

- version string `0.5.0hw2.1` @ `0x0800c660` → app 0.5.0, hardware 2.1
- build stamp `1602121419` @ `0x0800c67c` → 2020-10-07 21:43:39 UTC
- module/interface names: `appDrive`, `appLights`, `appSys`, `appLightSense`, `drvBattery`,
  `hwiAdc`, `hwiBuzz`, `hwiDrive`, `hwiEnc`, `hwiLed`, `hwiLightSense`, `hwiLine`, `hwiMic`,
  `hwiPower`, `hwiSonar`, `bleCom`
- internal command protocol is **text**, one command per line:
  `<get|set|call> <interface> <op> [args…]`, responses are JSON-ish
  (`"key":value … "msg":text`, prefixed with `:proxy` / `greeting`)
- the app talks to an external BLE module over a USART (`BLE-boot`, `BLE-RSP HELLO`,
  `BLE-RSP GAP SET MODE: `, `PPRT`, `PPRET`, `+ack`)

## Option bytes

`AA55 FF00 FF00 FF00 FF00 FF00 FF00 FF00` — magic `0x55AA`, user byte `0xFF`,
write-protection bytes all `0xFF` ⇒ **no sector write protection, no read protection**.

## How it was captured

No root, no `dfu-util` on the host: the tool runs in a container that gets the usbfs node.

```bash
docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
  dfu-util -d 0483:df11 -a 0 -s 0x08000000 -Z 131072 -U stm32_flash.bin
```

Notes that cost time, so they are recorded here:

- `dfu-util` 0.11's `-U` **requires a file argument** and refuses to overwrite an existing file
  (exit 73) — write to a temp name and move.
- `:leave` resets the MCU out of DFU mode; after that the device is gone from USB and every
  following command fails with "No DFU capable USB device available". Do **all** reads first,
  `:leave` only at the very end.
- A plain `-v /dev/bus/usb:/dev/bus/usb` is not enough: docker's default device cgroup blocks
  usbfs character devices (`LIBUSB_ERROR_IO`). Either `--privileged` or an explicit
  `--device=/dev/bus/usb/BBB/DDD` grant is needed.
- The ROM bootloader refuses reads outside flash/option bytes, so the chip ID
  (`DBGMCU_IDCODE` @ `0xE0042000`) cannot be read this way.

## Restoring

```bash
docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
  dfu-util -d 0483:df11 -a 0 -s 0x08000000 -D stm32_flash.bin
```

Re-read afterwards and compare with `SHA256SUMS` to confirm a byte-identical restore.
