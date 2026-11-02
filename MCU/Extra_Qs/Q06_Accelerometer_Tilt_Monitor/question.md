# Q6. Onboard accelerometer tilt safety monitor

**Task:** Build a tilt safety monitor with the onboard accelerometer (SPI): verify the sensor ID, sample every 100 ms, show the tilt direction with the LEDs (hysteresis), raise an alarm after 3 s of continuous tilt, clear it after 2 s of safe position, re-calibrate the level when PA0 is held 1 s, and show an error state if the sensor ID is wrong.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                     Setting                                                      Reason
SPI1                     Full-Duplex Master, Hardware NSS Disable, 8 bit, MSB         Pins PA5/PA6/PA7 (AF5) are hard-wired
                         first, Prescaler 16 (1 MHz), CPOL High, CPHA 2 Edge, NSS     to the sensor. Mode 3 works for both
                         software                                                     LIS3DSH and LIS302DL
PE3                      GPIO_Output, initial level High, very high speed, label      Chip select, active low, software
                         LIS_CS                                                       controlled
PE0, PE1                 Leave unconfigured                                           INT1/INT2 are not needed for polling.
                                                                                      PE0 would clash with PA0 on EXTI line 0
PD12-PD15                GPIO_Output, initial Low                                     Direction and alarm LEDs
PA0                      GPIO_Input, no pull                                          Recalibration button (board has
                                                                                      pull-down); read by polling
TIM3                     Internal Clock, PSC 15999, ARR 99, global interrupt          100 ms sampling schedule
                         enabled
```

### Design decisions (write these in your report)

```text
Parameter                     Value                                          Reason
Sampling period               100 ms (TIM3)                                  Timer-based schedule as required
Tilt-enter threshold          300 mg (about 17.5 degrees)                    Chosen above noise and level offset; confirm with
                                                                             your own raw readings
Tilt-leave threshold          200 mg (hysteresis)                            Stops flicker around the threshold
Alarm delay                   30 samples = 3 s of continuous tilt            Spec item 7
Alarm clear                   20 samples = 2 s continuously safe             Spec item 8
Recalibrate                   PA0 held 10 samples = 1 s                      Spec item 4
LED map                       +X PD12, -X PD14, +Y PD13, -Y PD15             Suggested direction indication
```

### State machine

```text
LEVEL (all LEDs off) -> TILT (direction LED on, counting tilt_cnt) -> ALARM (all four LEDs blink) when
tilt_cnt reaches 30. ALARM -> LEVEL when safe_cnt reaches 20. If the sensor identity is wrong at start the
program enters ERROR (PD14 ON forever). Any sample that is not tilted resets tilt_cnt, so brief shaking
never raises the alarm.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
Choosing and documenting the threshold (spec item 5)
Put the board flat and record mg_x, mg_y (level noise, usually under 50 mg), then tilt it by about 20 degrees and
record again. Pick the enter-threshold about half way between the level noise and the tilted reading, and the
leave-threshold about two thirds of it. Write the readings in a table like this and keep it in the report:

Position                        mg_x            mg_y             mg_z                   Direction / LED
Level                                                            about 1000             none
Tilt +X side                                                                            PD12
Tilt -X side                                                                            PD14
Tilt +Y side                                                                            PD13
Tilt -Y side                                                                            PD15

Test condition                          Expected result                                       Actual            Pass/Fail
Level at start                          all LEDs OFF, no alarm
Slow tilt toward +X                     PD12 turns ON above 300 mg and stays until
                                        below 200 mg
Brief shaking (under 3 s)               LEDs flicker but no alarm, tilt_cnt resets
Sustained tilt 3 s                      alarm_on = 1, four LEDs blink every 500 ms
Level for 1 s after alarm               alarm stays (needs 2 s safe)
Level for 2 s after alarm               alarm clears
Hold PA0 for 1 s on a tilted surface    new reference, tilt about this surface treated as
                                        level
FORCE_ID_FAIL = 1                       only PD14 ON, program stops in error_state()

Onboard note: SPI1 and PE3 are connected on the board to the accelerometer; LEDs and PA0 are onboard;
TIM3 provides the schedule. No external sensor or alarm.
```
