# Q9. DMA-driven onboard LED pattern player (timer update to GPIOD BSRR)

**Task:** Play an 11-step LED pattern with no CPU involvement: TIM1 update events trigger DMA2 transfers into GPIOD BSRR. Count passes and update events, and let PA0 pause / resume the pattern.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                     Setting                                                            Reason
PD12-PD15                GPIO_Output, initial Low                                           LEDs written by DMA
TIM1                     Internal Clock, Prescaler 15999, Counter Period 199, repetition    16 MHz / (16 000 x 200) = 5 Hz
                         counter 0                                                          (200 ms steps)
TIM1 DMA Settings        Add TIM1_UP: DMA2 Stream 5, Channel 6, Circular, Memory            Update event requests one
                         to Peripheral, Memory increment ON, Peripheral increment           32-bit word per step. DMA2
                         OFF, Data width Word / Word, Priority Low                          reaches the GPIO (AHB) bus
NVIC                     TIM1 update interrupt (shared with TIM10) enabled; DMA2            Counting update events and
                         stream 5 global interrupt enabled; EXTI0 enabled                   transfer-complete, pause button
PA0                      GPIO_EXTI0, rising edge, no pull                                   Pause / resume

Why TIM1 and not TIM2 or TIM4? Those timers' update requests are served by DMA1, which cannot access the
GPIO registers on the AHB bus. TIM1 is on DMA2 (Stream 5, Channel 6). If CubeMX offers a different stream,
accept it as long as it is on DMA2.
```

### Calculation

```text
 Step time = (15999 + 1) * (199 + 1) / 16 MHz = 200 ms (5 Hz)
 BSRR word for a 4-bit mask m (bit0..3 = PD12..PD15):
     BSRR = (m << 12) | ((~m & 0xF) << 28)
            ^ set the wanted LEDs    ^ reset the other LEDs      (bits 16-31 reset, 0-15 set)
 Example m = 0x1 (PD12): 0x0000_1000 | 0xE000_0000 = 0xE000_1000
          m = 0x0 (all OFF): 0xF000_0000     m = 0xF (all ON): 0x0000_F000
```

### Program flow

```text
At every TIM1 update event the timer raises a DMA request. The DMA stream copies the next word of
led_pattern[] into the GPIOD BSRR (set/reset) register, increments the memory address, and when it reaches
the end of the table it restarts automatically (circular mode). The main loop does not write the LEDs. Pause is
done by stopping the timer counter, so no update events (and no DMA requests) occur; resume restarts the
counter and the pattern continues from the same index.
About BSRR: the question asks for DMA to GPIOD BSRR (the GPIO bit set/reset register). You do not write any
register yourself. BSRR_PATTERN only builds a 32-bit number, and &GPIOD->BSRR is only the address that you
hand to HAL_DMA_Start_IT() as the destination. Everything else is HAL.
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
 Common mistakes in this question
 HAL_TIM_Base_Start_DMA() is not suitable: it sends data to the timer's ARR register, not to BSRR. Use
 HAL_DMA_Start_IT() with the BSRR address plus __HAL_TIM_ENABLE_DMA().
 Using DMA1 (TIM2 or TIM4 update) will not drive the GPIO pins. Use TIM1 with DMA2.
 Memory data width must be Word (32-bit) because BSRR is a 32-bit register. Half Word would write only half of the
 set/reset bits.
 A pattern value that only sets bits will never turn an LED off; BSRR_PATTERN also writes the reset bits.

Test condition                           Expected result                                       Actual           Pass/Fail
Run                                      LEDs repeat the 11-step pattern every 2.2 s
dma_index                                0 to 10, then back to 0; dma_tc_count increases
                                         by 1 per pass
uev_count                                increases by 5 per second
All-OFF step / all-ON step               no LED / all four LEDs
Single-LED steps                         PD12, PD13, PD14, PD15 one after another
Identical consecutive entries            PD15 stays on for 3 steps (600 ms); dma_index
                                         still advances
PA0 press                                pattern freezes, dma_index and uev_count stop;
                                         debugger still connected
PA0 press again                          pattern resumes at the same step

Onboard note: TIM1, DMA2, GPIOD BSRR, PA0 and onboard LEDs only.
```
