/* ===========================================================================
 * Q10. Watchdog fault-recovery controller (IWDG)
 * Scenario question from the guide (Q10)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
typedef enum { RST_UNKNOWN = 0, RST_POWER, RST_PIN, RST_IWDG, RST_WWDG, RST_SOFTWARE, RST_LOWPOWER } rst_t;

volatile rst_t     reset_cause = RST_UNKNOWN;        /* visible in the debugger */
volatile uint8_t   heartbeat   = 0;                  /* set every 100 ms        */
volatile uint8_t   fault_mode = 0;                   /* 1 = deliberate fault    */
volatile uint16_t hold_ticks = 0;                    /* PA0 held, 100 ms units */
volatile uint32_t wdg_refresh_count = 0;
volatile uint32_t main_loops = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void read_reset_cause(void)
{
  if      (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) reset_cause = RST_IWDG;     /* most specific first */
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) reset_cause = RST_WWDG;
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) reset_cause = RST_SOFTWARE;
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) reset_cause = RST_LOWPOWER;
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) || __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)) reset_cause = RST_POWER;
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) reset_cause = RST_PIN;
  else                                           reset_cause = RST_UNKNOWN;
  __HAL_RCC_CLEAR_RESET_FLAGS();                  /* record first, then clear all flags */
}

static uint8_t health_ok(void)                    /* all checks must pass to refresh the watchdog */
{
  if (fault_mode)                              return 0;   /* deliberate test fault          */
  if (HAL_TIM_Base_GetState(&htim3) != HAL_TIM_STATE_BUSY) return 0;   /* heartbeat timer must run */
  if (hold_ticks > 1000)                       return 0;   /* sanity check of a software counter */
  return 1;
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
read_reset_cause();                               /* step 1 */
if (reset_cause == RST_IWDG)                      /* step 2: show the reason for 3 s */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  for (int i = 0; i < 30; i++)
  {
    HAL_Delay(100);
    HAL_IWDG_Refresh(&hiwdg);                     /* the IWDG is already running: keep it fed */
  }
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);   /* healthy */
HAL_TIM_Base_Start_IT(&htim3);                         /* step 3: heartbeat */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
static uint8_t blink_div = 0;
while (1)
{

  main_loops++;
  if (heartbeat)
  {
    heartbeat = 0;
    if (health_ok()) { HAL_IWDG_Refresh(&hiwdg); wdg_refresh_count++; }   /* step 4 */
    if (++blink_div >= 5) { blink_div = 0; HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); } /* live: 500 ms */
  }

  if (fault_mode)                                  /* step 5: deliberate stall, no more refresh */
  {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    while (1) { }                                  /* the IWDG will reset the MCU */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  heartbeat = 1;
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
  {
    if (hold_ticks < 1000) hold_ticks++;
    if (hold_ticks >= 30) fault_mode = 1;          /* 30 x 100 ms = 3 s */
  }
  else hold_ticks = 0;
}
/* USER CODE END 4 */
