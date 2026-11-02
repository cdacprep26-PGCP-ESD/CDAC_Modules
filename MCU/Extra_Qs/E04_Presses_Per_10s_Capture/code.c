/* ===========================================================================
 * E4. Button presses per 10 s using input capture
 * EXTRA question (E4)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define DEBOUNCE_MS 50u
volatile uint32_t presses_now  = 0;     /* count inside the current window      */
volatile uint32_t presses_last = 0;     /* result of the last finished window   */
volatile uint32_t windows_done = 0;
volatile uint32_t ignored_edges = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void leds_show(uint8_t mask)          /* bit0..3 -> PD12..PD15, HAL only */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
leds_show(0x0);
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);     /* capture the button edges   */
HAL_TIM_Base_Start_IT(&htim3);                  /* 10 s measuring window      */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  static uint32_t last_edge = 0;
  static uint8_t  have_edge = 0;

  if (htim->Instance != TIM2 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) return;

  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
  if (have_edge && (now - last_edge) < DEBOUNCE_MS) { ignored_edges++; return; }
  last_edge = now;
  have_edge = 1;
  presses_now++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;             /* TIM2 base interrupt also lands here */

  presses_last = presses_now;
  presses_now  = 0;
  windows_done++;
  leds_show((uint8_t)((presses_last > 15u) ? 15u : presses_last));
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
