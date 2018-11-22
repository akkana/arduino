
/*
 * A membat button handler for a DF Robot LCD Keypad Shield.
 *
 * The shield exposes digital buttons on the header at the upper right:
 * 0 1 2 3 11 12 13
 * It also exposes analog buttons at lower right:
 * I haven't tested but I believe these are analog 1-5
 * (analog 0 is used for the buttons).
 */

#include "membat.h"
#include "DFRobotButtons.h"

#include <Arduino.h>

// Buttons on the shield with names that don't reflect the shield's names:
#define btnRIGHT  btnCLEAR
#define btnLEFT   btnSTARTSTOP

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

DFRobotButtons::DFRobotButtons()
{
}

// read the buttons
unsigned int DFRobotButtons::read_buttons()
{
    adc_key_in = analogRead(0);      // read the value from the sensor
    if (adc_key_in > 1000) return btnNONE;

#define V1_0
#ifdef V1_0
    /* For V1.0 comment the other thresholds and use these: */
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnSTARTSTOP;
    if (adc_key_in < 790)  return btnSELECT;
#else
    /* For V1.1 us these thresholds: */
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnSTARTSTOP;
    if (adc_key_in < 850)  return btnSELECT;
#endif

    return btnNONE;  // when all others fail, return this...
}
