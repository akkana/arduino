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

// Don't define DEBUGSERIAL if you're using pins 0 and 1 for anything.
#undef DEBUGSERIAL

// How many signals have to be sent to the motor driver for half-step mode:
#define HALFSTEP 8

// Stepper pins should correspond with ULN2003's pins In1, In3, In2, In4.
// Plugging the ULN2003 board directly into the Arduino, pointed out:
//AccelStepper stepper(HALFSTEP, 7, 5, 6, 4);
// Plugging the ULN2003 board directly into the Arduino, pointed inward
// (needs insulation else the extra pins might contact something):
// Note, in this configuration, you can't use Serial for debugging,
// because pins 0 and 1 are serial RX and TX.
AccelStepper stepper(HALFSTEP, 0, 2, 1, 3);

TrackerIO *trackerIO = 0;

#ifdef LCDKEYPADSHIELD

#include "LCDKeypadShield.h"

#else // LCDKEYPADSHIELD

#include "SimpleIO.h"

#endif // LCDKEYPADSHIELD

/***************************************************************
 * Hopefully, you shouldn't have to change anything below here.
 ***************************************************************/

// Fudge factor for this specific mount's geometry.
// This has to be tuned with field testing.
// 1.82 was seeming fairly close for solar rate.
// 1.7 is still a little too fast.
#define FUDGE 1.8

// 1 RPM on the drive gear (2 RPM on the stepper's output shaft):
#define ONERPM   (FUDGE * 2. * 4096/60)

#define LONGPRESS 3000    // milliseconds

// Is the button currently pressed?
//static unsigned int gButtonState = 0;

// Are we in the middle of rewinding?
static int sRewinding = 0;

// When did the current button press start?
// Holding the button for more than 3 seconds starts a rewind.
//static int gButtonPressTime = 0;

// We'll update these two according to the state of the buttons:
// Current mode goes from 0 to NUM_MODES*2: if it's >= NUM_MODES
// then we're going backward.
static unsigned int gCurMode = 1;

// gSpeedModes declared in modes.h:
speedMode_t gSpeedModes[] = {
    {                   -0.0, "Rewinding" },
    {                    1.0, "Sidereal"  },
    { 0.99727411741240609002, "Solar"     },
    { 0.97633136094674556213, "Lunar"     },
    { 0.99972741174124060900, "King"      }
};

/* Figure out what speed we should be driving, in steps per second,
 * according to the current mode.
 * If mode's high bit (mode & 8) is set, drive in the opposite direction.
 * Also update the LED to show the mode.
 *
 * This is only called when we change speeds, not every loop.
 */
int calcSpeed(unsigned int mode)
{
    int curSpeed;

    // Are we rewinding?
    if (mode == 0)
        curSpeed = REWIND_SPEED;
    else if (mode == NUM_MODES)
        curSpeed = -REWIND_SPEED;
    else {
        curSpeed = gSpeedModes[gCurMode].multiplier * ONERPM;
        if (mode > NUM_MODES)
            curSpeed = -curSpeed;
    }

#ifdef DEBUGSERIAL
    Serial.print("speed = ");
    Serial.println(curSpeed);
#endif

    return curSpeed;
}

void setup()
{
#ifdef DEBUGSERIAL
    Serial.begin(9600);
#endif

    stepper.setMaxSpeed(1000);
    stepper.setSpeed(calcSpeed(gCurMode));     // steps per second
    //stepper.setAcceleration(50.);

#ifdef LCDKEYPADSHIELD
    // select the pins used on the LCD panel
    trackerIO = new LCDKeypadShield(8, 9, 4, 5, 6, 7);
#else // LCDKEYPADSHIELD
    unsigned int LEDpins[] = { 10, 11, 12, 13 };
    trackerIO = new SimpleIO(8, 9, LEDpins, (sizeof LEDpins / sizeof *LEDpins));
#endif // LCDKEYPADSHIELD

    trackerIO->init();
    trackerIO->showMode(gCurMode, gSpeedModes[gCurMode].name);
}

void startRewinding(unsigned int mode)
{
#ifdef DEBUGSERIAL
    Serial.println("Start rewinding");
#endif

    sRewinding = 1;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(calcSpeed(mode));
}

void stopRewinding()
{
#ifdef DEBUGSERIAL
    Serial.println("Stop rewinding");
#endif
    sRewinding = 0;
    stepper.setSpeed(0);
    delay(100);
    stepper.setSpeed(calcSpeed(gCurMode));
}

void loop()
{
    unsigned int newmode = trackerIO->checkButtonState(gCurMode);

    if (sRewinding && !REWINDING(newmode)) {
        stopRewinding();
        trackerIO->showMode(gCurMode, gSpeedModes[gCurMode].name);
    }

    if (newmode != gCurMode) {
        if (REWINDING(newmode)) {
            if (!sRewinding)
                startRewinding(newmode);
            // else just go on rewinding but don't change anything
        }
        else {
            gCurMode = newmode;
            int speed = calcSpeed(gCurMode);
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

        trackerIO->showMode(newmode, gSpeedModes[newmode].name);
    }

    stepper.runSpeed();

    delay(1000);
}

#if 0
void loop_old()
{
    trackerIO->checkButtonState(gCurState);

#ifdef DEBUGSERIAL
    Serial.print("Rewind switch: ");
    Serial.println(rewindSwitchPressed);
#endif

    if (rewindSwitchPressed && !sRewinding)
        startRewinding();

    else if (sRewinding && !rewindSwitchPressed)
        stopRewinding();

    // Don't bother checking button state if we're rewinding.
    if (!sRewinding) {
        // If the button is pressed, increment mode and update the LEDs.
        if (newBtnState && !gButtonState) {            // Press
            gButtonPressTime = millis();
        }
        else if (gButtonState && !newBtnState) {       // Release
            if (!sRewinding) {
                gCurMode = (gCurMode + 1) & 0xf;

#ifdef DEBUGSERIAL
                Serial.print("Incrementing mode to ");
                Serial.println(gCurMode);
#endif
            }
            gButtonPressTime = 0;
            sRewinding = 0;
            stepper.setSpeed(calcSpeed());
        }
        else if (gButtonState && newBtnState && !sRewinding) {        // Hold
            int time = millis();
            if (time - gButtonPressTime > LONGPRESS) {
                startRewinding();
                gButtonPressTime = 0;
            }
        }
        gButtonState = newBtnState;
    }

    stepper.runSpeed();

    delay(500);
}
#endif

