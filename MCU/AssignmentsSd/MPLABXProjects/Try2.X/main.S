.include "m328pdef.inc"

.org 0x0000
    rjmp RESET

RESET:
    sbi DDRB, PB0

MAIN:
    sbi PORTB, PB0
    rcall DELAY
    cbi PORTB, PB0
    rcall DELAY
    rjmp MAIN

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