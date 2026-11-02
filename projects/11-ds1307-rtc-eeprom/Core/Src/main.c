/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BL_ON  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
#define BL_OFF HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)

/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;


/* USER CODE BEGIN PV */

volatile int sec;
volatile int minute;
volatile int hour;

volatile int day;
volatile int month;
volatile int year;
volatile int dow;

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);


/* USER CODE BEGIN PFP */

void lcd_ins(unsigned char c);
void lcd_putch(char c);
void lcd_clear(void);
void lcd_puts(const char *s);
void lcd_goto(int pos);
void lcd_init(void);

unsigned char b2d(unsigned char d);
unsigned char d2b(unsigned char d);

void startRTC(void);
uint8_t RTC_Check(void);
uint8_t RTC_IsConnected(void);

void writeRTC(void);
HAL_StatusTypeDef readRTC(void);

const char* getDayName(uint8_t day);

void LCD_Display_DateTime(void);

void readMemory(void);
void writeMemory(void);
void initCount(void);

void UART_Receive_String(char *buffer, uint16_t size);
void LCD_Display_UART_String(char *str);

/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* ============================================================
   BCD TO DECIMAL
   ============================================================ */

unsigned char b2d(unsigned char d)
{
    return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}


/* ============================================================
   DECIMAL TO BCD
   ============================================================ */

unsigned char d2b(unsigned char d)
{
    return (((d / 10) << 4) & 0xF0) | ((d % 10) & 0x0F);
}


/* ============================================================
   START RTC
   ============================================================ */

void startRTC(void)
{
    uint8_t ss;

    HAL_I2C_Mem_Read(&hi2c2,
                     0xD0,
                     0x00,
                     I2C_MEMADD_SIZE_8BIT,
                     &ss,
                     1,
                     500);

    /* Clear CH bit */
    ss = ss & 0x7F;

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x00,
                      I2C_MEMADD_SIZE_8BIT,
                      &ss,
                      1,
                      500);
}


/* ============================================================
   RTC CHECK
   ============================================================ */

uint8_t RTC_Check(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c2,
                              0xD0,
                              3,
                              100) == HAL_OK)
    {
        return 1;
    }

    return 0;
}


/* ============================================================
   RTC WRITE
   ============================================================ */

void writeRTC(void)
{
    uint8_t dd;
    uint8_t mm;
    uint8_t yy;
    uint8_t hh;
    uint8_t mts;
    uint8_t ss;
    uint8_t day_of_week;


    dd = d2b(day);

    mm = d2b(month);


    /* Convert year to 00-99 */

    yy = year;

    if (yy >= 2000)
    {
        yy = yy - 2000;
    }

    yy = d2b(yy);


    hh  = d2b(hour);
    mts = d2b(minute);
    ss  = d2b(sec);


    /* Day of week must be 1-7 */

    day_of_week = (uint8_t)dow;


    /* Seconds */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x00,
                      I2C_MEMADD_SIZE_8BIT,
                      &ss,
                      1,
                      500);


    /* Minutes */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x01,
                      I2C_MEMADD_SIZE_8BIT,
                      &mts,
                      1,
                      500);


    /* Hours */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x02,
                      I2C_MEMADD_SIZE_8BIT,
                      &hh,
                      1,
                      500);


    /* Day of week */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x03,
                      I2C_MEMADD_SIZE_8BIT,
                      &day_of_week,
                      1,
                      500);


    /* Date */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x04,
                      I2C_MEMADD_SIZE_8BIT,
                      &dd,
                      1,
                      500);


    /* Month */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x05,
                      I2C_MEMADD_SIZE_8BIT,
                      &mm,
                      1,
                      500);


    /* Year */

    HAL_I2C_Mem_Write(&hi2c2,
                      0xD0,
                      0x06,
                      I2C_MEMADD_SIZE_8BIT,
                      &yy,
                      1,
                      500);
}


/* ============================================================
   RTC READ
   ============================================================ */

