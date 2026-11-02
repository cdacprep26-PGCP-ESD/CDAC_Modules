/* ===========================================================================
 * E3. Sunrise: PWM fade from 0 % to 100 % in exactly 10 s
 * EXTRA question (E3)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define FADE_TICKS  1000u               /* 1000 x 10 ms = 10 s                  */
volatile uint16_t ccr       = 0;        /* mirror of CCR1                       */
volatile uint32_t ticks     = 0;        /* step timer ticks so far              */
volatile uint8_t  fade_done = 0;
volatile uint8_t  duty_pct  = 0;        /* for the debugger                     */
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);          /* 1 kHz PWM on PD12            */
HAL_TIM_Base_Start_IT(&htim3);                     /* 10 ms step timer             */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;

  if (ticks < FADE_TICKS)
  {
    ticks++;
    ccr = (uint16_t)ticks;                                   /* +1 per tick       */
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, ccr);
    duty_pct = (uint8_t)(ccr / 10u);
  }
  else if (!fade_done)
  {
    fade_done = 1;
    HAL_TIM_Base_Stop_IT(&htim3);                            /* stay at 100 %     */
  }
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
