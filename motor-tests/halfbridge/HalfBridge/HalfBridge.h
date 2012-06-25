/* -*- c -*- */

// Drive a pair of motors connected to a L293NE or SN754410 H-bridge chip.
// http://itp.nyu.edu/physcomp/Labs/DCMotorControl
// Each motor has two logic pins and one enable pin.
// The enable pin is the analog pin that controls the speed;
// the logic pins control the direction; setting both to the
// same value stops the motor.
// 

// Represent each motor's speed as an integer between -255 and 255.
// We'll map that into appropriate signals to send to the motor.
// When speed > 0, direction will have logic pin 0 high, 1 low;
// speed < 0 will be the reverse.

#ifndef HalfBridge_h
#define HalfBridge_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <wiring.h>
#endif

// This def causes trouble on some versions of Arduino:
#undef abs

class Motor {
  public:
    int mEnablePin;
    int mLogicPins[2];

    int mSpeed;

    Motor(int enablePin, int logicPin1, int logicPin2);
    void init();
    void setSpeed(int speed);
};

typedef class Motor Motor;


/*
 * Sample usage:

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

*/

#endif /* HalfBridge_h */
