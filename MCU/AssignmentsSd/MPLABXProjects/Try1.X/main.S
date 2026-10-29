 #include <avr/io.h>

    .text
    .global main
    .type main, @function

    main:
        sbi _SFR_IO_ADDR(DDRB), 0       /* PB0 as output */

    loop:
        sbi _SFR_IO_ADDR(PORTB), 0      /* LED on */
        rcall delay
        cbi _SFR_IO_ADDR(PORTB), 0      /* LED off */
        rcall delay
        rjmp loop

    /* Approximate delay for a 16 MHz clock. */
    delay:
        ldi r18, 25

    delay_outer:
        ldi r19, 255

    delay_middle:
        ldi r20, 255

    delay_inner:
        dec r20
        brne delay_inner
        dec r19
        brne delay_middle
        dec r18
        brne delay_outer
        ret
