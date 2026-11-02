# Q8. RTC-based access event logger

**Task:** Make an RTC access-event logger: each PA0 press stores a time-stamp (hh:mm:ss, date) in an 8-slot ring buffer, PD13 lights 500 ms, every 5th event copies the last five records and blinks PD15 three times, and an invalid RTC state is shown on PD14 and recoverable with PA0.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                          Setting                                                    Reason
RTC                           Activate Clock Source, Activate Calendar; 24-hour          Calendar needed for date and time
                              format
Clock tab                     RTC source mux = LSI RC (about 32 kHz)                     No external crystal allowed
RTC prescalers                Asynchronous 127, Synchronous 249                          32 000 / (128 x 250) = 1 Hz
Initial time/date             For example 10:30:50, Sunday 18-10-26 (year 26, not        Time ends in :50 so the minute
                              00)                                                        rollover is seen after 10 s
PA0                           GPIO_EXTI0, rising edge, no pull; NVIC EXTI0 enabled,      Press = event request
                              priority 1
SysTick priority              0 (more urgent than EXTI0)                                 HAL_GetTick works inside the
                                                                                         callback
PD12-PD15                     GPIO_Output, initial Low                                   PD12 ready, PD13 activity, PD14
                                                                                         error, PD15 five-event signal

Design
 Record : event number, hh:mm:ss, day, month, year             -> struct event_rec_t
 Ring    : 8 slots, head = next slot to write; head = (head + 1) % 8
 Last 5 : copied into last5[0..4] (oldest first) whenever event_count is a multiple of 5
 Debounce: ignore presses less than 200 ms after the last accepted press
 LEDs    : PD12 ON = ready | PD13 500 ms on each event | PD15 three blinks at 5, 10, 15 ... |
           PD14 ON = RTC invalid / state error (then PD12 OFF)
 Error   : set when HAL_RTC_GetTime/GetDate fails, INITS flag is clear, or fields are out of range.
           Cleared by pressing PA0: the program reloads the default date/time and, if that succeeds,
           clears the error and returns to READY.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
 Things the examiner may ask
 Why GetTime then GetDate? Reading the date unlocks the shadow registers; without it the next time read returns
 the old value.
 Held button: only the rising edge raises EXTI, so holding PA0 gives one event, not many.
 Rapid presses: two presses closer than 200 ms are treated as one. This is a design choice; reduce DEBOUNCE_MS to
 50 ms if rapid presses must be counted.
 Why not print or delay in the callback? The callback only stores data and sets flags; LED timing is done with
 HAL_GetTick() in main(), so the callback stays short and PA0 is never missed.
 RTC accuracy: LSI is only about +/-20 %; the timestamps drift. A real product would use LSE.

Test condition                         Expected result                                       Actual           Pass/Fail
Single press                           event_count = 1, record 1 stored, PD13 on for
                                       0.5 s
Hold button for 3 s                    only one event
Two presses 100 ms apart               second ignored (debounce)
Press 5                                PD15 blinks 3 times, last5 holds events 1-5,
                                       snapshot_no = 1
Press 10                               last5 holds events 6-10 (ring buffer wrapped)
Presses around hh:mm:59 -> :00         minute increments, seconds restart at 00
Force rtc_error = 1 in the             PD14 ON, PD12 OFF; next press reloads the time
debugger                               and clears the error

Onboard note: RTC on the internal LSI, PA0 onboard button, onboard LEDs, SRAM ring buffer, SWD.
```
