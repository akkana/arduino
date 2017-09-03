// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

//
// To set the time on a new RTC module:
// touch this file (to make sure it will be recompiled),
// pull the battery out of the RTC module (try pushing it from the
// back with a wooden toothpick), wait a while, then re-insert it,
// then do this:
//   make; make download; ardmonitor 57600
// Don't try to do the steps separately; the RTC might initialize incorrectly.
// Check the time printed out to verify it's working.
// 

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () {
  Serial.begin(57600);

  Wire.begin();
  Serial.println("Called Wire.begin");

  rtc.begin();
  Serial.println("Called rtc.begin");

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");

    // Uncomment this if you want to set the RTC ...
    if (1) {
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(__DATE__, __TIME__));
      Serial.print("Set date to ");
      Serial.println(__DATE__);
      Serial.print("Set time to ");
      Serial.println(__TIME__);
    }
  }
  else Serial.println("RTC is running");
}

void loop () {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print("Unix time since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

#if 0
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
#endif

    Serial.println();
    delay(3000);
}
