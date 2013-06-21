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

#include "HalfBridge.h"

Motor::Motor(int enablePin, int logicPin0, int logicPin1)
{
    mEnablePin = enablePin;
    mLogicPins[0] = logicPin0;
    mLogicPins[1] = logicPin1;
}

void Motor::init()
{
    pinMode(mEnablePin, OUTPUT);
    pinMode(mLogicPins[0], OUTPUT);
    pinMode(mLogicPins[1], OUTPUT);

    mSpeed = 0;
    analogWrite(mEnablePin, 0);
    digitalWrite(mLogicPins[0], LOW);
    digitalWrite(mLogicPins[1], LOW);
}

#define DIFFSIGN(a, b) ((a >= 0) != (b >= 0))

#define DEBUG 1

//
// Speed is between -255 and 255. Negative speed will reverse direction.
//
void Motor::setSpeed(int speed)
{
#ifdef DEBUG
    Serial.print("Motor ");
    Serial.print(mEnablePin);
    Serial.print("::setSpeed ");
    Serial.print(speed);
    Serial.print(" (currently ");
    Serial.print(mSpeed);
    Serial.println(")");
#endif

    if (speed == mSpeed)
        return;

    // If we're changing to speed 0, no need to ramp down -- just quit.
    if (speed == 0) {
        digitalWrite(mLogicPins[0], LOW);
        digitalWrite(mLogicPins[1], LOW);

        analogWrite(mEnablePin, 0);
        mSpeed = 0;

#ifdef DEBUG
        Serial.print("Going to zero speed on pin ");
        Serial.println(mEnablePin);
#endif
        return;
    }

    // Are we changing direction?
    if (DIFFSIGN(speed, mSpeed) || (speed != 0 && mSpeed == 0))
    {
#ifdef DEBUG
        Serial.print("Changing speed/direction to ");
        Serial.println(speed);
#endif
        // Let the motor coast while we change direction:
        digitalWrite(mEnablePin, 0);
        mSpeed = 0;
        delay(50);

        // Set the direction: this is arbitrary, of course.
        // If you don't like it, swap motor wires.
        if (speed > 0) {
#ifdef DEBUG
            Serial.println("FORWARD");
#endif
            digitalWrite(mLogicPins[0], HIGH);
            digitalWrite(mLogicPins[1], LOW);
        }
        else {
#ifdef DEBUG
            Serial.println("BACKWARD");
#endif
            digitalWrite(mLogicPins[0], LOW);
            digitalWrite(mLogicPins[1], HIGH);
        }
    }

    analogWrite(mEnablePin, abs(speed));

    mSpeed = speed;
}


