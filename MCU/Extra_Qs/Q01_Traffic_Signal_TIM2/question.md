# Q1. Traffic signal timing using a TIM2 interrupt

**Task:** Make a traffic signal on the onboard LEDs: green (PD12) for 2 s, then red (PD14) for 1 s, repeating forever. Use a TIM2 timer interrupt for all timing (no delay loops). PD13 and PD15 stay off. Count complete cycles.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting                                            Reason
PD12, PD13, PD14, PD15            GPIO_Output, push-pull, initial Low, no pull       Onboard LEDs. PD12 = GO (green),
                                                                                     PD14 = STOP (red), PD13/PD15 stay
                                                                                     OFF
TIM2                              Clock source Internal Clock, Counter Mode Up       Counts the 16 MHz clock
TIM2 Prescaler                    15999                                              16 MHz / 16 000 = 1 kHz tick (1 ms)
TIM2 Counter Period (ARR)         99                                                 100 ticks = 100 ms
TIM2 NVIC                         TIM2 global interrupt enabled                      Update event must reach the CPU
```

### Calculation

```text
 (PSC + 1) * (ARR + 1) = f_timer * T = 16 000 000 * 0.1 s = 1 600 000
 PSC = 15999 -> 16 000
 ARR = 99     -> 100           16 000 * 100 = 1 600 000     T = 100 ms
 GREEN: 2 s / 100 ms = 20 interrupts        RED: 1 s / 100 ms = 10 interrupts
```

### Program flow

```text
Start: GREEN on, tick100 = 0. Each 100 ms the callback increments tick100. In state GREEN, when tick100
reaches 20 the program switches to RED and clears the counter. In state RED, when it reaches 10 the program
switches back to GREEN. A single traffic_set() function writes the LEDs, so only one LED state is ever active.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe and test
Live Expressions: phase, tick100, cycles_done. tick100 counts 0 to 19 in GREEN, then 0 to 9 in RED. With a
stopwatch the green lasts 2 s and the red 1 s.

Test condition                         Expected result                                      Actual           Pass/Fail
Power up                               Green ON immediately, red OFF
After 2.0 s                            Green OFF, red ON, phase = 1
After 3.0 s total                      Red OFF, green ON, cycles_done = 1
Boundary: tick100 = 19 and 20          Switch happens exactly at 20
Run 1 minute                           20 cycles (60 s / 3 s)

Onboard note: uses only PD12, PD14 (LEDs), TIM2 and the NVIC; no external hardware.
```
