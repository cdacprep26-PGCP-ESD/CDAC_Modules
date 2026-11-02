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
