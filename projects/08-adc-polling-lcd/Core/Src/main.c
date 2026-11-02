/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);

/* LCD Functions */
void LCD_Enable(void);
void LCD_Send4Bits(uint8_t data);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Init(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);


/* USER CODE BEGIN 0 */

/* ================= LCD ENABLE ================= */

void LCD_Enable(void)
{
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

    HAL_Delay(1);
}


/* ================= SEND 4 BITS ================= */

void LCD_Send4Bits(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port,
                      LCD_D4_Pin,
                      (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D5_GPIO_Port,
                      LCD_D5_Pin,
                      (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D6_GPIO_Port,
                      LCD_D6_Pin,
                      (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D7_GPIO_Port,
                      LCD_D7_Pin,
                      (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_Enable();
}


/* ================= SEND COMMAND ================= */

void LCD_SendCommand(uint8_t cmd)
{
    /* RS = 0 for command */

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port,
                      LCD_RS_Pin,
                      GPIO_PIN_RESET);

    LCD_Send4Bits(cmd >> 4);

    LCD_Send4Bits(cmd & 0x0F);

    HAL_Delay(2);
}


/* ================= SEND DATA ================= */

void LCD_SendData(uint8_t data)
{
    /* RS = 1 for data */

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port,
                      LCD_RS_Pin,
                      GPIO_PIN_SET);

    LCD_Send4Bits(data >> 4);

    LCD_Send4Bits(data & 0x0F);

    HAL_Delay(1);
}


/* ================= LCD INITIALIZATION ================= */

void LCD_Init(void)
{
    HAL_Delay(20);

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port,
                      LCD_RS_Pin,
                      GPIO_PIN_RESET);

    /* Initialization sequence */

    LCD_Send4Bits(0x03);
    HAL_Delay(5);

    LCD_Send4Bits(0x03);
    HAL_Delay(1);

    LCD_Send4Bits(0x03);
    HAL_Delay(1);

    LCD_Send4Bits(0x02);

    /* 4-bit mode, 2 lines, 5x8 font */
    LCD_SendCommand(0x28);

    /* Display ON, cursor OFF */
    LCD_SendCommand(0x0C);

    /* Entry mode */
    LCD_SendCommand(0x06);

    /* Clear display */
    LCD_SendCommand(0x01);

    HAL_Delay(2);
}


/* ================= LCD CURSOR ================= */

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0)
    {
        address = 0x80 + col;
    }
    else
    {
        address = 0xC0 + col;
    }

    LCD_SendCommand(address);
}


/* ================= LCD PRINT ================= */

void LCD_Print(char *str)
{
    while (*str)
    {
        LCD_SendData(*str);
        str++;
    }
}


