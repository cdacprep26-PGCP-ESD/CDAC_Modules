/* ###########################################################################
 *  STM32F407 Discovery - ALL SCENARIO CODES (HAL only)
 *  Q1-Q10  : scenario questions from the Lab Exam Guide
 *  S11-S22 : more scenario questions from the guide
 *  E1-E10  : extra practice questions
 *
 *  Every section below is a set of USER CODE snippets for ONE question.
 *  Do NOT compile this file as it is (the questions use different CubeMX
 *  settings). Create one project per question, apply the configuration from that
 *  question's question.md, and copy the matching section into main.c.
 * ########################################################################### */



/* ############################ Q1. Traffic signal timing using a TIM2 interrupt ############################ */
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



/* ############################ Q2. Onboard LED brightness indication using PWM (TIM4 CH1) ############################ */
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



/* ############################ Q3. Four-stage sequence using output compare (TIM4) ############################ */
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



/* ############################ Q4. Independent LED toggle rates using output-compare interrupts ############################ */
/* ===========================================================================
 * Q4. Independent LED toggle rates using output-compare interrupts
 * Scenario question from the guide (Q4)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
static const uint16_t led_pin[4] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };
static const uint32_t oc_chan[4] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };
static const uint32_t interval[4] = { 200, 400, 600, 1000 };       /* ticks = ms */
volatile uint32_t toggle_count[4];      /* how many toggles each LED made       */
volatile uint32_t period_ms[4];         /* measured time between two toggles    */
static   uint32_t last_ms[4];
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_3);
HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM4) return;

  uint8_t i;
  switch (htim->Channel)
  {
    case HAL_TIM_ACTIVE_CHANNEL_1: i = 0; break;
    case HAL_TIM_ACTIVE_CHANNEL_2: i = 1; break;
    case HAL_TIM_ACTIVE_CHANNEL_3: i = 2; break;
    case HAL_TIM_ACTIVE_CHANNEL_4: i = 3; break;
    default: return;
  }

  HAL_GPIO_TogglePin(GPIOD, led_pin[i]);                        /* visible toggle          */
  toggle_count[i]++;

  uint32_t now = HAL_GetTick();                                 /* measurement only        */
  period_ms[i] = now - last_ms[i];
  last_ms[i]   = now;

  uint32_t prev = __HAL_TIM_GET_COMPARE(htim, oc_chan[i]);      /* previous compare value */
  uint32_t next = (prev + interval[i]) % 65536u;                /* ARR + 1 = 65536        */
  __HAL_TIM_SET_COMPARE(htim, oc_chan[i], next);                /* schedule next match    */
}
/* USER CODE END 4 */



/* ############################ Q5. Measure the user-button interval using input capture (TIM2 CH1) ############################ */
/* ===========================================================================
 * Q5. Measure the user-button interval using input capture (TIM2 CH1)
 * Scenario question from the guide (Q5)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define DEBOUNCE_MS 50u
volatile uint32_t t1 = 0, t2 = 0;
volatile uint32_t interval_ms = 0;
volatile uint8_t capture_state = 0;        /* 0 = waiting 1st press, 1 = waiting 2nd press */
volatile uint8_t result_ready = 0;

volatile uint32_t valid_edges = 0, ignored_edges = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static uint32_t elapsed_ticks(uint32_t a, uint32_t b, uint32_t arr)
{
  /* ARR + 1 overflows to 0 in uint32 when ARR = 0xFFFFFFFF, which still gives the right answer */
  return (b >= a) ? (b - a) : (arr + 1u - a + b);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);             /* start capture with interrupt */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (result_ready)
  {
    result_ready = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);        /* blink PD15 briefly */
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  static uint32_t last_edge = 0;
  static uint8_t have_edge = 0;

  if (htim->Instance != TIM2 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) return;

  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);      /* hardware time stamp */

  if (have_edge && elapsed_ticks(last_edge, now, 0xFFFFFFFFu) < DEBOUNCE_MS)
  {
    ignored_edges++;                    /* bounce: ignore */
    return;
  }
  last_edge = now;
  have_edge = 1;
  valid_edges++;

  if (capture_state == 0)               /* first press */
  {
    t1 = now;
    capture_state = 1;
  }
  else                                  /* second press */
  {
    t2 = now;
    interval_ms = elapsed_ticks(t1, t2, 0xFFFFFFFFu);
    result_ready = 1;
    capture_state = 0;
  }
}
/* USER CODE END 4 */



/* ############################ Q6. Onboard accelerometer tilt safety monitor ############################ */
/* ===========================================================================
 * Q6. Onboard accelerometer tilt safety monitor
 * Scenario question from the guide (Q6)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN Includes */
#include <stdlib.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
#define LIS_CS_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define LIS_CS_HIGH()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define ID_LIS3DSH       0x3F
#define ID_LIS302DL      0x3B
#define FORCE_ID_FAIL    0               /* set to 1 to test the identity-failure case */

#define TILT_ON_MG        300                 /* enter tilt state      */
#define TILT_OFF_MG       200                 /* leave tilt state      */
#define ALARM_SAMPLES     30                  /* 30 x 100 ms = 3 s     */
#define CLEAR_SAMPLES     20                  /* 20 x 100 ms = 2 s     */
#define HOLD_SAMPLES      10                  /* 10 x 100 ms = 1 s     */

typedef enum { SENSOR_NONE = 0, SENSOR_LIS3DSH, SENSOR_LIS302DL } sensor_t;

volatile sensor_t sensor = SENSOR_NONE;
volatile uint8_t who_am_i;
volatile int16_t raw_x, raw_y, raw_z;                  /* raw counts                          */

