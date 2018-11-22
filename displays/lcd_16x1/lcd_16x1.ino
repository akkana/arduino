/* -*- c-mode -*- */

/*
 * Arduino display test for a 16x1 LCD,
 * such as a Densitron 2, LM50 or GDM1601A.
 * http://shallowsky.com/blog/hardware/surplus-lcd-arduino.html
 *
 * Wiring: set up so as to avoid the pins used for the ISP,
 * to make it easier to use with a bare breadboard atmega328.
 * 1st 3 pins on the display go to Gnd, 5v, Contrast pot.
 * Next 3 go to pin 3, Gnd, pin 4.
 * Then skip 4 pins, and the final 4 go to pins 5, 6, 7, 8.
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

void clear()
{
    lcd.setCursor(0, 0);
    lcd.print("        ");
    lcd.setCursor(40, 0);
    lcd.print("        ");
}

// The LiquidCrystal library doesn't work right with these single-line LCDs;
// you have to trick them. Initialize as 16, 2 even though it's really 16x1;
// then use lcd.setCursor(0, 0) for the first 8 characters,
// lcd.setCursor(40, 0) for the second.
// Thanks to: https://forum.arduino.cc/index.php?topic=442583.0
void setup()
{
    lcd.begin(16, 2);     // or lcd.begin(8,2);

    lcd.print("First8ch");
    lcd.setCursor(40, 0);
    lcd.print("Last8ch.");
    delay(5000);
    clear();
}

void loop()
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);

    lcd.setCursor(40, 0);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
    delay(100);
}

