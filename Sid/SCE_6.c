/* USER CODE BEGIN Header */
/**
  * Q6. Onboard accelerometer tilt safety monitor
  * Board  : STM32F407 Discovery (LIS3DSH on SPI1, CS = PE3)
  * Timer  : TIM6, PSC = 1599, ARR = 999  ->  100 ms tick (16 MHz HSI)
  * Button : PA0 (hold >= 1 s to re-zero the level reference)
  * LEDs   : PD12 (+X), PD13 (+Y), PD14 (-X), PD15 (-Y)
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
/* ---- Tuning (document your final values after observing raw readings) ---- */
#define THRESHOLD   4000        // counts away from level reference (~15 deg at +/-2 g)
#define ALL_LEDS    (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)

/* ---- Sensor data (watch these in Live Expressions over SWD) ---- */
uint8_t  TxBuf[2];
uint8_t  Return[6];
uint8_t  WhoAmI;
int16_t  Rx_x, Rx_y, Rx_z;

/* ---- Tilt logic ---- */
int16_t  refX, refY;            // level reference
int16_t  dx, dy;                // difference from level
uint8_t  tilted = 0;
uint8_t  alarm  = 0;
uint16_t tiltCount  = 0;        // consecutive ticks tilted   (30 = 3 s)
uint16_t safeCount  = 0;        // consecutive ticks safe     (20 = 2 s)
uint16_t btnCount   = 0;        // consecutive ticks PA0 held (10 = 1 s)
uint16_t blinkCount = 0;        // blink divider              (5  = 500 ms)
volatile uint8_t tick = 0;      // set every 100 ms by TIM6
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void ReadAccel(void);
/* USER CODE END PFP */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */

  /* ---- 1. WHO_AM_I check ---- */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
  TxBuf[0] = 0x0F | 0x80;                       // read WHO_AM_I
  HAL_SPI_Transmit(&hspi1, TxBuf, 1, 50);
  HAL_SPI_Receive(&hspi1, &WhoAmI, 1, 50);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

  if (WhoAmI != 0x3F)                           // LIS3DSH should return 0x3F
  {
      HAL_GPIO_WritePin(GPIOD, ALL_LEDS, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);   // error state: PD14 ON
      while (1) { }
  }

  /* ---- 2. CTRL_REG4: 25 Hz, X/Y/Z enabled ---- */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
  TxBuf[0] = 0x20;
  TxBuf[1] = 0x47;
  HAL_SPI_Transmit(&hspi1, TxBuf, 2, 50);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

  /* ---- 3. Level reference at startup ---- */
  HAL_Delay(100);                               // let first sample be ready
  ReadAccel();
  refX = Rx_x;
  refY = Rx_y;

  /* ---- 4. Start the 100 ms timer tick ---- */
  HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (tick)                                   // runs once every 100 ms
    {
        tick = 0;
        ReadAccel();

        /* ---- A. PA0 held for 1 s (10 ticks) -> new level reference ---- */
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) btnCount++;
        else btnCount = 0;

        if (btnCount == 10)
        {
            refX = Rx_x;
            refY = Rx_y;
        }

        /* ---- B. Distance from level ---- */
        dx = Rx_x - refX;
        dy = Rx_y - refY;
        tilted = (abs(dx) > THRESHOLD) || (abs(dy) > THRESHOLD);

        /* ---- C. Alarm timing ---- */
        if (alarm == 0)
        {
            if (tilted) tiltCount++;
            else        tiltCount = 0;          // must be continuous

            if (tiltCount >= 30)                // 3 s tilted -> alarm
            {
                alarm = 1;
                safeCount = 0;
                blinkCount = 0;
                HAL_GPIO_WritePin(GPIOD, ALL_LEDS, GPIO_PIN_SET);
            }
        }
        else
        {
            if (!tilted) safeCount++;
            else         safeCount = 0;         // must be continuous

            if (safeCount >= 20)                // 2 s safe -> clear alarm
            {
                alarm = 0;
                tiltCount = 0;
            }
        }

        /* ---- D. LED output ---- */
        if (alarm)
        {
            blinkCount++;
            if (blinkCount >= 5)                // toggle every 500 ms
            {
                blinkCount = 0;
                HAL_GPIO_TogglePin(GPIOD, ALL_LEDS);
            }
        }
        else
        {
            HAL_GPIO_WritePin(GPIOD, ALL_LEDS, GPIO_PIN_RESET);

            if (tilted)
            {
                if (abs(dx) >= abs(dy))         // X is dominant
                {
                    if (dx > 0) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                    else        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
                }
                else                            // Y is dominant
                {
                    if (dy > 0) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
                    else        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
                }
            }
        }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  */
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function (100 ms period)
  */
static void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1599;                  // 16 MHz / 1600 = 10 kHz
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;                      // 10 kHz / 1000 = 10 Hz
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /* PE3: accelerometer chip select */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* PA0: user button (external pull-down on the board) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PD12..PD15: LEDs */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* Read X, Y, Z (6 bytes) from LIS3DSH starting at OUT_X_L (0x28) */
void ReadAccel(void)
{
    TxBuf[0] = 0x28 | 0x80;                     // read, auto-increment
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, TxBuf, 1, 50);
    HAL_SPI_Receive(&hspi1, Return, 6, 50);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

    Rx_x = (int16_t)((Return[1] << 8) | Return[0]);
    Rx_y = (int16_t)((Return[3] << 8) | Return[2]);
    Rx_z = (int16_t)((Return[5] << 8) | Return[4]);
}

/* Called by HAL every 100 ms (TIM6 update interrupt) */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) tick = 1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
