# S20. Free-fall and impact detector (accelerometer)

**Task:** Using the onboard accelerometer sampled every 10 ms, detect free fall (total acceleration under 300 mg for at least 50 ms) and light PD14 for 2 s; detect an impact (total acceleration above 2000 mg) and flash PD15 once. Count both events.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                                 Setting
SPI1, PE3, TIM3                      Exactly as in Q6, but TIM3 is PSC 15999, ARR 9 (10 ms)
Code reused from Q6                  accel_init(), lis_read(), lis_write(), lis_read_xyz() give mg_x, mg_y, mg_z
PD12-PD15                            GPIO_Output
```

### Calculation

```text
 total acceleration (mg) = sqrt(x^2 + y^2 + z^2) ; compare squares to avoid sqrt:
 free fall : x^2 + y^2 + z^2 < 300^2 = 90 000            impact: x^2 + y^2 + z^2 > 2000^2 = 4 000 000
 50 ms at 10 ms per sample = 5 consecutive samples
 At rest the total is about 1000 mg (1 g), so 1 000 000 in squared units
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Lying flat: mag_sq about 1 000 000. Gently drop the board a few cm onto a soft surface (hold the USB cable):
  freefall_count increases.
• Impact needs a firm tap on the table; LIS302DL saturates near 2.3 g, so test with a lower impact threshold
  on that board.
```
