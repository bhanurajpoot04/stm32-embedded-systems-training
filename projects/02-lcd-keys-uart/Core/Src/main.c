/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : LCD + KEY1 + KEY2 + UART
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);

void UART_SendString(char *str);


/* ========================================================================== */
/*                                  MAIN                                      */
/* ========================================================================== */

int main(void)
{
    /* HAL initialization */
    HAL_Init();

    /* System Clock */
    SystemClock_Config();

    /* GPIO initialization */
    MX_GPIO_Init();

    /* UART initialization */
    MX_USART1_UART_Init();

    /* LCD initialization */
    LCD_Init();

    /* Show READY */
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_SendString("READY");

    HAL_Delay(1000);

    LCD_Clear();


    /* ====================================================================== */
    /*                              MAIN LOOP                                 */
    /* ====================================================================== */

    while (1)
    {
        /* ================================================================ */
        /*                         KEY 1                                   */
        /* ================================================================ */

        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
        {
            /* LCD */
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_SendString("KEY 1");

            /* UART */
            UART_SendString("KEY 1\r\n");

            /* Wait until KEY1 is released */
            while (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
                   == GPIO_PIN_RESET)
            {
            }

            /* Debounce */
            HAL_Delay(50);
        }


        /* ================================================================ */
        /*                         KEY 2                                   */
        /* ================================================================ */

        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
        {
            /* LCD */
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_SendString("KEY 2");

            /* UART */
            UART_SendString("KEY 2\r\n");

            /* Wait until KEY2 is released */
            while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)
                   == GPIO_PIN_RESET)
            {
            }

            /* Debounce */
            HAL_Delay(50);
        }
    }
}


/* ========================================================================== */
/*                              UART STRING                                  */
/* ========================================================================== */

void UART_SendString(char *str)
{
    HAL_UART_Transmit(
        &huart1,
        (uint8_t *)str,
        strlen(str),
        HAL_MAX_DELAY
    );
}


/* ========================================================================== */
/*                              LCD COMMAND                                  */
/* ========================================================================== */

void LCD_SendCommand(uint8_t cmd)
{
    /* RS = 0 -> COMMAND */
    HAL_GPIO_WritePin(
        LCD_RS_GPIO_Port,
        LCD_RS_Pin,
        GPIO_PIN_RESET
    );


    /* ---------------- HIGH NIBBLE ---------------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        (cmd & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        (cmd & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        (cmd & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        (cmd & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* Enable pulse */
    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );


    /* ---------------- LOW NIBBLE ---------------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        (cmd & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        (cmd & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        (cmd & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        (cmd & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* Enable pulse */
    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);
}


/* ========================================================================== */
/*                              LCD DATA                                     */
/* ========================================================================== */

void LCD_SendData(uint8_t data)
{
    /* RS = 1 -> DATA */
    HAL_GPIO_WritePin(
        LCD_RS_GPIO_Port,
        LCD_RS_Pin,
        GPIO_PIN_SET
    );


    /* ---------------- HIGH NIBBLE ---------------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* Enable pulse */
    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );


    /* ---------------- LOW NIBBLE ---------------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* Enable pulse */
    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);
}


/* ========================================================================== */
/*                              LCD STRING                                   */
/* ========================================================================== */

void LCD_SendString(char *str)
{
    while (*str)
    {
        LCD_SendData((uint8_t)*str);
        str++;
    }
}


/* ========================================================================== */
/*                              LCD CLEAR                                    */
/* ========================================================================== */

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);

    HAL_Delay(2);
}


/* ========================================================================== */
/*                              LCD CURSOR                                   */
/* ========================================================================== */

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    if (row == 0)
    {
        LCD_SendCommand(0x80 + col);
    }
    else
    {
        LCD_SendCommand(0xC0 + col);
    }
}


/* ========================================================================== */
/*                              LCD INIT                                     */
/* ========================================================================== */

void LCD_Init(void)
{
    HAL_Delay(50);


    /* RS = 0 */
    HAL_GPIO_WritePin(
        LCD_RS_GPIO_Port,
        LCD_RS_Pin,
        GPIO_PIN_RESET
    );


    /* ================================================================ */
    /* 4-BIT INITIALIZATION                                             */
    /* ================================================================ */

    /* -------- Send 0x03 -------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        GPIO_PIN_SET
    );


    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(5);


    /* -------- Send 0x03 again -------- */

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);


    /* -------- Send 0x03 again -------- */

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);


    /* -------- Send 0x02 -> 4-bit mode -------- */

    HAL_GPIO_WritePin(
        LCD_D7_GPIO_Port,
        LCD_D7_Pin,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D6_GPIO_Port,
        LCD_D6_Pin,
        GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        LCD_D5_GPIO_Port,
        LCD_D5_Pin,
        GPIO_PIN_SET
    );

    HAL_GPIO_WritePin(
        LCD_D4_GPIO_Port,
        LCD_D4_Pin,
        GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        LCD_E_GPIO_Port,
        LCD_E_Pin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);


    /* ================================================================ */
    /* LCD COMMANDS                                                     */
    /* ================================================================ */

    LCD_SendCommand(0x28);      // 4-bit, 2 lines, 5x8
    LCD_SendCommand(0x0C);      // Display ON, cursor OFF
    LCD_SendCommand(0x01);      // Clear display

    HAL_Delay(2);

    LCD_SendCommand(0x06);      // Entry mode
}


/* ========================================================================== */
/*                              GPIO INIT                                    */
/* ========================================================================== */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* GPIO CLOCKS */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();


    /* ================================================================ */
    /* KEY1 + KEY2                                                      */
    /* ================================================================ */

    GPIO_InitStruct.Pin = KEY1_Pin | KEY2_Pin;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(
        KEY1_GPIO_Port,
        &GPIO_InitStruct
    );


    /* ================================================================ */
    /* LCD PINS                                                         */
    /* ================================================================ */

    GPIO_InitStruct.Pin =
        LCD_D7_Pin |
        LCD_D6_Pin |
        LCD_D5_Pin |
        LCD_D4_Pin |
        LCD_E_Pin |
        LCD_RS_Pin;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(
        LCD_RS_GPIO_Port,
        &GPIO_InitStruct
    );
}


/* ========================================================================== */
/*                              USART1 INIT                                  */
/* ========================================================================== */

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200;

    huart1.Init.WordLength = UART_WORDLENGTH_8B;

    huart1.Init.StopBits = UART_STOPBITS_1;

    huart1.Init.Parity = UART_PARITY_NONE;

    huart1.Init.Mode = UART_MODE_TX_RX;

    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    huart1.Init.OverSampling = UART_OVERSAMPLING_16;


    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}


/* ========================================================================== */
/*                           SYSTEM CLOCK                                    */
/* ========================================================================== */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    /* HSE + PLL */

    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE;

    RCC_OscInitStruct.HSEState =
        RCC_HSE_ON;

    RCC_OscInitStruct.HSEPredivValue =
        RCC_HSE_PREDIV_DIV1;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
        RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLMUL =
        RCC_PLL_MUL9;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }


    /* Clock configuration */

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV2;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}


/* ========================================================================== */
/*                              ERROR HANDLER                                */
/* ========================================================================== */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
