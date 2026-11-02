# S15. Short press and long press detection

**Task:** A press of the User button shorter than 1 s toggles the green LED (PD12). A press of 1 s or longer toggles the red LED (PD14). Ignore presses shorter than 30 ms (bounce). Record the last press length.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
PA0                               GPIO_EXTI0, Rising/Falling edge trigger detection, no pull; NVIC EXTI0 enabled
PD12, PD14                        GPIO_Output
```

### Calculation

```text
 press length = time of falling edge (release) - time of rising edge (press), measured with HAL_GetTick() in ms
 < 30 ms : bounce, ignore      30..999 ms : short press      >= 1000 ms : long press
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Press quickly: PD12 toggles; hold for 2 s: PD14 toggles on release. press_ms shows the length.
• A quick tap under 30 ms changes nothing.
```
