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
