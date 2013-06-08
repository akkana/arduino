/* -*- c -*- */

#include <HalfBridge.h>

#define DEBUG 1

// Motor 0 has a speed control on pin 9, direction controls on pins 2 and 3.
// Motor 1 uses 10 for speed control, 4 and 5 for direction.
Motor motors[2] = { Motor(9, 2, 3), Motor(10, 4, 5) };

#define FAST 230
#define SLOW 180

#define MAXSTEPS 14
#ifdef NOTDEF
int program[MAXSTEPS][2] = { {0, 0}, {SLOW, SLOW}, {SLOW, SLOW}, {SLOW, 0},
                             {SLOW, -SLOW}, {SLOW, -SLOW}, {0, 0},
                             {SLOW, SLOW}, {FAST, FAST}, {SLOW, SLOW},
                             {0, 0}, {-SLOW, -SLOW}, {0, -SLOW},
                             {0, 0} };
#endif

int program[][2] = { {0, 0}, {SLOW, SLOW}, {SLOW, SLOW}, {SLOW, 0},
                             {0, 0}, {0, -SLOW},
                             {0, 0} };

int NUMSTEPS = ((sizeof program) / (sizeof *program));

int curstep = 0;

void setup()
{
    motors[0].init();
    motors[1].init();

#ifdef DEBUG
    Serial.begin(9600);
    Serial.print(NUMSTEPS);
    Serial.println(" steps in program");
#endif

    // Long delay at the beginning, to allow time to unplug USB.
    delay(8000);
}

void loop()
{
    if (curstep < NUMSTEPS) {
        motors[0].setSpeed(program[curstep][0]);
        motors[1].setSpeed(program[curstep][1]);

#ifdef DEBUG
        Serial.print("Setting speeds: ");
        Serial.print(program[curstep][0]);
        Serial.print(", ");
        Serial.print(program[curstep][1]);
        Serial.println(".");
#endif

        ++curstep;
    }
    else {
        motors[0].setSpeed(0);
        motors[1].setSpeed(0);
    }

    delay(2000);
}
