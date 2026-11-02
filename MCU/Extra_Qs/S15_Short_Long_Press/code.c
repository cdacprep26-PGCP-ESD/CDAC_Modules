/* ===========================================================================
 * S15. Short press and long press detection
 * Scenario question from the guide (S15)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t press_ms = 0;               /* last accepted press length            */
volatile uint32_t short_count = 0, long_count = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t t_down = 0, last_edge = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;

  uint32_t now = HAL_GetTick();
  if ((now - last_edge) < 10u) return;                       /* ignore edges closer than 10 ms */
  last_edge = now;

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)   /* pin is high: button pressed    */
  {
    t_down = now;
  }
  else                                                       /* pin is low: button released    */
  {
    uint32_t len = now - t_down;
    if (len < 30u) return;                                   /* bounce                         */
    press_ms = len;
    if (len >= 1000u) { long_count++; HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); }
    else              { short_count++; HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); }
  }

}
/* USER CODE END 4 */
/* main loop: while (1) { } - everything is handled by the interrupt */
