#include <Arduino.h>

#include "MultiLCD.h"

LCD_OLED* led;

void setup()
{
    led = new LCD_OLED();

    led->setCursor(0, 0);
    led->print("1234");
    delay(10);
}

void loop()
{
    delay(1000);
}
