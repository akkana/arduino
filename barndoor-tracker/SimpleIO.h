#ifndef SimpleIO_h
#define SimpleIO_h

#include "TrackerIO.h"

class SimpleIO : public TrackerIO
{
private:
    unsigned int mButtonPin;
    unsigned int mRewindSwitch;
    unsigned int* mLEDPins;
    unsigned int mNumLEDs;

public:
    SimpleIO(unsigned int buttonPin, unsigned int rewindSwitch,
             unsigned int* LEDPins, unsigned int numLEDs);

    void init();

    // Check button state, including rewind, and return the appropriate mode:
    unsigned int checkButtonState(unsigned int startmode);

    // Is the switch-modes switch pressed?
    // Returns: 1 (switch up), -1 (switch down), 0 (don't switch).
    short modeSwitchPressed();

    // Is the rewind switch on?
    bool rewindPressed();

    // Show the mode:
    void showMode(unsigned int modeCode);
    void showMode(unsigned int modeCode, const char* extraString);
};

#endif /* SimpleIO_h */
