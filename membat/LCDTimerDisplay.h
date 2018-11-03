/*
 * Global definitions for membat.
 */

#ifndef LCDTimerDisplay_h
#define LCDTimerDisplay_h

#include "membat.h"

#include <LiquidCrystal.h>

class LCDTimerDisplay : TimerDisplay
{
  private:
    LiquidCrystal lcd;

  public:
    LCDTimerDisplay();

    // Overridden methods:
    void displayCurrentModelTime(Model* curmodel);
    void displayCurrentModel(Model* curmodel);

    // Pass 0-255.
    void setBrightness(unsigned int bright);
};

#endif /* LCDTimerDisplay_h */
