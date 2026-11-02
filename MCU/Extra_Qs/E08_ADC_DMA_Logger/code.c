/* ===========================================================================
 * E8. ADC + DMA temperature data logger (average, minimum, maximum)
 * EXTRA question (E8)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define N_SMP 64u
volatile uint16_t smp[N_SMP];           /* filled by DMA                          */
volatile uint8_t  buf_ready = 0;
volatile uint16_t raw_min = 0, raw_max = 0, raw_avg = 0;
volatile float    t_min = 0.0f, t_max = 0.0f, t_avg = 0.0f;
volatile uint32_t buffers_done = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static float raw_to_c(uint16_t raw)
{
  float v = (float)raw * 3.3f / 4095.0f;
  return (v - 0.76f) / 0.0025f + 25.0f;
}

static void analyse(void)
{
  uint32_t sum = 0;
  uint16_t mn = 4095u, mx = 0u;
  for (uint32_t i = 0; i < N_SMP; i++)
  {
    uint16_t s = smp[i];
    sum += s;
    if (s < mn) mn = s;
    if (s > mx) mx = s;
  }
  raw_avg = (uint16_t)(sum / N_SMP);
  raw_min = mn;
  raw_max = mx;
  t_avg = raw_to_c(raw_avg);
  t_min = raw_to_c(mn);
  t_max = raw_to_c(mx);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_DMA(&hadc1, (uint32_t *)smp, N_SMP);   /* arm ADC + DMA, wait for trigger */
HAL_TIM_Base_Start(&htim3);                          /* TRGO every 50 ms                */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (buf_ready)
  {
    buf_ready = 0;
    analyse();
    buffers_done++;
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);          /* heartbeat: one toggle per 3.2 s */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)   /* buffer completely filled */
{
  if (hadc->Instance == ADC1) buf_ready = 1;
}
/* USER CODE END 4 */
