# E9. Pedestrian crossing controller

**Task:** Cars normally have GREEN (PD12). When a pedestrian presses the User button, the request is remembered, but the light may only change after the green has lasted at least 5 s. Then: ORANGE PD13 for 2 s, then RED PD14 together with the blue WALK light PD15 for 5 s, then back to GREEN. Button presses during orange, red or walk are ignored. Count completed crossings.

## 1. Configuration
```text
Item                    Setting                                              Reason
TIM3                    Internal Clock, PSC 15999, ARR 99, global            100 ms state tick
                        interrupt enabled
PA0                     GPIO_EXTI0, rising edge, no pull; NVIC EXTI0         Pedestrian request
                        enabled
PD12 / PD13 / PD14 /    GPIO_Output, initial Low                             Green / orange / red / walk
PD15
```

## 2. Calculations
```text
Tick = 100 ms        min green = 5 s / 0.1 s = 50 ticks
orange = 2 s = 20 ticks       red + walk = 5 s = 50 ticks
masks: GREEN 0x1 (PD12) | ORANGE 0x2 (PD13) | WALK 0xC (PD14 + PD15)
```

## 3. Program flow
State machine in the TIM3 callback. GREEN: count ticks; if ped_request and ticks >= 50 go to ORANGE.
ORANGE: after 20 ticks go to WALK and clear the request. WALK: after 50 ticks go back to GREEN. The button callback
sets ped_request only while the state is GREEN.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: xs, ticks, ped_request, crossings.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Power up | PD12 (green) ON | | |
| Press at 2 s of green | Nothing yet; orange starts at 5 s of green | | |
| Press at 10 s of green | Orange starts at the next tick | | |
| Orange 2 s | Then PD14 + PD15 ON for 5 s | | |
| Press during orange/red | Ignored, ped_request stays 0 | | |
| After the walk phase | Green again, crossings = 1 | | |

