/*
 * Global definitions for membat.
 */

#ifndef membat_h
#define membat_h

/************ A basic model definition *************/
class Model
{
public:
    char mName[9];
    unsigned long mAlarmTime;    // Seconds before we'll give an alarm
    unsigned long mStartTime;    // Seconds since bootup when this model's clock started
    unsigned long mRunTime;      // How many seconds has this model run so far?

    Model(const char* const name, unsigned long alarmtime);
};

/************ Input buttons *************/

// Which button to use for which function:
// Let btnSELECT select models, btnUP and btnDOWN adjust alarm time.
#define btnNONE      0
#define btnSTARTSTOP 1
#define btnSELECT    2
#define btnCLEAR     3
#define btnUP        4
#define btnDOWN      5

class TimerButtons
{
  public:
    TimerButtons();

    // The important routine: check whether a button is pressed and
    // return the code for it.
    virtual unsigned int read_buttons() = 0;

    virtual void debounce();
};

/************ Display *************/
class TimerDisplay
{
  public:
    TimerDisplay();

    // Utility routine: convert seconds to mm:ss
    virtual const char* timeString(long int secs);

    // Override these methods in your subclass:
    virtual void displayCurrentModelTime(Model* curmodel) = 0;
    virtual void displayCurrentModel(Model* curmodel) = 0;

    // Override this method if you support it.
    // Pass 0-255.
    virtual void setBrightness(unsigned int bright);
};

#endif /* membat.h */

