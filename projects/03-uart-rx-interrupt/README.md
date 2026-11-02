# UART receive with interrupt

Receives one byte at a time with `HAL_UART_Receive_IT` and acts on it in the receive-complete callback: `1` toggles the RED LED, `2` toggles the GREEN LED, and `0` is meant to switch both off. The receive is re-armed after every byte.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- UART receive interrupt and callback
- Re-arming `HAL_UART_Receive_IT`
- Controlling LEDs from a PC terminal

## Setup

- Project name in STM32CubeIDE: `RXTX`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | RED | Output |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB15 | GREEN | Output |

## How to test

1. Send the characters `1`, `2` and `0` from a serial terminal on UART1.

## Notes

- In the `0` branch the red LED write uses the GREEN port with the RED pin (`HAL_GPIO_WritePin(GREEN_GPIO_Port, RED_Pin, ...)`), so the red LED may not switch off. It should use `RED_GPIO_Port`.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
