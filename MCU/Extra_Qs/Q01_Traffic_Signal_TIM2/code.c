/* ===========================================================================
 * Q1. Traffic signal timing using a TIM2 interrupt
 * Scenario question from the guide (Q1)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define GREEN_PHASE_TICKS 20u           /* 20 x 100 ms = 2 s */
#define RED_PHASE_TICKS    10u          /* 10 x 100 ms = 1 s */
typedef enum { PHASE_GREEN = 0, PHASE_RED = 1 } phase_t;
volatile phase_t   phase       = PHASE_GREEN;
volatile uint16_t tick100      = 0;     /* software counter, 100 ms units */
volatile uint32_t cycles_done = 0;      /* complete green+red cycles      */
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void traffic_set(phase_t p)
{
  /* PD13 and PD15 always OFF in this question */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (p == PHASE_GREEN) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (p == PHASE_RED)   ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
traffic_set(PHASE_GREEN);
HAL_TIM_Base_Start_IT(&htim2);             /* start TIM2 with update interrupt */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM2) return;   /* shared by all timers */

  tick100++;
  if (phase == PHASE_GREEN && tick100 >= GREEN_PHASE_TICKS)
  {
    phase = PHASE_RED;   tick100 = 0;   traffic_set(phase);
  }
  else if (phase == PHASE_RED && tick100 >= RED_PHASE_TICKS)
  {
    phase = PHASE_GREEN; tick100 = 0;   cycles_done++; traffic_set(phase);
  }
}
/* USER CODE END 4 */
/* main loop stays empty: while (1) { } - everything happens in the interrupt */
