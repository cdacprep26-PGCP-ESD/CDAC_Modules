# E7. Low-power blinker (Sleep mode between timer ticks)

**Task:** PD12 must flash once per second (ON for 100 ms, OFF for 900 ms). Between the timer ticks the processor must stay in Sleep mode. Use a 100 ms timer interrupt as the wake-up source. Explain which interrupts can wake the CPU.

## 1. Configuration
```text
Item                    Setting                                              Reason
TIM3                    Internal Clock, PSC 15999, ARR 99, global            100 ms wake-up tick
                        interrupt enabled
PD12                    GPIO_Output, initial Low                             Flashing LED
SYS                     Debug = Serial Wire                                  Debugger stays connected
```

## 2. Calculations
```text
Tick = 16 000 * 100 / 16 MHz = 100 ms
tick_cnt counts 1..10:  tick 10 -> LED ON (tick_cnt reset to 0);  next tick (count 1) -> LED OFF
ON time = 100 ms, period = 10 x 100 ms = 1 s
```

## 3. Program flow
The main loop only does: suspend SysTick, WFI (Sleep), resume SysTick. SysTick is suspended because its 1 ms
interrupt would wake the CPU 1000 times per second. Only TIM3 (and any other enabled interrupt such as EXTI)
wakes the core; the callback updates the LED, then the CPU sleeps again.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: tick_cnt, wake_count. Wake-up sources: every enabled interrupt (TIM3 here, SysTick only if not suspended, EXTI0 if enabled). WFI vs WFE: WFI wakes on an interrupt that is enabled in the NVIC.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Run | PD12 flashes once per second, 100 ms ON | | |
| wake_count | Increases by 10 per second | | |
| Debugger | Stays connected (DBGMCU sleep bit set) | | |
| Remove HAL_SuspendTick() | CPU wakes every 1 ms, sleep saves almost nothing (observe wake-ups) | | |

