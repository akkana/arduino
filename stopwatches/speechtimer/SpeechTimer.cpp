#include "SpeechTimer.h"

#include "rotary-encoder.h"

// Brightness, as a percentage. 40 is good for debugging but for
// something that can be seen across the room, brighter is better.
#define BRIGHTNESS 80

#define NUM_LEDS    7
#define FRAMES_PER_SECOND 15  // 60

SpeechTimer::SpeechTimer(LedControl* dpy, unsigned int minutes,
                         unsigned int startstoppin,
                         CRGB* lightstring,
                         unsigned int uppin, unsigned int downpin)
{
    mDpy = dpy;
    mGoalMinutes = minutes;
    mWarningSecs = warningSeconds(minutes);

    // Create the buttons:
    mStartBtn = new TimerButton(startstoppin);

    // If up and down pins weren't specified, use a rotary encoder:
    if (uppin == ST_NOPIN || downpin == ST_NOPIN) {
        mUpBtn = 0;
        mDownBtn = 0;
        initRotaryEncoder();
    }
    else {
        mUpBtn = new TimerButton(uppin);
        mDownBtn = new TimerButton(downpin);
    }

    mLightString = lightstring;

    mStartTime = 0;
    mGoalTime = 0;

    mElapsed = 0;

    mOldEncPos = 0;

    displayGoalTime();

    showColor(ST_BLACK);
}

void SpeechTimer::setPixel(int i, int red, int green, int blue)
{
    mLightString[i].red = red;
    mLightString[i].green = green;
    mLightString[i].blue = blue;
}

