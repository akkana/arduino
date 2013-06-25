#include <Stepper.h>

const int stepsPerRevolution = 64;
// The motor is actually 64 steps per revolution, but then it's geared down
// so it actually takes 32 of those, 2048 steps, to go the full way around.
int numsteps = 32;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 11, 9);

int direction = 1;

void setup()
{
    myStepper.setSpeed(100);
}

void loop()
{
    direction = -direction;

    myStepper.step(direction * stepsPerRevolution * numsteps);
    delay(1000);
}
