/*
 * A membat buttons using the DFrobot LCD button shield.
 */

class DFRobotButtons : TimerButtons
{
  public:
    DFRobotButtons();

    // The important routine: check whether a button is pressed and
    // return the code for it.
    unsigned int read_buttons();
};
