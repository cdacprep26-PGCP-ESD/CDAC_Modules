# STM32F407 Scenario Questions Pack

Everything is **HAL only** (no register-level code), for the STM32F407G-DISC1 board in STM32CubeIDE.

## What is inside
- `All_Scenario_Codes.c` : one .c file with the code of **every** question (Q1-Q10, S11-S22, E1-E10).
- One folder per question, each with
  - `question.md` : the task, the **CubeMX configuration**, calculations, program flow, what to observe and a test table
  - `code.c` : the code for that question (USER CODE snippets)
- `Extra_Questions_Practice.md` : the 10 extra questions as a clean practice sheet (no solutions) plus 10 more prompts to solve yourself.

Q1-Q10 and S11-S22 are taken from your Lab Exam Guide. **E1-E10 are new extra questions** with full configuration, code and tests.

## How to use one question
1. STM32CubeIDE: File > New > STM32 Project > Board Selector > STM32F407G-DISC1. Initialize all peripherals with default mode = **No**.
2. SYS > Debug = Serial Wire. Clock: HSI 16 MHz, no PLL (all calculations assume 16 MHz).
3. Apply the pins, timers and NVIC settings from `question.md`. Save the .ioc to generate code.
4. Copy each block from `code.c` into the matching `USER CODE BEGIN / END` markers in `main.c`. Never write outside the markers.
5. Build, debug, add the listed variables to Live Expressions, and fill the test table.

## Index
| ID | Question | Folder |
|---|---|---|
| Q1 | Q1. Traffic signal timing using a TIM2 interrupt | `Q01_Traffic_Signal_TIM2/` |
| Q2 | Q2. Onboard LED brightness indication using PWM (TIM4 CH1) | `Q02_PWM_Brightness_TIM4/` |
| Q3 | Q3. Four-stage sequence using output compare (TIM4) | `Q03_Output_Compare_Sequence/` |
| Q4 | Q4. Independent LED toggle rates using output-compare interrupts | `Q04_Independent_Toggle_Rates/` |
| Q5 | Q5. Measure the user-button interval using input capture (TIM2 CH1) | `Q05_Input_Capture_Button_Interval/` |
| Q6 | Q6. Onboard accelerometer tilt safety monitor | `Q06_Accelerometer_Tilt_Monitor/` |
| Q7 | Q7. Internal temperature monitor using ADC and DMA | `Q07_ADC_DMA_Temperature/` |
| Q8 | Q8. RTC-based access event logger | `Q08_RTC_Event_Logger/` |
| Q9 | Q9. DMA-driven onboard LED pattern player (timer update to GPIOD BSRR) | `Q09_DMA_LED_Pattern_BSRR/` |
| Q10 | Q10. Watchdog fault-recovery controller (IWDG) | `Q10_IWDG_Fault_Recovery/` |
| S11 | S11. Metronome with selectable tempo | `S11_Metronome/` |
| S12 | S12. Reaction-time game | `S12_Reaction_Game/` |
| S13 | S13. Stopwatch with the User button | `S13_Stopwatch/` |
| S14 | S14. Four LEDs with different brightness using PWM (visible output) | `S14_PWM_Four_LEDs/` |
| S15 | S15. Short press and long press detection | `S15_Short_Long_Press/` |
| S16 | S16. Simple temperature indicator with single-conversion ADC (polling) | `S16_ADC_Polling_Temperature/` |
| S17 | S17. RTC alarm every minute | `S17_RTC_Alarm/` |
| S18 | S18. Memory-to-memory DMA copy and verification | `S18_DMA_Memory_Copy/` |
| S19 | S19. Sleep mode until the button is pressed | `S19_Sleep_Mode_Wakeup/` |
| S20 | S20. Free-fall and impact detector (accelerometer) | `S20_FreeFall_Impact/` |
| S21 | S21. Measure the LSI frequency and calculate the IWDG reload | `S21_LSI_Measure_IWDG/` |
| S22 | S22. Countdown timer | `S22_Countdown_Timer/` |
| E1 | E1. Two-tone alarm with start/stop button | `E01_Two_Tone_Alarm/` |
| E2 | E2. Debounced press counter (0-15 in binary) | `E02_Press_Counter_Binary/` |
| E3 | E3. Sunrise: PWM fade from 0 % to 100 % in exactly 10 s | `E03_Sunrise_PWM_Fade/` |
| E4 | E4. Button presses per 10 s using input capture | `E04_Presses_Per_10s_Capture/` |
| E5 | E5. Temperature trend indicator (rising / steady / falling) | `E05_Temperature_Trend/` |
| E6 | E6. Press-pattern recorder and replay | `E06_Press_Pattern_Recorder/` |
| E7 | E7. Low-power blinker (Sleep mode between timer ticks) | `E07_Low_Power_Blinker/` |
| E8 | E8. ADC + DMA temperature data logger (average, minimum, maximum) | `E08_ADC_DMA_Logger/` |
| E9 | E9. Pedestrian crossing controller | `E09_Pedestrian_Crossing/` |
| E10 | E10. Watchdog with two health flags | `E10_IWDG_Two_Flags/` |

## Common helper
Many S and E programs use `leds_show(mask)` (bit0..3 = PD12..PD15). It is included in the `code.c` of each question that needs it.
