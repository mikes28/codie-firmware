# Codie robot firmware — rebuilt from the flash dump

This is a clean-room-style reconstruction of the robot's original firmware
(`0.5.0hw2.1`, build 2020-10-07) from a raw STM32 flash dump.  It is meant to be
**edited, rebuilt and re-flashed**.

```
fw/
  include/codie.h        shared contract (interface registry, protocol, module API)
  src/startup_*.c        reset vector, weak IRQ handlers, crt glue
  src/system_stm32g0.c   clock tree setup (SystemInit)
  src/main.c             boot sequence + main loop
  modules/               one file per original module (hwi_*, app_*, proxy, …)
  linker/stm32g071rb.ld  128 KB flash @0x08000000, 16 KB RAM @0x20000000
  flash.sh               upload over USB DFU + read-back verify
```

## Build

```bash
cd fw
make            # -> build/fw.elf, build/fw.bin
make size       # section sizes
```

The toolchain is auto-detected: an `arm-none-eabi-gcc` on `$PATH`, or the
bundled one in `../tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin`.
Override with `make TOOLCHAIN_PATH=/path/to/bin`.

Flags: `-mcpu=cortex-m0plus -mthumb -Os`, no HAL, no dynamic allocation.

## Flash

The robot exposes itself as a USB DFU device only while its ROM bootloader is
running (the application never enables USB).  So flashing means forcing the
chip into DFU mode (BOOT0 pin / reset into bootloader) and then:

```bash
cd fw
make flash                       # uses build/fw.bin
# or
./flash.sh build/fw.bin
```

`flash.sh` pads the image to a whole number of 2 KB flash pages, writes it at
0x08000000 and reads the flash back to verify.  It uses a native `dfu-util` when
available and falls back to the docker image used for the original dump:

```bash
docker run --rm --privileged -v /dev/bus/usb:/dev/bus/usb dfu \
  dfu-util -d 0483:df11 -a 0 -s 0x08000000 -D build/fw.bin
```

Do **all** reads before `:leave` — the ROM bootloader resets the MCU out of DFU
mode as soon as it is told to leave, and the device then disappears.

## The command protocol (unchanged from the original)

Requests, one per line, arriving over the BLE module's USART:

```
<get|set|call> <interface> <op> [args...]
```

Replies:

```
get|set|call <interface> <op> [value]          # interface answer
call :proxy message <level> <text> [extra]      # system message
call :proxy greeting "k":"v" … "msg:<text>"    # boot greeting
```

`level` is one of `???` `inf` `dbg` `wrn` `err`.

Exposed interfaces: `hwiAdc`, `hwiLed`, `hwiDrive`, `drvBattery`, `hwiSonar`,
`bleCom`, `hwiBuzz`, `hwiMic`, `hwiEnc`, `hwiLine`, `hwiPower`,
`hwiLightSense`, `appLights`, `appDrive`, `appSys`, `appLightSense`.

## Adding a new op

1. open the module file in `fw/modules/`
2. add a branch in its handler that compares `opname` and answers with
   `resp_value(op, "ifaceName", opname, value)`
3. `make && make flash`

## Hardware notes

The part is a Cortex-M0+ with 128 KB flash / 16 KB SRAM.  The register map used
here is the one the **original binary** uses (see `../MEMORY.md` for the full
list and the evidence) — GPIO lives on the 0x48000000 bus, USARTs use the
CR1/BRR/ISR/TDR layout, TIM15/16/17 sit at 0x40014000/44/48.  Keep those
addresses; they are what the board is wired to.

### The two serial links (roles matter!)

| | USART3 `0x40004800` | USART4 `0x40004C00` |
|---|---|---|
| role | AT / control channel to the BLE module | **command in, replies out** |
| pins | PB1+PB13 (AF4), PC4+PC5 (AF1), PA2 in | **PC10 TX / PC11 RX (AF0)** |
| baud | 14400, 2 stop bits | 460800, 2 stop bits |

`out_str()` / `out_char()` — and therefore the greeting, every value reply
and every error message — must go out on **USART4**.  USART3 only carries the
module's setup traffic ("BLE-boot", "PPRT"/"PPRET", "BLE-RSP …").
This is how the original does it (the output primitives' literal pool holds
0x40004C00) — see `../MEMORY.md` §4.

**USB is bootloading only**: the original never enables the USB peripheral,
so the robot's USB port is just the ROM bootloader's DFU interface.  A console
needs the USART4 header (PC10/PC11), e.g. through a USB-TTL adapter.

### Clock

The rebuild reproduces the original's clock tree: HSI16 → PLL (PLLMUL ×6) =
**96 MHz**, AHB = APB1 = APB2 = 96 MHz, flash ACR = 0x11 (1 wait state +
prefetch), SysTick reload 95999 (1 kHz).  **All UART baud and timer rate
constants must be derived from that clock** (timer clock = 2 × APB because the
APB prescaler is 1) — see `../MEMORY.md` §4.4 for the currently open bug.
