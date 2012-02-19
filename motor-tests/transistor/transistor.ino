/* -*- c -*- */

/*
 * Arduino basic motor control, using a transistor.
 * One direction only.
 * Wiring diagram: http://itp.nyu.edu/physcomp/Tutorials/HighCurrentLoads
 */

#define DELAYTIME   1500      // milliseconds

#define MOTOR 8      // pin controlling the motor (via the transistor)

#define SPEED 150

void setup()
{
    pinMode(MOTOR, OUTPUT);
}

/* Alternate between two directions and motionless.
 * Assume we start with both pins low, motor motionless.
 */
void loop()
{
    delay(DELAYTIME);

    analogWrite(MOTOR, SPEED);

    delay(DELAYTIME);

    analogWrite(MOTOR, 0);

    delay(DELAYTIME);
}