/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{
    /* USER CODE BEGIN 1 */

    int a;

    char str[50];

    char lcd_str[17];

    float v;

    /* USER CODE END 1 */


    /* MCU Configuration */

    HAL_Init();


    /* Configure system clock */

    SystemClock_Config();


    /* Initialize peripherals */

    MX_GPIO_Init();

    MX_ADC1_Init();

    MX_USART1_UART_Init();


    /* USER CODE BEGIN 2 */

    /* ADC Calibration */

    HAL_ADCEx_Calibration_Start(&hadc1);


    /* LCD Initialization */

    LCD_Init();


    /* LCD Backlight ON */

    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_13,
                      GPIO_PIN_SET);


    /* Startup message */

    LCD_SetCursor(0, 0);

    LCD_Print("ADC VOLTAGE");


    LCD_SetCursor(1, 0);

    LCD_Print("Initializing...");


    HAL_Delay(1000);


    LCD_SendCommand(0x01);

    /* USER CODE END 2 */


    /* Infinite loop */

    while (1)
    {
        /* ================= ADC START ================= */

        HAL_ADC_Start(&hadc1);


        /* Wait for conversion */

        HAL_ADC_PollForConversion(&hadc1, 10);


        /* Get ADC value */

        a = HAL_ADC_GetValue(&hadc1);


        /* ================= VOLTAGE ================= */

        /*
         * 12-bit ADC
         *
         * ADC range = 0 to 4095
         *
         * Reference = 3.3V
         */

        v = (a * 3.3f) / 4095.0f;


        /* ================= UART ================= */

        sprintf(str,
                "RAW:%d Volts: %.2f\r\n",
                a,
                v);


        HAL_UART_Transmit(&huart1,
                          (uint8_t *)str,
                          strlen(str),
                          500);


        /* ================= LCD LINE 1 ================= */

        LCD_SetCursor(0, 0);

        sprintf(lcd_str,
                "RAW: %4d       ",
                a);

        LCD_Print(lcd_str);


        /* ================= LCD LINE 2 ================= */

        LCD_SetCursor(1, 0);

        sprintf(lcd_str,
                "VOLT: %.2f V     ",
                v);

        LCD_Print(lcd_str);


        /* Update every 1 second */

        HAL_Delay(1000);
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};


    /* Oscillator configuration */

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


    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }


    /* ADC clock */

    PeriphClkInit.PeriphClockSelection =
        RCC_PERIPHCLK_ADC;


    PeriphClkInit.AdcClockSelection =
        RCC_ADCPCLK2_DIV6;


    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief ADC1 Initialization Function
  * @retval None
  */

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};


    hadc1.Instance = ADC1;


    hadc1.Init.ScanConvMode =
        ADC_SCAN_DISABLE;


    hadc1.Init.ContinuousConvMode =
        DISABLE;


    hadc1.Init.DiscontinuousConvMode =
        DISABLE;


    hadc1.Init.ExternalTrigConv =
        ADC_SOFTWARE_START;


    hadc1.Init.DataAlign =
        ADC_DATAALIGN_RIGHT;


    hadc1.Init.NbrOfConversion =
        1;


    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }


    /* ADC Channel 0 = PA0 */

    sConfig.Channel =
        ADC_CHANNEL_0;


    sConfig.Rank =
        ADC_REGULAR_RANK_1;


    sConfig.SamplingTime =
        ADC_SAMPLETIME_1CYCLE_5;


    if (HAL_ADC_ConfigChannel(&hadc1,
                              &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief USART1 Initialization Function
  * @retval None
  */

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;


    huart1.Init.BaudRate =
        115200;


    huart1.Init.WordLength =
        UART_WORDLENGTH_8B;


    huart1.Init.StopBits =
        UART_STOPBITS_1;


    huart1.Init.Parity =
        UART_PARITY_NONE;


    huart1.Init.Mode =
        UART_MODE_TX_RX;


    huart1.Init.HwFlowCtl =
        UART_HWCONTROL_NONE;


    huart1.Init.OverSampling =
        UART_OVERSAMPLING_16;


    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  * @retval None
  */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* ================= GPIO CLOCK ENABLE ================= */

    __HAL_RCC_GPIOD_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();


    /* ================= LCD INITIAL OUTPUT ================= */

    HAL_GPIO_WritePin(
        GPIOB,
        LCD_D7_Pin |
        LCD_D6_Pin |
        LCD_D5_Pin |
        LCD_D4_Pin |
        LCD_E_Pin |
        LCD_RS_Pin,
        GPIO_PIN_RESET
    );


    /* ================= LCD GPIO CONFIGURATION ================= */

    GPIO_InitStruct.Pin =
        LCD_D7_Pin |
        LCD_D6_Pin |
        LCD_D5_Pin |
        LCD_D4_Pin |
        LCD_E_Pin |
        LCD_RS_Pin;


    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;


    GPIO_InitStruct.Pull =
        GPIO_NOPULL;


    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(GPIOB,
                  &GPIO_InitStruct);


    /* ================= PC13 BACKLIGHT ================= */

    GPIO_InitStruct.Pin =
        GPIO_PIN_13;


    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;


    GPIO_InitStruct.Pull =
        GPIO_NOPULL;


    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(GPIOC,
                  &GPIO_InitStruct);


    /* Backlight ON */

    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_13,
                      GPIO_PIN_SET);
}


/**
  * @brief Error Handler
  */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif
