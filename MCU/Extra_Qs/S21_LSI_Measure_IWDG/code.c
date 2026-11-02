/* ===========================================================================
 * S21. Measure the LSI frequency and calculate the IWDG reload
 * Scenario question from the guide (S21)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t last_cap = 0, delta_ticks = 0, f_lsi_hz = 0, reload_2s = 0;
volatile uint8_t cap_state = 0;               /* 0 = discard first capture, 1 = measuring */
volatile uint8_t meas_done = 0;
static   uint32_t sum_f = 0; static uint8_t n_f = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIMEx_RemapConfig(&htim5, TIM_TIM5_LSI);              /* connect TIM5 channel 4 input to the LSI */
HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (meas_done)
  {
    meas_done = 0;
    f_lsi_hz = sum_f / 8u;                                  /* average of 8 measurements       */
    reload_2s = (2u * f_lsi_hz) / 64u - 1u;                 /* reload for ~2 s, prescaler 64   */
    HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_4);
    if (f_lsi_hz >= 28000u && f_lsi_hz <= 36000u) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    else                                          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM5 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_4) return;
  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
  if (cap_state == 0) { last_cap = now; cap_state = 1; return; }      /* first capture is only a start point */

  delta_ticks = now - last_cap;                                       /* unsigned: wrap safe */
  last_cap = now;
  if (delta_ticks != 0u) { sum_f += (8u * 16000000u) / delta_ticks; n_f++; }
  if (n_f >= 8u) meas_done = 1;
}
/* USER CODE END 4 */
