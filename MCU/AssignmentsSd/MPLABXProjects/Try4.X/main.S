#include <avr/io.h>

.text
.global main
.type main, @function
main:
    sbi _SFR_IO_ADDR(DDRB), DDB0
    cbi _SFR_IO_ADDR(DDRD), DDD2
    sbi _SFR_IO_ADDR(PORTD), PORTD2

MAIN:
    ; Skip next instruction if PD2 is LOW
    sbic _SFR_IO_ADDR(PIND), PIND2
    rjmp BUTTON_NOT_PRESSED

BUTTON_PRESSED:
    sbi _SFR_IO_ADDR(PORTB), PORTB0
    rjmp MAIN

BUTTON_NOT_PRESSED:
    cbi _SFR_IO_ADDR(PORTB), PORTB0
    rjmp MAIN