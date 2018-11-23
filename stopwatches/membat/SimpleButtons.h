/*
 * A membat buttons using a couple of plain pushbuttons.
 */

#ifndef SimpleButtons_h
#define SimpleButtons_h

class SimpleButtons : public TimerButtons
{
  private:
    int mStartStopPin;
    int mSelectPin;

    int mCurBtn;
    unsigned int mCurBtnMillis;

    int mInLongpress;

  public:
    SimpleButtons(int startstoppin, int selectpin);

    // The important routine: check whether a button is pressed and
    // return the code for it.
    unsigned int read_buttons();

    // SimpleButtons handles its own debouncing
    void debounce();

    // How many milliseconds constitutes a long press?
    // Typically a few seconds.
    const unsigned int LONG_PRESS_TIME = 2000;
};

#endif /* SimpleButtons_h */
