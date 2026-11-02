/* ===========================================================================
 * Q7. Internal temperature monitor using ADC and DMA
 * Scenario question from the guide (Q7)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define N_AVG        16u                    /* samples per channel to average    */
#define N_CH         2u                     /* rank1 = temperature, rank2 = VREFINT */
#define V25_TYP      0.76f                  /* volts at 25 C (typical)           */
#define SLOPE_TYP    0.0025f                /* volts per degree C (typical)      */
#define VREFINT_TYP 1.21f                   /* volts (typical)                   */
#define R1_DELTA     1.0f                   /* range thresholds, degrees C above baseline */
#define R2_DELTA     3.0f
#define HIGH_DELTA   6.0f

volatile uint16_t adc_buf[N_AVG * N_CH];    /* filled by DMA: T,V,T,V,...        */
volatile uint8_t adc_ready      = 0;        /* set by DMA half/full callbacks    */
volatile uint8_t adc_full_once = 0;         /* buffer completely filled at least once */
volatile uint32_t dma_pos = 0;              /* current DMA index in the buffer   */
volatile uint16_t avg_temp_raw, avg_vref_raw;
volatile float    vdda_v, vsense_v, temp_c; /* live values                       */
volatile float    disp_temp_c;              /* value used for the LEDs           */
volatile float    baseline_c;               /* first stable reading              */
volatile uint8_t baseline_set = 0, range_idx = 0;
volatile uint8_t frozen = 0;                /* 1 = snapshot mode (PA0)           */
volatile uint8_t vdda_ok = 1;               /* supply looks reasonable (3.0-3.6 V) */
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void set_leds(uint8_t mask)          /* bit0..3 -> PD12..PD15 */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static void process_adc(void)               /* average the latest 16 pairs and convert */
{
  uint32_t sum_t = 0, sum_v = 0;
  for (uint32_t i = 0; i < N_AVG; i++)
  {
    sum_t += adc_buf[2u * i];               /* even index: temperature sensor */
    sum_v += adc_buf[2u * i + 1u];          /* odd index : VREFINT            */
  }

    avg_temp_raw = (uint16_t)(sum_t / N_AVG);
    avg_vref_raw = (uint16_t)(sum_v / N_AVG);
    if (avg_vref_raw == 0) return;            /* avoid divide by zero */

    vdda_v   = VREFINT_TYP * 4095.0f / (float)avg_vref_raw;
    vsense_v = (float)avg_temp_raw * vdda_v / 4095.0f;
    temp_c   = (vsense_v - V25_TYP) / SLOPE_TYP + 25.0f;
    vdda_ok = (vdda_v > 3.0f && vdda_v < 3.6f);          /* reasonable-supply check */

    if (!baseline_set) { baseline_c = temp_c; baseline_set = 1; }
    if (!frozen) disp_temp_c = temp_c;                  /* frozen: keep old value */
}

static void update_leds(void)
{
  float rise = disp_temp_c - baseline_c;
  static uint32_t t_blink = 0;
  static uint8_t blink14 = 0;

    if      (rise >= HIGH_DELTA) range_idx = 3;
    else if (rise >= R2_DELTA)   range_idx = 2;
    else if (rise >= R1_DELTA)   range_idx = 1;
    else                         range_idx = 0;

    uint8_t mask = 0x1;                                  /* PD12 */
    if (range_idx >= 1) mask |= 0x2;                     /* PD13 */
    if (range_idx >= 2) mask |= 0x8;                     /* PD15 */
    if (range_idx == 3)                                  /* PD14 blinks every 250 ms */
    {
      if (HAL_GetTick() - t_blink >= 250u) { t_blink = HAL_GetTick(); blink14 ^= 1u; }
      if (blink14) mask |= 0x4;
    }
    set_leds(mask);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, N_AVG * N_CH);      /* arm ADC + DMA, wait for trigger */
HAL_TIM_Base_Start(&htim3);                                        /* TRGO every 100 ms               */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  dma_pos = N_AVG * N_CH - __HAL_DMA_GET_COUNTER(&hdma_adc1);      /* NDTR counts down */
  if (adc_ready && adc_full_once)
  {
    adc_ready = 0;
    process_adc();
  }
  if (baseline_set) update_leds();
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1) adc_ready = 1;       /* first half is new, second half older */
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1) { adc_full_once = 1; adc_ready = 1; }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)       /* PA0: live <-> frozen */
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;
  uint32_t now = HAL_GetTick();
  if ((now - last) < 200u) return;                   /* debounce */
  last = now;
  frozen ^= 1u;
  if (frozen) disp_temp_c = temp_c;                  /* take snapshot at the moment of freezing */
}
/* USER CODE END 4 */