HAL_StatusTypeDef readRTC(void)
{
    uint8_t data[7];

    HAL_StatusTypeDef status;


    status = HAL_I2C_Mem_Read(&hi2c2,
                              0xD0,
                              0x00,
                              I2C_MEMADD_SIZE_8BIT,
                              data,
                              7,
                              500);


    if (status != HAL_OK)
    {
        return status;
    }


    /* Seconds */

    sec = b2d(data[0] & 0x7F);


    /* Minutes */

    minute = b2d(data[1] & 0x7F);


    /* Hours */

    hour = b2d(data[2] & 0x3F);


    /* Day of week */

    dow = data[3] & 0x07;


    /* Date */

    day = b2d(data[4] & 0x3F);


    /* Month */

    month = b2d(data[5] & 0x1F);


    /* Year */

    year = b2d(data[6]) + 2000;


    return HAL_OK;
}


/* ============================================================
   DAY NAME
   ============================================================ */

const char* getDayName(uint8_t day)
{
    switch(day)
    {
        case 1:
            return "SUN";

        case 2:
            return "MON";

        case 3:
            return "TUE";

        case 4:
            return "WED";

        case 5:
            return "THU";

        case 6:
            return "FRI";

        case 7:
            return "SAT";

        default:
            return "---";
    }
}


/* ============================================================
   RTC CONNECTION CHECK
   ============================================================ */

uint8_t RTC_IsConnected(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c2,
                              0xD0,
                              3,
                              100) == HAL_OK)
    {
        return 1;
    }

    return 0;
}


/* ============================================================
   LCD DISPLAY DATE + TIME
   ============================================================ */

void LCD_Display_DateTime(void)
{
    char line1[17];
    char line2[17];


    /*
       First line:
       DD/MM/YYYY DAY

       Example:
       24/09/2026 THU
    */

    snprintf(line1,
             sizeof(line1),
             "%02d/%02d/%04d %s",
             day,
             month,
             year,
             getDayName(dow));


    /*
       Second line:
       HH:MM:SS

       Example:
       15:09:00
    */

    snprintf(line2,
             sizeof(line2),
             "%02d:%02d:%02d",
             hour,
             minute,
             sec);


    /* Clear LCD */

    lcd_clear();


    /* First line */

    lcd_goto(0);

    lcd_puts(line1);


    /* Second line */

    lcd_goto(0x40);

    lcd_puts(line2);
}


/* ============================================================
   EEPROM SETTINGS
   ============================================================ */

struct SETTING
{
    int count;
};

struct SETTING settings;


/* ============================================================
   EEPROM READ
   ============================================================ */

void readMemory(void)
{
    HAL_I2C_Mem_Read(&hi2c2,
                     0xA0,
                     0,
                     I2C_MEMADD_SIZE_16BIT,
                     (uint8_t*)&settings,
                     sizeof(settings),
                     50);
}


/* ============================================================
   EEPROM WRITE
   ============================================================ */

void writeMemory(void)
{
    HAL_I2C_Mem_Write(&hi2c2,
                      0xA0,
                      0,
                      I2C_MEMADD_SIZE_16BIT,
                      (uint8_t*)&settings,
                      sizeof(settings),
                      50);
}


/* ============================================================
   EEPROM COUNT INITIALIZATION
   ============================================================ */

void initCount(void)
{
    settings.count = 0;

    writeMemory();
}


/* ============================================================
   LCD COMMAND
   4-BIT MODE
   ============================================================ */

