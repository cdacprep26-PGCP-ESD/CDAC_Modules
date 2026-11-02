/* ===========================================================================
 * Q2. Onboard LED brightness indication using PWM (TIM4 CH1)
 * Scenario question from the guide (Q2)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define PWM_ARR    999u
#define PWM_STEP   5
#define LED_ALL    (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
volatile uint8_t step_due = 0;          /* set by TIM3 every 10 ms         */
volatile int16_t ccr_now    = 0;        /* mirror of CCR1                  */
volatile int8_t   dir       = +1;       /* +1 up, -1 down                  */
volatile uint8_t duty_pct = 0;          /* calculated duty in percent      */
volatile uint32_t dbg_cnt, dbg_arr, dbg_ccr;   /* debugger copies          */
/* USER CODE END PV */

/* USER CODE BEGIN 0 */

static uint8_t pct_from_ccr(uint16_t ccr)
{
  return (uint8_t)(((uint32_t)ccr * 100u) / (PWM_ARR + 1u));
}

static void show_range(uint8_t pct)
{
  uint16_t on = 0;                      /* pin mask of LEDs to light      */
  if      (pct >= 75) on = LED_ALL;                        /* 75-100 %: all four */
  else if (pct >= 50) on = GPIO_PIN_12 | GPIO_PIN_13;      /* 50-74 %            */
  else if (pct >= 25) on = GPIO_PIN_12;                    /* 25-49 %            */
  HAL_GPIO_WritePin(GPIOD, (uint16_t)(LED_ALL & ~on), GPIO_PIN_RESET);   /* LEDs that must be OFF */
  HAL_GPIO_WritePin(GPIOD, on, GPIO_PIN_SET);                            /* LEDs that must be ON */
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);          /* 1 kHz PWM on CH1 (PB6) */
__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
HAL_TIM_Base_Start_IT(&htim3);                     /* 10 ms step timer          */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (step_due)
  {
    step_due = 0;
    int16_t next = ccr_now + dir * PWM_STEP;
    if (next >= (int16_t)PWM_ARR) { next = PWM_ARR; dir = -1; }      /* top reached    */
    else if (next <= 0)           { next = 0;       dir = +1; }      /* bottom reached */
    ccr_now = next;
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, ccr_now);           /* change duty    */
    duty_pct = pct_from_ccr((uint16_t)ccr_now);
    show_range(duty_pct);
  }
  dbg_cnt = __HAL_TIM_GET_COUNTER(&htim4);                             /* CNT, ARR and CCR1 for Live Expressions */
  dbg_arr = __HAL_TIM_GET_AUTORELOAD(&htim4);
  dbg_ccr = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) step_due = 1;        /* short: only set a flag */
}
/* USER CODE END 4 */
