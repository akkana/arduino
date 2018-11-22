#include <Arduino.h>

#include "TimerButton.h"

TimerButton::TimerButton(unsigned int pin)
{
    mLastPress = 0;
    mLastRelease = 0;

    mPin = pin;
    pinMode(mPin, INPUT_PULLUP);
    // Older way to do this:
    //digitalWrite(mPin, HIGH);
}

int TimerButton::checkPress(unsigned long now)
{
    // The value read is inverted to use internal pullups.
    bool state = ! digitalRead(mPin);

    if (mLastPress > mLastRelease) {
        // The button was already pressed the last time we were called.
        if (state) {    // it's still pressed.
            // Is it a longpress?
            if (now - mLastPress > LONGPRESSTIME)
                return 2;
            return 0;
        }

        // Otherwise it's recently been released. Check the time
        // and return either a press or a longpress
        mLastRelease = now;
        // If we've been longpressing, return 0 now.
        if (now - mLastPress > LONGPRESSTIME)
            return 0;
        // else return the press.
        return 1;
    }

    // Else the button wasn't previously pressed. Is it now?
    if (state) {    // newly pressed
        mLastPress = now;
        // Don't return a press yet: we can't know whether it's
        // a normal press or a longpress until either the longpress
        // interval expires or the button is released.
        return 0;
    }
    return 0;
}

