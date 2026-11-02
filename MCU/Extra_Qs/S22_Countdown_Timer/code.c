/* ===========================================================================
 * S22. Countdown timer
 * Scenario question from the guide (S22)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* ---- HELPER used below: paste inside USER CODE BEGIN 0 (same block if the code below already has one) ---- */
static void leds_show(uint8_t mask)          /* bit0..3 -> PD12..PD15, HAL only */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}
/* ---- end helper ---- */

/* USER CODE BEGIN PV */
volatile int16_t remaining = 0;                /* seconds left                        */
volatile uint8_t finished = 0;                 /* set when the countdown reaches 0    */
static   uint8_t toggles_left = 0;
static   uint32_t t_next = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (finished) { finished = 0; toggles_left = 12; t_next = HAL_GetTick(); }

  if (toggles_left)                                                     /* alarm blinking */
  {
    if ((int32_t)(HAL_GetTick() - t_next) >= 0)
    {
       HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
       toggles_left--;
       t_next += 250u;
       if (toggles_left == 0) leds_show(0x0);
    }
  }
  else if (remaining > 45) leds_show(0xF);
  else if (remaining > 30) leds_show(0x7);
  else if (remaining > 15) leds_show(0x3);
  else if (remaining > 0) leds_show(0x1);
  else                      leds_show(0x0);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  if (remaining > 0) { remaining--; if (remaining == 0) finished = 1; }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  remaining += 10;
  if (remaining > 60) remaining = 60;                                   /* maximum 60 s */
}
/* USER CODE END 4 */
