
/*
 * A membat button handler for a couple of simple buttons.
 */

#include "membat.h"
#include "SimpleButtons.h"

#include <Arduino.h>

SimpleButtons::SimpleButtons(int startstoppin, int selectpin)
{
    mStartStopPin = startstoppin;
    mSelectPin = selectpin;

    // Enable internal pullups:
    pinMode(mStartStopPin, INPUT_PULLUP);
    pinMode(mSelectPin, INPUT_PULLUP);
}

// read the buttons
unsigned int SimpleButtons::read_buttons()
{
    if (! digitalRead(mStartStopPin))
        return btnSTARTSTOP;

    if (! digitalRead(mSelectPin))
        return btnSELECT;

    return btnNONE;
}