volatile int16_t mg_x, mg_y, mg_z;               /* milli-g (sensor independent)      */
volatile int16_t ref_x, ref_y;                   /* level reference in mg             */
volatile int16_t rel_x, rel_y;                   /* reading minus reference           */
volatile uint8_t direction = 0;                  /* 0 level, 1 +X, 2 -X, 3 +Y, 4 -Y   */
volatile uint8_t alarm_on = 0;
volatile uint16_t tilt_cnt = 0, safe_cnt = 0;
volatile uint8_t sample_due = 0;                 /* set every 100 ms by TIM3          */
static uint8_t    btn_cnt = 0, blink_ctr = 0, blink_state = 0;
/* bit n of the mask lights PD(12+n): +X -> PD12, -X -> PD14, +Y -> PD13, -Y -> PD15 */
static const uint8_t dir_mask[5] = { 0x0, 0x1, 0x4, 0x2, 0x8 };
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static uint8_t lis_read(uint8_t reg)
{
  uint8_t tx[2] = { (uint8_t)(reg | 0x80), 0x00 };   /* bit7 = 1: read              */
  uint8_t rx[2] = { 0, 0 };
  LIS_CS_LOW();
  HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 100);
  LIS_CS_HIGH();
  return rx[1];                                      /* rx[0] was received during the address byte */
}

static void lis_write(uint8_t reg, uint8_t val)
{
  uint8_t tx[2] = { (uint8_t)(reg & 0x3F), val };    /* bit7 = 0: write             */
  LIS_CS_LOW();
  HAL_SPI_Transmit(&hspi1, tx, 2, 100);
  LIS_CS_HIGH();
}

static uint8_t accel_init(void)
{
  LIS_CS_HIGH();
  HAL_Delay(10);                                     /* power-up time               */
  who_am_i = lis_read(0x0F);
  if (FORCE_ID_FAIL) who_am_i = 0x00;

    if (who_am_i == ID_LIS3DSH)
    {
      sensor = SENSOR_LIS3DSH;
      lis_write(0x25, 0x10);                         /* CTRL_REG6: address auto-increment */
      lis_write(0x24, 0x00);                         /* CTRL_REG5: +/-2 g                 */
      lis_write(0x20, 0x67);                         /* CTRL_REG4: 100 Hz, X/Y/Z enabled */
    }
    else if (who_am_i == ID_LIS302DL)
    {
      sensor = SENSOR_LIS302DL;
      lis_write(0x20, 0x47);                         /* CTRL_REG1: active, 100 Hz, XYZ on */
    }
    else
    {
      sensor = SENSOR_NONE;
      return 0;                                      /* wrong identity */
    }
    HAL_Delay(20);
    return 1;
}

static void lis_read_xyz(void)                        /* fills raw_* and mg_* */
{
  if (sensor == SENSOR_LIS3DSH)
  {
    uint8_t tx[7] = { 0x28 | 0x80, 0, 0, 0, 0, 0, 0 }; /* OUT_X_L with read bit + 6 dummies */
    uint8_t rx[7];
    LIS_CS_LOW();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 7, 100);
    LIS_CS_HIGH();
    raw_x = (int16_t)((rx[2] << 8) | rx[1]);          /* high byte << 8 | low byte, signed */
    raw_y = (int16_t)((rx[4] << 8) | rx[3]);
    raw_z = (int16_t)((rx[6] << 8) | rx[5]);
    mg_x = (int16_t)(((int32_t)raw_x * 60) / 1000);   /* 0.06 mg per LSB */
    mg_y = (int16_t)(((int32_t)raw_y * 60) / 1000);
    mg_z = (int16_t)(((int32_t)raw_z * 60) / 1000);
  }
  else if (sensor == SENSOR_LIS302DL)
  {
    raw_x = (int8_t)lis_read(0x29);                   /* one signed byte per axis */
    raw_y = (int8_t)lis_read(0x2B);
    raw_z = (int8_t)lis_read(0x2D);
    mg_x = raw_x * 18;                                /* 18 mg per LSB */
    mg_y = raw_y * 18;
    mg_z = raw_z * 18;
  }

}

static void set_leds(uint8_t mask)                    /* bit0..3 -> PD12..PD15 */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static void capture_reference(void)                   /* average 8 samples while the board is level */
{
  int32_t sx = 0, sy = 0;
  for (int i = 0; i < 8; i++) { lis_read_xyz(); sx += mg_x; sy += mg_y; HAL_Delay(12); }
  ref_x = (int16_t)(sx / 8);
  ref_y = (int16_t)(sy / 8);
}

static void error_state(void)                           /* wrong WHO_AM_I */
{
  set_leds(0x4);                                        /* only PD14 (red) ON */
  while (1) { }                                         /* stay here; SWD still works */
}

static void monitor_step(void)                        /* called every 100 ms from main() */
{
  /* --- PA0 held for 1 s -> new level reference --- */
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
  {
    if (btn_cnt <= HOLD_SAMPLES) btn_cnt++;
    if (btn_cnt == HOLD_SAMPLES)
    {
      capture_reference();
      tilt_cnt = 0; safe_cnt = 0; alarm_on = 0; direction = 0;
    }
  }
  else btn_cnt = 0;

    /* --- sample and subtract the reference --- */
    lis_read_xyz();
    rel_x = mg_x - ref_x;
    rel_y = mg_y - ref_y;
    int32_t ax = abs(rel_x), ay = abs(rel_y);
    int32_t big = (ax > ay) ? ax : ay;                  /* dominant horizontal axis */

    /* --- tilt detection with hysteresis --- */
    uint8_t tilted = (big > ((direction != 0) ? TILT_OFF_MG : TILT_ON_MG));
    if (tilted)
    {
      if (ax >= ay) direction = (rel_x > 0) ? 1 : 2;
      else           direction = (rel_y > 0) ? 3 : 4;
      safe_cnt = 0;
      if (tilt_cnt < 60000) tilt_cnt++;
    }
    else
    {
      direction = 0;
      tilt_cnt = 0;                                     /* any safe sample breaks the streak */
      if (safe_cnt < 60000) safe_cnt++;
    }

    /* --- alarm rules --- */
    if (!alarm_on && tilt_cnt >= ALARM_SAMPLES) { alarm_on = 1; safe_cnt = 0; }    /* 3 s tilted */
    if (alarm_on && safe_cnt >= CLEAR_SAMPLES) { alarm_on = 0; tilt_cnt = 0; }     /* 2 s safe   */

    /* --- outputs --- */
    if (alarm_on)
    {
      if (++blink_ctr >= 5) { blink_ctr = 0; blink_state ^= 1u; }    /* toggle every 500 ms */
      set_leds(blink_state ? 0xF : 0x0);                             /* all four blink      */
    }
    else
    {
      blink_ctr = 0; blink_state = 0;
      set_leds(dir_mask[direction]);
    }
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
set_leds(0x0);
if (!accel_init()) error_state();                       /* spec item 2 */
capture_reference();                                    /* level reference at start-up */
HAL_TIM_Base_Start_IT(&htim3);                          /* 100 ms schedule */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (sample_due) { sample_due = 0; monitor_step(); }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) sample_due = 1;         /* short: only a flag */
}
/* USER CODE END 4 */



