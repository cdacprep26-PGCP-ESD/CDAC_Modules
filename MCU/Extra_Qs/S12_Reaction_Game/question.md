# S12. Reaction-time game

**Task:** PD12 shows 'ready'. After a random delay of 2 to 5 s PD15 lights and a stopwatch starts. The user presses PA0 as fast as possible. Show the reaction time with the LEDs (under 200 ms: all four, under 300 ms: three, under 500 ms: two, otherwise one) for 3 s, then play again. A press before PD15 lights is a false start (PD14 for 1 s).

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                                Setting
TIM2                                Internal Clock, PSC 15999 (1 ms tick), ARR 4294967295, no interrupt; started with
                                    HAL_TIM_Base_Start
PA0                                 GPIO_EXTI0, rising edge, no pull; NVIC enabled
PD12-PD15                           GPIO_Output
```

### Calculation

```text
 reaction_ms = t_press - t_go            (both are TIM2 counter values in ms, unsigned subtraction)
 random delay = 2000 + (something unpredictable) % 3000      -> 2000..4999 ms
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Watch reaction_ms, st, rounds. A typical human reaction is 200-300 ms.
• The stopwatch value is taken in the callback, so the main loop speed does not affect the measurement.
```
