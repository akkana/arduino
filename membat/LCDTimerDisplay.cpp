/*
 * Timer display on a 2x16 segment LCD
 */

#include "LCDTimerDisplay.h"

#include <Arduino.h>

LCDTimerDisplay::LCDTimerDisplay() : lcd(8, 9, 4, 5, 6, 7)
{
    lcd.begin(16, 2);              // start the library
}

void LCDTimerDisplay::displayCurrentModelTime(Model* curmodel)
{
    lcd.setCursor(0, 1);
    lcd.print(timeString(curmodel->mRunTime));
}

void LCDTimerDisplay::displayCurrentModel(Model* curmodel)
{
    lcd.setCursor(0, 0);
    lcd.print(curmodel->mName);
    lcd.setCursor(10, 0);          // move cursor to line 0, col 10
    //lcd.print(curmodel->mAlarmTime / 60);
    lcd.print(timeString(curmodel->mAlarmTime));

    displayCurrentModelTime(curmodel);
}

void LCDTimerDisplay::setBrightness(unsigned int bright)
{
    analogWrite(10, bright);
}

// select the pins used on the LCD panel

