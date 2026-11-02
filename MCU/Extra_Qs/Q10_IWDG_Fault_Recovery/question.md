# Q10. Watchdog fault-recovery controller (IWDG)

**Task:** Build a watchdog fault-recovery controller: detect the reset cause, show an IWDG reset on PD14 for 3 s, refresh the IWDG only when health checks pass, and create a deliberate fault by holding PA0 for 3 s.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                         Setting                                                   Reason
IWDG                         Activated, Prescaler 64, Reload 999                       Timeout = 1000 x 64 / 32 kHz = 2.0 s
                                                                                       (nominal LSI)
TIM3                         Internal Clock, PSC 15999, ARR 99, global interrupt       100 ms heartbeat
                             enabled
PA0                          GPIO_Input, no pull                                       Held 3 s = deliberate fault

Item                       Setting                                                    Reason
PD12-PD15                  GPIO_Output, initial Low                                   PD12 healthy, PD13 live blink, PD14
                                                                                      reset cause
```

### Timeout calculation (write both)

```text
 Timeout = (Reload + 1) * Prescaler / f_LSI = 1000 * 64 / f_LSI
 f_LSI = 32 kHz (nominal): 2.00 s      f_LSI = 40 kHz: 1.60 s      f_LSI = 17 kHz: 3.76 s
 The LSI is an RC oscillator, so the real timeout is between about 1.4 s and 3.8 s.
 To get 2 s exactly you must measure the LSI (TIM5 CH4 with TI4_RMP = LSI) and recompute Reload.
```

### Program flow

```text
1. Read the RCC reset flags first, store the cause in reset_cause, then clear the flags.
2. If the cause was IWDG: PD14 ON for 3 s. The watchdog is already running, so the code refreshes it every
  100 ms during this 3 s wait (a plain HAL_Delay(3000) would trigger another reset!).
3. Start TIM3. Every 100 ms its callback sets heartbeat, and counts how long PA0 is held.
4. Main loop: when heartbeat is set and all health checks pass, refresh the IWDG. Every 500 ms toggle
  PD13.
5. If PA0 is held 30 heartbeats (3 s), fault_mode = 1: main() stops refreshing (stalls in an endless loop). The
  IWDG expires within about 2 s and resets the chip; step 1 then shows PD14 for 3 s.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
How to run the test correctly (spec items 7 and 8)
• Healthy test: Run the program (green Run button or Resume) and leave it for 30 s: PD12 steady, PD13
  blinks every 0.5 s, wdg_refresh_count rises 10 per second, reset_cause does not change.
• Fault test: hold PA0 for 3 s. PD12 turns off, PD13 stops, and about 2 s later the board resets. PD14 then
  lights for 3 s and reset_cause = RST_IWDG (3). Release the button after the reset, otherwise the 3 s hold
  starts again.
• Debugger caution: a breakpoint or pause freezes the IWDG counter in the debug configuration. If you
  stop at a breakpoint, the watchdog will not fire, and when you resume it continues counting from where it
  stopped. A paused debugger is therefore not proof that the system is healthy and cannot be used to test
  the reset. Always judge the watchdog with the program running freely (or with the board running
  stand-alone after programming).
Test condition                          Expected result                                        Actual         Pass/Fail
Healthy for 60 s                        no reset, wdg_refresh_count about 600
Hold PA0 3 s                            refresh stops, reset within about 2 s
After reset                             PD14 ON 3 s, reset_cause = 3 (IWDG), then
                                        PD12 ON and PD13 blinks
Power-cycle                             reset_cause = power (1), PD14 stays OFF
Press the black RESET button            reset_cause = pin (2)
Pause at a breakpoint 5 s               no reset (IWDG frozen); not proof of health

Onboard note: IWDG and LSI are internal; PA0 and LEDs are onboard; no external fault switch.
```
