/*
 * A membat display using the DFrobot LCD button shield.
 */

#ifndef DFRobotTimerDisplay_h
#define DFRobotTimerDisplay_h

#include "membat.h"

#include <LiquidCrystal.h>

class DFRobotTimerDisplay : TimerDisplay
{
  private:
    LiquidCrystal lcd;

  public:
    DFRobotTimerDisplay();

    // Overridden methods:
    void displayCurrentModelTime(Model* curmodel);
    void displayCurrentModel(Model* curmodel);

    // Pass 0-255.
    void setBrightness(unsigned int bright);
};

#endif /* DFRobotTimerDisplay_h */
