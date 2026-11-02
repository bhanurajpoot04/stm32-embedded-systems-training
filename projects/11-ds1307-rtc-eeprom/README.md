# DS1307 RTC and I2C EEPROM with LCD

I2C2 is used to talk to a DS1307 real-time clock (address `0xD0`) and an I2C EEPROM (address `0xA0`, 16-bit addressing). At start-up the program checks that the RTC answers, shows `RTC CONNECTED` or `RTC NOT FOUND` on the LCD, reads the time, and increments a boot counter stored in the EEPROM. The date and time are shown on the LCD and the loop reads the RTC. KEY1 resets the stored count, lights the RED LED, sounds the buzzer and sends `COUNT RESET`. KEY2 lights the GREEN LED and sets the RTC to the fixed value written in the source (24 September 2026, 15:09:00), then sends `RTC SET`.

![The training kit](../../media/laptop-cubemx-and-kit.jpg)

Demo clips from the training are in the [main README](../../README.md#demo-videos).

## What it demonstrates

- I2C memory read and write (`HAL_I2C_Mem_Read` and `HAL_I2C_Mem_Write`)
- BCD handling for RTC registers
- Storing a counter in non-volatile EEPROM
- LCD date and time display

## Setup

- Project name in STM32CubeIDE: `ds1307`
- CubeMX target: `STM32F103CBTx`
- UART1: 115200 baud, 8N1
- The project was configured with STM32CubeMX and uses the STM32 HAL

## Pins (from the CubeMX configuration)

| Pin | Label | Function |
|---|---|---|
| PA8 | RED | Output |
| PA9 | - | USART1_TX |
| PA10 | - | USART1_RX |
| PB1 | BUZZ | Output |
| PB4 | LCD_D3 | Output |
| PB5 | LCD_D2 | Output |
| PB6 | LCD_D1 | Output |
| PB7 | LCD_D0 | Output |
| PB8 | LCD_E | Output |
| PB9 | LCD_RS | Output |
| PB10 | - | I2C2_SCL |
| PB11 | - | I2C2_SDA |
| PB12 | KEY2 | Input |
| PB13 | KEY1 | Input |
| PB15 | GREEN | Output |

## How to test

1. Flash the program, power-cycle the board a few times and watch the count increase. Press KEY2 once to set the clock, and KEY1 to reset the count.

## Notes

- The time that KEY2 sets is hard-coded in `main.c`. Change it before using the clock.

## Build

See [`docs/getting-started.md`](../../docs/getting-started.md). The HAL drivers are not stored in this repository: open the `.ioc` file and generate the code to restore them.
