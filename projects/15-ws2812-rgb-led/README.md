# WS2812 RGB LED by bit-banging

Drives a WS2812B addressable RGB LED from one GPIO pin (PA7) by generating the 24-bit data stream in software with timed `nop` loops. The program sends the colour values `0xFF0000`, `0x00FF00` and `0x0000FF` in turn, 500 ms apart.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Timing-critical bit-banged protocol
- 24-bit colour values (`0xFF0000`, `0x00FF00`, `0x0000FF`)
- Cycle-counted delays

## Setup

- Project name in STM32CubeIDE: `WS2812`
- CubeMX target: `STM32F103CBTx`
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA7 | WS2812b | Output |

## How to test

1. Connect the WS2812B data line to PA7 and flash the program: the LED changes colour every 500 ms.

## Notes

- WS2812 LEDs expect the colour bytes in green, red, blue order, so the displayed colours may differ from the names of the values.
- The `nop` counts depend on the 72 MHz clock and the compiler optimisation level.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
