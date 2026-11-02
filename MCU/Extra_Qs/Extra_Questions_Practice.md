# Extra Questions - Practice Sheet (no solutions)

Solve on paper first (configuration, calculations, flow, code, test table), then compare with the folder of the same number.

## E1. Two-tone alarm with start/stop button
A two-tone alarm uses the four onboard LEDs. While the alarm is running, PD12+PD14 light for 250 ms, then PD13+PD15 light for 250 ms, and the two pairs keep alternating. Each press of the User button (PA0) starts the alarm if it is stopped, or stops it (all LEDs off) if it is running. Use a timer interrupt for the 250 ms timing. No HAL_Delay in the alarm logic. Count the tone changes.

*Solution folder:* `E01_Two_Tone_Alarm/`

## E2. Debounced press counter (0-15 in binary)
Count presses of the User button with an EXTI interrupt and show the count (0-15) in binary on PD12 (bit 0) to PD15 (bit 3). The sixteenth press wraps the count back to 0. Presses that come less than 100 ms after the previous accepted press are bounce and must be ignored; keep a counter of ignored edges.

*Solution folder:* `E02_Press_Counter_Binary/`

## E3. Sunrise: PWM fade from 0 % to 100 % in exactly 10 s
PD12 must be driven by hardware PWM (TIM4 CH1). After start-up its brightness rises smoothly from 0 % to 100 % in exactly 10 seconds and then stays at 100 %. Calculate the PWM period, the CCR step and the step timer. Use a timer interrupt for the steps; no delay loops.

*Solution folder:* `E03_Sunrise_PWM_Fade/`

## E4. Button presses per 10 s using input capture
Count how many times the User button is pressed in every 10-second window using TIM2 input capture on PA0. At the end of each window, show the count in binary on the four LEDs (values above 15 show 15) and start a new window. Ignore edges closer than 50 ms (bounce).

*Solution folder:* `E04_Presses_Per_10s_Capture/`

## E5. Temperature trend indicator (rising / steady / falling)
Read the internal temperature sensor every 2 s with ADC1 polling (average of 8 readings) and store the last 10 temperatures in a ring buffer. When the buffer is full, compare the newest and the oldest value: PD12 ON if the temperature is rising by more than 0.5 C, PD14 ON if it is falling by more than 0.5 C, otherwise PD13 ON (steady).

*Solution folder:* `E05_Temperature_Trend/`

## E6. Press-pattern recorder and replay
Record four button presses as a 4-bit code: a press shorter than 1 s is bit 0 (short), a press of 1 s or longer is bit 1 (long); presses shorter than 30 ms are bounce and ignored. PD13 toggles after every accepted press as feedback. After the fourth press the code is replayed with a timer: one bit every 500 ms, green PD12 for a 1 and red PD14 for a 0 (first recorded bit first). Then all LEDs go off and recording starts again. Presses during the replay are ignored.

*Solution folder:* `E06_Press_Pattern_Recorder/`

## E7. Low-power blinker (Sleep mode between timer ticks)
PD12 must flash once per second (ON for 100 ms, OFF for 900 ms). Between the timer ticks the processor must stay in Sleep mode. Use a 100 ms timer interrupt as the wake-up source. Explain which interrupts can wake the CPU.

*Solution folder:* `E07_Low_Power_Blinker/`

## E8. ADC + DMA temperature data logger (average, minimum, maximum)
Sample the internal temperature sensor every 50 ms using ADC1 triggered by a timer, and let circular DMA store 64 samples in a buffer without CPU help. Each time the buffer is full, compute the average, minimum and maximum (raw and in degrees C) and toggle PD12 as a heartbeat.

*Solution folder:* `E08_ADC_DMA_Logger/`

## E9. Pedestrian crossing controller
Cars normally have GREEN (PD12). When a pedestrian presses the User button, the request is remembered, but the light may only change after the green has lasted at least 5 s. Then: ORANGE PD13 for 2 s, then RED PD14 together with the blue WALK light PD15 for 5 s, then back to GREEN. Button presses during orange, red or walk are ignored. Count completed crossings.

*Solution folder:* `E09_Pedestrian_Crossing/`

## E10. Watchdog with two health flags
The IWDG may only be refreshed when both health flags are set in a 2 s window: (1) the 100 ms heartbeat timer is running and (2) the operator pressed PA0 at least once ("operator alive"). If a window ends without both flags, the watchdog is not refreshed and the chip resets. After an IWDG reset PD14 shows the reason for 3 s. PD12 = healthy, PD13 toggles after every successful refresh. Test by not pressing the button.

*Solution folder:* `E10_IWDG_Two_Flags/`

## More prompts to solve yourself (no solution given)
1. Knight-rider: a single lit LED moves PD12 -> PD13 -> PD14 -> PD15 -> PD14 -> PD13 ... every 150 ms using TIM3. PA0 changes the speed between 150, 300 and 600 ms.
2. Dimmer: PD12 PWM (TIM4 CH1). Short press raises the duty by 10 % (wraps from 100 % to 0 %), long press (1 s) switches the LED fully off. Show the duty on the other three LEDs as 0-3 bars.
3. Door chime: input capture on PA0 measures press length; a press under 0.5 s plays one flash on PD12, 0.5-1.5 s two flashes, over 1.5 s three flashes on PD14.
4. Rain-gauge counter: count button presses in an RTC-timed hour; at every full minute store the count in a 10-entry ring buffer and show the newest count in binary.
5. Tilt-controlled dimmer: read the accelerometer X axis every 20 ms and map -1000..+1000 mg to a 0-100 % PWM duty on PD12.
6. Frequency generator: TIM2 output compare toggles an internal flag at 1 Hz, 2 Hz, 5 Hz or 10 Hz (PA0 changes the frequency); PD12 follows the flag. Verify the periods with HAL_GetTick().
7. DMA waveform: DMA2 + TIM1 write a 16-step binary-count pattern to GPIOD BSRR; PA0 reverses the direction by switching between two tables.
8. Safe-start interlock: LEDs may only be switched on 3 s after power-up and only if the IWDG did not cause the last reset; otherwise PD14 blinks.
9. Over-temperature alarm: ADC + DMA temperature, alarm (PD14 blinks) when the average rises 5 C above the start value for 10 s; clears when it is below +3 C for 5 s.
10. Two-button style input with one button: single press = mode +1 (0-3), double press within 400 ms = mode reset to 0. Show the mode in binary.
