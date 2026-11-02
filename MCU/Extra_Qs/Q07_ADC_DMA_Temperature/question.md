# Q7. Internal temperature monitor using ADC and DMA

**Task:** Monitor the internal temperature using ADC1 + DMA: temperature sensor and VREFINT, triggered by TIM3 every 100 ms, circular DMA buffer, average of 16 samples, VDDA correction, LED ranges above the start-up baseline, and PA0 to freeze / unfreeze the display.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                        Setting                                                         Reason
ADC1 mode                   Tick 'Temperature Sensor Channel' and 'Vrefint Channel'         Internal channels IN16 and IN17
ADC1 clock                  Prescaler PCLK2 divided by 4, 12-bit, right aligned             4 MHz ADC clock
Scan Conversion Mode        Enabled                                                         Two channels per trigger
Continuous Conversion       Disabled                                                        The timer triggers each scan
DMA Continuous              Enabled                                                         Circular DMA keeps getting
Requests                                                                                    requests
External trigger            Timer 3 Trigger Out event, Rising edge                          100 ms hardware trigger
Number of conversions       2                                                               Rank 1 and rank 2
Rank 1                      Temperature Sensor Channel, Sampling Time 480 Cycles            At least 10 us required: 480 / 4
                                                                                            MHz = 120 us
Rank 2                      Vrefint Channel, Sampling Time 480 Cycles                       Same reason

Item                       Setting                                                        Reason
ADC1 DMA                   DMA2 Stream 0, Channel 0, Peripheral to Memory, Circular,      Result is 16-bit; buffer fills
                           Memory increment on, Half Word / Half Word                     forever
TIM3                       PSC 15999, ARR 99, Trigger Output (TRGO) = Update Event        100 ms trigger pulses (no TIM3
                                                                                          interrupt needed)
PA0                        GPIO_EXTI0, rising edge, no pull; NVIC EXTI0 enabled           Live/frozen switch
PD12-PD15                  GPIO_Output                                                    Range display and alarm
```

### Calculation and assumptions

```text
 Trigger period: (15999 + 1) * (99 + 1) / 16 MHz = 100 ms   -> 10 scans per second
 Buffer: 16 samples * 2 channels = 32 half-words, interleaved [T, V, T, V ...]; full every 1.6 s
 Conversion time per channel: (480 + 12) / 4 MHz = 123 us   (two channels: 246 us, much less than 100 ms)
 VDDA   = 1.21 * 4095 / avg_vref          (VREFINT_nominal = 1.21 V, typical)
 Vsense = avg_temp * VDDA / 4095
 Temp   = (Vsense - 0.76) / 0.0025 + 25   (V25 = 0.76 V, Avg_Slope = 2.5 mV/C, typical, uncalibrated)
 Example: avg_temp = 930, avg_vref = 1500 -> VDDA = 3.30 V, Vsense = 0.750 V, Temp = 21 C

Assumptions to write: typical datasheet values (V25 = 0.76 V, slope 2.5 mV/degree C, VREFINT = 1.21 V)
are used, no factory calibration; absolute accuracy of several degrees is expected; therefore the LED ranges
use temperature rise above the start-up baseline.
```

### Display ranges

```text
Range            Rise above baseline                 LEDs
0                below 1.0 C                         PD12
1                1.0 to 3.0 C                        PD12 + PD13
2                3.0 to 6.0 C                        PD12 + PD13 + PD15
3 (high)         6.0 C or more                       PD12 + PD13 + PD15 and PD14 blinks (250 ms)

Warm the chip by touching it, or by running a busy loop, to see the range change. The thresholds are
variables, so you can lower them during the exam.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe in the debugger
• adc_buf[0..31]: even indexes around 900 (temperature), odd indexes around 1500 (VREFINT), values
     change by a few counts only.

• dma_pos: runs 0 to 31 and wraps in 1.6 s (10 scans per second x 2 values per scan). This proves circular
  mode.
• avg_temp_raw, avg_vref_raw, vdda_v (about 3.0 to 3.3 V on USB power), temp_c.
• Press PA0: frozen = 1; temp_c still changes but disp_temp_c and the LEDs stay fixed. Press again to resume.

Answer to task 8 (explain)
The temperature sensor and VREFINT are inside the microcontroller and connected to the ADC by internal
multiplexer inputs (IN16 and IN17). Nothing has to be wired to a pin, so the question uses only onboard
resources. The reading changes as the MCU warms up because the sensor measures the die temperature:
running the CPU, clocks and peripherals dissipates power, and the die temperature rises above the ambient
temperature until heat loss and heating balance. The sensor voltage also changes with VDDA, which is why
the VREFINT reading is used to correct for supply variation.

Test condition                            Expected result                                        Actual           Pass/Fail
Power up, wait 2 s                        baseline recorded, PD12 only
Press finger on the MCU                   temperature rises, ranges 1, 2, 3 appear
Range 3                                   PD14 blinks at about 4 Hz
PA0 press                                 snapshot mode; LEDs freeze
PA0 press again                           live monitoring resumes
Look at dma_pos                           cycles 0-31 continuously
vdda_v                                    between 3.0 and 3.6 V, vdda_ok = 1

Onboard note: internal ADC channels, TIM3 TRGO, DMA2, PA0 and LEDs only.
```
