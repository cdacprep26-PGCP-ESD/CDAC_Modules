/* ===========================================================================
 * S12. Reaction-time game
 * Scenario question from the guide (S12)
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
typedef enum { ST_NEW_ROUND, ST_WAIT, ST_GO, ST_RESULT, ST_FALSE } game_t;
volatile game_t   st = ST_NEW_ROUND;
volatile uint8_t press_flag = 0;
volatile uint32_t t_press = 0, t_go = 0, reaction_ms = 0, rounds = 0;
static   uint32_t t_mark = 0, wait_ms = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start(&htim2);                                /* free-running 1 ms stopwatch */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  switch (st)
  {
    case ST_NEW_ROUND:
      leds_show(0x1);                                    /* PD12 = ready */
      wait_ms = 2000u + ((__HAL_TIM_GET_COUNTER(&htim2) * 7u + HAL_GetTick()) % 3000u);
      t_mark = HAL_GetTick();
      press_flag = 0;
      st = ST_WAIT;
      break;

      case ST_WAIT:
        if (press_flag)                                    /* pressed too early: false start */
        { press_flag = 0; leds_show(0x4); t_mark = HAL_GetTick(); st = ST_FALSE; }
        else if ((HAL_GetTick() - t_mark) >= wait_ms)
        {
          leds_show(0x8);                                  /* PD15 = GO */

         t_go = __HAL_TIM_GET_COUNTER(&htim2);
         press_flag = 0;
         st = ST_GO;
       }
       break;

     case ST_GO:
       if (press_flag)
       {
         press_flag = 0;
         reaction_ms = t_press - t_go;
         if      (reaction_ms < 200u) leds_show(0xF);
         else if (reaction_ms < 300u) leds_show(0x7);
         else if (reaction_ms < 500u) leds_show(0x3);
         else                          leds_show(0x1);
         rounds++;
         t_mark = HAL_GetTick();
         st = ST_RESULT;
       }
       break;

     case ST_RESULT:                                      /* show result 3 s */
       if ((HAL_GetTick() - t_mark) >= 3000u) st = ST_NEW_ROUND;
       break;

     case ST_FALSE:                                       /* false start 1 s */
       if ((HAL_GetTick() - t_mark) >= 1000u) st = ST_NEW_ROUND;
       break;
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 50u) return;
  last = HAL_GetTick();
  t_press = __HAL_TIM_GET_COUNTER(&htim2);              /* time stamp taken at the instant of the press */
  press_flag = 1;
}
/* USER CODE END 4 */
