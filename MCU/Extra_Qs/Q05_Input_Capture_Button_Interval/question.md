# Q5. Measure the user-button interval using input capture (TIM2 CH1)

**Task:** Measure the time between two presses of the User button with TIM2 input capture. Ignore bounce, handle counter overflow, and blink PD15 when a measurement is ready.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                             Setting                                                 Reason
PA0                              TIM2_CH1 (AF1), GPIO pull = No pull-up/pull-down        Board has external pull-down; button
                                                                                         gives a rising edge
PD15                             GPIO_Output, initial Low                                Blinks to show a finished
                                                                                         measurement
TIM2                             Internal Clock, Channel1 = Input Capture direct         Hardware time-stamp
                                 mode
Prescaler / ARR                  15999 / 4294967295                                      1 ms tick, 32-bit counter, overflow
                                                                                         after 49.7 days
IC channel 1                     Rising Edge, Direct TI, Prescaler No division, Filter   Rising edge = press; filter removes
                                 15                                                      spikes (about 16 us)
NVIC                             TIM2 global interrupt enabled                           Capture interrupt
```

### Calculation

```text
 tick = 16 000 / 16 MHz = 1 ms
 interval_ms = t2 - t1                         (unsigned 32-bit: correct even if CNT wrapped once)
 general form (any ARR, at most one wrap):
     elapsed = (t2 >= t1) ? (t2 - t1) : (ARR + 1 - t1 + t2)
 Example: t1 = 1000, t2 = 3500 -> 2500 ms = 2.5 s
```

### Program flow

```text
The first valid rising edge stores t1 and moves to state WAIT_SECOND. The second valid edge stores t2,
calculates the interval, sets result_ready and returns to WAIT_FIRST. Edges that arrive less than 50 ms after
the last accepted edge (contact bounce) are ignored. The main loop blinks PD15 once when result_ready is
set; the callback itself stays short.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
 Testing the overflow case
 A 32-bit counter will not wrap in an exam. To prove the wrap logic, temporarily set ARR = 9999 (10 s) and pass 9999u
 instead of 0xFFFFFFFFu to elapsed_ticks(). Press once at 8 s and again at 3 s later: t1 = about 8000, t2 = about 1000
 and the function returns about 3000.

Test condition                         Expected result                                        Actual          Pass/Fail
Two presses 2 s apart                  interval_ms about 2000
Two presses 500 ms apart               about 500; PD15 blinks
Bouncing button                        ignored_edges increases; one valid_edges per
                                       press
Hold button long                       only one rising edge, one capture
Three presses                          first pair measured; third press starts a new pair

Test condition                            Expected result                                        Actual           Pass/Fail
Overflow test (ARR = 9999)                correct interval across the wrap

Onboard note: PA0 is the onboard User button; TIM2_CH1 is its alternate function; PD15 is an onboard LED.
```
