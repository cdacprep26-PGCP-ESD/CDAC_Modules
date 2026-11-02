/* ===========================================================================
 * S14. Four LEDs with different brightness using PWM (visible output)
 * Scenario question from the guide (S14)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
static const uint16_t profile[3][4] = {
   { 100, 250, 500, 900 },
   { 900, 500, 250, 100 },
   { 500, 500, 500, 500 }
};
volatile uint8_t prof_idx = 0, prof_changed = 1;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (prof_changed)
  {
    prof_changed = 0;
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, profile[prof_idx][0]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, profile[prof_idx][1]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, profile[prof_idx][2]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, profile[prof_idx][3]);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  prof_idx = (uint8_t)((prof_idx + 1u) % 3u);
  prof_changed = 1;
}
/* USER CODE END 4 */
