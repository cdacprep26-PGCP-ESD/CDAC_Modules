/* ===========================================================================
 * E7. Low-power blinker (Sleep mode between timer ticks)
 * EXTRA question (E7)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint32_t tick_cnt   = 0;       /* counts 100 ms ticks 0..10            */
volatile uint32_t wake_count = 0;       /* total number of wake-ups             */
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_DBGMCU_EnableDBGSleepMode();        /* keep the debugger connected in Sleep */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  HAL_SuspendTick();                                              /* SysTick would wake the CPU every ms */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);   /* sleep until an interrupt            */
  HAL_ResumeTick();                                               /* awake again                         */
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  wake_count++;
  tick_cnt++;
  if (tick_cnt >= 10u) { tick_cnt = 0; HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); }   /* ON  */
  else if (tick_cnt == 1u)             HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);   /* OFF */
}
/* USER CODE END 4 */
