# SWD debug

## Project description

The LED is set to blink. GPIO direct register programming: No HAL or LL drivers.
Debounced button & edge detection: Only changes rate once per press.
Software "millisecond" timer: by counting iterations and adding a blocking delay.

## Hardware

| Component | Details |
|---|---|
| MCU 1 | STM32F411CEU6 (Black Pill) |
| Programmer | ST-Link V2 (for STM32) |


## Software requirements

- STM32CUBE IDE
- STM32CUBE MX

## Configuration

- LED: PC13, GPIO Output
- Button: PA0, GPIO Input

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
