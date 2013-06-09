/* -*- c -*- */

// Motor Shield test
// by NKC Electronics
// Test Motor B
// http://mcukits.com/2009/03/12/assembling-the-freeduino-arduino-motor-shield/

// Motor pins
int dirApin = 13;     // Direction pin for motor A is Digital 13
int dirBpin = 12;     // Direction pin for motor B is Digital 12
int speedApin = 10;   // Speed pin for motor A is Digital 10 (PWM)
int speedBpin =  9;   // Speed pin for motor B is Digital 9 (PWM)

int speed = 255;     // 0 to 255, I think
int dir = 0;

void setup()
{
    pinMode(speedApin, OUTPUT);
    pinMode(speedBpin, OUTPUT);
    pinMode(dirApin, OUTPUT);
    pinMode(dirBpin, OUTPUT);
    delay(2000);
}

void loop()
{
    // Turn and move:
    digitalWrite(dirApin, dir);      // set direction
    digitalWrite(dirBpin, dir);      // set direction
    analogWrite(speedApin, speed);   // set speed (PWM)
    analogWrite(speedBpin, speed);   // set speed (PWM)
    delay(1200);

    // Stop for half a sec
    analogWrite(speedApin, 9);
    analogWrite(speedBpin, 9);
    delay(500);

    // reverse direction:
    dir = ((dir == 0) ? 1 : 0);      // change direction
}

