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
 *
 * It also supports two ways of showing the mode and letting the
 * user change mode: either on an LED bar using pins (set pin numbers
 * in LEDpins[], buttonpin and rewindSwitch),
 * or using a DF Robot (or similar) LCD Keypad Shield.
 */

#include "modes.h"

#include "TrackerIO.h"

/* Step angles for the 28BYJ-48:
 * Half-step mode: 8 step control signal sequence (recommended)
 *   5.625 degrees per step / 64 steps per one revolution
 *   of the internal motor shaft, so 4096 steps for the output shaft.
 * Full Step mode: 4 step control signal sequence
 *     11.25 degrees per step / 32 steps per one revolution
 *     of the internal motor shaft, so 2048 for the output shaft.
 */

#include <AccelStepper.h>

// Are we using the LCD keypad shield, or just simple LEDs?
#define LCDKEYPADSHIELD

// How many signals have to be sent to the motor driver for half-step mode:
#define HALFSTEP 8

// Stepper pins should correspond with ULN2003's pins In1, In3, In2, In4.
// Here are some common pin configurations:

// Plugging the ULN2003 board directly into the Arduino, pointed out:
//AccelStepper stepper(HALFSTEP, 7, 5, 6, 4);

// Plugging the ULN2003 board directly into the Arduino, pointed inward
// (needs insulation else the extra pins might contact something):
// Note, in this configuration, you can't use Serial for debugging,
// because pins 0 and 1 are serial RX and TX.
//AccelStepper stepper(HALFSTEP, 0, 2, 1, 3);

// Plugging the ULN2003 into the header on the DFRobot LCD Keypad Shield,
// pointed out away from the Arduino:
AccelStepper stepper(HALFSTEP, 13, 11, 12, 3);
TrackerIO *trackerIO = 0;

#ifdef LCDKEYPADSHIELD

#    include "LCDKeypadShield.h"

#else // LCDKEYPADSHIELD

#    include "SimpleIO.h"

#endif // LCDKEYPADSHIELD

/***************************************************************
 * Hopefully, you shouldn't have to change anything below here.
 ***************************************************************/

// Fudge factor for this specific mount's geometry.
// This has to be tuned with field testing.
// 1.82 was seeming fairly close for solar rate.
// 1.7 is still a little too fast.
#define FUDGE 1.

// 1 RPM on the drive gear (2 RPM on the stepper's output shaft):
#define ONERPM   (FUDGE * 2. * 4096/60)
#define REWIND_SPEED 900

#define LONGPRESS 3000    // milliseconds

// Are we in the middle of rewinding?
static int sRewinding = 0;

// We'll update these two according to the state of the buttons:
// Current mode goes from 0 to NUM_MODES*2: if it's >= NUM_MODES
// then we're going backward.
static unsigned int sCurMode = 1;

// When we change speed, we'll save it, so TrackerIO classes can access it:
unsigned gCurSpeed = 0;

void setup()
{
#ifdef DEBUGSERIAL
    Serial.begin(9600);
#endif

    // populate the speeds in gSpeedModes:
    initSpeeds(ONERPM, REWIND_SPEED);

#ifdef LCDKEYPADSHIELD
    // select the pins used on the LCD panel
    trackerIO = new LCDKeypadShield(8, 9, 4, 5, 6, 7);
#else // LCDKEYPADSHIELD
    unsigned int LEDpins[] = { 10, 11, 12, 13 };
    trackerIO = new SimpleIO(8, 9, LEDpins, (sizeof LEDpins / sizeof *LEDpins));
#endif // LCDKEYPADSHIELD

    trackerIO->init();
    trackerIO->showMode(sCurMode);

    stepper.setMaxSpeed(1000);
    stepper.setSpeed(gSpeedModes[sCurMode].speed);     // steps per second
    //stepper.setSpeed(50);
    //stepper.setAcceleration(50.);

#ifdef DEBUGSERIAL
    Serial.print("Set motor speed: ");
    Serial.println(gSpeedModes[sCurMode].speed);
#endif
}

void startRewinding(unsigned int mode)
{
#ifdef DEBUGSERIAL
    Serial.println("Start rewinding");
#endif

    sRewinding = 1;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(gSpeedModes[mode].speed);
}

void stopRewinding()
{
#ifdef DEBUGSERIAL
    Serial.println("Stop rewinding");
#endif
    sRewinding = 0;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(gSpeedModes[sCurMode].speed);
}


void loop()
{
    unsigned int newmode = trackerIO->checkButtonState(sCurMode);

    if (sRewinding && !isRewindMode(newmode)) {
        stopRewinding();
        trackerIO->showMode(sCurMode);
        stepper.runSpeed();
        return;
    }

    if (newmode != sCurMode) {
        if (isRewindMode(newmode)) {
            if (!sRewinding)
                startRewinding(newmode);
            // else just go on rewinding but don't change anything
        }
        else {
            sCurMode = newmode;
            int speed = gSpeedModes[sCurMode].speed;
#ifdef DEBUGSERIAL
            Serial.print("Set motor speed: ");
            Serial.println(speed);
#endif
            stepper.setSpeed(speed);
#ifdef DEBUGSERIAL
            Serial.print("Changed mode: ");
            Serial.print(newmode);
            Serial.print(", speed: ");
            Serial.print(speed);
            Serial.print(", ");
            Serial.println(gSpeedModes[newmode].name);
#endif
        }

        trackerIO->showMode(newmode);
    }

    stepper.runSpeed();
}

#ifdef NOTDEF

#endif
