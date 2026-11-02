/* ===========================================================================
 * E1. Two-tone alarm with start/stop button
 * EXTRA question (E1)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  alarm_on   = 0;       /* 1 = alarm running                  */
volatile uint8_t  tone       = 0;       /* 0 = PD12+PD14, 1 = PD13+PD15       */
volatile uint32_t tone_count = 0;       /* number of tone changes             */
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
HAL_TIM_Base_Start_IT(&htim3);          /* timer always runs; alarm_on decides */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3 || !alarm_on) return;
  tone ^= 1u;
  tone_count++;
  leds_show(tone ? 0xA : 0x5);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;   /* debounce */
  last = HAL_GetTick();

  alarm_on ^= 1u;
  if (alarm_on)
  {
    tone = 0;
    __HAL_TIM_SET_COUNTER(&htim3, 0);   /* first tone gets a full 250 ms */
    leds_show(0x5);
  }
  else
  {
    leds_show(0x0);
  }
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