/* ############################ Q7. Internal temperature monitor using ADC and DMA ############################ */
/* ===========================================================================
 * Q7. Internal temperature monitor using ADC and DMA
 * Scenario question from the guide (Q7)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define N_AVG        16u                    /* samples per channel to average    */
#define N_CH         2u                     /* rank1 = temperature, rank2 = VREFINT */
#define V25_TYP      0.76f                  /* volts at 25 C (typical)           */
#define SLOPE_TYP    0.0025f                /* volts per degree C (typical)      */
#define VREFINT_TYP 1.21f                   /* volts (typical)                   */
#define R1_DELTA     1.0f                   /* range thresholds, degrees C above baseline */
#define R2_DELTA     3.0f
#define HIGH_DELTA   6.0f

volatile uint16_t adc_buf[N_AVG * N_CH];    /* filled by DMA: T,V,T,V,...        */
volatile uint8_t adc_ready      = 0;        /* set by DMA half/full callbacks    */
volatile uint8_t adc_full_once = 0;         /* buffer completely filled at least once */
volatile uint32_t dma_pos = 0;              /* current DMA index in the buffer   */
volatile uint16_t avg_temp_raw, avg_vref_raw;
volatile float    vdda_v, vsense_v, temp_c; /* live values                       */
volatile float    disp_temp_c;              /* value used for the LEDs           */
volatile float    baseline_c;               /* first stable reading              */
volatile uint8_t baseline_set = 0, range_idx = 0;
volatile uint8_t frozen = 0;                /* 1 = snapshot mode (PA0)           */
volatile uint8_t vdda_ok = 1;               /* supply looks reasonable (3.0-3.6 V) */
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void set_leds(uint8_t mask)          /* bit0..3 -> PD12..PD15 */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static void process_adc(void)               /* average the latest 16 pairs and convert */
{
  uint32_t sum_t = 0, sum_v = 0;
  for (uint32_t i = 0; i < N_AVG; i++)
  {
    sum_t += adc_buf[2u * i];               /* even index: temperature sensor */
    sum_v += adc_buf[2u * i + 1u];          /* odd index : VREFINT            */
  }

    avg_temp_raw = (uint16_t)(sum_t / N_AVG);
    avg_vref_raw = (uint16_t)(sum_v / N_AVG);
    if (avg_vref_raw == 0) return;            /* avoid divide by zero */

    vdda_v   = VREFINT_TYP * 4095.0f / (float)avg_vref_raw;
    vsense_v = (float)avg_temp_raw * vdda_v / 4095.0f;
    temp_c   = (vsense_v - V25_TYP) / SLOPE_TYP + 25.0f;
    vdda_ok = (vdda_v > 3.0f && vdda_v < 3.6f);          /* reasonable-supply check */

    if (!baseline_set) { baseline_c = temp_c; baseline_set = 1; }
    if (!frozen) disp_temp_c = temp_c;                  /* frozen: keep old value */
}

static void update_leds(void)
{
  float rise = disp_temp_c - baseline_c;
  static uint32_t t_blink = 0;
  static uint8_t blink14 = 0;

    if      (rise >= HIGH_DELTA) range_idx = 3;
    else if (rise >= R2_DELTA)   range_idx = 2;
    else if (rise >= R1_DELTA)   range_idx = 1;
    else                         range_idx = 0;

    uint8_t mask = 0x1;                                  /* PD12 */
    if (range_idx >= 1) mask |= 0x2;                     /* PD13 */
    if (range_idx >= 2) mask |= 0x8;                     /* PD15 */
    if (range_idx == 3)                                  /* PD14 blinks every 250 ms */
    {
      if (HAL_GetTick() - t_blink >= 250u) { t_blink = HAL_GetTick(); blink14 ^= 1u; }
      if (blink14) mask |= 0x4;
    }
    set_leds(mask);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, N_AVG * N_CH);      /* arm ADC + DMA, wait for trigger */
HAL_TIM_Base_Start(&htim3);                                        /* TRGO every 100 ms               */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  dma_pos = N_AVG * N_CH - __HAL_DMA_GET_COUNTER(&hdma_adc1);      /* NDTR counts down */
  if (adc_ready && adc_full_once)
  {
    adc_ready = 0;
    process_adc();
  }
  if (baseline_set) update_leds();
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1) adc_ready = 1;       /* first half is new, second half older */
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1) { adc_full_once = 1; adc_ready = 1; }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)       /* PA0: live <-> frozen */
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;
  uint32_t now = HAL_GetTick();
  if ((now - last) < 200u) return;                   /* debounce */
  last = now;
  frozen ^= 1u;
  if (frozen) disp_temp_c = temp_c;                  /* take snapshot at the moment of freezing */
}
/* USER CODE END 4 */



/* ############################ Q8. RTC-based access event logger ############################ */
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



/* ############################ Q9. DMA-driven onboard LED pattern player (timer update to GPIOD BSRR) ############################ */
/* ===========================================================================
 * Q9. DMA-driven onboard LED pattern player (timer update to GPIOD BSRR)
 * Scenario question from the guide (Q9)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
/* build one BSRR word: wanted LEDs set, the other three of PD12..PD15 reset */
#define BSRR_PATTERN(m) ( ((uint32_t)(m) << 12) | ((uint32_t)((~(m)) & 0xFu) << 28) )

