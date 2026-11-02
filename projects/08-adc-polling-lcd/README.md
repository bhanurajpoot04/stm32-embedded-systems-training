# ADC voltage on LCD and UART (polling)

Reads ADC1 channel 0 by polling, converts the 12-bit value to volts with `v = raw * 3.3 / 4095`, and shows the raw value and the voltage on the 16x2 LCD. The same reading is sent over UART1 as `RAW:<n> Volts: <v>` once per second. The ADC is calibrated at start-up.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- ADC calibration and single conversion polling
- 12-bit value to voltage conversion
- LCD and UART output with `sprintf`

## Setup

- Project name in STM32CubeIDE: `ADC`
- CubeMX target: `STM32F103C8Tx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB4 | LCD_7 | Output |
| PB5 | LCD_6 | Output |
| PB6 | LCD_5 | Output |
| PB7 | LCD_4 | Output |
| PB8 | LCD_EN | Output |
| PB9 | LCD_RS | Output |

## How to test

1. Connect a variable voltage (for example the kit's potentiometer) to the ADC input and watch the LCD or a serial terminal.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
