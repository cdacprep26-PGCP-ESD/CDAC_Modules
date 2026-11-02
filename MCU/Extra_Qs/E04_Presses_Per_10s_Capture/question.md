# E4. Button presses per 10 s using input capture

**Task:** Count how many times the User button is pressed in every 10-second window using TIM2 input capture on PA0. At the end of each window, show the count in binary on the four LEDs (values above 15 show 15) and start a new window. Ignore edges closer than 50 ms (bounce).

## 1. Configuration
```text
Item                    Setting                                              Reason
PA0                     TIM2_CH1 (AF1), no pull                              Board has external pull-down
TIM2                    Internal Clock, Channel1 = Input Capture direct      Hardware time stamp
                        mode, PSC 15999, ARR 4294967295
IC channel 1            Rising edge, Direct TI, prescaler No division,       Filter removes spikes
                        Filter 15
TIM2 NVIC               TIM2 global interrupt enabled                        Capture interrupt
TIM3 (window)           Internal Clock, PSC 15999, ARR 9999, global          10 s window
                        interrupt enabled
PD12-PD15               GPIO_Output, initial Low                             Result display
```

## 2. Calculations
```text
TIM2 tick = 16 000 / 16 MHz = 1 ms          debounce: now - last_edge < 50 ticks = bounce
Window    = (15999 + 1) * (9999 + 1) / 16 MHz = 10 s
Display   = min(count, 15) as binary.   Example: 6 presses -> 0110b -> PD13 + PD14
```

## 3. Program flow
Capture callback (TIM2): read the captured time stamp, reject bounce, otherwise increment presses_now.
Window callback (TIM3, every 10 s): copy presses_now to presses_last, clear presses_now, update the LEDs.
Both callbacks are short and use different timer instances.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: presses_now, presses_last, windows_done, ignored_edges.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Press 3 times in 10 s | After the window: presses_last = 3, LEDs 0011b (PD12 + PD13) | | |
| No press in a window | presses_last = 0, all LEDs OFF | | |
| 20 presses in a window | LEDs show 1111b (limited to 15) | | |
| Bouncing | ignored_edges rises, count stays correct | | |
| Press just after a window ends | Counted in the new window | | |