static const uint32_t led_pattern[] = {
   BSRR_PATTERN(0x0),                   /* all OFF                                 */
   BSRR_PATTERN(0xF),                   /* all ON                                  */
   BSRR_PATTERN(0x0),                   /* all OFF                                 */
   BSRR_PATTERN(0x1),                   /* single LED PD12                         */
   BSRR_PATTERN(0x2),                   /* PD13                                    */
   BSRR_PATTERN(0x4),                   /* PD14                                    */
   BSRR_PATTERN(0x8),                   /* PD15                                    */
   BSRR_PATTERN(0x8),                   /* identical entry: PD15 stays on          */
   BSRR_PATTERN(0x8),                   /* identical entry again                   */
   BSRR_PATTERN(0x4),                   /* back down                               */
   BSRR_PATTERN(0x2)
};
#define PATTERN_LEN (sizeof(led_pattern) / sizeof(led_pattern[0]))

volatile uint32_t dma_index   = 0;      /* current position in the table            */
volatile uint32_t dma_tc_count = 0;     /* completed passes through the table       */
volatile uint32_t uev_count   = 0;      /* timer update events seen                 */
volatile uint8_t paused       = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void pattern_xfer_cplt(DMA_HandleTypeDef *hdma)         /* transfer-complete callback */
{
  (void)hdma;
  dma_tc_count++;                                              /* one full pass finished      */
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
hdma_tim1_up.XferCpltCallback = pattern_xfer_cplt;
HAL_DMA_Start_IT(&hdma_tim1_up, (uint32_t)led_pattern,         /* source: table in Flash      */
                 (uint32_t)&GPIOD->BSRR,                       /* destination: BSRR (fixed)   */
                 PATTERN_LEN);                                 /* number of 32-bit words      */

__HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);                    /* update event -> DMA request */
HAL_TIM_Base_Start_IT(&htim1);                                   /* start timer + count events */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  /* NDTR counts down from PATTERN_LEN; index = PATTERN_LEN - NDTR. The loop does NOT touch the LEDs. */
  dma_index = PATTERN_LEN - __HAL_DMA_GET_COUNTER(&hdma_tim1_up);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) uev_count++;                  /* observation only */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)                   /* PA0: pause / resume */
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;
  uint32_t now = HAL_GetTick();
  if ((now - last) < 200u) return;                               /* debounce */
  last = now;

  paused ^= 1u;
  if (paused) HAL_TIM_Base_Stop_IT(&htim1);                      /* no update events -> no DMA steps */
  else        HAL_TIM_Base_Start_IT(&htim1);                     /* continue from the same index     */
}
/* USER CODE END 4 */



/* ############################ Q10. Watchdog fault-recovery controller (IWDG) ############################ */
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



/* ############################ S11. Metronome with selectable tempo ############################ */
/* ===========================================================================
 * S11. Metronome with selectable tempo
 * Scenario question from the guide (S11)
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
static const uint16_t beat_ticks[3] = { 1000, 667, 500 };                 /* ms per beat */
static const uint8_t tempo_led[3] = { 0x2, 0x4, 0x8 };                    /* PD13, PD14, PD15 */
volatile uint8_t tempo_idx = 0, tempo_changed = 1, flash_on = 0;
volatile uint32_t beat_count = 0, flash_off_at = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (tempo_changed)                                       /* update the tempo indicator in main */
  {
    tempo_changed = 0;
    leds_show(tempo_led[tempo_idx]);
  }
  if (flash_on && (int32_t)(HAL_GetTick() - flash_off_at) >= 0)     /* beat flash lasts 50 ms */
  {
    flash_on = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)          /* once per beat */
{
  if (htim->Instance != TIM3) return;
  beat_count++;
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  flash_off_at = HAL_GetTick() + 50u;

    flash_on = 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;   /* debounce */
  last = HAL_GetTick();
  tempo_idx = (uint8_t)((tempo_idx + 1u) % 3u);
  __HAL_TIM_SET_AUTORELOAD(&htim3, beat_ticks[tempo_idx] - 1u);     /* new beat period         */
  __HAL_TIM_SET_COUNTER(&htim3, 0);                                 /* restart the beat now    */
  tempo_changed = 1;
}
/* USER CODE END 4 */

/* leds_show() from the helper above must be in USER CODE BEGIN 0 */



/* ############################ S12. Reaction-time game ############################ */
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



/* ############################ S13. Stopwatch with the User button ############################ */
/* ===========================================================================
 * S13. Stopwatch with the User button
 * Scenario question from the guide (S13)
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
typedef enum { SW_ZERO, SW_RUN, SW_STOP } sw_t;
volatile sw_t     sw = SW_ZERO;
volatile uint32_t ticks100 = 0;                     /* number of 100 ms ticks   */
volatile uint32_t elapsed_ms = 0;                   /* look at this in Live Expressions */
volatile uint8_t button_flag = 0;
/* USER CODE END PV */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (button_flag)
  {
    button_flag = 0;
    if (sw == SW_ZERO)      { sw = SW_RUN; HAL_TIM_Base_Start_IT(&htim3); }           /* press 1: start */
    else if (sw == SW_RUN) { sw = SW_STOP; HAL_TIM_Base_Stop_IT(&htim3); }            /* press 2: stop */
    else                    { sw = SW_ZERO; ticks100 = 0; leds_show(0x0); }           /* press 3: reset */
  }
  elapsed_ms = ticks100 * 100u;
  if (sw != SW_ZERO) leds_show((uint8_t)((ticks100 / 10u) & 0xFu));                   /* seconds, binary */
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) ticks100++;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  button_flag = 1;
}
/* USER CODE END 4 */



/* ############################ S14. Four LEDs with different brightness using PWM (visible output) ############################ */
/* ===========================================================================
 * S14. Four LEDs with different brightness using PWM (visible output)
 * Scenario question from the guide (S14)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
static const uint16_t profile[3][4] = {
   { 100, 250, 500, 900 },
   { 900, 500, 250, 100 },
   { 500, 500, 500, 500 }
};
volatile uint8_t prof_idx = 0, prof_changed = 1;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (prof_changed)
  {
    prof_changed = 0;
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, profile[prof_idx][0]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, profile[prof_idx][1]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, profile[prof_idx][2]);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, profile[prof_idx][3]);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  prof_idx = (uint8_t)((prof_idx + 1u) % 3u);
  prof_changed = 1;
}
/* USER CODE END 4 */



