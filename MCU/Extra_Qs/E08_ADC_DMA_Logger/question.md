# E8. ADC + DMA temperature data logger (average, minimum, maximum)

**Task:** Sample the internal temperature sensor every 50 ms using ADC1 triggered by a timer, and let circular DMA store 64 samples in a buffer without CPU help. Each time the buffer is full, compute the average, minimum and maximum (raw and in degrees C) and toggle PD12 as a heartbeat.

## 1. Configuration
```text
Item                    Setting                                              Reason
ADC1                    Tick Temperature Sensor Channel; Clock Prescaler     Internal channel IN16
                        PCLK2/4; 12 bit, right aligned; Scan Disabled;
                        Continuous Disabled; DMA Continuous Requests Enabled
ADC1 trigger            External trigger = Timer 3 Trigger Out event,        50 ms hardware trigger
                        Rising edge
ADC1 Rank 1             Temperature Sensor Channel, Sampling Time 480 Cycles Needs at least 10 us
ADC1 DMA                DMA2 Stream 0, Channel 0, Peripheral to Memory,      16-bit data, buffer fills
                        Circular, Memory increment ON, Half Word / Half Word forever
TIM3                    PSC 15999, ARR 49, Trigger Output (TRGO) = Update    50 ms trigger, no interrupt
                        Event
PD12                    GPIO_Output                                          Heartbeat
```

## 2. Calculations
```text
Trigger period = 16 000 * 50 / 16 MHz = 50 ms -> 20 samples per second
Buffer 64 samples fills in 64 * 50 ms = 3.2 s
Conversion time = (480 + 12) / 4 MHz = 123 us (much shorter than 50 ms)
Temp = ((raw * 3.3 / 4095) - 0.76) / 0.0025 + 25     (VDDA = 3.3 V assumed)
Average uses a 32-bit sum: 64 * 4095 = 262 080 (would overflow a 16-bit variable)
```

## 3. Program flow
TIM3 TRGO starts each conversion; the DMA moves the result into smp[] and wraps back to index 0 after 64
samples (circular). The DMA transfer-complete callback sets buf_ready. main() analyses the buffer, then
toggles PD12.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: smp, raw_min, raw_avg, raw_max, t_min, t_avg, t_max, buffers_done.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| Run 4 s | buffers_done = 1, PD12 toggled once | | |
| smp[0..63] | All values near 900-1000, changing by a few counts | | |
| raw_min <= raw_avg <= raw_max | Always true | | |
| Warm the chip | raw falls slightly (temperature rises), t_avg rises | | |
| Check DMA | smp[] is overwritten again after 3.2 s (circular mode) | | |

