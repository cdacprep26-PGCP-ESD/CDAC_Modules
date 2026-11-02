# E2. Debounced press counter (0-15 in binary)

**Task:** Count presses of the User button with an EXTI interrupt and show the count (0-15) in binary on PD12 (bit 0) to PD15 (bit 3). The sixteenth press wraps the count back to 0. Presses that come less than 100 ms after the previous accepted press are bounce and must be ignored; keep a counter of ignored edges.

## 1. Configuration
```text
Item                    Setting                                              Reason
PA0                     GPIO_EXTI0, rising edge, no pull; NVIC EXTI0         Press = rising edge
                        enabled, priority 1
PD12-PD15               GPIO_Output, initial Low                             Binary display, PD12 = LSB
SysTick priority        0                                                    HAL_GetTick in the callback
```

## 2. Calculations
```text
count = (count + 1) & 0xF          (the AND with 0xF keeps only 4 bits, so 15 + 1 -> 0)
Examples: 5 presses = 0101b -> PD12 + PD14      13 presses = 1101b -> PD12 + PD14 + PD15
16 presses = 0000b (wrapped)
```

## 3. Program flow
Everything happens in the EXTI callback: check the debounce time, increment the count with wrap-around, and
write the four LEDs with leds_show(). The main loop is empty.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: count, presses, ignored.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Power up | count = 0, all LEDs OFF | | |
| 1 press | PD12 ON (0001b) | | |
| 5 presses | PD12 + PD14 ON (0101b) | | |
| 15 presses | All four ON (1111b) | | |
| 16th press | All OFF, count = 0, presses = 16 | | |
| Two presses 50 ms apart | Second one ignored, ignored = 1 | | |
| Hold the button | Only one count (only the rising edge interrupts) | | |