/* ############################ S15. Short press and long press detection ############################ */
/* ===========================================================================
 * S15. Short press and long press detection
 * Scenario question from the guide (S15)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t press_ms = 0;               /* last accepted press length            */
volatile uint32_t short_count = 0, long_count = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t t_down = 0, last_edge = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;

  uint32_t now = HAL_GetTick();
  if ((now - last_edge) < 10u) return;                       /* ignore edges closer than 10 ms */
  last_edge = now;

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)   /* pin is high: button pressed    */
  {
    t_down = now;
  }
  else                                                       /* pin is low: button released    */
  {
    uint32_t len = now - t_down;
    if (len < 30u) return;                                   /* bounce                         */
    press_ms = len;
    if (len >= 1000u) { long_count++; HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); }
    else              { short_count++; HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); }
  }

}
/* USER CODE END 4 */
/* main loop: while (1) { } - everything is handled by the interrupt */



/* ############################ S16. Simple temperature indicator with single-conversion ADC (polling) ############################ */
/* ===========================================================================
 * S16. Simple temperature indicator with single-conversion ADC (polling)
 * Scenario question from the guide (S16)
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
volatile uint16_t adc_raw = 0, adc_avg = 0;
volatile float    temp_c = 0.0f, baseline_c = 0.0f;
volatile uint8_t baseline_set = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static uint16_t adc_read_once(void)
{
  HAL_ADC_Start(&hadc1);                                   /* start one conversion         */
  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)    /* wait for end of conversion   */
    adc_raw = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  return adc_raw;
}
/* leds_show() helper from the start of this chapter must also be here */
/* USER CODE END 0 */

/* USER CODE BEGIN WHILE */
while (1)
{
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) sum += adc_read_once();     /* 8 readings, 32-bit sum        */
  adc_avg = (uint16_t)(sum / 8u);

  float vsense = (float)adc_avg * 3.3f / 4095.0f;
  temp_c = (vsense - 0.76f) / 0.0025f + 25.0f;
  if (!baseline_set) { baseline_c = temp_c; baseline_set = 1; }

  float rise = temp_c - baseline_c;
  if      (rise >= 6.0f) leds_show(0xF);
  else if (rise >= 3.0f) leds_show(0x7);
  else if (rise >= 1.0f) leds_show(0x3);
  else                   leds_show(0x1);

  HAL_Delay(500);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */



/* ############################ S17. RTC alarm every minute ############################ */
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



/* ############################ S18. Memory-to-memory DMA copy and verification ############################ */
/* ===========================================================================
 * S18. Memory-to-memory DMA copy and verification
 * Scenario question from the guide (S18)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
#define N_WORDS 64u
uint32_t src[N_WORDS], dst[N_WORDS];
volatile uint8_t dma_done = 0;
volatile uint32_t dma_us = 0, cpu_us = 0;
volatile uint8_t copy_ok = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void dma_copy_done(DMA_HandleTypeDef *hdma) { (void)hdma; dma_done = 1; }
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start(&htim2);
for (uint32_t i = 0; i < N_WORDS; i++) { src[i] = 0xA5A50000u + i; dst[i] = 0; }   /* test data */

/* --- CPU copy, timed --- */
uint32_t t0 = __HAL_TIM_GET_COUNTER(&htim2);
for (uint32_t i = 0; i < N_WORDS; i++) dst[i] = src[i];
cpu_us = __HAL_TIM_GET_COUNTER(&htim2) - t0;
for (uint32_t i = 0; i < N_WORDS; i++) dst[i] = 0;                                  /* clear again */

/* --- DMA copy, timed --- */
hdma_memtomem_dma2_stream0.XferCpltCallback = dma_copy_done;     /* transfer-complete callback */
t0 = __HAL_TIM_GET_COUNTER(&htim2);
HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)src, (uint32_t)dst, N_WORDS);
while (!dma_done) { }                                            /* wait for the callback */
dma_us = __HAL_TIM_GET_COUNTER(&htim2) - t0;

/* --- verify --- */
copy_ok = 1;
for (uint32_t i = 0; i < N_WORDS; i++) if (dst[i] != src[i]) { copy_ok = 0; break; }
HAL_GPIO_WritePin(GPIOD, copy_ok ? GPIO_PIN_12 : GPIO_PIN_14, GPIO_PIN_SET);
/* USER CODE END 2 */



/* ############################ S19. Sleep mode until the button is pressed ############################ */
/* ===========================================================================
 * S19. Sleep mode until the button is pressed
 * Scenario question from the guide (S19)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t wake_count = 0;
volatile uint8_t woke = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_DBGMCU_EnableDBGSleepMode();                  /* keep the debugger connected during Sleep */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);          /* awake */
  HAL_Delay(2000);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  woke = 0;
  HAL_SuspendTick();                                         /* stop SysTick, or it wakes the CPU every ms */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);   /* sleep until an interrupt */
  HAL_ResumeTick();                                          /* back awake */

  if (woke)
  {
    wake_count++;
    for (int i = 0; i < 6; i++) { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); HAL_Delay(150); }        /* 3 blinks */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0) woke = 1;                         /* the interrupt itself wakes the core */
}
/* USER CODE END 4 */



/* ############################ S20. Free-fall and impact detector (accelerometer) ############################ */
/* ===========================================================================
 * S20. Free-fall and impact detector (accelerometer)
 * Scenario question from the guide (S20)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
/* ---- reused from Q6: sensor defines and variables ---- */
#define LIS_CS_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define LIS_CS_HIGH()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define ID_LIS3DSH       0x3F
#define ID_LIS302DL      0x3B
#define FORCE_ID_FAIL    0               /* set to 1 to test the identity-failure case */

#define TILT_ON_MG        300                 /* enter tilt state      */
#define TILT_OFF_MG       200                 /* leave tilt state      */
#define ALARM_SAMPLES     30                  /* 30 x 100 ms = 3 s     */
#define CLEAR_SAMPLES     20                  /* 20 x 100 ms = 2 s     */
#define HOLD_SAMPLES      10                  /* 10 x 100 ms = 1 s     */

