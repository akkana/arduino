
/*
 * A membat button handler for a couple of simple buttons.
 */

#include "membat.h"
#include "SimpleButtons.h"

#include <Arduino.h>

unsigned long MAX_MILLIS = 0xFFFFFFFF;

SimpleButtons::SimpleButtons(int startstoppin, int selectpin)
{
    mStartStopPin = startstoppin;
    mSelectPin = selectpin;

    // Enable internal pullups:
    pinMode(mStartStopPin, INPUT_PULLUP);
    pinMode(mSelectPin, INPUT_PULLUP);

    mCurBtn = btnNONE;
    mCurBtnMillis = millis();

    mInLongpress = false;
}

void SimpleButtons::debounce()
{
}

// Read the buttons.
// Return a non-NONE value only on button release or longpress.
unsigned int SimpleButtons::read_buttons()
{
    unsigned long newMillis = millis();
    int newButton;

    if (! digitalRead(mStartStopPin))
        //Serial.println("startstop"),
        newButton = btnSTARTSTOP;

    else if (! digitalRead(mSelectPin))
        //Serial.println("select"),
        newButton = btnSELECT;

    else
        newButton = btnNONE;

    // Check for long press:
    if (newButton != btnNONE && newButton == mCurBtn
        && newMillis - mCurBtnMillis > LONG_PRESS_TIME)
    {
        // Already in a longpress? Ignore further button holding.
        if (mInLongpress)
            return btnNONE;

        mInLongpress = true;

        // Which buttons have a longpress action?
        if (mCurBtn == btnSTARTSTOP) {
            return btnCLEAR;
        }

        return btnNONE;
    }

    // Did nothing change?
    if (newButton == mCurBtn)
        return btnNONE;

    // Something definitely changed.

    // Not a longpress. Is it a button release?
    if (newButton == btnNONE) {
        newButton = mCurBtn;       // Save the button that was just released
        mCurBtn = btnNONE;
        mCurBtnMillis = newMillis;

        // Is this the end of a longpress? If so, ignore.
        if (mInLongpress) {
            mInLongpress = false;
            return btnNONE;
        }

        // debounce
        delay(50);
        return newButton;
    }

    // Must be a new button press.
    mCurBtn = newButton;
    mCurBtnMillis = newMillis;

    // debounce
    delay(50);

    return btnNONE;
}
