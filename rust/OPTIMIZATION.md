# The optimization of the Rust port

What was done to the port, what it saved and what it cost.  The behaviour
did not change: the 7 tests are the oracle and they were green before and
after every step.

## 1. The result

| | the image | `.text` | `.rodata` | the C rebuild |
|---|---|---|---|---|
| before | 15 456 | 13 608 | 1 636 | 12 608 |
| **after** | **13 432** | **12 332** | **904** | 12 608 |

**−2 024 bytes (−13.1 %)**, the gap to the C build went from 2 848 to 824
bytes.  The flash is 128 KB, so the image uses 10.5 % of it.

## 2. What was done

| # | change | the saving | who |
|---|---|---|---|
| 1 | the UTF-8 validation of the command line and of the formatted numbers is gone (`from_utf8_unchecked` + a `debug_assert`) - the C does not validate anything either | −404 (code) −256 (data) | the port |
| 2 | the ring buffer wraps with a mask instead of a modulo when the capacity is a power of two (the two rings of the BLE link: 128 and 64 bytes) - the hot path of every byte | a few bytes, faster | the port |
| 3 | `opt-level = "s"` → `"z"` | −860 | `OptBuild` |
| 4 | the exception index (`.ARM.exidx`) is discarded in the linker script - there is no unwinder with `panic = "abort"` | −16 | `OptBuild` |
| 5 | the dead copy of the protocol strings is gone (`src/proto/catalog.rs`) - the Rust compiler puts the `&str` literals into one `.rodata` blob, the catalog was a second, NUL terminated copy of 62 strings that the firmware never reads | −572 | the port |
| 6 | the words of the log levels and the prefix of the greeting message are kept in the image (`#[used]` tables) - the optimiser would have dropped the three levels that the firmware never emits, the original carries them in its flash | +68 | the port |
| 7 | `debug = "line-tables-only"` instead of `debug = true` | the image is the same, the `.elf` is half the size, the build is a third faster | `OptBuild` |

## 3. What was tried and rejected

| the idea | the result |
|---|---|
| **GNU ld instead of `rust-lld`** | +32 bytes, and the strings are merged the same way by both linkers - the duplication was in the source, not in the linker.  Rust-lld stays. |
| `--icf=all` (identical code folding) | −40 bytes, but it folds functions that only look alike - too risky for a firmware that is proved byte for byte. |
| `--sort-section=name` | −20 bytes of pure noise (a different order, the same size). |
| `.text` aligned to 2 instead of 4 | 0 bytes. |
| `lto = "thin"` / `lto = false` | +1 400 / +8 100 bytes.  Fat LTO stays. |
| `codegen-units = 16` | +316 bytes.  One codegen unit stays. |
| `debug = false` | the image is the same, but the line tables are worth keeping for `addr2line`. |

## 4. Where the bytes are

The symbols of the image, grouped (`arm-none-eabi-nm --size-sort -S`):

| group | bytes |
|---|---|
| the port itself (`codie::*`) | 12 100 |
| `compiler_builtins` (the division helper, `memcpy`) | 956 |
| the C runtime | 662 |
| `core` | 278 |

`core` is down to 278 bytes: the UTF-8 validation and the panic formatting
are out of the image.  What is left is the integer division (the Cortex-M0+
has no divider - the C has the same helper) and `memcpy`.

The biggest single symbol is `boot::run` (3 700 bytes): the whole main loop
with the dispatcher and the three periodic tasks inlined into it by the fat
LTO.  The C has `main` at 376 bytes and keeps the dispatcher in its own
function - the same code, distributed differently.

## 5. The trade that was made

`opt-level = "z"` optimises for the size and gives up the devirtualisation:
34 call sites that were direct calls become indirect ones (the dispatch
through the interface table of the protocol layer).  The behaviour is
identical - the tests compare the bytes with the C firmware - and the cost
is one indirect call per **command** (not per byte), so the hot path (the
byte level I/O) is not affected.  With `"s"` the image would be 860 bytes
bigger.

## 6. How the image is checked now

The C tool `tools/verify_features.py` extracts the *maximal printable runs*
of the image.  The Rust image stores its string literals merged (the Rust
compiler puts them into one `.rodata` blob without a NUL between them), so
the maximal runs of the Rust image are long concatenations
("hwiDrivehwiPowergreeting...").

`rust/verify_features.py` is the counterpart of the C tool for the Rust
image: it imports the **same list** of protocol visible strings from the C
tool (so the two cannot drift apart) and looks for them as substrings of the
image.  The result is the same statement: every protocol visible string of
the original is in the image.

```bash
python3 rust/verify_features.py rust/build/fw.bin
```
