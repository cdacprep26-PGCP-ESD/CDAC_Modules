/* ===========================================================================
 * E10. Watchdog with two health flags
 * EXTRA question (E10)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  hb_flag       = 0;      /* set by TIM3 every 100 ms            */
volatile uint8_t  btn_flag      = 0;      /* set by PA0 (operator alive)         */
volatile uint8_t  window_ticks  = 0;      /* 100 ms ticks in the current window  */
volatile uint8_t  reset_by_iwdg = 0;
volatile uint32_t refresh_count = 0, missed_windows = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
reset_by_iwdg = (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) ? 1u : 0u;   /* read first ...  */
__HAL_RCC_CLEAR_RESET_FLAGS();                                              /* ... then clear  */

if (reset_by_iwdg)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  for (int i = 0; i < 30; i++) { HAL_Delay(100); HAL_IWDG_Refresh(&hiwdg); }   /* 3 s, keep the IWDG fed */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (window_ticks >= 20u)                          /* 2 s window finished */
  {
    window_ticks = 0;
    if (hb_flag && btn_flag)
    {
      HAL_IWDG_Refresh(&hiwdg);
      refresh_count++;
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    }
    else
    {
      missed_windows++;                             /* no refresh: the IWDG will reset the chip */
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    }
    hb_flag = 0;
    btn_flag = 0;
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  hb_flag = 1;
  if (window_ticks < 255u) window_ticks++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 100u) return;
  last = HAL_GetTick();
  btn_flag = 1;
}
/* USER CODE END 4 */
