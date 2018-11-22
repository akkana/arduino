#ifndef TimerButton_h
#define TimerButton_h

// A class for pushbuttons that handles debouncing and longpress.

// times in milliseconds
#define DEBOUNCETIME 500
#define LONGPRESSTIME 2000

class TimerButton
{

protected:
    unsigned int mPin;

    unsigned long mLastPress;
    unsigned long mLastRelease;

public:
    TimerButton(unsigned int pin);

    // Check for presses.
    // Returns:
    //     1 if the button has been pressed and released,
    //     2 if it's currently being longpressed,
    //     0 otherwise.
    int checkPress(unsigned long time_in_ms);

};

#endif