typedef enum { SENSOR_NONE = 0, SENSOR_LIS3DSH, SENSOR_LIS302DL } sensor_t;

volatile sensor_t sensor = SENSOR_NONE;
volatile uint8_t who_am_i;
volatile int16_t raw_x, raw_y, raw_z;                  /* raw counts                          */

volatile int16_t mg_x, mg_y, mg_z;               /* milli-g (sensor independent)      */
/* ---- end of Q6 part ---- */
volatile uint8_t sample_due = 0;

volatile uint32_t freefall_count = 0, impact_count = 0;
volatile int32_t mag_sq = 0;
static   uint8_t ff_samples = 0;
static   uint32_t ff_led_off = 0, imp_led_off = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
/* ---- reused from Q6: SPI access and accel_init() ---- */
static uint8_t lis_read(uint8_t reg)
{
  uint8_t tx[2] = { (uint8_t)(reg | 0x80), 0x00 };   /* bit7 = 1: read              */
  uint8_t rx[2] = { 0, 0 };
  LIS_CS_LOW();
  HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 100);
  LIS_CS_HIGH();
  return rx[1];                                      /* rx[0] was received during the address byte */
}

static void lis_write(uint8_t reg, uint8_t val)
{
  uint8_t tx[2] = { (uint8_t)(reg & 0x3F), val };    /* bit7 = 0: write             */
  LIS_CS_LOW();
  HAL_SPI_Transmit(&hspi1, tx, 2, 100);
  LIS_CS_HIGH();
}

static uint8_t accel_init(void)
{
  LIS_CS_HIGH();
  HAL_Delay(10);                                     /* power-up time               */
  who_am_i = lis_read(0x0F);
  if (FORCE_ID_FAIL) who_am_i = 0x00;

    if (who_am_i == ID_LIS3DSH)
    {
      sensor = SENSOR_LIS3DSH;
      lis_write(0x25, 0x10);                         /* CTRL_REG6: address auto-increment */
      lis_write(0x24, 0x00);                         /* CTRL_REG5: +/-2 g                 */
      lis_write(0x20, 0x67);                         /* CTRL_REG4: 100 Hz, X/Y/Z enabled */
    }
    else if (who_am_i == ID_LIS302DL)
    {
      sensor = SENSOR_LIS302DL;
      lis_write(0x20, 0x47);                         /* CTRL_REG1: active, 100 Hz, XYZ on */
    }
    else
    {
      sensor = SENSOR_NONE;
      return 0;                                      /* wrong identity */
    }
    HAL_Delay(20);
    return 1;
}

static void lis_read_xyz(void)                        /* fills raw_* and mg_* */
{
  if (sensor == SENSOR_LIS3DSH)
  {
    uint8_t tx[7] = { 0x28 | 0x80, 0, 0, 0, 0, 0, 0 }; /* OUT_X_L with read bit + 6 dummies */
    uint8_t rx[7];
    LIS_CS_LOW();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 7, 100);
    LIS_CS_HIGH();
    raw_x = (int16_t)((rx[2] << 8) | rx[1]);          /* high byte << 8 | low byte, signed */
    raw_y = (int16_t)((rx[4] << 8) | rx[3]);
    raw_z = (int16_t)((rx[6] << 8) | rx[5]);
    mg_x = (int16_t)(((int32_t)raw_x * 60) / 1000);   /* 0.06 mg per LSB */
    mg_y = (int16_t)(((int32_t)raw_y * 60) / 1000);
    mg_z = (int16_t)(((int32_t)raw_z * 60) / 1000);
  }
  else if (sensor == SENSOR_LIS302DL)
  {
    raw_x = (int8_t)lis_read(0x29);                   /* one signed byte per axis */
    raw_y = (int8_t)lis_read(0x2B);
    raw_z = (int8_t)lis_read(0x2D);
    mg_x = raw_x * 18;                                /* 18 mg per LSB */
    mg_y = raw_y * 18;
    mg_z = raw_z * 18;
  }

}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
if (!accel_init()) { HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); while (1) { } }    /* as in Q6 */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (sample_due)
  {
    sample_due = 0;
    lis_read_xyz();                                          /* fills mg_x, mg_y, mg_z */
    mag_sq = (int32_t)mg_x * mg_x + (int32_t)mg_y * mg_y + (int32_t)mg_z * mg_z;

     if (mag_sq < 90000) { if (ff_samples < 255) ff_samples++; } else ff_samples = 0;
     if (ff_samples == 5)                                     /* exactly 50 ms of free fall */
     {
       freefall_count++;
       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
       ff_led_off = HAL_GetTick() + 2000u;
     }
     if (mag_sq > 4000000)                                    /* impact */
     {
       impact_count++;
       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
       imp_led_off = HAL_GetTick() + 200u;
     }
  }
  if (ff_led_off && (int32_t)(HAL_GetTick() - ff_led_off) >= 0) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); ff_led_off = 0;
  }
  if (imp_led_off && (int32_t)(HAL_GetTick() - imp_led_off) >= 0) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET); imp_led_off = 0;
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) sample_due = 1;
}
/* USER CODE END 4 */



/* ############################ S21. Measure the LSI frequency and calculate the IWDG reload ############################ */
/* ===========================================================================
 * S21. Measure the LSI frequency and calculate the IWDG reload
 * Scenario question from the guide (S21)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */
