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

#define TOPSPEED 180
#define STEP 4

void loop()
{
    int i;
    Serial.print("Ramping up to ");
    Serial.println(TOPSPEED);
    for (i=0; i <= TOPSPEED; i += STEP) {
        motors[0].setSpeed(i);
        motors[1].setSpeed(i);
        //delay(10);
    }
    Serial.println("Ramping down to 0");
    for (i=TOPSPEED; i >= 0; i -= STEP) {
        motors[0].setSpeed(i);
        motors[1].setSpeed(i);
    }
    motors[0].setSpeed(0);
    motors[1].setSpeed(0);
    Serial.println("Sitting at zero for 4 seconds");
    delay(4000);

    Serial.print("Backwards: Ramping up to ");
    Serial.println(-TOPSPEED);
    for (i=0; i <= TOPSPEED; i += STEP) {
        motors[0].setSpeed(-i);
        motors[1].setSpeed(-i);
        //delay(10);
    }
    Serial.println("Ramping down to 0");
    for (i=TOPSPEED; i >= 0; i -= STEP) {
        motors[0].setSpeed(-i);
        motors[1].setSpeed(-i);
    }
    motors[0].setSpeed(0);
    motors[1].setSpeed(0);
    Serial.println("Sitting at zero for 4 seconds");
    delay(4000);

    Serial.println("Turning in a wide circle");
    for (i=0; i <= TOPSPEED; i += STEP) {
        motors[0].setSpeed(i);
        motors[1].setSpeed((int)(i/4));
        //delay(10);
    }
    Serial.println("Ramping down to 0");
    for (i=TOPSPEED; i >= 0; i -= STEP) {
        motors[0].setSpeed(i);
        motors[1].setSpeed((int)(i/4));
    }
    motors[0].setSpeed(0);
    motors[1].setSpeed(0);
    Serial.println("Sitting at zero for 4 seconds");
    delay(4000);
}
