/*
 * MemBat: it's BatTimer with memory!
 * A timer for model airplane batteries: sound an alarm after a fixed time.
 * With a display, buttons and lots of model memories,
 * and a passive piezo buzzer.
 * Copyright 2018 by Akkana Peck; share and enjoy under the GPLv2 or later.
 */

/* You can use different sets of buttons.
 * Write a .cpp file for your buttons that defines
 *   unsigned int read_buttons()
 * and returns the codes defined in membat.h.
 */

#include "membat.h"

#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// What pin is the passive piezo buzzer on?
#define BUZZER 3

// What note to play?
#define HIGHNOTE   5500
#define MEDNOTE   3000

// This is for daylight use, no need for the backlight:
// (but set it to 50 when testing indoors)
int gBrightness = 40;

#define NUM_MODELS 10

unsigned int gCurModel = 0;
boolean gRunning = false;

class Model
{
public:
    char mName[9];
    unsigned long mAlarmTime;    // Seconds before we'll give an alarm
    unsigned long mStartTime;    // Seconds since bootup when this model's clock started
    unsigned long mRunTime;      // How many seconds has this model run so far?

    Model(const char* const name, unsigned long alarmtime);
};

Model::Model(const char* const name, unsigned long alarmtime)
{
    sprintf(mName, "%-8s", name);
    mAlarmTime = alarmtime;
    mStartTime = 0;
    mRunTime = 0;
}

// Hardwire models:
Model* gModels[NUM_MODELS];


/*****
 *  ADD YOUR PLANES HERE: name (8 chars max) and number of seconds til alarm.
 */
#define MINUTES * 60
void initModels()
{
    int i = 0;

    // Short timeout, for testing:
    gModels[i++] = new Model("Test", 30);

    // Real models:
    gModels[i++] = new Model("Skunk", 6 MINUTES);
    gModels[i++] = new Model("Wildwing", 8 MINUTES);
    gModels[i++] = new Model("IFO", 6 MINUTES);

    // End with at least one null model:
    gModels[i++] = new Model("\0", 0);
}

void debounce()
{
    tone(BUZZER, HIGHNOTE);
    while (read_buttons() != btnNONE)
        ;
    noTone(BUZZER);
}

// Print mm:ss time string at the current LCD position.
void printTime(long int secs)
{
    char timestr[15];
    unsigned int mins = secs / 60;
    secs = secs % 60;
    sprintf(timestr, "%2d:%02d  ", (int)mins, (int)secs);
    lcd.print(timestr);
}

void displayCurrentModelTime()
{
    lcd.setCursor(0, 1);
    printTime(gModels[gCurModel]->mRunTime);
}

void displayCurrentModel()
{
    lcd.setCursor(0, 0);
    lcd.print(gModels[gCurModel]->mName);
    lcd.setCursor(10, 0);          // move cursor to line 0, col 10
    //lcd.print(gModels[gCurModel]->mAlarmTime / 60);
    printTime(gModels[gCurModel]->mAlarmTime);

    displayCurrentModelTime();
}

void setBrightness()
{
    analogWrite(10, gBrightness);
}

void overtimeBuzzer()
{
    unsigned long mils = millis();
    unsigned long oversecs = gModels[gCurModel]->mRunTime
        - gModels[gCurModel]->mAlarmTime;

    // Sound an obnoxious alarm for the first few seconds:
    if (oversecs < 5)
    {
        if (mils % 500 < 250)
            tone(BUZZER, MEDNOTE);
        else
            noTone(BUZZER);
        return;
    }

    // But nobody could fly with that going on all the time,
    // so tone it down to an occasional warning beep after that:
    if (oversecs % 20 == 0 && mils % 500 < 200)
        tone(BUZZER, MEDNOTE);
    else
        noTone(BUZZER);
}

void setup()
{
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);

    lcd.begin(16, 2);              // start the library

    initModels();

    setBrightness();

    displayCurrentModel();
}

void loop()
{
    unsigned long secs = millis() / 1000;     // seconds since power-up

    unsigned int button = read_buttons();

    switch (button)
    {
        case btnSTARTSTOP:
            gRunning = !gRunning;
            if (gRunning) {
                gModels[gCurModel]->mStartTime =
                    secs - gModels[gCurModel]->mRunTime;

                // Double beep means the timer is running:
                tone(BUZZER, HIGHNOTE);
                delay(100);
                noTone(BUZZER);
                delay(100);
                tone(BUZZER, HIGHNOTE);
                delay(100);
                noTone(BUZZER);
            }

            debounce();
            break;

        case btnSELECT:
            if (gRunning)
                break;
            ++gCurModel;
            if (gCurModel >= NUM_MODELS || gModels[gCurModel]->mAlarmTime == 0)
                gCurModel = 0;
            displayCurrentModel();
            debounce();
            break;

        case btnCLEAR:
            if (gRunning)
                break;
            gModels[gCurModel]->mRunTime = 0;
            noTone(BUZZER);
            displayCurrentModel();
            debounce();
            break;

        case btnUP:
            gBrightness += 30;
            if (gBrightness > 255)
                gBrightness = 255;
            setBrightness();
            debounce();
            break;

        case btnDOWN:
            gBrightness -= 30;
            if (gBrightness < 0)
               gBrightness = 0;
            setBrightness();
            debounce();
            break;

        case btnNONE:
            if (!gRunning)
                break;
            gModels[gCurModel]->mRunTime =
                secs - gModels[gCurModel]->mStartTime;
            displayCurrentModelTime();

            if (gModels[gCurModel]->mRunTime > gModels[gCurModel]->mAlarmTime)
                overtimeBuzzer();
            break;
    }

    delay(20);
}
