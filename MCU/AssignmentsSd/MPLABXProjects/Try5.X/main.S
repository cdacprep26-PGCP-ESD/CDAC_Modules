#include <avr/io.h>

.text
.global main
.type main, @function
main:
    ldi r16, 0x0F
    out _SFR_IO_ADDR(DDRB), r16
    ldi r16, 0x01

MAIN:
    out _SFR_IO_ADDR(PORTB), r16
    rcall DELAY
    lsl r16
    cpi r16, 0x10
    brne MAIN
    ldi r16, 0x01
    rjmp MAIN

DELAY:
    ldi r18, 30
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