/* ===========================================================================
 * Q9. DMA-driven onboard LED pattern player (timer update to GPIOD BSRR)
 * Scenario question from the guide (Q9)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
/* build one BSRR word: wanted LEDs set, the other three of PD12..PD15 reset */
#define BSRR_PATTERN(m) ( ((uint32_t)(m) << 12) | ((uint32_t)((~(m)) & 0xFu) << 28) )

static const uint32_t led_pattern[] = {
   BSRR_PATTERN(0x0),                   /* all OFF                                 */
   BSRR_PATTERN(0xF),                   /* all ON                                  */
   BSRR_PATTERN(0x0),                   /* all OFF                                 */
   BSRR_PATTERN(0x1),                   /* single LED PD12                         */
   BSRR_PATTERN(0x2),                   /* PD13                                    */
   BSRR_PATTERN(0x4),                   /* PD14                                    */
   BSRR_PATTERN(0x8),                   /* PD15                                    */
   BSRR_PATTERN(0x8),                   /* identical entry: PD15 stays on          */
   BSRR_PATTERN(0x8),                   /* identical entry again                   */
   BSRR_PATTERN(0x4),                   /* back down                               */
   BSRR_PATTERN(0x2)
};
#define PATTERN_LEN (sizeof(led_pattern) / sizeof(led_pattern[0]))

volatile uint32_t dma_index   = 0;      /* current position in the table            */
volatile uint32_t dma_tc_count = 0;     /* completed passes through the table       */
volatile uint32_t uev_count   = 0;      /* timer update events seen                 */
volatile uint8_t paused       = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void pattern_xfer_cplt(DMA_HandleTypeDef *hdma)         /* transfer-complete callback */
{
  (void)hdma;
  dma_tc_count++;                                              /* one full pass finished      */
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
hdma_tim1_up.XferCpltCallback = pattern_xfer_cplt;
HAL_DMA_Start_IT(&hdma_tim1_up, (uint32_t)led_pattern,         /* source: table in Flash      */
                 (uint32_t)&GPIOD->BSRR,                       /* destination: BSRR (fixed)   */
                 PATTERN_LEN);                                 /* number of 32-bit words      */

__HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);                    /* update event -> DMA request */
HAL_TIM_Base_Start_IT(&htim1);                                   /* start timer + count events */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  /* NDTR counts down from PATTERN_LEN; index = PATTERN_LEN - NDTR. The loop does NOT touch the LEDs. */
  dma_index = PATTERN_LEN - __HAL_DMA_GET_COUNTER(&hdma_tim1_up);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) uev_count++;                  /* observation only */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)                   /* PA0: pause / resume */
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;
  uint32_t now = HAL_GetTick();
  if ((now - last) < 200u) return;                               /* debounce */
  last = now;

  paused ^= 1u;
  if (paused) HAL_TIM_Base_Stop_IT(&htim1);                      /* no update events -> no DMA steps */
  else        HAL_TIM_Base_Start_IT(&htim1);                     /* continue from the same index     */
}
/* USER CODE END 4 */
