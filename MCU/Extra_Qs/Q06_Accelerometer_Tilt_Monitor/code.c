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
