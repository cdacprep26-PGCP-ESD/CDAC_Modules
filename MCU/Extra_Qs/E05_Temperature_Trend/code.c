/* ===========================================================================
 * E5. Temperature trend indicator (rising / steady / falling)
 * EXTRA question (E5)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define N_LOG     10u
#define STEADY_C  0.5f
volatile float    temp_log[N_LOG];
volatile uint8_t  log_head  = 0;        /* next slot to write                   */
volatile uint8_t  log_count = 0;        /* how many slots hold real data        */
volatile float    temp_c    = 0.0f, trend_c = 0.0f;
volatile uint8_t  trend     = 0;        /* 0 steady, 1 rising, 2 falling        */
volatile uint8_t  sample_due = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void leds_show(uint8_t mask)          /* bit0..3 -> PD12..PD15, HAL only */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static uint16_t adc_read_once(void)
{
  uint16_t v = 0;
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) v = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  return v;
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
leds_show(0x0);
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (sample_due)
  {
    sample_due = 0;

    uint32_t sum = 0;
    for (int i = 0; i < 8; i++) sum += adc_read_once();
    float raw = (float)(sum / 8u);
    temp_c = ((raw * 3.3f / 4095.0f) - 0.76f) / 0.0025f + 25.0f;

    temp_log[log_head] = temp_c;                         /* store            */
    log_head = (uint8_t)((log_head + 1u) % N_LOG);
    if (log_count < N_LOG) log_count++;

    if (log_count == N_LOG)                              /* buffer is full   */
    {
      float oldest = temp_log[log_head];                 /* next to overwrite = oldest */
      float newest = temp_log[(log_head + N_LOG - 1u) % N_LOG];
      trend_c = newest - oldest;

      if      (trend_c >  STEADY_C) { trend = 1; leds_show(0x1); }   /* PD12 rising  */
      else if (trend_c < -STEADY_C) { trend = 2; leds_show(0x4); }   /* PD14 falling */
      else                          { trend = 0; leds_show(0x2); }   /* PD13 steady  */
    }
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) sample_due = 1;
}
/* USER CODE END 4 */
