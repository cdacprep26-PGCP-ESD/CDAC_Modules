# S21. Measure the LSI frequency and calculate the IWDG reload

**Task:** The IWDG is driven by the inaccurate LSI. Measure the real LSI frequency with TIM5 input capture (the channel can be connected to the LSI internally), then calculate the reload value that gives a 2 s timeout with prescaler 64. Show the result in the debugger and use the LEDs to show whether the LSI is in the 28-36 kHz range.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                               Setting
TIM5 (32-bit)                      Clock Source Internal Clock; Channel4 = Input Capture direct mode; PSC 0 (16 MHz
                                   tick); ARR 4294967295; Polarity Rising; Input Capture Prescaler Div8; Filter 0; TIM5
                                   global interrupt enabled
Pin                                CubeMX may assign a pin to TIM5_CH4 (PA3); it is not used because the capture input
                                   is switched to the LSI by software
PD12-PD15                          GPIO_Output
```

### Calculation

```text
 Div8 prescaler: one capture every 8 LSI periods
 delta (ticks) = 8 * 16 000 000 / f_LSI          (f_LSI = 32 kHz -> 4 000 000 ticks)
 f_LSI = 8 * 16 000 000 / delta
 Reload for 2 s, prescaler 64 : Reload = 2 * f_LSI / 64 - 1           (32 kHz -> 999)
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• delta_ticks about 4 000 000 (varies with the chip), f_lsi_hz 28 000-36 000, reload_2s close to 999.
• To use it, put this reload value into the IWDG settings of Q10 in the next build. The IWDG cannot be
  reconfigured after it starts.
```
