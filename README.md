# SWD debug

## Project description

The led is set to blink.
The button controls the led blinking at non predefined rate.
The program is implemented via direct acccess to registers without using HAL.

## Hardware

| Component | Details |
|---|---|
| MCU 1 | STM32F411CEU6 (Black Pill) |
| Programmer | ST-Link V2 (for STM32) |


## Software requirements

- STM32CUBE IDE
- STM32CUBE MX

## Configuration


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
