# Two ADC channels with DMA

ADC1 scans two channels (PA1 and PA5) continuously and a DMA transfer fills a two-element array. The main loop sends the two raw values over UART1 as `value0,value1` once per second.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- ADC scan mode with two channels
- Continuous conversion with DMA (`HAL_ADC_Start_DMA`)
- ADC calibration

## Setup

- Project name in STM32CubeIDE: `ADC2`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA1 | - | ADC IN1 |
| PA5 | - | ADC IN5 |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |

## How to test

1. Vary the voltages on PA1 and PA5 and watch the two values on a serial terminal.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
