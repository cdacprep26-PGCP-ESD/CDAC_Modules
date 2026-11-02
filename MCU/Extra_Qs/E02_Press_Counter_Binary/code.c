/* ===========================================================================
 * E2. Debounced press counter (0-15 in binary)
 * EXTRA question (E2)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  count    = 0;         /* 0..15, shown on the LEDs            */
volatile uint32_t presses  = 0;         /* all accepted presses (no wrap)      */
volatile uint32_t ignored  = 0;         /* edges rejected as bounce            */
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
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;

  uint32_t now = HAL_GetTick();
  if ((now - last) < 100u) { ignored++; return; }       /* bounce */
  last = now;

  presses++;
  count = (uint8_t)((count + 1u) & 0xFu);               /* wraps 15 -> 0 */
  leds_show(count);
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
