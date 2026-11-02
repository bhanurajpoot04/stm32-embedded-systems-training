# FreeRTOS: two LED tasks

Uses FreeRTOS through the CMSIS-RTOS API with two tasks. The default task (normal priority) toggles the RED LED every 500 ms, and a second task (idle priority) toggles the GREEN LED every 127 ms, each using `osDelay`.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Creating tasks with `osThreadCreate`
- `osDelay` and task priorities
- FreeRTOS set up through STM32CubeMX

## Setup

- Project name in STM32CubeIDE: `RTOS`
- CubeMX target: `STM32F103CBTx`
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | RED | Output |
| PB4 | LCD_D7 | Output |
| PB5 | LCD_D6 | Output |
| PB6 | LCD_D5 | Output |
| PB7 | LCD_D4 | Output |
| PB8 | LCD_E | GPXTI8 |
| PB9 | LCD_RS | Output |
| PB12 | KEY2 | Input |
| PB13 | KEY1 | Input |
| PB15 | GREEN | Output |

## How to test

1. Flash the program: the RED and GREEN LEDs blink at different rates independently.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
