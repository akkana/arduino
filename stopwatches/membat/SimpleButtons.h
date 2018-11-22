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

  public:
    SimpleButtons(int startstoppin, int selectpin);

    // The important routine: check whether a button is pressed and
    // return the code for it.
    unsigned int read_buttons();
};

#endif /* SimpleButtons_h */
