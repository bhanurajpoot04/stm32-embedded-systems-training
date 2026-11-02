# Multiplication table over UART

Waits for one character on UART1. If it receives a digit, it prints that digit's multiplication table from 1 to 10 back over UART, one line per product, for example `3 x 4 = 12`.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- UART receive with timeout (`HAL_UART_Receive`)
- `sprintf` formatting
- UART transmit of text

## Setup

- Project name in STM32CubeIDE: `Table`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |

## How to test

1. Send a digit such as `7` from a serial terminal: the board replies with the 7 times table.

## Notes

- Only a single digit `0` to `9` is expected. Other characters give meaningless tables.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
