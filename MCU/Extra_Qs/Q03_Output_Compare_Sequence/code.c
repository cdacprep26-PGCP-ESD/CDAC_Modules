/* ===========================================================================
 * Q3. Four-stage sequence using output compare (TIM4)
 * Scenario question from the guide (Q3)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
static const uint16_t led_pin[4] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };
static const uint32_t oc_chan[4] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };
volatile uint32_t event_ccr[4];         /* compare value that fired (ticks)   */
volatile uint32_t event_tick_ms[4];     /* HAL_GetTick() based check          */
volatile uint8_t events_done = 0;       /* bit0..bit3 = event 1..4 happened   */
uint32_t t_start_ms;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
t_start_ms = HAL_GetTick();
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);   /* event at 250 ms */
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);   /* event at 500 ms */
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_3);   /* event at 750 ms */
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);   /* event at 1000 ms */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM4) return;

  uint8_t i;
  switch (htim->Channel)                        /* which channel matched? */
  {
    case HAL_TIM_ACTIVE_CHANNEL_1: i = 0; break;
    case HAL_TIM_ACTIVE_CHANNEL_2: i = 1; break;
    case HAL_TIM_ACTIVE_CHANNEL_3: i = 2; break;
    case HAL_TIM_ACTIVE_CHANNEL_4: i = 3; break;
    default: return;
  }

  HAL_GPIO_WritePin(GPIOD, led_pin[i], GPIO_PIN_SET);       /* LED on and stays on */
  event_ccr[i]     = __HAL_TIM_GET_COMPARE(htim, oc_chan[i]);
  event_tick_ms[i] = HAL_GetTick() - t_start_ms;            /* about 250/500/750/1000 */
  events_done     |= (uint8_t)(1u << i);

  /* one-shot: stop only this channel's interrupt. Do NOT use HAL_TIM_OC_Stop_IT():
     it also stops the whole timer counter. TIM_IT_CC1 << i is CC1IE..CC4IE.        */
  __HAL_TIM_DISABLE_IT(htim, (TIM_IT_CC1 << i));
}
/* USER CODE END 4 */
