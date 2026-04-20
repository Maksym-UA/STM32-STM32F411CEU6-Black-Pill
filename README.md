# Bidirectional LED/Blink Control: ESP32-S3 ↔ STM32F411CEU6 via UART

## Project description

Two-way button-controlled LED/blink system over UART:

- Pressing the **button on ESP32-S3** sends command `'T'` to STM32 → toggles STM32 LED blinking.
- Pressing the **button on STM32** sends command `'T'` to ESP32 → toggles ESP32 onboard RGB LED blinking (500 ms interval).

Both sides use the same single-byte command protocol: `'T'` = toggle blink state.

## Hardware

| Component | Details |
|---|---|
| MCU 1 | Espressif ESP32-S3-Wroom-1-n16r8 |
| MCU 2 | STM32F411CEU6 (Black Pill) |
| Programmer | ST-Link V2 (for STM32) |

## Wiring

### UART connection (ESP32-S3 ↔ STM32)

```
ESP32-S3    STM32F411
--------    ---------
GPIO17 TX → RX (e.g. PA10 / USART1)
GPIO18 RX ← TX (e.g. PA9  / USART1)
GND       — GND  (common ground required)
```

### ESP32-S3 pins

```
GPIO0  - BOOT button (active low, built-in pull-up)
RGB_BUILTIN (fallback GPIO48) - onboard RGB LED (NeoPixel)
GPIO17 - UART1 TX → STM32 RX
GPIO18 - UART1 RX ← STM32 TX
```

## Protocol

| Sender | Byte | Effect on receiver |
|---|---|---|
| ESP32 button pressed | `'T'` (0x54) | STM32 toggles its LED blink |
| STM32 button pressed | `'T'` (0x54) | ESP32 toggles its LED blink |

- Baud rate: **115200** on both sides (8N1, no flow control)
- Debounce: 40 ms edge-triggered on ESP32 side

## Software requirements

- STM32CUBE IDE
- STM32CUBE MX

## Configuration

Protocol:
- Baud: 115200, 8N1, no flow control
- Receives 'T' (0x54) → toggle STM32 LED blink (500 ms)
- Sends 'T' (0x54) on button press → ESP32 toggles its LED blink
STM32CubeIDE / HAL setup:
USART1 config (.ioc):
- Mode:        Asynchronous
- Baud Rate:   115200
- Word Length: 8 bits
- Stop Bits:   1
- Parity:      None

Button GPIO (e.g. PC13 on Black Pill — active low):
- Mode:       Input
- Pull:       Pull-up

LED GPIO (e.g. PC13 or dedicated pin):
- Mode:       Output Push-Pull


## Project structure

```
.
├── Core/
│   ├── Inc/
│   │   ├── app.h
│   │   ├── main.h
│   │   ├── stm32f4xx_hal_conf.h
│   │   └── stm32f4xx_it.h
│   ├── Src/
│   │   ├── app.c
│   │   ├── main.c
│   │   ├── stm32f4xx_hal_msp.c
│   │   ├── stm32f4xx_it.c
│   │   ├── syscalls.c
│   │   ├── sysmem.c
│   │   └── system_stm32f4xx.c
│   |── Startup/
├── Drivers/
├──Beetroot.ioc
├── README.md
├── STM32F411CEUX_FLASH.ld
└── STM32F411CEUX_RAM.ld
```

## Contact

Feedback: max.savin3@gmail.com
