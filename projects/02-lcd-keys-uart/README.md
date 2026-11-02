# 16x2 LCD driver with keys and UART

A 4-bit HD44780 LCD driver written from scratch on plain GPIO pins (initialisation sequence, command, data, string, clear and cursor positioning). The LCD shows `READY` at start-up. Pressing KEY1 or KEY2 shows `KEY 1` or `KEY 2` on the LCD and sends the same text over UART1. The program waits for the key to be released and debounces for 50 ms.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Bit-banged 4-bit LCD interface (RS, E, D4 to D7)
- UART transmit with `HAL_UART_Transmit`
- Key release wait and debounce

## Setup

- Project name in STM32CubeIDE: `LCD`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB4 | LCD_D7 | Output |
| PB5 | LCD_D6 | Output |
| PB6 | LCD_D5 | Output |
| PB7 | LCD_D4 | Output |
| PB8 | LCD_E | Output |
| PB9 | LCD_RS | Output |
| PB12 | KEY1 | Input |
| PB13 | KEY2 | Input |

## How to test

1. Flash the program: the LCD shows `READY`, then press KEY1 or KEY2.
2. Open a serial terminal on UART1 to see `KEY 1` or `KEY 2`.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
