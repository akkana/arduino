#ifndef LCDKeypadShield_h
#define LCDKeypadShield_h

#include <LiquidCrystal.h>

#include "TrackerIO.h"

class LCDKeypadShield : public LiquidCrystal, public TrackerIO {
public:
    static uint8_t btnRIGHT, btnUP, btnDOWN, btnLEFT, btnSELECT, btnNONE;

    LCDKeypadShield(uint8_t rs, uint8_t enable,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    void init();

    unsigned short read_LCD_buttons();

    // TrackerIO implementation:

    unsigned int checkButtonState(unsigned int startmode);

    // Is the switch-modes switch pressed?
    // Returns: 1 (switch up), -1 (switch down), 0 (don't switch).
    short modeSwitchPressed();

    // Is the rewind switch on?
    bool rewindPressed();

    // Show the mode:
    void showMode(unsigned int modeCode, const char* modeString);

};

#endif

