#define __SFR_OFFSET 0
#include <avr/io.h>

.global main

main:
    sbi DDRB, PB0       ; Configure PB0 as output

MAIN_LOOP:
    sbi PORTB, PB0      ; Turn LED ON
    rcall DELAY         ; Wait so you can actually see it turn on
    cbi PORTB, PB0      ; Turn LED OFF
    rcall DELAY         ; Wait so you can actually see it turn off
    rjmp MAIN_LOOP      ; Loop back to turn it on again

; --- Nested Delay Loop ---
DELAY:
    ldi r18, 40
D1:
    ldi r19, 255
D2:
    ldi r20, 255
D3:
    dec r20
    brne D3
    dec r19
    brne D2
    dec r18
    brne D1
    ret