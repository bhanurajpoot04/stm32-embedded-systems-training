# Two timers: LED blink and a software clock

TIM1 and TIM2 both run with prescaler 5000-1 and period 7200-1. The TIM1 callback toggles LED1 and LED2. The TIM2 callback keeps a software clock (`hh`, `mm`, `ss`) and sends it over UART1 as `hh,mm,ss` each time it fires.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- Two timers in interrupt mode
- Software time keeping
- Formatted UART output from an interrupt callback

## Setup

- Project name in STM32CubeIDE: `TIMER2`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | LED1 | Output |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB15 | LED2 | Output |

## How to test

1. Open a serial terminal on UART1: lines such as `00,00,05` appear as the clock counts.

## Notes

- The main loop also toggles LED2 continuously, as written.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
