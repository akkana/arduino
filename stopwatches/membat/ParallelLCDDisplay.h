/*
 * A membat display using the DFrobot LCD button shield.
 */

#ifndef ParallelLCDDisplay_h
#define ParallelLCDDisplay_h

#include "membat.h"

#include <LiquidCrystal.h>

class ParallelLCDDisplay : public TimerDisplay
{
  private:
    LiquidCrystal lcd;

  public:
    ParallelLCDDisplay(int rs, int en, int d4, int d5, int d6, int d7);

    // Overridden methods:
    void displayCurrentModelTime(Model* curmodel);
    void displayCurrentModel(Model* curmodel);

    // There's no backlight, so no need for setBrightness.
};

#endif /* ParallelLCDDisplay_h */
