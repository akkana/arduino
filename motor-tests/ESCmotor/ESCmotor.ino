/* -*- c -*- */

// Drive a toy truck using two electronic speed controllers,
// one for driving and one for steering,
// and the Servo library.

#include <Servo.h>

#define MOTOR_PIN        8
#define STEER_PIN       10

Servo motorServo;
Servo steerServo;

// The GWS ESCs I'm using have a weird arming sequence:
// move to low throttle, wait a second, then move to zero.
void armESC(Servo servo)
{
    digitalWrite(13, HIGH);
    servo.write(0);
    delay(1200);
    servo.write(70);
    delay(1200);
    servo.write(0);
    delay(1200);
    digitalWrite(13, LOW);
}

void setup()
{
    motorServo.attach(MOTOR_PIN);
    steerServo.attach(STEER_PIN);

    armESC(motorServo);
    armESC(steerServo);
}

/* Drive at a speed from 1 to 10 */
void drive(int speed)
{
    if (speed == 0) {
        motorServo.write(0);
        return;
    }

// Speeds. Servo simulates angles, so range is 0 to 180.
// But the ESC only does anything between 70 and 140.
// Add load, and anything under 100 really doesn't do anything noticable.
#define SLOW  100
#define FAST  140
    int ESCspeed = speed * (FAST - SLOW) / 10 + SLOW;
    motorServo.write(ESCspeed);
}

// Move the steering motor. direc can be -1, 1, or 0 --
// but unfortunately I don't currently have a way to drive a motor
// in both directions, so any nonzero value will steer the same way.
// 0 moves back to center.
#define STEER_SPEED 120
void steer(int direc)
{
    if (direc != 0) {
        digitalWrite(13, HIGH);
        steerServo.write(STEER_SPEED);
    }
    else {
        steerServo.write(0);
        digitalWrite(13, LOW);
    }
}

int steering = 0;

void loop()
{
    //digitalWrite(13, HIGH);
    steer(steering);
    steering = !steering;
    drive(5);
    delay(1000);       // drive

    // Stop everything
    //digitalWrite(13, LOW);
    motorServo.write(0);
    delay(2000);       // sit 2 secs
}

