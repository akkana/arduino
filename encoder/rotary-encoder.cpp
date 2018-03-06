/* Interrupt-based Rotary Encoder code
 * by Akkana Peck,
 * based on code by Simon Merrett,
 * http://www.instructables.com/id/Improved-Arduino-Rotary-Encoder-Reading/
 * in turn based on insight from Oleg Mazurov, Nick Gammon, rt, Steve Spence
 *
 * This isn't a class because
 * a) you can only have one of them, at least on an Uno,
 *    because pins 2 and 3 are the only ones capable of interrupts, and
 * b) C++ only lets you use static functions for callbacks like interrupts,
 *    not class functions, and Arduino doesn't let you pass arguments
 *    like "this" to an interrupt handler.
 */

#include <Arduino.h>

// Pins must allow digital interrupts:
static int pinA = 2;
static int pinB = 3;

// Flags to let us know we're expecting a rising edge
// and the encoder has arrived at a detent:
volatile byte aFlag = 0;
volatile byte bFlag = 0;

// Store the positions of the encoders.
volatile int encoderPos = 0;

static void PinA_interrupt()
{
    cli(); //stop interrupts happening before we read pin values

    // read all eight pin values then strip away all but pinA and pinB's values
    volatile int reading = PIND & 0xC;

    //check that we have both pins at detent (HIGH) and that
    // we are expecting detent on this pin's rising edge
    if (reading == B00001100 && aFlag) {
        --encoderPos;
        bFlag = bFlag = 0;
    }

    else if (reading == B00000100)
        bFlag = 1;
        // signal that we're expecting pinB to signal the transition to detent
        // from free rotation

    sei(); //restart interrupts
}

static void PinB_interrupt()
{
    cli(); //stop interrupts happening before we read pin values
    // read all eight pin values then strip away all but pinA and pinB's values
    volatile int reading = PIND & 0xC;

    //check that we have both pins at detent (HIGH) and that
    // we are expecting detent on this pin's rising edge
    if (reading == B00001100 && bFlag) {
        encoderPos ++; //increment the encoder's position count
        bFlag = aFlag = 0;
    }
    else if (reading == B00001000)
        aFlag = 1;
        // signal that we're expecting pinA to signal the transition to detent
        // from free rotation

    sei(); //restart interrupts
}

// Public functions
void initRotaryEncoder()
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    // set an interrupt on each pin, looking for a rising edge signal
    attachInterrupt(0, PinA_interrupt, RISING);
    attachInterrupt(1, PinB_interrupt, RISING);
}

int getRotaryPosition()
{
    return encoderPos;
}

void resetRotaryPosition(int pos)
{
    encoderPos = pos;
}

