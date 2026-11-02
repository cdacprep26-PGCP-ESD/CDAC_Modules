/* ===========================================================================
 * Q5. Measure the user-button interval using input capture (TIM2 CH1)
 * Scenario question from the guide (Q5)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define DEBOUNCE_MS 50u
volatile uint32_t t1 = 0, t2 = 0;
volatile uint32_t interval_ms = 0;
volatile uint8_t capture_state = 0;        /* 0 = waiting 1st press, 1 = waiting 2nd press */
volatile uint8_t result_ready = 0;

volatile uint32_t valid_edges = 0, ignored_edges = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static uint32_t elapsed_ticks(uint32_t a, uint32_t b, uint32_t arr)
{
  /* ARR + 1 overflows to 0 in uint32 when ARR = 0xFFFFFFFF, which still gives the right answer */
  return (b >= a) ? (b - a) : (arr + 1u - a + b);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);             /* start capture with interrupt */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (result_ready)
  {
    result_ready = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);        /* blink PD15 briefly */
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  static uint32_t last_edge = 0;
  static uint8_t have_edge = 0;

  if (htim->Instance != TIM2 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) return;

  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);      /* hardware time stamp */

  if (have_edge && elapsed_ticks(last_edge, now, 0xFFFFFFFFu) < DEBOUNCE_MS)
  {
    ignored_edges++;                    /* bounce: ignore */
    return;
  }
  last_edge = now;
  have_edge = 1;
  valid_edges++;

  if (capture_state == 0)               /* first press */
  {
    t1 = now;
    capture_state = 1;
  }
  else                                  /* second press */
  {
    t2 = now;
    interval_ms = elapsed_ticks(t1, t2, 0xFFFFFFFFu);
    result_ready = 1;
    capture_state = 0;
  }
}
/* USER CODE END 4 */
