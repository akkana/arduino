/* -*- c-mode -*- */

/*
 * Arduino display test for a 16x1 LCD,
 * such as a Densitron 2, LM50 or GDM1601A.
 * For wiring see http://shallowsky.com/blog/hardware/surplus-lcd-arduino.html
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// For tutorial at: http://www.ladyada.net/learn/lcd/charlcd.html
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void clear()
{
    lcd.setCursor(0, 0);
    lcd.print("        ");
    lcd.setCursor(40, 0);
    lcd.print("        ");
}

// https://forum.arduino.cc/index.php?topic=442583.0
void setup()
{
    lcd.begin(16, 2);     // or lcd.begin(8,2);

    lcd.print("First8ch");
    lcd.setCursor(40, 0); // Works for showing characters on the right half of GDM1601A controlled 16x1 LCD displays.
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

