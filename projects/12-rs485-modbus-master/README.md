# RS485 Modbus RTU master

UART1 is used with an RS485 transceiver whose direction pin is `REDE` (PA8, high to transmit, low to listen). When KEY is pressed the program builds a Modbus RTU request (slave address 1, function 3, read 2 holding registers starting at 0) with a CRC-16, sends it, waits for the reply, checks the address, function code and CRC, and shows `SW1` and `SW2` on the LCD from the returned register values. After the transmit the program waits for the transmission-complete flag before switching the transceiver back to receive.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- RS485 half-duplex direction control
- Modbus RTU frame format and CRC-16
- UART receive with timeout
- LCD output

## Setup

- Project name in STM32CubeIDE: `RS485`
- CubeMX target: `STM32F103C8Tx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | REDE | Output |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB4 | LCD_D7 | Output |
| PB5 | LCD_D6 | Output |
| PB6 | LCD_D5 | Output |
| PB7 | LCD_D4 | Output |
| PB8 | LCD_E | Output |
| PB9 | LCD_RS | Output |
| PB13 | KEY | Input |
| PB15 | GREEN | Output |

## How to test

1. This program is the master side. Connect it over RS485 to a Modbus slave (address 1) that serves 2 holding registers, then press KEY.

## Notes

- The slave side program is not part of this repository.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
