# S18. Memory-to-memory DMA copy and verification

**Task:** Copy an array of 64 words from RAM to another array using DMA, verify that the copy is correct and light PD12 if correct, otherwise PD14. Measure how long the DMA copy takes with a 1 us timer and compare with a CPU copy loop.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
DMA                               System Core > DMA > MemToMem tab > Add: Stream DMA2 Stream 0, Mode
                                  Normal, Increment Address on for Source and Destination, Data Width Word / Word.
                                  NVIC: DMA2 stream0 interrupt enabled
TIM2                              Internal Clock, PSC 15 (1 us tick), ARR 4294967295; no interrupt
PD12, PD14                        GPIO_Output
```

### Calculation

```text
 Size = 64 words * 4 bytes = 256 bytes
 Timer tick = 16 MHz / 16 = 1 MHz = 1 us -> time_us = end - start
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• copy_ok = 1 and PD12 ON. dma_us and cpu_us are a few microseconds each; DMA wins for large blocks and
  frees the CPU.
• Do not place the arrays in CCM RAM: DMA cannot access it.
```
