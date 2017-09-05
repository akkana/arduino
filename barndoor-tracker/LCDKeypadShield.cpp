
#include "LCDKeypadShield.h"

#include "modes.h"

#include <Arduino.h>

/*
 * The shield exposes digital buttons on the header at the upper right:
 * 0 1 2 3 11 12 13
 */

// define some values used by the panel and buttons

static int sBrightness = 127;
#define BRIGHTNESS_CHANGE 25
#define BRIGHTNESS_PIN    10

uint8_t LCDKeypadShield::btnRIGHT  = 0;
uint8_t LCDKeypadShield::btnUP     = 1;
uint8_t LCDKeypadShield::btnDOWN   = 2;
uint8_t LCDKeypadShield::btnLEFT   = 3;
uint8_t LCDKeypadShield::btnSELECT = 4;
uint8_t LCDKeypadShield::btnNONE   = 5;

static long sBtnDownTime = 0;    // will be from millis()
static bool sRewinding = false;

#define LONGPRESS  2000    // milliseconds

LCDKeypadShield::LCDKeypadShield(uint8_t rs, uint8_t enable,
                                 uint8_t d0, uint8_t d1,
                                 uint8_t d2, uint8_t d3)
    : LiquidCrystal(rs, enable, d0, d1, d2, d3)
{
}

void LCDKeypadShield::init()
{
    begin(16, 2);              // start the library
    analogWrite(BRIGHTNESS_PIN, sBrightness);
    setCursor(0, 0);
    print("Hello, world"); // print a simple message
}

// read the buttons
unsigned short LCDKeypadShield::read_LCD_buttons()
{
    int adc_key_in = analogRead(0);      // read the value from the sensor
    // buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // so add approx 50 to those values and check to see if we are close
    if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;

    // For V1.0 comment the other threshold and use the one below:
    /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
     */

    return btnNONE;  // when all others fail, return this.
}

// TrackerIO implementation:

unsigned int LCDKeypadShield::checkButtonState(unsigned int startmode)
{
    unsigned short btn = read_LCD_buttons();

    // Are we rewinding? Once we start rewinding, we'll continue
    // until another button press cancels it.
    if (sRewinding && (btn == btnNONE || btn == btnSELECT))
        return rewindMode(startmode);

    if (btn == btnNONE) {
        if (sBtnDownTime)
            sBtnDownTime = 0;

        // Otherwise, no button is pressed so don't change the mode.
        return startmode;
    }

    // A button has been pressed. We'll need to know the time.
    long curtime = millis();

    // Was it the rewind button, and if so, has it been held long enough
    // to consider it a longpress?
    if (btn == btnSELECT) {
        if (sBtnDownTime == 0) {
            sBtnDownTime = curtime;
            return startmode;
        }

        if (curtime - sBtnDownTime >= LONGPRESS) {
            sRewinding = true;
            return rewindMode(startmode);
        }
    }

    // Okay, it's not the rewind button. So cancel any rewind.
    // And if we were rewinding, don't act on the button or advance the mode,
    // just cancel the rewind.
    if (sRewinding) {
        sRewinding = false;
        return startmode;
    }

    // Was a button pressed already?
    // These buttons require debouncing.
    if (sBtnDownTime > 0) {
        // A button was already pressed, let's assume it's the same one
        // and we already changed the mode.
        return startmode;
    }

    // Now we know a button was pressed and it wasn't previously pressed.
    // It's okay to act on this button.
    sBtnDownTime = curtime;

    if (btn == btnLEFT) {
        sBrightness -= BRIGHTNESS_CHANGE;
        if (sBrightness < 0)
            sBrightness = 0;
        analogWrite(BRIGHTNESS_PIN, sBrightness);
        return startmode;
    }

    if (btn == btnRIGHT) {
        sBrightness += BRIGHTNESS_CHANGE;
        if (sBrightness > 255)
            sBrightness = 255;
        analogWrite(BRIGHTNESS_PIN, sBrightness);
        return startmode;
    }

    if (btn == btnDOWN)
        // The DOWN button is pressed. Advance the mode, skipping rewind modes.
        return nextMode(startmode);

    // It was some other button, so ignore it.
    return startmode;
}

// Is the switch-modes switch pressed?
// Returns: 1 (switch up), -1 (switch down), 0 (don't switch).
short LCDKeypadShield::modeSwitchPressed()
{
    return 0;
}

// Is the rewind switch on?
bool LCDKeypadShield::rewindPressed()
{
    return false;
}

// Show the mode:
void LCDKeypadShield::showMode(unsigned int modeCode)
{
    static char line[17];

#ifdef DEBUGSERIAL
    Serial.print("showMode(");
    Serial.print(modeCode);
    Serial.println(")");
#endif /* DEBUGSERIAL */

    snprintf(line, 16, "%d: %-16s                ", modeCode, gSpeedModes[modeCode].name);
    setCursor(0, 0);
    print(line);
#ifdef DEBUGSERIAL
    Serial.println(line);
#endif /* DEBUGSERIAL */

    snprintf(line, 16, "speed %d                ", gSpeedModes[modeCode].speed);
    setCursor(0, 1);
    print(line);
#ifdef DEBUGSERIAL
    Serial.println(line);
#endif /* DEBUGSERIAL */
}

void LCDKeypadShield::showMode(unsigned int modeCode, const char* extraString)
{
    static char line[17];

#ifdef DEBUGSERIAL
    Serial.print("showMode(");
    Serial.print(modeCode);
    Serial.print(", ");
    Serial.print(extraString);
    Serial.println(")");
#endif /* DEBUGSERIAL */

    snprintf(line, 16, "%s %d                ",
             gSpeedModes[modeCode].name, gSpeedModes[modeCode].speed);
    setCursor(0, 0);
    print(line);
#ifdef DEBUGSERIAL
    Serial.println(line);
#endif /* DEBUGSERIAL */

    setCursor(0, 1);
    snprintf(line, 16, "%16s", extraString);
    print(line);
#ifdef DEBUGSERIAL
    Serial.println(extraString);
#endif /* DEBUGSERIAL */
}
