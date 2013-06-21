/* -*- c -*- */

#include <HalfBridge.h>

#define DEBUG 1

// Motor 0 has a speed control on pin 9, direction controls on pins 2 and 3.
// Motor 1 uses 10 for speed control, 4 and 5 for direction.
Motor motors[2] = { Motor(9, 2, 3), Motor(10, 4, 5) };

void setup()
{
    motors[0].init();
    motors[1].init();

#ifdef DEBUG
    Serial.begin(9600);
#endif

    // Long delay at the beginning, to allow time to unplug USB.
    delay(7000);
}

void loop()
{
    int i;
    Serial.println("Ramping up to 255");
    for (i=0; i<255; i++) {
        motors[0].setSpeed(i);
        motors[1].setSpeed(i);
        delay(10);
    }
    Serial.println("Ramping down to 0");
    for (i=255; i>0; i--) {
        motors[0].setSpeed(i);
        motors[1].setSpeed(i);
        delay(10);
    }
    //motors[0].setSpeed(0);
    //motors[1].setSpeed(0);
    Serial.println("Should be at zero now");
    delay(4000);

#ifdef NOTDEF
    delay(700);

    for (i=0; i<255; i++) {
        motors[0].setSpeed(-i);
        motors[1].setSpeed(-i);
        delay(10);
    }
    for (i=255; i>0; i--) {
        motors[0].setSpeed(-i);
        motors[1].setSpeed(-i);
        delay(10);
    }
    //motors[0].setSpeed(0);
    //motors[1].setSpeed(0);
    delay(700);
#endif
}
