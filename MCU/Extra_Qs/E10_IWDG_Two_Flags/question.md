# E10. Watchdog with two health flags

**Task:** The IWDG may only be refreshed when both health flags are set in a 2 s window: (1) the 100 ms heartbeat timer is running and (2) the operator pressed PA0 at least once ("operator alive"). If a window ends without both flags, the watchdog is not refreshed and the chip resets. After an IWDG reset PD14 shows the reason for 3 s. PD12 = healthy, PD13 toggles after every successful refresh. Test by not pressing the button.

## 1. Configuration
```text
Item                    Setting                                              Reason
IWDG                    Activated, Prescaler 64, Reload 1999                 Timeout = 2000 x 64 / 32 kHz = 4 s
TIM3                    Internal Clock, PSC 15999, ARR 99, global            100 ms heartbeat
                        interrupt enabled
PA0                     GPIO_EXTI0, rising edge, no pull; NVIC EXTI0         "Operator alive" flag
                        enabled
PD12-PD15               GPIO_Output, initial Low                             Status LEDs
```

## 2. Calculations
```text
Timeout = (Reload + 1) * Prescaler / f_LSI = 2000 * 64 / f_LSI
f_LSI = 32 kHz: 4.00 s     40 kHz: 3.20 s     47 kHz: 2.72 s     17 kHz: 7.53 s
The software window is 2 s (20 heartbeats) and must be shorter than the SHORTEST possible timeout (2.7 s).
If one window is missed, the IWDG expires between about 2.7 s and 7.5 s after the last good refresh.
```

## 3. Program flow
Start-up: read and clear the RCC reset flags. If the cause was the IWDG, show PD14 for 3 s while feeding the
watchdog (it is already running). Then start TIM3. TIM3 sets hb_flag and counts window_ticks. When window_ticks
reaches 20 the main loop checks both flags: both set -> refresh, PD13 toggles, PD12 ON; otherwise missed_windows++
and PD12 OFF (no refresh). Flags are cleared for the next window.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: hb_flag, btn_flag, window_ticks, refresh_count, missed_windows, reset_by_iwdg. Run freely (not paused at a breakpoint) to see the reset.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Press PA0 every 1-2 s | No reset, refresh_count rises 1 per 2 s, PD13 toggles | | |
| Stop pressing | PD12 OFF after the first missed window; reset about 3-7 s after the last refresh | | |
| After reset | PD14 ON for 3 s, reset_by_iwdg = 1, then PD12 ON | | |
| Power cycle | reset_by_iwdg = 0, PD14 stays OFF | | |
| Breakpoint pause | IWDG is frozen in debug: no reset; not proof of health | | |

