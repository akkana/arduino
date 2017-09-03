// A simple digital clock, using the Adafruit 7-segment LED backpack
// and the DS1307 RTC module.

#include <Wire.h>            // For I2C
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"    // Needed by LEDBackpack for other hardware
#include "RTClib.h"

RTC_DS1307 RTC;
Adafruit_7segment clockdisp = Adafruit_7segment();

#define DEBUG 1

void setup()
{
#if DEBUG
    Serial.begin(57600);
    Serial.println("Arduino digital clock")
#endif
    Wire.begin();
    RTC.begin();
    clockdisp.begin(0x70);

    // Brightness goes from 0 to 15
    clockdisp.setBrightness(0);
}

void loop()
{
    DateTime now = RTC.now();
    int decimalTime = now.hour() * 100 + now.minute();
#if DEBUG
    Serial.println(decimalTime);
#endif
    clockdisp.print(decimalTime);
    clockdisp.drawColon(true);
    clockdisp.writeDisplay();

    delay(1000);
    clockdisp.drawColon(false);
    clockdisp.writeDisplay();

    delay(1000);

}
