# S19. Sleep mode until the button is pressed

**Task:** After start-up, PD12 is on for 2 s ('awake'), then the processor goes to Sleep mode with all LEDs off. Pressing PA0 wakes it; PD13 blinks 3 times, PD12 stays on 2 s, and the board sleeps again. Count the wake-ups.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                                 Setting
PA0                                  GPIO_EXTI0, rising edge, no pull; NVIC EXTI0 enabled
PD12, PD13                           GPIO_Output
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• After the 2 s the board is asleep (no LED). A press wakes it. wake_count increases by 1 per press.
• WFI = Wait For Interrupt. Without HAL_SuspendTick() the 1 ms SysTick interrupt would wake the CPU
  immediately and the sleep would be useless.
```
