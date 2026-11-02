# E1. Two-tone alarm with start/stop button

**Task:** A two-tone alarm uses the four onboard LEDs. While the alarm is running, PD12+PD14 light for 250 ms, then PD13+PD15 light for 250 ms, and the two pairs keep alternating. Each press of the User button (PA0) starts the alarm if it is stopped, or stops it (all LEDs off) if it is running. Use a timer interrupt for the 250 ms timing. No HAL_Delay in the alarm logic. Count the tone changes.

## 1. Configuration
```text
Item                    Setting                                              Reason
PD12-PD15               GPIO_Output, push-pull, initial Low, no pull         Onboard LEDs
TIM3                    Internal Clock, PSC 15999, ARR 249, global           250 ms interrupt
                        interrupt enabled
PA0                     GPIO_EXTI0, rising edge, no pull; NVIC EXTI0         Start / stop button
                        enabled, priority 1
SysTick priority        0 (more urgent than EXTI0)                           HAL_GetTick works in callback
SYS                     Debug = Serial Wire; clock HSI 16 MHz, no PLL
```

## 2. Calculations
```text
(PSC + 1) * (ARR + 1) = 16 000 * 250 = 4 000 000 cycles
T = 4 000 000 / 16 MHz = 0.25 s = 250 ms
LED masks (bit0..3 = PD12..PD15):
   tone 0 : PD12 + PD14 -> 0101b = 0x5
   tone 1 : PD13 + PD15 -> 1010b = 0xA
```

## 3. Program flow
TIM3 runs all the time. Its callback does nothing while alarm_on = 0. When alarm_on = 1 it flips 'tone'
and writes the matching mask. The button callback only toggles alarm_on (after a 200 ms debounce), restarts
the timer counter so the first tone lasts a full 250 ms, and clears the LEDs when stopping.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: alarm_on, tone, tone_count (rises 4 per second while running).

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Power up | All LEDs OFF, alarm_on = 0 | | |
| Press PA0 once | PD12+PD14 ON at once, alarm_on = 1 | | |
| After 250 ms | PD13+PD15 ON, tone_count = 1 | | |
| After 1 s | Pairs have alternated 4 times | | |
| Press PA0 again | All LEDs OFF, alarm stops, tone_count frozen | | |
| Press again | Alarm restarts with PD12+PD14 | | |

