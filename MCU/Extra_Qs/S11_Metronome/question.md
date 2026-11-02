# S11. Metronome with selectable tempo

**Task:** PD12 flashes briefly on every beat. Each press of the User button cycles the tempo 60, 90, 120 beats per minute; PD13, PD14, PD15 show which tempo is active.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
TIM3                              Internal Clock, PSC 15999 (1 ms tick), ARR 999 (60 BPM), interrupt enabled
PA0                               GPIO_EXTI0, rising edge, no pull; NVIC EXTI0 priority 1
PD12-PD15                         GPIO_Output, initial Low
```

### Calculation

```text
 beat period = 60 s / BPM:   60 BPM -> 1000 ms    90 BPM -> 666.7 ms (667 ticks)      120 BPM -> 500 ms
 ARR = ticks - 1 :           999                   666                                 499
 error at 90 BPM: 0.3 ms per beat (0.05 %), acceptable
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• beat_count rises 1 per second at 60 BPM, 1.5 per second at 90, 2 per second at 120.
• Why reset the counter? If the counter is already above the new, smaller ARR it would have to count up to
     65 535 before wrapping, giving a very long first beat.
```
