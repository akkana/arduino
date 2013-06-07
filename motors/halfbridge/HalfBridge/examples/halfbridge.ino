/* -*- c -*- */

#include <HalfBridge.h>

Motor motors[2] = { Motor(9, 2, 3), Motor(10, 4, 5) };

void setup()
{
    motors[0].init();
    motors[1].init();
}

#define FAST 200
#define SLOW 130

void loop()
{
    if (motors[0].mSpeed == 0 ) {
        motors[0].setSpeed(SLOW);
        motors[1].setSpeed(-SLOW);
    } else if (abs(motors[0].mSpeed) >= FAST) {
        motors[0].setSpeed(0);
        motors[1].setSpeed(0);
    } else {
        motors[0].setSpeed(FAST);
        motors[1].setSpeed(FAST);
    }

    delay(2000);
}
