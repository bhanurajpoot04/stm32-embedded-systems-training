# SPI master sending key commands

SPI1 is configured as a master (two lines, prescaler 64) with the chip select on PA4 driven by software. Pressing KEY1 sends the byte `0x01` and pressing KEY2 sends `0x02`. The chip select is pulled low only for the duration of each transfer.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- SPI master transmit (`HAL_SPI_Transmit`)
- Software chip select
- Key debounce

## Setup

- Project name in STM32CubeIDE: `SPI`
- CubeMX target: `STM32F103CBTx`
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA4 | - | SPI1_NSS |
| PA5 | - | SPI1_SCK |
| PA7 | - | SPI1_MOSI |
| PB12 | KEY2 | Input |
| PB13 | KEY1 | Input |

## How to test

1. This program is the master side. Connect it to a second SPI device that reads the bytes `0x01` and `0x02`, and press KEY1 and KEY2.

## Notes

- The slave side program is not part of this repository.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
