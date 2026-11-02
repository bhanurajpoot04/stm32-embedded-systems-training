# Timer interrupt blink (TIM1)

TIM1 is configured with prescaler 5000-1 and period 7200-1. With the 72 MHz clock set in CubeMX this gives an update interrupt about every 0.5 s, and the callback `HAL_TIM_PeriodElapsedCallback` toggles the LED.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Timer base in interrupt mode (`HAL_TIM_Base_Start_IT`)
- Computing prescaler and period from the clock
- Period elapsed callback

## Setup

- Project name in STM32CubeIDE: `TIMER1`
- CubeMX target: `STM32F103CBTx`
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | LED | Input |

## How to test

1. Flash the program and watch the LED.

## Notes

- The main loop also toggles the same LED continuously, as written. If the timer blink is not clearly visible, remove that line from the `while (1)` loop.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
