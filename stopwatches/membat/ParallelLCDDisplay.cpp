/*
 * Timer display on a 2x16 segment LCD
 */

#include "ParallelLCDDisplay.h"

#include <Arduino.h>

ParallelLCDDisplay::ParallelLCDDisplay(int rs, int en,
                                       int d4, int d5, int d6, int d7)
    : lcd(rs, en, d4, d5, d6, d7)
{
    lcd.begin(16, 2);
    // It's really 16x1 but apparently the LCD library needs this.
}

void ParallelLCDDisplay::displayCurrentModelTime(Model* curmodel)
{
    lcd.setCursor(0, 0);
    lcd.print(timeString(curmodel->mRunTime));

    lcd.setCursor(6, 0);
    lcd.print((int)(curmodel->mAlarmTime / 60));
}

void ParallelLCDDisplay::displayCurrentModel(Model* curmodel)
{
    lcd.setCursor(40, 0);                // Second 8 characters
    lcd.print(curmodel->mName);

    lcd.setCursor(5, 0);
    lcd.print("/");
    lcd.setCursor(6, 0);
    lcd.print((int)(curmodel->mAlarmTime / 60));

    displayCurrentModelTime(curmodel);  // Will use the 1st 8 characters
}


