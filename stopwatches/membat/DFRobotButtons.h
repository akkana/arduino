/*
 * A membat buttons using the DFrobot LCD button shield.
 */

#ifndef DFRobotButtons_h
#define DFRobotButtons_h

class DFRobotButtons : TimerButtons
{
  public:
    DFRobotButtons();

    // The important routine: check whether a button is pressed and
    // return the code for it.
    unsigned int read_buttons();
};

#endif /* DFRobotButtons_h */