/* USER CODE BEGIN PV */
volatile uint32_t last_cap = 0, delta_ticks = 0, f_lsi_hz = 0, reload_2s = 0;
volatile uint8_t cap_state = 0;               /* 0 = discard first capture, 1 = measuring */
volatile uint8_t meas_done = 0;
static   uint32_t sum_f = 0; static uint8_t n_f = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIMEx_RemapConfig(&htim5, TIM_TIM5_LSI);              /* connect TIM5 channel 4 input to the LSI */
HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_4);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (meas_done)
  {
    meas_done = 0;
    f_lsi_hz = sum_f / 8u;                                  /* average of 8 measurements       */
    reload_2s = (2u * f_lsi_hz) / 64u - 1u;                 /* reload for ~2 s, prescaler 64   */
    HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_4);
    if (f_lsi_hz >= 28000u && f_lsi_hz <= 36000u) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    else                                          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM5 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_4) return;
  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
  if (cap_state == 0) { last_cap = now; cap_state = 1; return; }      /* first capture is only a start point */

  delta_ticks = now - last_cap;                                       /* unsigned: wrap safe */
  last_cap = now;
  if (delta_ticks != 0u) { sum_f += (8u * 16000000u) / delta_ticks; n_f++; }
  if (n_f >= 8u) meas_done = 1;
}
/* USER CODE END 4 */



/* ############################ S22. Countdown timer ############################ */
/* ===========================================================================
 * S22. Countdown timer
 * Scenario question from the guide (S22)
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
volatile int16_t remaining = 0;                /* seconds left                        */
volatile uint8_t finished = 0;                 /* set when the countdown reaches 0    */
static   uint8_t toggles_left = 0;
static   uint32_t t_next = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (finished) { finished = 0; toggles_left = 12; t_next = HAL_GetTick(); }

  if (toggles_left)                                                     /* alarm blinking */
  {
    if ((int32_t)(HAL_GetTick() - t_next) >= 0)
    {
       HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
       toggles_left--;
       t_next += 250u;
       if (toggles_left == 0) leds_show(0x0);
    }
  }
  else if (remaining > 45) leds_show(0xF);
  else if (remaining > 30) leds_show(0x7);
  else if (remaining > 15) leds_show(0x3);
  else if (remaining > 0) leds_show(0x1);
  else                      leds_show(0x0);
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  if (remaining > 0) { remaining--; if (remaining == 0) finished = 1; }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;
  last = HAL_GetTick();
  remaining += 10;
  if (remaining > 60) remaining = 60;                                   /* maximum 60 s */
}
/* USER CODE END 4 */



/* ############################ E1. Two-tone alarm with start/stop button ############################ */
/* ===========================================================================
 * E1. Two-tone alarm with start/stop button
 * EXTRA question (E1)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  alarm_on   = 0;       /* 1 = alarm running                  */
volatile uint8_t  tone       = 0;       /* 0 = PD12+PD14, 1 = PD13+PD15       */
volatile uint32_t tone_count = 0;       /* number of tone changes             */
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
HAL_TIM_Base_Start_IT(&htim3);          /* timer always runs; alarm_on decides */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3 || !alarm_on) return;
  tone ^= 1u;
  tone_count++;
  leds_show(tone ? 0xA : 0x5);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 200u) return;   /* debounce */
  last = HAL_GetTick();

  alarm_on ^= 1u;
  if (alarm_on)
  {
    tone = 0;
    __HAL_TIM_SET_COUNTER(&htim3, 0);   /* first tone gets a full 250 ms */
    leds_show(0x5);
  }
  else
  {
    leds_show(0x0);
  }
}
/* USER CODE END 4 */
/* main loop: while (1) { } */



/* ############################ E2. Debounced press counter (0-15 in binary) ############################ */
/* ===========================================================================
 * E2. Debounced press counter (0-15 in binary)
 * EXTRA question (E2)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  count    = 0;         /* 0..15, shown on the LEDs            */
volatile uint32_t presses  = 0;         /* all accepted presses (no wrap)      */
volatile uint32_t ignored  = 0;         /* edges rejected as bounce            */
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
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0) return;

  uint32_t now = HAL_GetTick();
  if ((now - last) < 100u) { ignored++; return; }       /* bounce */
  last = now;

  presses++;
  count = (uint8_t)((count + 1u) & 0xFu);               /* wraps 15 -> 0 */
  leds_show(count);
}
/* USER CODE END 4 */
/* main loop: while (1) { } */



/* ############################ E3. Sunrise: PWM fade from 0 % to 100 % in exactly 10 s ############################ */
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



/* ############################ E4. Button presses per 10 s using input capture ############################ */
/* ===========================================================================
 * E4. Button presses per 10 s using input capture
 * EXTRA question (E4)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define DEBOUNCE_MS 50u
volatile uint32_t presses_now  = 0;     /* count inside the current window      */
volatile uint32_t presses_last = 0;     /* result of the last finished window   */
volatile uint32_t windows_done = 0;
volatile uint32_t ignored_edges = 0;
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
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);     /* capture the button edges   */
HAL_TIM_Base_Start_IT(&htim3);                  /* 10 s measuring window      */
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  static uint32_t last_edge = 0;
  static uint8_t  have_edge = 0;

  if (htim->Instance != TIM2 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) return;

  uint32_t now = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
  if (have_edge && (now - last_edge) < DEBOUNCE_MS) { ignored_edges++; return; }
  last_edge = now;
  have_edge = 1;
  presses_now++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;             /* TIM2 base interrupt also lands here */

  presses_last = presses_now;
  presses_now  = 0;
  windows_done++;
  leds_show((uint8_t)((presses_last > 15u) ? 15u : presses_last));
}
/* USER CODE END 4 */
/* main loop: while (1) { } */



/* ############################ E5. Temperature trend indicator (rising / steady / falling) ############################ */
/* ===========================================================================
 * E5. Temperature trend indicator (rising / steady / falling)
 * EXTRA question (E5)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define N_LOG     10u
#define STEADY_C  0.5f
volatile float    temp_log[N_LOG];
volatile uint8_t  log_head  = 0;        /* next slot to write                   */
volatile uint8_t  log_count = 0;        /* how many slots hold real data        */
volatile float    temp_c    = 0.0f, trend_c = 0.0f;
volatile uint8_t  trend     = 0;        /* 0 steady, 1 rising, 2 falling        */
volatile uint8_t  sample_due = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static void leds_show(uint8_t mask)          /* bit0..3 -> PD12..PD15, HAL only */
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (uint16_t)((mask & 0xFu) << 12), GPIO_PIN_SET);
}

