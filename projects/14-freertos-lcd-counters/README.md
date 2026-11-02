# FreeRTOS: two tasks writing to the LCD

Two FreeRTOS tasks share the LCD. The default task increments a counter and shows `R1: <n>` on the first line every 500 ms, and the second task shows `R2: <n>` on the second line every 1000 ms. A semaphore `myLCDSem` is created in the project, and the LCD driver is the bit-banged 4-bit driver written for this program.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Two tasks with different periods
- CMSIS-RTOS semaphore creation
- Sharing one display between tasks

## Setup

- Project name in STM32CubeIDE: `RTOS2`
- CubeMX target: `STM32F103CBTx`
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PB4 | LCD_D7 | Output |
| PB5 | LCD_D6 | Output |
| PB6 | LCD_D5 | Output |
| PB7 | LCD_D4 | Output |
| PB8 | LCD_EN | Output |
| PB9 | LCD_RS | Output |

## How to test

1. Flash the program: two counters count at different speeds on the two LCD lines.

## Notes

- The semaphore is created but the task bodies do not take it before writing to the LCD.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
