# MEMORY_MAP — peripheral bases and register offsets

All addresses come from the machine code of the original flash dump
(`stm32_flash.bin`), **not** from a data sheet. They are what the board is
wired to. `rust/src/hal/addr.rs` holds them and must not be "fixed" to match a
datasheet.

The exact ST part number is not pinned (the GPIO bus at 0x48000000 plus the
G4-style USART plus 31 IRQs does not match one single published ST map).

---

## 1. Memory map

| region | address | size |
|---|---|---|
| Flash | `0x08000000` | 128 KB |
| RAM | `0x20000000` | 16 KB |
| Initial stack pointer | `0x20003FFF` | |
| Vector table | `0x08000000` | 48 words (SP + Reset + 15 system + 31 IRQ) |

RAM layout (from the dump): `.data` = flash `0x0800C720`..`0x0800D624` → RAM
`0x20000000`..`0x20000F04`, `.bss` = `0x20000F08`..`0x20001030`.

Reset magic words (RAM): `0x200025A8 == -0x281a0858` (= 0xD7E5F7A8) and
`0x200017A8 == 0x19C3FE40`.

---

## 2. Peripheral base addresses

| peripheral | base | used by |
|---|---|---|
| GPIOA | `0x48000000` | encoders, line sensor, fault poll |
| GPIOB | `0x48000400` | BLE (PB1/PB13), LED data (PB14), buzzer (PB8) |
| GPIOC | `0x48000800` | motors (PC6-9), enable (PC2), fault input (PC14), power-hold (PC12), power-good (PC13), Vsens (PC15) |
| GPIOF | `0x48001400` | charge state (PF6) |
| TIM1 | `0x40012C00` | wheel encoders |
| TIM2 | `0x40000000` | wheel encoders |
| TIM3 | `0x40000400` | motor drive |
| TIM6 | `0x40001000` | millisecond tick (original) |
| TIM15 | `0x40014000` | ARGB LED engine + DMA1 ch5 |
| TIM16 | `0x40014400` | buzzer / tone |
| TIM17 | `0x40014800` | sonar capture + 2nd LED engine |
| USART1 | `0x40013800` | |
| USART3 | `0x40004800` | BLE module AT (14400) |
| USART4 | `0x40004C00` | command + reply (460800) |
| ADC1 | `0x40012400` | battery / mic / light |
| DMA1 | `0x40020000` | ADC DMA, LED DMA |
| RCC | `0x40021000` | clock |
| FLASH | `0x40022000` | |
| IWDG | `0x40003000` | watchdog |
| SYSCFG | `0x40010000` | EXTI configuration |
| EXTI | `0x40010400` | external interrupts |
| NVIC | `0xE000E100` | Cortex-M NVIC |
| NVIC_IPR | `0xE000E400` | priority registers |
| SysTick | `0xE000E010` | millisecond tick (port) |

---

## 3. GPIO register offsets

Standard layout, proven by the dump's helpers:

| offset | register |
|---|---|
| `0x00` | MODER |
| `0x04` | OTYPER |
| `0x08` | OSPEEDR |
| `0x0C` | PUPDR |
| `0x10` | IDR |
| `0x14` | ODR |
| `0x18` | BSRR |
| `0x1C` | LCKR (BRR on some parts; the port uses 0x28 for clear) |
| `0x20` | AFR0 (AFRL) |
| `0x24` | AFR1 (AFRH) |

**Important:** the port's `gpio::reset()` and `gpio::write()` clear at offset
`0x28` (halfword store), not at `0x1C` (BRR). Dump evidence:
`FUN_080097E8 @0x800980e: strh r1,[r3,#40]` and
`FUN_08009E0C @0x8009e20: strh r1,[r2,#40]`.

---

## 4. Timer register offsets

| offset | register |
|---|---|
| `0x00` | CR1 |
| `0x04` | CR2 |
| `0x08` | SMCR |
| `0x0C` | DIER |
| `0x10` | SR |
| `0x14` | EGR |
| `0x18` | CCMR1 |
| `0x1C` | CCMR2 |
| `0x20` | CCER |
| `0x24` | CNT |
| `0x28` | PSC |
| `0x2C` | ARR |
| `0x30` | RCR |
| `0x34` | CCR1 |
| `0x38` | CCR2 |
| `0x3C` | CCR3 |
| `0x40` | CCR4 |
| `0x44` | BDTR |

