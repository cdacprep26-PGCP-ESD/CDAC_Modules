# Q4. Independent LED toggle rates using output-compare interrupts

**Task:** Make the four LEDs toggle at independent rates (first intervals 200, 400, 600 and 1000 ms) using TIM4 output-compare interrupts. The timing must not drift and must survive the 16-bit counter wrap-around.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                           Setting                                             Reason
PD12-PD15                      GPIO_Output, initial Low                            Toggled in the callback
TIM4                           Internal Clock, CH1-CH4 Output Compare, Toggle      Pins PB6-PB9 unconnected
                               on Match
Prescaler / ARR                15999 / 65535                                       1 ms tick; natural 16-bit wrap
Pulse (first CCR)              200 / 400 / 600 / 1000                              First interval of each LED
NVIC                           TIM4 global interrupt enabled                       Compare interrupts
```

### Calculation

```text
 interval ticks: PD12 = 200, PD13 = 400, PD14 = 600, PD15 = 1000 (1 tick = 1 ms)
 next_compare = previous_compare + interval_ticks   (modulo 65536, the real counter period ARR + 1)
 Example PD12: 200, 400, 600 ... 65400, 65600 mod 65536 = 64, 264 ... (still exactly 200 ms apart)
```

### Program flow

```text
Each compare match triggers the callback. It toggles the LED that belongs to the active channel and writes
CCR = old CCR + interval. The code never resets the counter and never reads CNT. Wrap-around is automatic
because CCR and CNT are both 16-bit. Per-LED counters and tick-differences let you verify the four rates in
the debugger.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
 Why not CNT + interval?
 If the interrupt is delayed by 30 us, CNT + interval makes that event 30 us late and every later event is shifted too
 (drift). old CCR + interval keeps every event on the ideal grid. If your teacher uses a counter with a different ARR (the
 manual used 5999), take the modulus (ARR + 1) instead of 65536.

Test condition                          Expected result                                          Actual              Pass/Fail
After 10 s                              toggle_count about 50 / 25 / 16 / 10
period_ms[0..3]                         200 / 400 / 600 / 1000 (+/- 1 ms)
Counter wrap at 65.536 s                rates unchanged, no glitch
Pause at a breakpoint 2 s then          events catch up; grid unchanged
resume
Blink period of PD12                    400 ms (one toggle per 200 ms)

Onboard note: only TIM4 compare channels and onboard LEDs.
```
