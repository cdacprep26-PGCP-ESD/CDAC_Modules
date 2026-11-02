/* ===========================================================================
 * S17. RTC alarm every minute
 * Scenario question from the guide (S17)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint8_t alarm_flag = 0;
volatile uint32_t alarm_count = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void set_alarm_every_minute(void)
{
  RTC_AlarmTypeDef a = {0};
  a.AlarmTime.Hours = 0; a.AlarmTime.Minutes = 0; a.AlarmTime.Seconds = 30;      /* seconds = 30 */
  a.AlarmTime.SubSeconds = 0;
  a.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  a.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  a.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES; /* ignore these */
  a.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
  a.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  a.AlarmDateWeekDay = 1;
  a.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &a, RTC_FORMAT_BIN) != HAL_OK) Error_Handler();
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);         /* running indicator */
set_alarm_every_minute();
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (alarm_flag)
  {
    alarm_flag = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);     /* blink for 1 s (blocking is fine in main) */
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)     /* called by the RTC alarm interrupt */
{
  alarm_count++;
  alarm_flag = 1;                                            /* short: only set flags */
}
/* USER CODE END 4 */
