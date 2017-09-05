#include "SimpleIO.h"

#include "modes.h"

#include <Arduino.h>

SimpleIO::SimpleIO(unsigned int _buttonPin, unsigned int _rewindSwitch,
                   unsigned int* _LEDPins, unsigned int _numLEDs)
{
    mButtonPin = _buttonPin;
    mRewindSwitch = _rewindSwitch;
    mLEDPins = _LEDPins;
    mNumLEDs = _numLEDs;
}

void SimpleIO::init()
{
    pinMode(mButtonPin, INPUT);
    pinMode(mRewindSwitch, INPUT);

    for (unsigned int i=0; i < mNumLEDs; ++i)
        pinMode(mLEDPins[i], OUTPUT);
}

// Is the switch-modes switch pressed?
// Returns: 1 (switch up), -1 (switch down), 0 (don't switch).
short SimpleIO::modeSwitchPressed()
{
    return digitalRead(mButtonPin);
}

// Is the rewind switch on?
bool SimpleIO::rewindPressed()
{
    return digitalRead(mRewindSwitch);
}

unsigned int SimpleIO::checkButtonState(unsigned int startmode)
{
    // Are we rewinding?
    if (rewindPressed()) {
        return rewindMode(startmode);
    }

    // If nothing is pressed, don't change the mode:
    if (!modeSwitchPressed())
        return startmode;

    return nextMode(startmode);
}

// Show the mode:
void SimpleIO::showMode(unsigned int modeCode, const char* const extraString)
{
    showMode(modeCode);
}

void SimpleIO::showMode(unsigned int modeCode)
{
    int bitmode = modeCode;
    unsigned int i;

    if (isRewindMode(modeCode)) {
        // All LEDs high:
        for (i=0; i < mNumLEDs; ++i)
            digitalWrite(mLEDPins[i], HIGH);
    }
    else {
        // Else get the mode from modeSpeeds[mode].
        for (i=0; i < mNumLEDs; ++i) {
            if (bitmode & 1)
                digitalWrite(mLEDPins[i], HIGH);
            else
                digitalWrite(mLEDPins[i], LOW);
            bitmode >>= 1;
        }
    }
}
