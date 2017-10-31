// main.c
//
// Blink for ATtiny85.
// Connect red LED at pin 2 (PB3)
//

#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    // set Data Direction on pin B to output:
    DDRB = 0b00001000;   // or 0x8
    while (1) {
        // set PB3 (pin 3 on the chip) high.
        PORTB = 0b00001000;   // or 0x8
        _delay_ms(50);
        // set PB3 low
        PORTB = 0b00000000;
        _delay_ms(100);
    }

    return 1;
}
