/* -*- c -*- */

/*
 * Arduino basic motor control
 */

#define DELAYTIME   1000      // milliseconds

int motorPins[2] = { 5, 6 };  // plug the motor leads into these pins
int direction = 0;            // toggle between 0 and 1

void setup()
{
    pinMode(motorPins[0], OUTPUT);
    digitalWrite(motorPins[0], LOW);
    pinMode(motorPins[1], OUTPUT);
    digitalWrite(motorPins[1], LOW);
}

/* Alternate between two directions and motionless.
 * Assume we start with both pins low, motor motionless.
 */
void loop()
{
    delay(DELAYTIME);

    digitalWrite(motorPins[direction], HIGH);

    delay(DELAYTIME);

    digitalWrite(motorPins[direction], LOW);

    direction = !direction;
}

