/* ===========================================================================
 * Q4. Independent LED toggle rates using output-compare interrupts
 * Scenario question from the guide (Q4)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
static const uint16_t led_pin[4] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };
static const uint32_t oc_chan[4] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };
static const uint32_t interval[4] = { 200, 400, 600, 1000 };       /* ticks = ms */
volatile uint32_t toggle_count[4];      /* how many toggles each LED made       */
volatile uint32_t period_ms[4];         /* measured time between two toggles    */
static   uint32_t last_ms[4];
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_3);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM4) return;

  uint8_t i;
  switch (htim->Channel)
  {
    case HAL_TIM_ACTIVE_CHANNEL_1: i = 0; break;
    case HAL_TIM_ACTIVE_CHANNEL_2: i = 1; break;
    case HAL_TIM_ACTIVE_CHANNEL_3: i = 2; break;
    case HAL_TIM_ACTIVE_CHANNEL_4: i = 3; break;
    default: return;
  }

  HAL_GPIO_TogglePin(GPIOD, led_pin[i]);                        /* visible toggle          */
  toggle_count[i]++;

  uint32_t now = HAL_GetTick();                                 /* measurement only        */
  period_ms[i] = now - last_ms[i];
  last_ms[i]   = now;

  uint32_t prev = __HAL_TIM_GET_COMPARE(htim, oc_chan[i]);      /* previous compare value */
  uint32_t next = (prev + interval[i]) % 65536u;                /* ARR + 1 = 65536        */
  __HAL_TIM_SET_COMPARE(htim, oc_chan[i], next);                /* schedule next match    */
}
/* USER CODE END 4 */
