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

/*
 * Which type of buttons and display to use:
 */
#undef USE_DFROBOT
#ifdef USE_DFROBOT

#include "DFRobotButtons.h"
#include "DFRobotTimerDisplay.h"

DFRobotButtons buttons;
DFRobotTimerDisplay display;

#else

#include "SimpleButtons.h"
#include "ParallelLCDDisplay.h"

SimpleButtons buttons(9, 10);
ParallelLCDDisplay display(3, 4, 5, 6, 7, 8);

#endif

// What pin is the passive piezo buzzer on?
#define BUZZER 2

// What note to play?
#define HIGHNOTE   5500
#define MEDNOTE   3000

// This is for daylight use, no need for the backlight:
// (but set it to 50 when testing indoors)
int gBrightness = 40;

#define NUM_MODELS 10

unsigned int gCurModel = 0;
boolean gRunning = false;

Model::Model(const char* const name, unsigned long alarmtime)
{
    sprintf(mName, "%-8s", name);
    mAlarmTime = alarmtime;
    mStartTime = 0;
    mRunTime = 0;
}

Model* gModels[NUM_MODELS];

/*****
 *  ADD YOUR PLANES HERE: name (8 chars max) and number of seconds til alarm.
 *  Eventually it may be possible to add new ones in the field.
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

void beepDebounce()
{
    buttons.debounce();
    tone(BUZZER, HIGHNOTE);
    delay(80);
    noTone(BUZZER);
}

// Convert seconds to mm:ss
const char* TimerDisplay::timeString(long int secs)
{
    static char timestr[15];
    unsigned int mins = secs / 60;
    secs = secs % 60;
    sprintf(timestr, "%2d:%02d  ", (int)mins, (int)secs);
    return timestr;
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

    initModels();

    display.setBrightness(gBrightness);

    display.displayCurrentModel(gModels[gCurModel]);

    Serial.begin(9600);
    delay(500);
    Serial.println("La la la");
    /*
    */
}

void loop()
{
    unsigned long secs = millis() / 1000;     // seconds since power-up

    unsigned int button = buttons.read_buttons();
    if (button != btnNONE)
        Serial.println(button);

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

            beepDebounce();
            break;

        case btnSELECT:
            if (gRunning)
                break;
            ++gCurModel;
            if (gCurModel >= NUM_MODELS || gModels[gCurModel]->mAlarmTime == 0)
                gCurModel = 0;
            display.displayCurrentModel(gModels[gCurModel]);
            beepDebounce();
            break;

        case btnCLEAR:
            if (gRunning)
                break;
            gModels[gCurModel]->mRunTime = 0;
            noTone(BUZZER);
            display.displayCurrentModel(gModels[gCurModel]);
            beepDebounce();
            break;

        case btnUP:
            gBrightness += 30;
            if (gBrightness > 255)
                gBrightness = 255;
            display.setBrightness(gBrightness);
            beepDebounce();
            break;

        case btnDOWN:
            gBrightness -= 30;
            if (gBrightness < 0)
               gBrightness = 0;
            display.setBrightness(gBrightness);
            beepDebounce();
            break;

        case btnNONE:
            if (!gRunning)
                break;
            gModels[gCurModel]->mRunTime =
                secs - gModels[gCurModel]->mStartTime;
            display.displayCurrentModelTime(gModels[gCurModel]);

            if (gModels[gCurModel]->mRunTime > gModels[gCurModel]->mAlarmTime)
                overtimeBuzzer();
            break;
    }
}
