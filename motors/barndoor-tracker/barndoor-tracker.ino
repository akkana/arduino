/* A barn-door tracking motor,
 * using a 28BYJ-48 unipolar stepper motor with a ULN2003 driver.
 *
 * Uses the AccelStepper library in its simplest fixed-speed mode.
 * AccelStepper is apparently more efficient than Stepper,
 * and unlike Stepper it supports half-step mode,
 * which is supposedly better for the 28BYJ-48 as well as being less jerky.
 *
 * The program also supports adjusting the speed, and rewinding, and
 * supports LEDs indicating current speed, for use when debugging rates.
 */

/* Step angles for the 28BYJ-48:
 * Half-step mode: 8 step control signal sequence (recommended)
 *   5.625 degrees per step / 64 steps per one revolution
 *   of the internal motor shaft, so 4096 steps for the output shaft.
 * Full Step mode: 4 step control signal sequence
 *     11.25 degrees per step / 32 steps per one revolution
 *     of the internal motor shaft, so 2048 for the output shaft.
 */

#include <AccelStepper.h>

#define HALFSTEP 8
#define NUMLED (sizeof LEDpins / sizeof *LEDpins)

// Plugging the ULN2003 board directly into the Arduino uses these pins:
AccelStepper stepper(HALFSTEP, 7, 5, 6, 4);

// Leaving these for LEDs and buttons:
int buttonpin = 8;
int LEDpins[] = { 10, 11, 12, 13 };

// Is the button currently pressed?
int buttonState = 0;

#define LONGPRESS 3000    // milliseconds

#define REWIND_SPEED -900

// When did the current button press start?
// Holding the button for more than 3 seconds starts a rewind.
int buttonPressTime = 0;

int mode = 0;
int rewinding = 0;

/* Figure out what speed we should be driving, in steps per second,
 * according to the current mode.
 */
int calcSpeed()
{
    int speed;

    if (rewinding)
        speed = REWIND_SPEED;

    else
        speed = 4096/60 + mode * 5;

    Serial.print("Set speed to ");
    Serial.println(speed);

    return (speed);
}

void setup()
{
    Serial.begin(9600);

    stepper.setMaxSpeed(1000);
    stepper.setSpeed(calcSpeed());     // steps per second
    //stepper.setAcceleration(50.);

    for (unsigned int i=0; i < NUMLED; ++i)
        pinMode(LEDpins[i], OUTPUT);

    pinMode(buttonpin, INPUT);
}

/* Show the mode on the four LEDs, as a binary number. */
void showMode()
{
    unsigned int i;

    if (rewinding) {
        for (i=0; i<NUMLED; ++i)
            digitalWrite(LEDpins[i], HIGH);
        return;
    }

    unsigned short bitmode = mode;

    //Serial.println(bitmode);
    for (i=0; i<NUMLED; ++i) {
        if (bitmode & 1)
            digitalWrite(LEDpins[i], HIGH);
        else
            digitalWrite(LEDpins[i], LOW);
        bitmode >>= 1;
    }
}

void loop()
{
    /* If the button is pressed, increment mode and update the LEDs. */
    int newBtnState = digitalRead(buttonpin);
    if (newBtnState && !buttonState) {            // Press
        buttonPressTime = millis();
    }
    else if (buttonState && !newBtnState) {       // Release
        if (!rewinding) {
            Serial.print("Incrementing mode to ");
            Serial.println(++mode);
        }
        buttonPressTime = 0;
        rewinding = 0;
        stepper.setSpeed(calcSpeed());
    }
    else if (buttonState && newBtnState && !rewinding) {        // Hold
        int time = millis();
        if (time - buttonPressTime > LONGPRESS) {
            rewinding = 1;
            buttonPressTime = 0;
            stepper.setSpeed(0);
            delay(100);
            stepper.setSpeed(calcSpeed());
            Serial.print("Rewinding, Speed is ");
            Serial.println(calcSpeed());
        }
    }
    buttonState = newBtnState;

    showMode();

    stepper.runSpeed();

    //delay(500);
}

