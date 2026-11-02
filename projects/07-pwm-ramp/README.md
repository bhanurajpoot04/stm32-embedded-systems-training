# PWM brightness ramp (TIM1)

TIM1 channel 1 generates PWM with prescaler 0 and period 999. The duty cycle ramps up from 1 to 999 and then back down to 1 in steps of 2 ms, so the LED fades in and out. Each duty value is also sent over UART1.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- PWM output (`HAL_TIM_PWM_Start`)
- Changing duty with `__HAL_TIM_SET_COMPARE`
- UART logging of the duty value

## Setup

- Project name in STM32CubeIDE: `PWM`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA2 | - | USART2_TX |
| PA3 | - | USART2_RX |
| PA8 | - | S_TIM1_CH1 |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB12 | KEY1 | Output |
| PB13 | KEY2 | Output |

## How to test

1. Flash the program: the PWM LED fades up and down. A serial terminal shows the duty values.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
