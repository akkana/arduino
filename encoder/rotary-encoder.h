//
// Create a rotary encoder on Uno pins 2 and 3.
// Use interrupts to read it, so in theory, you can be doing other
// things (though not delay()) and still not miss many events.
// You can only have one encoder, because the uno only has
// those two interrupt-capable pins, and because Arduino interrupts
// can't pass information about which pin or which encoder the
// interrupt is for.
//
// Originally based on code by Simon Merrett,
// http://www.instructables.com/id/Improved-Arduino-Rotary-Encoder-Reading/
// in turn based on insight from Oleg Mazurov, Nick Gammon, rt, Steve Spence
//

extern void initRotaryEncoder();
extern int getRotaryPosition();
extern void resetRotaryPosition(int pos);
