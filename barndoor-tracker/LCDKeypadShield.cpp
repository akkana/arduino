
#include "LCDKeypadShield.h"

#include "modes.h"

#include <Arduino.h>

/*
 * The shield exposes digital buttons on the header at the upper right:
 * 0 1 2 3 11 12 13
 */

// define some values used by the panel and buttons

static int brightness = 127;

uint8_t LCDKeypadShield::btnRIGHT  = 0;
uint8_t LCDKeypadShield::btnUP     = 1;
uint8_t LCDKeypadShield::btnDOWN   = 2;
uint8_t LCDKeypadShield::btnLEFT   = 3;
uint8_t LCDKeypadShield::btnSELECT = 4;
uint8_t LCDKeypadShield::btnNONE   = 5;

LCDKeypadShield::LCDKeypadShield(uint8_t rs, uint8_t enable,
                                 uint8_t d0, uint8_t d1,
                                 uint8_t d2, uint8_t d3)
    : LiquidCrystal(rs, enable, d0, d1, d2, d3)
{
}

void LCDKeypadShield::init()
{
    begin(16, 2);              // start the library
    analogWrite(10, brightness);
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

    // Are we rewinding?
    if (btn == btnSELECT) {
        if (startmode < NUM_MODES)
            return 0;
        else
            return NUM_MODES;
    }

    if (btn == btnLEFT) {
        brightness -= 10;
        if (brightness < 10)
            brightness == 10;
        analogWrite(10, brightness);
        return startmode;
    }

    if (btn == btnRIGHT) {
        brightness += 10;
        if (brightness > 255)
            brightness == 255;
        analogWrite(10, brightness);
        return startmode;
    }

    if (btn != btnDOWN)
        return startmode;

    // The DOWN button is pressed. Advance the mode, skipping rewind modes.
    if ((++startmode) % NUM_MODES == 0)
        ++startmode;
    startmode %= NUM_MODES * 2;
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
void LCDKeypadShield::showMode(unsigned int modeCode, const char* modeString)
{
    static char line[17];
    snprintf(line, 16, "%-16s", "showMode: ");
    setCursor(0, 0);
    print(line);
    snprintf(line, 16, "%-16s", modeString);
    setCursor(0, 1);
    print(line);
}
