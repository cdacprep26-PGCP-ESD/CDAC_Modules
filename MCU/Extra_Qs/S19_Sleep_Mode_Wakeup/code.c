/* ===========================================================================
 * S19. Sleep mode until the button is pressed
 * Scenario question from the guide (S19)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t wake_count = 0;
volatile uint8_t woke = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_DBGMCU_EnableDBGSleepMode();                  /* keep the debugger connected during Sleep */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);          /* awake */
  HAL_Delay(2000);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  woke = 0;
  HAL_SuspendTick();                                         /* stop SysTick, or it wakes the CPU every ms */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);   /* sleep until an interrupt */
  HAL_ResumeTick();                                          /* back awake */

  if (woke)
  {
    wake_count++;
    for (int i = 0; i < 6; i++) { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); HAL_Delay(150); }        /* 3 blinks */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0) woke = 1;                         /* the interrupt itself wakes the core */
}
/* USER CODE END 4 */
