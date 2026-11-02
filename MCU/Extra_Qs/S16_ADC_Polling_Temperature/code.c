/* ===========================================================================
 * S16. Simple temperature indicator with single-conversion ADC (polling)
 * Scenario question from the guide (S16)
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
volatile uint16_t adc_raw = 0, adc_avg = 0;
volatile float    temp_c = 0.0f, baseline_c = 0.0f;
volatile uint8_t baseline_set = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static uint16_t adc_read_once(void)
{
  HAL_ADC_Start(&hadc1);                                   /* start one conversion         */
  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)    /* wait for end of conversion   */
    adc_raw = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  return adc_raw;
}
/* leds_show() helper from the start of this chapter must also be here */
/* USER CODE END 0 */

/* USER CODE BEGIN WHILE */
while (1)
{
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) sum += adc_read_once();     /* 8 readings, 32-bit sum        */
  adc_avg = (uint16_t)(sum / 8u);

  float vsense = (float)adc_avg * 3.3f / 4095.0f;
  temp_c = (vsense - 0.76f) / 0.0025f + 25.0f;
  if (!baseline_set) { baseline_c = temp_c; baseline_set = 1; }

  float rise = temp_c - baseline_c;
  if      (rise >= 6.0f) leds_show(0xF);
  else if (rise >= 3.0f) leds_show(0x7);
  else if (rise >= 1.0f) leds_show(0x3);
  else                   leds_show(0x1);

  HAL_Delay(500);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */
