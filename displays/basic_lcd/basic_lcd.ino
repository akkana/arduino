/* -*- c-mode -*- */

/*
 * Arduino display test.
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// For tutorial at: http://www.ladyada.net/learn/lcd/charlcd.html
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup()
{
    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);

    // Set autoscrolling if desired:
    //lcd.autoscroll();

    // Print a message to the LCD.
    lcd.print("hello, world!");
}

void loop()
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
    delay(100);
}

