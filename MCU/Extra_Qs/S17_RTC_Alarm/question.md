# S17. RTC alarm every minute

**Task:** Using the RTC on the LSI, trigger an alarm interrupt every minute when the seconds value is 30. At each alarm blink PD13 for 1 s and count the alarms. PD12 shows 'running'.

## 1. Configuration, calculations and program flow

### Configuration

```text
Item                              Setting
RTC                               Activate Clock Source, Activate Calendar, Alarm A: Internal Alarm; RTC clock source
                                  LSI; Asynch 127, Synch 249; set time 10:30:00
NVIC                              RTC alarm interrupt through EXTI line 17 enabled
PD12, PD13                        GPIO_Output
```

### Calculation

```text
 Alarm matches only the seconds field: the mask ignores date, hours and minutes
 -> the alarm fires once every minute when seconds = 30 (10:30:30, 10:31:30, ...)
```

## 2. Code
See `code.c` in this folder (all blocks go into the matching USER CODE markers).

## 3. What to observe and test
```text
What to observe
• Start at 10:30:00; the first alarm comes after 30 s, then every 60 s. alarm_count increases each time.

• Alarm A repeats automatically because the mask ignores hours and minutes. A fixed '10-second period'
  needs the RTC wake-up timer instead.
```
