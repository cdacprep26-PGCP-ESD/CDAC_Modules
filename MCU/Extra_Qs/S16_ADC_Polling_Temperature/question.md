# S16. Simple temperature indicator with single-conversion ADC (polling)

**Task:** Read the internal temperature sensor every 500 ms using ADC1 polling, average 8 readings and show four ranges on the LEDs relative to the first reading (baseline): under +1 C: PD12; +1 to +3 C: PD12+PD13; +3 to +6 C: three LEDs; +6 C or more: all four.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
ADC1                              Tick Temperature Sensor Channel; Clock Prescaler PCLK2/4; 12 bit; Scan Disabled;
                                  Continuous Disabled; External trigger = Software; Rank 1 = Temperature Sensor
                                  Channel, Sampling Time 480 Cycles
PD12-PD15                         GPIO_Output
```

### Calculation

```text
 Assume VDDA = 3.3 V (no VREFINT in this simple version)
 Vsense = reading * 3.3 / 4095       T = (Vsense - 0.76) / 0.0025 + 25
 Conversion time = (480 + 12) / 4 MHz = 123 us per reading; 8 readings = 1 ms
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• adc_avg is about 900-1000 at room temperature; temp_c roughly 25-45 C (not accurate without calibration).

• Touch the chip with a finger: after some seconds the temperature rises and more LEDs light.
```