void SpeechTimer::showColor(unsigned int color, unsigned long nowMillis)
{
    static int chase = 0;

    for(int i = 0; i < NUM_LEDS; i++) {
        if (color == ST_BRIGHTRED)
            setPixel(i, BRIGHTNESS*3, 0, 0);

        else if (color == ST_GREEN)
            setPixel(i, 0, BRIGHTNESS, 0);

        else if (color == ST_YELLOW)
            setPixel(i, BRIGHTNESS * 1.1, BRIGHTNESS * .75, 0);

        else if (color == ST_RED)
            setPixel(i, BRIGHTNESS, 0, 0);

        else
            setPixel(i, 0, 0, 0);
    }

    // Some attention-grabbing animation if we're more than 30 seconds over
    if (nowMillis) {
        unsigned long tenths = nowMillis / 100;
        Serial.println("Chasing ");
        setPixel(tenths % NUM_LEDS, BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);
    }
    chase = (chase+1) % NUM_LEDS;

    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void SpeechTimer::updateGoalTime(int amt)
{
    mGoalMinutes += amt;
    mWarningSecs = warningSeconds(mGoalMinutes);
    if (mGoalMinutes < 1)
        mGoalMinutes = 1;
    displayGoalTime();
}

//
// tick() is called a few times per second, whether or not we're timing.
// It updates the display, checks the buttons and acts accordingly.
//
void SpeechTimer::tick()
{
    unsigned long nowMillis = millis();
    unsigned long nowSecs = nowMillis / 1000;

    // Check the various button states.

    // Is the start button pressed? Check for longpress too.
    int startpressed = mStartBtn->checkPress(nowMillis);
    if (startpressed > 1) {
        if (!mStartTime) {
            // We're not running: safe to reset the clock.
            mStartTime = 0;
            mGoalTime = 0;
            mElapsed = 0;
        }
        // Else, if we're running, ignore longpress
    }

    else if (startpressed) {    // press but not longpress. Toggle the timer.
        // Is the clock stopped? Start it.
        Serial.println("Starting the clock");
        if (!mElapsed) {
            mStartTime = nowSecs;
            mGoalTime = mStartTime + mGoalMinutes * 60;
            mElapsed = 0;
        }

        // Is the clock running? Pause it.
        else if (mStartTime) {
            mElapsed = nowSecs - mStartTime;
            mStartTime = 0;
            mGoalTime = 0;

            // In case someone spun the rotary encoder while the timer
            // was running, ignore it.
            if (!mUpBtn)
                mOldEncPos = getRotaryPosition();
        }

        // else the clock is paused, so restart it.
        else {
            mStartTime = nowSecs - mElapsed;
            mGoalTime = mStartTime + mGoalMinutes * 60;
        }
    }

    // Check the up and down buttons, but only if the clock isn't running.
    if (!mStartTime) {
        if (!mUpBtn) {     // rotary encoder, not buttons
            int encoderPos = getRotaryPosition();
            if (encoderPos > mOldEncPos)
                updateGoalTime(1);
            else if (encoderPos < mOldEncPos)
                updateGoalTime(-1);
            mOldEncPos = encoderPos;
        }
        else if (mUpBtn->checkPress(nowMillis)) {
            updateGoalTime(1);
        }
        else if (mDownBtn->checkPress(nowMillis)) {
            updateGoalTime(-1);
        }
    }

    // All buttons are checked. Now update the display and check lights.

    if (mStartTime)      // clock is running, so update elapsed time
        mElapsed = (nowSecs - mStartTime);

    // Shouldn't need to do this if we're paused, but it doesn't hurt.
    displayElapsedTime();

    /*
    Serial.print("nowSecs: ");
    Serial.print(nowSecs);
    Serial.print(", mGoalTime: ");
    Serial.print(mGoalTime);
    Serial.println();
    */
    if (!mGoalTime)
        showColor(ST_BLACK);

    // Flashing warning always starts at 30 seconds over,
    // regardless of the speech length
    else if (nowSecs >= mGoalTime + 30)
        showColor(ST_BRIGHTRED, nowMillis);

    else if (nowSecs >= mGoalTime)
        showColor(ST_RED);
    else if (nowSecs >= mGoalTime - mWarningSecs)
        showColor(ST_YELLOW);
    else if (nowSecs >= mGoalTime - mWarningSecs*2)
        showColor(ST_GREEN);
    else
        showColor(ST_BLACK);

    /*
    Serial.print("now is ");
    Serial.print(nowSecs);
    Serial.print(", goalTime is ");
    Serial.print(mGoalTime);
    Serial.print(", warning secs ");
    Serial.print(mWarningSecs);
    Serial.println();
    */
}

// Display a number on 7-segment digits startdigit and startdigit-1.
// if decimal is true, add a decimal point on the right digit, startdigit-1
void SpeechTimer::display2digits(unsigned int val, unsigned int leftdigit,
                                 boolean leadingzero, boolean leftjust,
                                 boolean decimal)
{
    if (val >= 10) {
        mDpy->setDigit(0, leftdigit,  (int)(val / 10), false);
        mDpy->setDigit(0, leftdigit-1,       val % 10, decimal);
    }
    else if (leadingzero) {
        mDpy->setDigit(0, leftdigit,     0, false);
        mDpy->setDigit(0, leftdigit-1, val, decimal);
    }
    else if (leftjust) {
        mDpy->setDigit(0, leftdigit,    val, false);
        mDpy->setChar( 0, leftdigit-1,  ' ', decimal);
    }
    else {
        mDpy->setChar( 0, leftdigit,    ' ', false);
        mDpy->setDigit(0, leftdigit-1,  val, decimal);
    }

}

int SpeechTimer::warningSeconds(int speechMinutes)
{
    // Very short speeches, give 30 seconds warning per light:
    if (speechMinutes < 3)
        return 30;
    // Up to 15 minutes, give one minute per light:
    if (speechMinutes < 15)
        return 60;
    // 15 and above, give 2:30 per light
    return 150;
}

void SpeechTimer::displayGoalTime()
{
    display2digits(mGoalMinutes, 7, false, true, false);
}

void SpeechTimer::displayElapsedTime()
{
    unsigned int minutes = mElapsed / 60;
    unsigned int seconds = mElapsed % 60;
    display2digits(minutes, 3, false, false, true);
    display2digits(seconds, 1,  true,  true, false);
}

