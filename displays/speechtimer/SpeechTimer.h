#ifndef SpeechTimer_h
#define SpeechTimer_h

#include <LedControl.h>    // For the MAX7219 8x7-segment LED display
#include <FastLED.h>       // For the NeoPixel jewel

#include "TimerButton.h"

// Colors for showColor():
#define ST_BLACK       0
#define ST_GREEN       1
#define ST_YELLOW      2
#define ST_RED         3
#define ST_BRIGHTRED   4

class SpeechTimer {

protected:
    // Start and goal times are in seconds
    unsigned long mStartTime;
    unsigned long mGoalTime;

    // Elapsed time is in seconds
    unsigned int mElapsed;

    // Goal minutes
    unsigned int mGoalMinutes;

    // The amount of warning we give on the lights is related to goal minutes:
    unsigned int mWarningSecs;

    TimerButton* mStartBtn;
    TimerButton* mUpBtn;
    TimerButton* mDownBtn;

    LedControl* mDpy;

    CRGB* mLightString;

    void display2digits(unsigned int val, unsigned int startdigit,
                        boolean leadingzero=true, boolean leftjust=false,
                        boolean decimal=false);

    void displayGoalTime();
    void displayElapsedTime();

    void setPixel(int i, int red, int green, int blue);

    // How many seconds warning do we get for each light?
    // It's 30 seconds for short speeches, 1 minute for longer ones,
    // up to 2.5 minutes for long speeches.
    int warningSeconds(int speechMinutes);

public:
    // seg: the max7219 8-segment display.
    // startstoppin: Pin for the start/stop button (use internal pullup)
    // updownpins: Two pins, for up and down (internal pullup).
    // lightstring: some sort of FastLED object
    SpeechTimer(LedControl* dpy, unsigned int minutes,
                unsigned int startstoppin,
                unsigned int uppin, unsigned int downpin,
                CRGB* lightstring);

    // Call this for each tick of the clock, perhaps from loop().
    void tick();

    // Set all the LEDs to a color;
    // or, if nowMillis is specified, animate the given color.
    void showColor(unsigned int color, unsigned long nowMillis=0);

};

#endif
