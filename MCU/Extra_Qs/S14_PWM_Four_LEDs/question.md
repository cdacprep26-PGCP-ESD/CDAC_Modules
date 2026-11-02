# S14. Four LEDs with different brightness using PWM (visible output)

**Task:** Use TIM4 channels 1-4 so that the timer itself drives PD12-PD15 and the four LEDs have different brightness. Each press of PA0 switches between three brightness profiles.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
TIM4                              Internal Clock, Channel1-4 = PWM Generation CH1-CH4, PSC 15, ARR 999
Pins                              Click PD12 > TIM4_CH1, PD13 > TIM4_CH2, PD14 > TIM4_CH3, PD15 > TIM4_CH4
                                  (AF2). The pins PB6-PB9 become free. They are not GPIO_Output here
PWM channels                      Mode PWM mode 1, Pulse 0, Polarity High, Output Compare Preload Enable
PA0                               GPIO_EXTI0, rising edge, no pull
```

### Calculation

```text
 f_pwm = 16 MHz / (16 * 1000) = 1 kHz                      CCR = duty * 1000
 Profile 0: 10 %, 25 %, 50 %, 90 % -> CCR 100, 250, 500, 900
 Profile 1: 90 %, 50 %, 25 %, 10 % -> CCR 900, 500, 250, 100
 Profile 2: all 50 %                -> CCR 500 each
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Brightness difference is clearly visible between 10 % and 90 %. The eye is not linear, so 50 % looks brighter
  than half.
• Read the real compare values with __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1) in Live Expressions.
```