void lcd_ins(unsigned char c)
{
    /* RS = 0 for command */

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port,
                      LCD_RS_Pin,
                      GPIO_PIN_RESET);


    /* Higher nibble */

    HAL_GPIO_WritePin(LCD_D0_GPIO_Port,
                      LCD_D0_Pin,
                      (c & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D1_GPIO_Port,
                      LCD_D1_Pin,
                      (c & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D2_GPIO_Port,
                      LCD_D2_Pin,
                      (c & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D3_GPIO_Port,
                      LCD_D3_Pin,
                      (c & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);


    /* Enable */

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_RESET);


    /* Lower nibble */

    HAL_GPIO_WritePin(LCD_D0_GPIO_Port,
                      LCD_D0_Pin,
                      (c & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D1_GPIO_Port,
                      LCD_D1_Pin,
                      (c & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D2_GPIO_Port,
                      LCD_D2_Pin,
                      (c & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D3_GPIO_Port,
                      LCD_D3_Pin,
                      (c & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);


    /* Enable */

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_RESET);
}


/* ============================================================
   LCD CHARACTER
   ============================================================ */

void lcd_putch(char c)
{
    /* RS = 1 for data */

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port,
                      LCD_RS_Pin,
                      GPIO_PIN_SET);


    /* Higher nibble */

    HAL_GPIO_WritePin(LCD_D0_GPIO_Port,
                      LCD_D0_Pin,
                      (c & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D1_GPIO_Port,
                      LCD_D1_Pin,
                      (c & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D2_GPIO_Port,
                      LCD_D2_Pin,
                      (c & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D3_GPIO_Port,
                      LCD_D3_Pin,
                      (c & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);


    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_RESET);


    /* Lower nibble */

    HAL_GPIO_WritePin(LCD_D0_GPIO_Port,
                      LCD_D0_Pin,
                      (c & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D1_GPIO_Port,
                      LCD_D1_Pin,
                      (c & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D2_GPIO_Port,
                      LCD_D2_Pin,
                      (c & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D3_GPIO_Port,
                      LCD_D3_Pin,
                      (c & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);


    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(LCD_E_GPIO_Port,
                      LCD_E_Pin,
                      GPIO_PIN_RESET);
}


/* ============================================================
   LCD CLEAR
   ============================================================ */

void lcd_clear(void)
{
    lcd_ins(0x01);

    HAL_Delay(2);
}


/* ============================================================
   LCD STRING
   ============================================================ */

void lcd_puts(const char *s)
{
    while (*s)
    {
        lcd_putch(*s);

        s++;

        HAL_Delay(1);
    }
}


/* ============================================================
   LCD GOTO
   ============================================================ */

void lcd_goto(int pos)
{
    lcd_ins(0x80 + pos);
}


/* ============================================================
   LCD INITIALIZATION
   ============================================================ */

void lcd_init(void)
{
    HAL_Delay(20);

    lcd_ins(0x03);

    HAL_Delay(5);

    lcd_ins(0x03);

    HAL_Delay(5);

    lcd_ins(0x03);

    HAL_Delay(1);

    lcd_ins(0x02);

    HAL_Delay(1);

    /* 4-bit, 2-line, 5x7 font */

    lcd_ins(0x28);

    /* Display ON, cursor OFF, blink OFF */

    lcd_ins(0x0C);

    /* Clear */

    lcd_ins(0x01);

    HAL_Delay(2);

    /* Entry mode */

    lcd_ins(0x06);
}


/* ============================================================
   UART STRING RECEIVE
   ============================================================ */

void UART_Receive_String(char *buffer, uint16_t size)
{
    uint16_t i = 0;

    uint8_t ch;


    /* Wait for first actual character */

    while (1)
    {
        HAL_UART_Receive(&huart1,
                         &ch,
                         1,
                         HAL_MAX_DELAY);

        if (ch != '\r' && ch != '\n')
        {
            break;
        }
    }


    /* Receive until ENTER */

    while (i < size - 1)
    {
        buffer[i++] = ch;


        HAL_UART_Receive(&huart1,
                         &ch,
                         1,
                         HAL_MAX_DELAY);


        if (ch == '\r' || ch == '\n')
        {
            break;
        }
    }


    /* Null terminate */

    buffer[i] = '\0';


    /* Flush extra characters */

    if (ch != '\r' && ch != '\n')
    {
        while (1)
        {
            HAL_UART_Receive(&huart1,
                             &ch,
                             1,
                             HAL_MAX_DELAY);

            if (ch == '\r' || ch == '\n')
            {
                break;
            }
        }
    }
}


/* ============================================================
   DISPLAY UART STRING ON LCD
   ============================================================ */

void LCD_Display_UART_String(char *str)
{
    uint8_t i = 0;


    lcd_clear();


    /* First line */

    lcd_goto(0);

    while (str[i] != '\0' && i < 16)
    {
        lcd_putch(str[i]);

        i++;
    }


    /* Second line */

    lcd_goto(0x40);

    while (str[i] != '\0' && i < 32)
    {
        lcd_putch(str[i]);

        i++;
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

    char str[128];

    /* Avoid unused-variable warning */
    (void)str;

    /* USER CODE END 1 */


    /* MCU Configuration */

    HAL_Init();


    /* Configure system clock */

    SystemClock_Config();


    /* Initialize peripherals */

    MX_GPIO_Init();

    MX_USART1_UART_Init();

    MX_I2C2_Init();


    /* USER CODE BEGIN 2 */


    /* ========================================================
       LCD INITIALIZATION
       ======================================================== */

    lcd_init();

    lcd_ins(0x0C);


    /* ========================================================
       LCD BACKLIGHT ON
       ======================================================== */

    BL_ON;


    /* ========================================================
       START RTC
       ======================================================== */

    startRTC();


    /* ========================================================
       CHECK RTC
       ======================================================== */

    if (RTC_IsConnected())
    {
        lcd_clear();

        lcd_goto(0);

        lcd_puts("RTC CONNECTED");

        HAL_Delay(1000);
    }
    else
    {
        lcd_clear();

        lcd_goto(0);

        lcd_puts("RTC NOT FOUND");

        HAL_Delay(2000);
    }


    /* ========================================================
       READ RTC
       ======================================================== */

    readRTC();


    /* ========================================================
       EEPROM
       ======================================================== */

    readMemory();

    settings.count++;

    writeMemory();


    /* USER CODE END 2 */


    /* Infinite loop */

    while (1)
    {
        /* ====================================================
           KEY1
           RED LED + BUZZER + RESET EEPROM COUNT
           ==================================================== */

        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port,
                             KEY1_Pin) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(RED_GPIO_Port,
                              RED_Pin,
                              GPIO_PIN_SET);

            HAL_GPIO_WritePin(BUZZ_GPIO_Port,
                              BUZZ_Pin,
                              GPIO_PIN_SET);


            settings.count = 0;

            writeMemory();


            HAL_UART_Transmit(&huart1,
                              (uint8_t*)"COUNT RESET\r\n",
                              strlen("COUNT RESET\r\n"),
                              500);
        }
        else
        {
            HAL_GPIO_WritePin(RED_GPIO_Port,
                              RED_Pin,
                              GPIO_PIN_RESET);

            HAL_GPIO_WritePin(BUZZ_GPIO_Port,
                              BUZZ_Pin,
                              GPIO_PIN_RESET);
        }


        /* ====================================================
           KEY2
           GREEN LED + SET RTC
           ==================================================== */

        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,
                             KEY2_Pin) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GREEN_GPIO_Port,
                              GREEN_Pin,
                              GPIO_PIN_SET);


            /*
               RTC SETTING

               24 September 2026
               15:09:00
               Thursday = 5
            */

            year   = 2026;
            month  = 9;
            day    = 24;

            hour   = 15;
            minute = 9;
            sec    = 0;

            dow    = 5;


            writeRTC();


            HAL_UART_Transmit(&huart1,
                              (uint8_t*)"RTC SET\r\n",
                              strlen("RTC SET\r\n"),
                              500);


            HAL_Delay(300);
        }
        else
        {
            HAL_GPIO_WritePin(GREEN_GPIO_Port,
                              GREEN_Pin,
                              GPIO_PIN_RESET);
        }


        /* ====================================================
           READ RTC
           ==================================================== */

        if (readRTC() == HAL_OK)
        {
            LCD_Display_DateTime();
        }
        else
        {
            /* RTC communication failed */

            lcd_clear();

            lcd_goto(0);

            lcd_puts("RTC READ ERROR");


            HAL_UART_Transmit(&huart1,
                              (uint8_t*)"RTC READ ERROR\r\n",
                              strlen("RTC READ ERROR\r\n"),
                              500);
        }


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


    /* CPU, AHB and APB clocks */

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
}


/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */

static void MX_I2C2_Init(void)
{
    hi2c2.Instance = I2C2;

    hi2c2.Init.ClockSpeed = 100000;

    hi2c2.Init.DutyCycle =
        I2C_DUTYCYCLE_2;

    hi2c2.Init.OwnAddress1 = 0;

    hi2c2.Init.AddressingMode =
        I2C_ADDRESSINGMODE_7BIT;

    hi2c2.Init.DualAddressMode =
        I2C_DUALADDRESS_DISABLE;

    hi2c2.Init.OwnAddress2 = 0;

    hi2c2.Init.GeneralCallMode =
        I2C_GENERALCALL_DISABLE;

    hi2c2.Init.NoStretchMode =
        I2C_NOSTRETCH_DISABLE;


    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200;

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
  * @param None
  * @retval None
  */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* GPIO Ports Clock Enable */

    __HAL_RCC_GPIOC_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();


    /* ========================================================
       INITIAL OUTPUT STATES
       ======================================================== */

    /* LCD Backlight OFF initially */

    HAL_GPIO_WritePin(LCD_BL_GPIO_Port,
                      LCD_BL_Pin,
                      GPIO_PIN_RESET);


    /* LCD + BUZZER + GREEN OFF */

    HAL_GPIO_WritePin(GPIOB,
                      BUZZ_Pin |
                      GREEN_Pin |
                      LCD_D3_Pin |
                      LCD_D2_Pin |
                      LCD_D1_Pin |
                      LCD_D0_Pin |
                      LCD_E_Pin |
                      LCD_RS_Pin,
                      GPIO_PIN_RESET);


    /* RED OFF */

    HAL_GPIO_WritePin(RED_GPIO_Port,
                      RED_Pin,
                      GPIO_PIN_RESET);


    /* ========================================================
       LCD BACKLIGHT PIN
       ======================================================== */

    GPIO_InitStruct.Pin =
        LCD_BL_Pin;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(LCD_BL_GPIO_Port,
                  &GPIO_InitStruct);


    /* ========================================================
       LCD + BUZZER + GREEN
       ======================================================== */

    GPIO_InitStruct.Pin =
        BUZZ_Pin |
        GREEN_Pin |
        LCD_D3_Pin |
        LCD_D2_Pin |
        LCD_D1_Pin |
        LCD_D0_Pin |
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


    /* ========================================================
       KEY2
       ======================================================== */

    GPIO_InitStruct.Pin =
        KEY2_Pin;

    GPIO_InitStruct.Mode =
        GPIO_MODE_INPUT;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;


    HAL_GPIO_Init(KEY2_GPIO_Port,
                  &GPIO_InitStruct);


    /* ========================================================
       KEY1
       ======================================================== */

    GPIO_InitStruct.Pin =
        KEY1_Pin;

    GPIO_InitStruct.Mode =
        GPIO_MODE_INPUT;

    GPIO_InitStruct.Pull =
        GPIO_PULLUP;


    HAL_GPIO_Init(KEY1_GPIO_Port,
                  &GPIO_InitStruct);


    /* ========================================================
       RED LED
       ======================================================== */

    GPIO_InitStruct.Pin =
        RED_Pin;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(RED_GPIO_Port,
                  &GPIO_InitStruct);
}


/* USER CODE BEGIN 4 */


/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */

void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */

    /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
