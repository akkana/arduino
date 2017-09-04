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
        if (startmode < NUM_MODES)
            return 0;
        else
            return NUM_MODES;
    }

    // If nothing is pressed, don't change the mode:
    if (!modeSwitchPressed())
        return startmode;

    // The mode button is pressed, so advance to the next mode --
    // but not rewind.
    if ((++startmode) % NUM_MODES == 0)
        ++startmode;
    startmode %= NUM_MODES * 2;
    return startmode;
}

// Show the mode:
void SimpleIO::showMode(unsigned int modeCode, const char* const modeString)
{
    int bitmode = modeCode;
    unsigned int i;

    if (REWINDING(modeCode)) {
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
