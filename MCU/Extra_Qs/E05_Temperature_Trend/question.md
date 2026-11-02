# E5. Temperature trend indicator (rising / steady / falling)

**Task:** Read the internal temperature sensor every 2 s with ADC1 polling (average of 8 readings) and store the last 10 temperatures in a ring buffer. When the buffer is full, compare the newest and the oldest value: PD12 ON if the temperature is rising by more than 0.5 C, PD14 ON if it is falling by more than 0.5 C, otherwise PD13 ON (steady).

## 1. Configuration
```text
Item                    Setting                                              Reason
ADC1                    Tick Temperature Sensor Channel; Clock Prescaler     Internal channel IN16
                        PCLK2/4; 12 bit; Scan Disabled; Continuous Disabled;
                        External trigger = Software; Rank 1 = Temperature
                        Sensor Channel, Sampling Time 480 Cycles
TIM3                    Internal Clock, PSC 15999, ARR 1999, global          2 s sampling schedule
                        interrupt enabled
PD12-PD15               GPIO_Output, initial Low                             Trend display
```

## 2. Calculations
```text
Sample period = 16 000 * 2000 / 16 MHz = 2 s          9 intervals between oldest and newest = 18 s
Vsense = raw * 3.3 / 4095       Temp = (Vsense - 0.76) / 0.0025 + 25   (VDDA assumed 3.3 V, typical sensor data)
trend = newest - oldest         > +0.5 C rising | < -0.5 C falling | otherwise steady
Ring index: oldest = log[head] when the buffer is full (head = next slot to write)
```

## 3. Program flow
TIM3 only sets sample_due. The main loop takes 8 ADC readings, averages them, stores the value at log[head],
advances head = (head + 1) % 10 and, once 10 values exist, computes the trend and updates the LEDs.

## 4. Code
See `code.c` in this folder.

## 5. What to observe
Live Expressions: temp_c, temp_log[0..9], log_head, log_count, trend_c, trend. Absolute values are approximate (no calibration); only the change matters.

## 6. Test table
| Test condition | Expected result | Actual | Pass/Fail |
|---|---|---|---|
| First 18 s | Buffer filling, log_count rises 1 to 10, LEDs OFF | | |
| Board untouched | PD13 (steady) after the buffer is full | | |
| Warm the chip with a finger | trend_c becomes positive, PD12 ON | | |
| Remove finger, let it cool | trend_c negative, PD14 ON | | |
| Boundary: trend_c = +0.5 | Not rising (needs to be above 0.5), PD13 stays ON | | |

