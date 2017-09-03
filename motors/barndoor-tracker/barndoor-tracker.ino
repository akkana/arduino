/* A barn-door tracking motor,
 * using a 28BYJ-48 unipolar stepper motor with a ULN2003 driver.
 *
 * Uses the AccelStepper library in its simplest fixed-speed mode.
 * AccelStepper is apparently more efficient than Stepper,
 * and unlike Stepper it supports half-step mode,
 * which is supposedly better for the 28BYJ-48 as well as being less jerky.
 *
 * The program also supports multiple rates, both directions, and rewinding,
 * and supports LEDs indicating current mode (rate + direction).
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

// DEBUGSERIAL must be undefined to use pins that include 0 and 1
#undef DEBUGSERIAL

// How many signals have to be sent to the motor driver for half-step mode:
#define HALFSTEP 8

// Fudge factor for this specific mount's geometry.
// This has to be tuned with field testing.
// 1.82 was seeming fairly close for solar rate.
// 1.7 is still a little too fast.
#define FUDGE 1.8

// 1 RPM on the drive gear (2 RPM on the stepper's output shaft):
#define ONERPM   (FUDGE * 2. * 4096/60)

// Special tracking rate multipliers:
// http://www.ascom-standards.org/Help/Platform/html/T_ASCOM_DeviceInterface_DriveRates.htm
/*
driveSidereal     0    Sidereal tracking rate (15.041 arcseconds per second)
driveLunar        1    Lunar tracking rate (14.685 arcseconds per second).
driveSolar         2    Solar tracking rate (15.0 arcseconds per second).
driveKing          3    King tracking rate (15.0369 arcseconds per second).
*/
#define SIDEREAL 1.0
#define SOLAR    0.99727411741240609002
#define LUNAR    0.97633136094674556213
#define KING     0.99972741174124060900

// Six speeds to cycle through:
float modeSpeeds[] = {  SIDEREAL * ONERPM,  LUNAR * ONERPM,  SOLAR * ONERPM,
                       -SIDEREAL * ONERPM, -LUNAR * ONERPM, -SOLAR * ONERPM };
int numModes = (sizeof modeSpeeds) / (sizeof *modeSpeeds);

#define REWIND_SPEED 900

#define LONGPRESS 3000    // milliseconds

#define NUMLED (sizeof LEDpins / sizeof *LEDpins)

// Stepper pins should correspond with ULN2003's pins In1, In3, In2, In4.
// Plugging the ULN2003 board directly into the Arduino, pointed out:
//AccelStepper stepper(HALFSTEP, 7, 5, 6, 4);
// Plugging the ULN2003 board directly into the Arduino, pointed inward
// (needs insulation else the extra pins might contact something):
// Note, in this configuration, you can't use Serial for debugging,
// because pins 0 and 1 are serial RX and TX.
AccelStepper stepper(HALFSTEP, 0, 2, 1, 3);

// Leaving these for LEDs and buttons:
int buttonpin = 8;
int rewindSwitch = 9;
int LEDpins[] = { 10, 11, 12, 13 };

// Is the button currently pressed?
int buttonState = 0;

// When did the current button press start?
// Holding the button for more than 3 seconds starts a rewind.
int buttonPressTime = 0;

int mode = 0;
int rewinding = 0;

/* Figure out what speed we should be driving, in steps per second,
 * according to the current mode.
 * If mode's high bit (mode & 8) is set, drive in the opposite direction.
 * Also update the LED to show the mode.
 */
int calcSpeed()
{
    int bitmode = mode % numModes;
    int speed = (int)(modeSpeeds[bitmode]);
    unsigned int i;

    // Rewinding must be the opposite sign from the direction we're going:
    if (rewinding) {
        // All LEDs high:
        for (i=0; i<NUMLED; ++i)
            digitalWrite(LEDpins[i], HIGH);

#ifdef DEBUGSERIAL
        Serial.println("Rewinding");
#endif

        if (speed > 0)
            return -REWIND_SPEED;
        else
            return REWIND_SPEED;
    }

#ifdef DEBUGSERIAL
    Serial.print("Mode = ");
    Serial.print(bitmode);
    Serial.print(", speed = ");
    Serial.println(speed);
#endif

    // Else get the mode from modeSpeeds[mode].
    for (i=0; i<NUMLED; ++i) {
        if (bitmode & 1)
            digitalWrite(LEDpins[i], HIGH);
        else
            digitalWrite(LEDpins[i], LOW);
        bitmode >>= 1;
    }

    // Whatever we just did, now light the high LED if speed is negative.
    if (speed > 0)
        digitalWrite(LEDpins[NUMLED-1], LOW);
    else
        digitalWrite(LEDpins[NUMLED-1], HIGH);

    return speed;
}

void setup()
{
#ifdef DEBUGSERIAL
    Serial.begin(9600);
#endif

    stepper.setMaxSpeed(1000);
    stepper.setSpeed(calcSpeed());     // steps per second
    //stepper.setAcceleration(50.);

    for (unsigned int i=0; i < NUMLED; ++i)
        pinMode(LEDpins[i], OUTPUT);

    pinMode(buttonpin, INPUT);
}

void startRewinding()
{
#ifdef DEBUGSERIAL
    Serial.print("Start rewinding");
#endif

    rewinding = 1;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(calcSpeed());

#ifdef DEBUGSERIAL
    Serial.print("Rewinding, Speed is ");
    Serial.println(calcSpeed());
#endif
}

void stopRewinding()
{
#ifdef DEBUGSERIAL
    Serial.print("Stop rewinding");
#endif
    rewinding = 0;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(calcSpeed());
}

void loop()
{
    // Is the rewind switch on?
    int rewindSwitchPressed = digitalRead(rewindSwitch);

#ifdef DEBUGSERIAL
    Serial.print("Rewind switch: ");
    Serial.println(rewindSwitchPressed);
#endif

    if (rewindSwitchPressed && !rewinding)
        startRewinding();

    else if (rewinding && !rewindSwitchPressed)
        stopRewinding();

    // Don't bother checking button state if we're rewinding.
    if (!rewinding) {
        // If the button is pressed, increment mode and update the LEDs.
        int newBtnState = digitalRead(buttonpin);
        if (newBtnState && !buttonState) {            // Press
            buttonPressTime = millis();
        }
        else if (buttonState && !newBtnState) {       // Release
            if (!rewinding) {
                mode = (mode + 1) & 0xf;

#ifdef DEBUGSERIAL
                Serial.print("Incrementing mode to ");
                Serial.println(mode);
#endif
            }
            buttonPressTime = 0;
            rewinding = 0;
            stepper.setSpeed(calcSpeed());
        }
        else if (buttonState && newBtnState && !rewinding) {        // Hold
            int time = millis();
            if (time - buttonPressTime > LONGPRESS) {
                startRewinding();
                buttonPressTime = 0;
            }
        }
        buttonState = newBtnState;
    }

    stepper.runSpeed();

    //delay(500);
}

