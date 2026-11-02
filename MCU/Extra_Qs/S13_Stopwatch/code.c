/* ===========================================================================
 * S13. Stopwatch with the User button
 * Scenario question from the guide (S13)
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
typedef enum { SW_ZERO, SW_RUN, SW_STOP } sw_t;
volatile sw_t     sw = SW_ZERO;
volatile uint32_t ticks100 = 0;                     /* number of 100 ms ticks   */
volatile uint32_t elapsed_ms = 0;                   /* look at this in Live Expressions */
volatile uint8_t button_flag = 0;
/* USER CODE END PV */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (button_flag)
  {
    button_flag = 0;
    if (sw == SW_ZERO)      { sw = SW_RUN; HAL_TIM_Base_Start_IT(&htim3); }           /* press 1: start */
    else if (sw == SW_RUN) { sw = SW_STOP; HAL_TIM_Base_Stop_IT(&htim3); }            /* press 2: stop */
    else                    { sw = SW_ZERO; ticks100 = 0; leds_show(0x0); }           /* press 3: reset */
  }
  elapsed_ms = ticks100 * 100u;
  if (sw != SW_ZERO) leds_show((uint8_t)((ticks100 / 10u) & 0xFu));                   /* seconds, binary */
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) ticks100++;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  button_flag = 1;
}
/* USER CODE END 4 */
