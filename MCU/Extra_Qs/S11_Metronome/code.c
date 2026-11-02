/* ===========================================================================
 * S11. Metronome with selectable tempo
 * Scenario question from the guide (S11)
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
static const uint16_t beat_ticks[3] = { 1000, 667, 500 };                 /* ms per beat */
static const uint8_t tempo_led[3] = { 0x2, 0x4, 0x8 };                    /* PD13, PD14, PD15 */
volatile uint8_t tempo_idx = 0, tempo_changed = 1, flash_on = 0;
volatile uint32_t beat_count = 0, flash_off_at = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (tempo_changed)                                       /* update the tempo indicator in main */
  {
    tempo_changed = 0;
    leds_show(tempo_led[tempo_idx]);
  }
  if (flash_on && (int32_t)(HAL_GetTick() - flash_off_at) >= 0)     /* beat flash lasts 50 ms */
  {
    flash_on = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)          /* once per beat */
{
  if (htim->Instance != TIM3) return;
  beat_count++;
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  flash_off_at = HAL_GetTick() + 50u;

    flash_on = 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;   /* debounce */
  last = HAL_GetTick();
  tempo_idx = (uint8_t)((tempo_idx + 1u) % 3u);
  __HAL_TIM_SET_AUTORELOAD(&htim3, beat_ticks[tempo_idx] - 1u);     /* new beat period         */
  __HAL_TIM_SET_COUNTER(&htim3, 0);                                 /* restart the beat now    */
  tempo_changed = 1;
}
/* USER CODE END 4 */

/* leds_show() from the helper above must be in USER CODE BEGIN 0 */