---

## 5. USART register offsets

G4-style layout, proven by `FUN_0800B54C` (reads +0x08/+0x1C) and
`FUN_08007D88` (writes TDR at +0x28):

| offset | register |
|---|---|
| `0x00` | CR1 |
| `0x04` | CR2 |
| `0x08` | CR3 |
| `0x0C` | BRR |
| `0x10` | GTPR |
| `0x14` | RTOR |
| `0x18` | RQR |
| `0x1C` | ISR |
| `0x20` | ICR |
| `0x24` | RDR |
| `0x28` | TDR |

---

## 6. ADC1 register offsets

| offset | register |
|---|---|
| `0x00` | ISR |
| `0x08` | CR |
| `0x0C` | CFGR1 |
| `0x14` | SMPR |
| `0x28` | CHSELR |
| `0x40` | DR |

ADC1_CCR is at `0x40012440` (ADC1 base + 0x40... actually the dump uses
`0x40012440` for the common CCR).

ADC channel masks: `[0x02, 0x04, 0x20, 0x40]` (IN1, IN2, IN5, IN6).
Sample times: `4, 6, 6, 6`.

---

## 7. DMA1

Base: `0x40020000`. Channel blocks at `DMA1 + 0x08 + (n-1)*0x14`:

| channel | address |
|---|---|
| CH1 | `0x40020008` |
| CH2 | `0x4002001C` |
| CH3 | `0x40020030` |
| CH4 | `0x40020044` |
| CH5 | `0x40020058` |
| CH6 | `0x4002006C` |
| CH7 | `0x40020080` |

Channel register offsets within a block: CR `0x00`, CNDTR `0x04`, CPAR `0x08`,
CMAR `0x0C`.

DMA1 interrupt flags at `DMA1 + 0x04` (ISR), per-channel mask:
- CH1: `0x0000000F`, CH2: `0x000000F0`, CH3: `0x00000F00`,
  CH4: `0x0000F000`, CH5: `0x000F0000`

ADC uses DMA1 CH2. LED engine uses DMA1 CH5. Sonar uses DMA1 CH1.

---

## 8. RCC register offsets

| offset | register |
|---|---|
| `0x00` | CR |
| `0x04` | CFGR |
| `0x08` | CIR |
| `0x14` | AHBENR |
| `0x18` | APB2ENR |
| `0x1C` | APB1ENR |
| `0x2C` | (AUX / reset flags) |
| `0x30` | CSR |
| `0x34` | CCIPR |

Flash ACR at `0x40022000 + 0x00`.

**Clock enables (original's bit assignments, shifted by one vs published maps):**
- GPIOA `0x20000`, GPIOB `0x40000`, GPIOC `0x80000` (AHBENR)
- APB2: TIM15/16/17 `0x20000`
- APB1: USART3 `0x40000`, USART4 `0x80000`

**Clock tree (FUN_08002958):** HSI16 → PLL (×6) = 96 MHz SYSCLK;
AHB = APB1 = APB2 = 96 MHz; flash `ACR = 0x11`.

---

## 9. IWDG

Base: `0x40003000`. Registers:
- `+0x00` KR / PR (key register / prescaler)
- `+0x04` RLR (reload)
- `+0x08` WLR (window)

Init sequence (FUN_0800A640): `0x5555`→+0x00, `2`→+0x04, `0xFFE`→+0x08,
`0xAAAA`→+0x00 (refresh), `0xCCCC`→+0x00 (enable).
Kick: `0xAAAA`→+0x00 every main-loop iteration.

---

## 10. EXTI / SYSCFG

SYSCFG base `0x40010000`, EXTI base `0x40010400`.

EXTI offsets: RTSR `0x08`, FTSR `0x0C`, PR `0x14`.

EXTICR offset for line `n`: `((n>>2)+2)<<2`, field shift `(n & 3) << 2`.
For line 13 (power-good, GPIOC): EXTICR4 (`0x14`), bits 15:12.

NVIC IPR address: `0xE000E400 + (irq>>2)*4`, byte `(irq & 3) << 3`,
priority in top of byte (`prio << 6`).

---

## 11. SysTick

Base `0xE000E010`. Offsets: CTRL `0x00`, LOAD `0x04`, VAL `0x08`.
Port reload: 95999 (96 MHz / 96000 = 1 kHz).
