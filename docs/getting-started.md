# Getting started

## What you need

- STM32CubeIDE (it includes STM32CubeMX)
- A programmer for the board, for example an ST-LINK
- A serial terminal (115200 baud, 8N1) for the programs that use UART1 (PA9 = TX, PA10 = RX)

## Open a program

1. In STM32CubeIDE choose **File, Import, General, Existing Projects into Workspace**.
2. Select the folder of one program, for example `projects/01-led-buzzer-key`, and finish.
3. The HAL drivers (and FreeRTOS for programs 13 and 14) are not stored in this repository. Open the project's `.ioc` file and choose **Project, Generate Code**. CubeIDE downloads the STM32F1 firmware package if needed and restores the `Drivers` (and `Middlewares`) folders.
4. Build the project and run it on the board.

## Notes

- Only the application sources, the `.ioc` file, the startup file, the linker script and the Eclipse project files are kept, so each program is small and can be rebuilt from its configuration.
- Programs use different CubeMX targets (`STM32F103C8Tx` or `STM32F103CBTx`). If your board has a different part, change the device in the `.ioc` file before generating the code.
