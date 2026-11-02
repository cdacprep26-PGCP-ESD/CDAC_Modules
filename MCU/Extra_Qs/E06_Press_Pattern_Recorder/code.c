/* ===========================================================================
 * E6. Press-pattern recorder and replay
 * EXTRA question (E6)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
typedef enum { ST_REC = 0, ST_REPLAY = 1 } rec_state_t;
volatile rec_state_t rstate    = ST_REC;
volatile uint8_t     code      = 0;      /* recorded bits, bit0 = first press   */
volatile uint8_t     n_rec     = 0;      /* presses recorded so far (0..4)      */
volatile uint8_t     replay_idx = 0;
volatile uint32_t    last_len  = 0;      /* last accepted press length in ms    */
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
  static uint32_t t_down = 0, last_edge = 0;
  if (GPIO_Pin != GPIO_PIN_0 || rstate != ST_REC) return;     /* ignore during replay */

  uint32_t now = HAL_GetTick();
  if ((now - last_edge) < 10u) return;                        /* ignore edges closer than 10 ms */
  last_edge = now;

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)    /* pressed  */
  {
    t_down = now;
  }
  else                                                        /* released */
  {
    uint32_t len = now - t_down;
    if (len < 30u) return;                                    /* bounce   */
    last_len = len;

    uint8_t bit = (len >= 1000u) ? 1u : 0u;
    code |= (uint8_t)(bit << n_rec);
    n_rec++;
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);                   /* feedback */

    if (n_rec >= 4u)                                          /* code complete: start replay */
    {
      rstate = ST_REPLAY;
      replay_idx = 0;
      leds_show(0x0);
      __HAL_TIM_SET_COUNTER(&htim3, 0);
      HAL_TIM_Base_Start_IT(&htim3);
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3 || rstate != ST_REPLAY) return;

  if (replay_idx < 4u)
  {
    uint8_t bit = (uint8_t)((code >> replay_idx) & 1u);
    leds_show(bit ? 0x1 : 0x4);                               /* PD12 = 1, PD14 = 0 */
    replay_idx++;
  }
  else                                                        /* replay finished */
  {
    leds_show(0x0);
    HAL_TIM_Base_Stop_IT(&htim3);
    code = 0; n_rec = 0;
    rstate = ST_REC;
  }
}
/* USER CODE END 4 */
/* main loop: while (1) { } */