static uint16_t adc_read_once(void)
{
  uint16_t v = 0;
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) v = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  return v;
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
leds_show(0x0);
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (sample_due)
  {
    sample_due = 0;

    uint32_t sum = 0;
    for (int i = 0; i < 8; i++) sum += adc_read_once();
    float raw = (float)(sum / 8u);
    temp_c = ((raw * 3.3f / 4095.0f) - 0.76f) / 0.0025f + 25.0f;

    temp_log[log_head] = temp_c;                         /* store            */
    log_head = (uint8_t)((log_head + 1u) % N_LOG);
    if (log_count < N_LOG) log_count++;

    if (log_count == N_LOG)                              /* buffer is full   */
    {
      float oldest = temp_log[log_head];                 /* next to overwrite = oldest */
      float newest = temp_log[(log_head + N_LOG - 1u) % N_LOG];
      trend_c = newest - oldest;

      if      (trend_c >  STEADY_C) { trend = 1; leds_show(0x1); }   /* PD12 rising  */
      else if (trend_c < -STEADY_C) { trend = 2; leds_show(0x4); }   /* PD14 falling */
      else                          { trend = 0; leds_show(0x2); }   /* PD13 steady  */
    }
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) sample_due = 1;
}
/* USER CODE END 4 */



/* ############################ E6. Press-pattern recorder and replay ############################ */
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



/* ############################ E7. Low-power blinker (Sleep mode between timer ticks) ############################ */
/* ===========================================================================
 * E7. Low-power blinker (Sleep mode between timer ticks)
 * EXTRA question (E7)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint32_t tick_cnt   = 0;       /* counts 100 ms ticks 0..10            */
volatile uint32_t wake_count = 0;       /* total number of wake-ups             */
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_DBGMCU_EnableDBGSleepMode();        /* keep the debugger connected in Sleep */
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  HAL_SuspendTick();                                              /* SysTick would wake the CPU every ms */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);   /* sleep until an interrupt            */
  HAL_ResumeTick();                                               /* awake again                         */
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  wake_count++;
  tick_cnt++;
  if (tick_cnt >= 10u) { tick_cnt = 0; HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); }   /* ON  */
  else if (tick_cnt == 1u)             HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);   /* OFF */
}
/* USER CODE END 4 */



/* ############################ E8. ADC + DMA temperature data logger (average, minimum, maximum) ############################ */
/* ===========================================================================
 * E8. ADC + DMA temperature data logger (average, minimum, maximum)
 * EXTRA question (E8)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
#define N_SMP 64u
volatile uint16_t smp[N_SMP];           /* filled by DMA                          */
volatile uint8_t  buf_ready = 0;
volatile uint16_t raw_min = 0, raw_max = 0, raw_avg = 0;
volatile float    t_min = 0.0f, t_max = 0.0f, t_avg = 0.0f;
volatile uint32_t buffers_done = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
static float raw_to_c(uint16_t raw)
{
  float v = (float)raw * 3.3f / 4095.0f;
  return (v - 0.76f) / 0.0025f + 25.0f;
}

static void analyse(void)
{
  uint32_t sum = 0;
  uint16_t mn = 4095u, mx = 0u;
  for (uint32_t i = 0; i < N_SMP; i++)
  {
    uint16_t s = smp[i];
    sum += s;
    if (s < mn) mn = s;
    if (s > mx) mx = s;
  }
  raw_avg = (uint16_t)(sum / N_SMP);
  raw_min = mn;
  raw_max = mx;
  t_avg = raw_to_c(raw_avg);
  t_min = raw_to_c(mn);
  t_max = raw_to_c(mx);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_DMA(&hadc1, (uint32_t *)smp, N_SMP);   /* arm ADC + DMA, wait for trigger */
HAL_TIM_Base_Start(&htim3);                          /* TRGO every 50 ms                */
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (buf_ready)
  {
    buf_ready = 0;
    analyse();
    buffers_done++;
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);          /* heartbeat: one toggle per 3.2 s */
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)   /* buffer completely filled */
{
  if (hadc->Instance == ADC1) buf_ready = 1;
}
/* USER CODE END 4 */



/* ############################ E9. Pedestrian crossing controller ############################ */
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



/* ############################ E10. Watchdog with two health flags ############################ */
/* ===========================================================================
 * E10. Watchdog with two health flags
 * EXTRA question (E10)
 * Board: STM32F407G-DISC1 | IDE: STM32CubeIDE | HAL only (no register code)
 * HOW TO USE: create a CubeMX project with the settings in question.md, generate
 * code, then copy each block below into the SAME  USER CODE BEGIN/END  markers of
 * main.c. This file is a set of snippets, it is not compiled on its own.
 * =========================================================================== */

/* USER CODE BEGIN PV */
volatile uint8_t  hb_flag       = 0;      /* set by TIM3 every 100 ms            */
volatile uint8_t  btn_flag      = 0;      /* set by PA0 (operator alive)         */
volatile uint8_t  window_ticks  = 0;      /* 100 ms ticks in the current window  */
volatile uint8_t  reset_by_iwdg = 0;
volatile uint32_t refresh_count = 0, missed_windows = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
reset_by_iwdg = (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) ? 1u : 0u;   /* read first ...  */
__HAL_RCC_CLEAR_RESET_FLAGS();                                              /* ... then clear  */

if (reset_by_iwdg)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  for (int i = 0; i < 30; i++) { HAL_Delay(100); HAL_IWDG_Refresh(&hiwdg); }   /* 3 s, keep the IWDG fed */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
  if (window_ticks >= 20u)                          /* 2 s window finished */
  {
    window_ticks = 0;
    if (hb_flag && btn_flag)
    {
      HAL_IWDG_Refresh(&hiwdg);
      refresh_count++;
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    }
    else
    {
      missed_windows++;                             /* no refresh: the IWDG will reset the chip */
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    }
    hb_flag = 0;
    btn_flag = 0;
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM3) return;
  hb_flag = 1;
  if (window_ticks < 255u) window_ticks++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last = 0;
  if (GPIO_Pin != GPIO_PIN_0 || (HAL_GetTick() - last) < 100u) return;
  last = HAL_GetTick();
  btn_flag = 1;
}
/* USER CODE END 4 */
