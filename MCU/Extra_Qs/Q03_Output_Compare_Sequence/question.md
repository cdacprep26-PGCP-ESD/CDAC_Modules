# Q3. Four-stage sequence using output compare (TIM4)

**Task:** Use TIM4 output compare so that four events happen at 250, 500, 750 and 1000 ms after start. Each event lights the next LED (PD12, PD13, PD14, PD15) and records the compare value and time. Explain how a compare register works.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                           Setting                                             Reason
PD12-PD15                      GPIO_Output, initial Low                            LEDs lit by software in the callback
TIM4                           Internal Clock, CH1-CH4 = Output Compare CHx        Pins PB6-PB9 stay unconnected
TIM4 Prescaler / ARR           15999 / 65535                                       1 ms tick; counter runs 65.536 s
                                                                                   before wrapping
Each channel                   Mode Toggle on Match, Polarity High, OC preload     As required
                               Disable
Pulse (CCR1..CCR4)             250 / 500 / 750 / 1000                              Absolute counts = milliseconds
TIM4 NVIC                      TIM4 global interrupt enabled                       Compare interrupts
```

### Calculation

```text
 tick = (15999 + 1) / 16 MHz = 1 ms
 t1 = 250 * 1 ms = 250 ms       t2 = 500 * 1 ms = 500 ms
 t3 = 750 * 1 ms = 750 ms       t4 = 1000 * 1 ms = 1000 ms
```

### Program flow

```text
All four channels start together. When CNT reaches CCR1 (250) the first compare interrupt runs and the
callback finds the active channel, turns on PD12 and records the event. The same happens for the other three
channels. Because each LED is only set (not toggled) it stays ON. The callback disables only that channel's
compare interrupt (__HAL_TIM_DISABLE_IT), so the event happens once and the other channels continue. The
toggle action of the timer pin (PB6-PB9) happens automatically in hardware but is not wired.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
Answer to task 6 (explain)
The counter keeps running from the moment it starts. A compare register CCRx holds an absolute count, so
CCR1 = 250 means 'act when CNT = 250'. It is not a delay measured from the previous event. The 1 ms tick
(PSC = 15999) converts the count into time: time = CCR x 1 ms. So the four values 250, 500, 750 and 1000

give events at 250, 500, 750 and 1000 ms after the start.

Test condition                         Expected result                                    Actual             Pass/Fail
Start                                  All LEDs OFF, CNT counting up
CNT = 250                              PD12 ON; events_done = 0x1; event_ccr[0] =
                                       250
CNT = 500 / 750 / 1000                 PD13 / PD14 / PD15 ON in order
After 1 s                              all four LEDs ON, events_done = 0xF
event_tick_ms[i]                       about 250, 500, 750, 1000 (+/- 1 ms)
After 65.5 s                           counter wraps; no more events (interrupts
                                       disabled)

Onboard note: TIM4 compare channels with unconnected pins, onboard LEDs and debugger.
```
