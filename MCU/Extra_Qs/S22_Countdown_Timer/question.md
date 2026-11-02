# S22. Countdown timer

**Task:** Every press of the User button adds 10 s to a countdown (maximum 60 s). The countdown decreases once per second. The LEDs show the remaining time: more than 45 s: all four; more than 30 s: three; more than 15 s: two; more than 0: one. When it reaches 0 all LEDs blink at 2 Hz for 3 s, then turn off.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
TIM3                              Internal Clock, PSC 15999, ARR 999 (1 s), interrupt enabled; started in main
PA0                               GPIO_EXTI0, rising edge, no pull; NVIC EXTI0 enabled
PD12-PD15                         GPIO_Output
```

### Calculation

```text
 1 s interrupt: (15999 + 1) * (999 + 1) / 16 MHz = 1 s             remaining -= 1 each interrupt
 Blink at 2 Hz = toggle every 250 ms; 3 s = 12 toggles
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Press 3 times: remaining = 30, three LEDs lit, decreasing every second. At 0: 3 s of blinking, then all LEDs
  off.
• Test boundaries: 61 s attempt (clamped to 60), pressing again during the countdown, pressing during the
  blink phase.
```
