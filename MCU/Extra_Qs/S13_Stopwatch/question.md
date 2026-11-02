# S13. Stopwatch with the User button

**Task:** First press starts a stopwatch, second press stops it, third press resets it. While running, the four LEDs show the elapsed whole seconds as a binary number (0 to 15, then it wraps). The exact time in milliseconds is read in the debugger.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                               Setting
TIM3                               Internal Clock, PSC 15999, ARR 99 (100 ms), interrupt enabled
PA0                                GPIO_EXTI0, rising edge, no pull
PD12-PD15                          GPIO_Output
```

### Calculation

```text
 interrupt every 100 ms -> elapsed_ms = ticks100 * 100          seconds = ticks100 / 10
 LEDs = seconds mod 16 in binary: 5 s = 0101 -> PD12 and PD14 ON ; 13 s = 1101 -> PD12, PD14, PD15
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Compare elapsed_ms with a phone stopwatch; accuracy is about 100 ms (the tick) plus the HSI tolerance.
• Improve the resolution by using ARR = 9 (10 ms tick) and dividing by 100 for seconds.
```
