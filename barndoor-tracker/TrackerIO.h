#ifndef TrackerIO_h
#define TrackerIO_h

// An interface (abstract base class) for IO for the Barn-Door Tracker.
// Input: a way to switch modes and a way to start and stop rewinding.
// Output: display the current mode and whether we're rewinding.

class TrackerIO
{
public:
    virtual void init();

    // Check button state, including rewind, and return the appropriate mode:
    virtual unsigned int checkButtonState(unsigned int startmode) = 0;

    // XXX NEXT TWO ARE PROBABLY GOING AWAY:

    // Is the switch-modes switch pressed?
    // Returns: 1 (switch up), -1 (switch down), 0 (don't switch).
    virtual short modeSwitchPressed() = 0;

    // Is the rewind switch on?
    virtual bool rewindPressed() = 0;

    // Show the mode:
    virtual void showMode(unsigned int modeCode, const char* modeString) = 0;
};

#endif // TrackerIO_h

