// main.c
//
// Blink an ATtiny85.
// Connect first LED at pin 2 (PB3)
// Optionally, connect another one on PB2 and they'll alternate.
//

#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    // Set Data Direction to output on port B, pins 2 and 3:
    DDRB = 0b00001100;
    while (1) {
        // set PB3 high
        PORTB = 0b00001000;
        _delay_ms(1000);
        // set PB3 low
        PORTB = 0b00000100;
        _delay_ms(1000);
    }

    return 1;
}
