/*
 * BatTimer
 * A timer for model airplane batteries: sound an alarm after a fixed time.
 * With a display, buttons and lots of model memories,
 * and a passive piezo buzzer.
 * Copyright 2018 by Akkana Peck; share and enjoy under the GPLv2 or later.
 */

#include <LiquidCrystal.h>

/*******************************************************
This program will test the LCD panel and the buttons
Based on a prograb by Mark Bramwell, July 2010
********************************************************/

/*
 * Akk note:
 * This is for a DF Robot LCD Keypad Shield.
 * The shield exposes digital buttons on the header at the upper right:
 * 0 1 2 3 11 12 13
 * It also exposes analog buttons at lower right:
 * I haven't tested but I believe these are analog 1-5
 * (analog 0 is used for the buttons).
 */

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

// Buttons on the shield:
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Which button to use for which function:
// Let btnSELECT select models, btnUP and btnDOWN adjust alarm time.
#define btnSTARTSTOP btnLEFT
#define btnCLEAR     btnRIGHT

// What pin is the passive piezo buzzer on?
#define BUZZER 3

// What note to play?
#define HIGHNOTE   4500
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
    unsigned long mAlarmTime;    // Minutes before we'll give an alarm
    unsigned long mStartTime;    // Seconds since bootup when this model's clock started
    unsigned long mRunTime;      // How many seconds has this model run so far?

    Model(const char* const name, int alarmtime);
};

Model::Model(const char* const name, int alarmtime)
{
    sprintf(mName, "%-8s", name);
    mAlarmTime = alarmtime;
    mStartTime = 0;
    mRunTime = 0;
}

// Hardwire models:
Model* gModels[NUM_MODELS];

/*****
 *  ADD YOUR PLANES HERE: name (8 chars max) and number of minutes.
 */
void initModels()
{
    gModels[0] = new Model("Skunk", 1);
    gModels[1] = new Model("Wildwing", 8);
    gModels[2] = new Model("IFO", 6);
    gModels[3] = new Model("\0", 0);
}

// read the buttons
unsigned int read_LCD_buttons()
{
    adc_key_in = analogRead(0);      // read the value from the sensor
    if (adc_key_in > 1000) return btnNONE;

#define V1_0
#ifdef V1_0
    /* For V1.0 comment the other thresholds and use these: */
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
#else
    /* For V1.1 us these thresholds: */
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;
#endif

    return btnNONE;  // when all others fail, return this...
}

void debounce()
{
    tone(BUZZER, HIGHNOTE);
    while (read_LCD_buttons() != btnNONE)
        ;
    noTone(BUZZER);
}

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
    int uptime = millis() / 1000;
    lcd.setCursor(10, 1);
    lcd.print(uptime);
}

void displayCurrentModel()
{
    lcd.setCursor(0, 0);
    lcd.print(gModels[gCurModel]->mName);
    lcd.setCursor(10, 0);          // move cursor to line 0, col 10
    lcd.print(gModels[gCurModel]->mAlarmTime);

    displayCurrentModelTime();
}

void setBrightness()
{
    analogWrite(10, gBrightness);
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
    unsigned long mils = millis();           // milliseconds since power-up
    unsigned long secs = mils / 1000;        // seconds since power-up

    unsigned int button = read_LCD_buttons();

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

            if (gModels[gCurModel]->mRunTime >
                gModels[gCurModel]->mAlarmTime * 60) {
                if (mils % 500 < 250)
                    tone(BUZZER, MEDNOTE);
                else
                    noTone(BUZZER);
            }
            break;
    }

    delay(20);
}
