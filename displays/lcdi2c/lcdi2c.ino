#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fruitless attempt to get this to work with the cheapo 4x7-segment LED:
LiquidCrystal_I2C lcd(0x51, 4, 1);

void setup()
{
    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.print("Hello, world!");

    //lcd.print("8888");
}

void loop()
{
    // Do nothing here...
}


