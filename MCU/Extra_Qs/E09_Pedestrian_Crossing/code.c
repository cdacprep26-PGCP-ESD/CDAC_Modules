/* ===========================================================================
 * E9. Pedestrian crossing controller
 * EXTRA question (E9)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
typedef enum { XS_GREEN = 0, XS_ORANGE, XS_WALK } xs_t;
volatile xs_t     xs          = XS_GREEN;
volatile uint16_t ticks       = 0;        /* 100 ms ticks in the current state  */
volatile uint8_t  ped_request = 0;
volatile uint32_t crossings   = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void leds_show(uint8_t mask)          /* bit0..3 -> PD12..PD15, HAL only */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static void apply_state(xs_t s)
{
  switch (s)
  {
    case XS_GREEN:  leds_show(0x1); break;       /* PD12                  */
    case XS_ORANGE: leds_show(0x2); break;       /* PD13                  */
    case XS_WALK:   leds_show(0xC); break;       /* PD14 red + PD15 walk  */
  }
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
apply_state(XS_GREEN);
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  if (ticks < 60000u) ticks++;                       /* clamp: no overflow in a long green */

  switch (xs)
  {
    case XS_GREEN:
      if (ped_request && ticks >= 50u) { xs = XS_ORANGE; ticks = 0; apply_state(xs); }
      break;
    case XS_ORANGE:
      if (ticks >= 20u) { xs = XS_WALK; ticks = 0; ped_request = 0; apply_state(xs); }
      break;
    case XS_WALK:
      if (ticks >= 50u) { xs = XS_GREEN; ticks = 0; crossings++; apply_state(xs); }
      break;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  if (xs == XS_GREEN) ped_request = 1;               /* remember the request only in GREEN */
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
