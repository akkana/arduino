/* Sunflower sketch: turn in the direction the light is brightest. */
#line 3

#include <Stepper.h>

const int stepsPerRevolution = 64;
// The motor is actually 64 steps per revolution, but then it's geared down
// so it actually takes 32 of those, 2048 steps, to go the full way around.

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 11, 9);

#define NUM_SAMPLES  7

// What analog pins are the light sensors on?
int leftlight = 1;
int rightlight = 0;
 
void setup()
{
    myStepper.setSpeed(100);

    Serial.begin(9600);
    Serial.println("Waiting 3 seconds");
    delay(3000);
}
 
 
void loop()
{
    int left = 0;
    int right = 0;
    for (int i=1; i < NUM_SAMPLES; ++i) {
        left  += analogRead(leftlight);
        right += analogRead(rightlight);
    }
    left /= NUM_SAMPLES;
    right /= NUM_SAMPLES;

    Serial.print("Light: ");
    Serial.print(left);
    Serial.print("   ");
    Serial.print(right);
    Serial.println("");

    if (left > right)
        myStepper.step(stepsPerRevolution);
    else
        myStepper.step(-stepsPerRevolution);
}

