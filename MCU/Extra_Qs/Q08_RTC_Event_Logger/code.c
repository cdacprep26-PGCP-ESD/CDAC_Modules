/* ===========================================================================
 * Q8. RTC-based access event logger
 * Scenario question from the guide (Q8)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define LOG_SIZE      8u
#define DEBOUNCE_MS   200u

typedef struct {
  uint16_t event_no;
  uint8_t hh, mm, ss, day, month, year;         /* year = 0..99 (26 = 2026) */

} event_rec_t;

volatile event_rec_t log_ring[LOG_SIZE];     /* ring buffer of all recent events         */
volatile uint8_t     log_head    = 0;        /* next slot to write                       */
volatile uint32_t    event_count = 0;
volatile event_rec_t last5[5];               /* snapshot of the five newest, oldest first */
volatile uint8_t     snapshot_no = 0;        /* increments each time last5 is refreshed */
volatile uint8_t     rtc_error   = 0;        /* 1 = invalid RTC / state error            */
volatile uint8_t     rtc_recover_req = 0;
volatile uint8_t     blink13_req = 0, blink15_req = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static HAL_StatusTypeDef rtc_set_default(void)
{
  RTC_TimeTypeDef t = {0};
  RTC_DateTypeDef d = {0};
  t.Hours = 10; t.Minutes = 30; t.Seconds = 50;           /* :50 so rollover is quick to test */
  t.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  t.StoreOperation = RTC_STOREOPERATION_RESET;
  d.WeekDay = RTC_WEEKDAY_SUNDAY; d.Date = 18; d.Month = RTC_MONTH_OCTOBER; d.Year = 26;
  if (HAL_RTC_SetTime(&hrtc, &t, RTC_FORMAT_BIN) != HAL_OK) return HAL_ERROR;
  return HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN);
}

static uint8_t rtc_valid(const RTC_TimeTypeDef *t, const RTC_DateTypeDef *d)
{
  if (__HAL_RTC_GET_FLAG(&hrtc, RTC_FLAG_INITS) == RESET) return 0;   /* calendar not initialised */
  return (t->Hours < 24 && t->Minutes < 60 && t->Seconds < 60 &&
          d->Month >= 1 && d->Month <= 12 && d->Date >= 1 && d->Date <= 31);
}

static void store_event(const RTC_TimeTypeDef *t, const RTC_DateTypeDef *d)
{
  uint8_t i = log_head;
  log_ring[i].event_no = (uint16_t)event_count;
  log_ring[i].hh = t->Hours; log_ring[i].mm = t->Minutes; log_ring[i].ss = t->Seconds;
  log_ring[i].day = d->Date; log_ring[i].month = d->Month; log_ring[i].year = d->Year;
  log_head = (uint8_t)((i + 1u) % LOG_SIZE);
}

static void copy_last5(void)
{
  for (uint8_t k = 0; k < 5; k++)                         /* oldest of the five first */
  {
    uint8_t idx = (uint8_t)((log_head + LOG_SIZE - 5u + k) % LOG_SIZE);
    last5[k].event_no = log_ring[idx].event_no;
    last5[k].hh = log_ring[idx].hh;   last5[k].mm = log_ring[idx].mm;   last5[k].ss = log_ring[idx].ss;
    last5[k].day = log_ring[idx].day; last5[k].month = log_ring[idx].month; last5[k].year = log_ring[idx].year;
  }
  snapshot_no++;
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);      /* READY state */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
static uint32_t t13_end = 0, t15_next = 0;
static uint8_t n15 = 0, t13_on = 0;
while (1)
{
  uint32_t now = HAL_GetTick();

  if (blink13_req) { blink13_req = 0; HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
                     t13_on = 1; t13_end = now + 500u; }            /* PD13 for 500 ms        */
  if (t13_on && (int32_t)(now - t13_end) >= 0) { HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
                                                 t13_on = 0; }

  if (blink15_req) { blink15_req = 0; n15 = 6; t15_next = now; }    /* 6 toggles = 3 blinks    */
  if (n15 && (int32_t)(now - t15_next) >= 0) { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
                                               n15--; t15_next = now + 150u; }

  if (rtc_error)                                                    /* ERROR state             */
  {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  }

  if (rtc_recover_req)                                              /* press while in error       */
  {
    rtc_recover_req = 0;
    if (rtc_set_default() == HAL_OK) rtc_error = 0;                 /* error cleared              */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last_ms = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;

  uint32_t now = HAL_GetTick();
  if ((now - last_ms) < DEBOUNCE_MS) return;              /* bounce / rapid press: ignore */
  last_ms = now;

  if (rtc_error) { rtc_recover_req = 1; return; }         /* a press asks main() to recover */

  RTC_TimeTypeDef t;
  RTC_DateTypeDef d;
  if (HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN) != HAL_OK ||   /* GetTime first ...         */
      HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK ||   /* ... then GetDate          */
      !rtc_valid(&t, &d))
  {
    rtc_error = 1;                                        /* PD14 shown by main() */
    return;
  }

  event_count++;
  store_event(&t, &d);
  blink13_req = 1;                                        /* PD13 500 ms */

  if ((event_count % 5u) == 0u)                           /* 5, 10, 15 ... */
  {
    copy_last5();                                         /* expose five records in the debugger */
    blink15_req = 1;                                      /* PD15 three blinks */
  }
}
/* USER CODE END 4 */
