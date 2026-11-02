# E6. Press-pattern recorder and replay

**Task:** Record four button presses as a 4-bit code: a press shorter than 1 s is bit 0 (short), a press of 1 s or longer is bit 1 (long); presses shorter than 30 ms are bounce and ignored. PD13 toggles after every accepted press as feedback. After the fourth press the code is replayed with a timer: one bit every 500 ms, green PD12 for a 1 and red PD14 for a 0 (first recorded bit first). Then all LEDs go off and recording starts again. Presses during the replay are ignored.

## 1. Configuration
```text
Item                    Setting                                              Reason
PA0                     GPIO_EXTI0, Rising/Falling edge trigger, no pull;    Press and release both needed
                        NVIC EXTI0 enabled
TIM3                    Internal Clock, PSC 15999, ARR 499, global           500 ms replay step
                        interrupt enabled (started only for the replay)
PD12-PD15               GPIO_Output, initial Low                             PD12 = 1, PD14 = 0, PD13 = feedback
SysTick priority        0                                                    HAL_GetTick in the callback
```

## 2. Calculations
```text
press length = release time - press time (HAL_GetTick, ms)
< 30 ms : bounce      30..999 ms : bit 0      >= 1000 ms : bit 1
code |= (bit << n_rec)     bit 0 of 'code' is the first press
Replay step = 16 000 * 500 / 16 MHz = 500 ms; 4 bits = 2 s, the 5th tick clears the LEDs
Example: short, long, long, short -> bits 0,1,1,0 -> code = 0110b = 0x6
```

## 3. Program flow
State REC: the EXTI callback measures each press and stores the bit. After the 4th bit the state changes to
REPLAY and TIM3 is started. State REPLAY: every TIM3 tick shows the next bit; after four bits the LEDs are
cleared, TIM3 stops and the state returns to REC.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: rstate, code, n_rec, last_len, replay_idx.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Short, long, long, short | code = 0x6, replay: red, green, green, red | | |
| Press of 20 ms | Ignored (bounce), n_rec unchanged | | |
| Press of exactly 1000 ms | Counted as long (bit 1) | | |
| Press during replay | Ignored | | |
| After replay | All LEDs OFF, n_rec = 0, ready for a new code | | |

