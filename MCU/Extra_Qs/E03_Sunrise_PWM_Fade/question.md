# E3. Sunrise: PWM fade from 0 % to 100 % in exactly 10 s

**Task:** PD12 must be driven by hardware PWM (TIM4 CH1). After start-up its brightness rises smoothly from 0 % to 100 % in exactly 10 seconds and then stays at 100 %. Calculate the PWM period, the CCR step and the step timer. Use a timer interrupt for the steps; no delay loops.

## 1. Configuration
```text
Item                    Setting                                              Reason
PD12                    TIM4_CH1 (AF2): click PD12 in the pinout and            The timer itself drives the LED
                        choose TIM4_CH1. PB6 becomes free
TIM4                    Internal Clock, Channel1 = PWM Generation CH1,          1 kHz PWM, 1000 steps
                        PSC 15, ARR 999
TIM4 CH1                PWM mode 1, Pulse 0, polarity High, OC preload Enable   Glitch-free duty changes
TIM3 (step timer)       Internal Clock, PSC 15999, ARR 9, global interrupt      10 ms tick
                        enabled
PD13-PD15               not used
```

## 2. Calculations
```text
PWM frequency = 16 MHz / ((15 + 1) * (999 + 1)) = 1000 Hz        duty = CCR / 1000
Step timer    = 16 000 * 10 / 16 MHz = 10 ms
Ticks in 10 s = 10 s / 10 ms = 1000
CCR step      = 1000 counts / 1000 ticks = +1 per tick
CCR = 1000 with ARR = 999 means the compare never matches the period end, so the output stays high: 100 %.
```

## 3. Program flow
TIM3 interrupts every 10 ms. The callback adds 1 to the CCR (and writes it with __HAL_TIM_SET_COMPARE).
After 1000 ticks CCR = 1000 (100 %). The step timer is then stopped, so the LED stays at full brightness.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: ccr, ticks, duty_pct, fade_done; also __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1).

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Start | LED OFF (CCR = 0) | | |
| After 5 s | ccr = 500, duty_pct = 50, LED half bright | | |
| After 10 s | ccr = 1000, fade_done = 1, LED full | | |
| After 12 s | Still full, no more interrupts | | |
| Stopwatch check | Fade takes 10 s +/- HSI tolerance (about 1 %) | | |

