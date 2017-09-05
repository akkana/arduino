#include "modes.h"

#ifdef DEBUGSERIAL
#include <Arduino.h>
#endif /* DEBUGSERIAL */

// Special tracking rate multipliers:
// http://www.ascom-standards.org/Help/Platform/html/T_ASCOM_DeviceInterface_DriveRates.htm
speedMode_t gSpeedModes[] = {
    {                    -0.0, 0, "Rewinding" },
    {                     1.0, 0, "Sidereal"  },
    {  0.99727411741240609002, 0, "Solar"     },
    {  0.97633136094674556213, 0, "Lunar"     },
    {  0.99972741174124060900, 0, "King"      },
    {                     0.0, 0, "Rewinding rev" },
    {                    -1.0, 0, "Sidereal rev" },
    { -0.99727411741240609002, 0, "Solar rev"    },
    { -0.97633136094674556213, 0, "Lunar rev"    },
    { -0.99972741174124060900, 0, "King rev"     }
};

unsigned int gNumModes = 10;

// What index specifies rewinding, forward or backward?
unsigned int rewindMode(unsigned int curmode)
{
    if (curmode >= 5)
        return 5;
    return 0;
}

// Does this mode mean we're rewinding, either forward or backward?
bool isRewindMode(unsigned short mode)
{
    return (mode == 0 || mode == 5);
}

void initSpeeds(int siderealSpeed, int rewindSpeed)
{
    for (unsigned int i=0; i<gNumModes; ++i) {
        if (i == 0)
            gSpeedModes[i].speed = -rewindSpeed;
        else if (i == 5)
            gSpeedModes[i].speed = rewindSpeed;
        else
            gSpeedModes[i].speed = siderealSpeed * gSpeedModes[i].multiplier;
    }
}

// Go to the next mode but skip over rewind modes
unsigned int nextMode(unsigned int mode)
{
#ifdef DEBUGSERIAL
    Serial.print("nextMode(");
    Serial.println(mode);
#endif /* DEBUGSERIAL */
    unsigned int nextmode = (mode + 1) % gNumModes;
    if (isRewindMode(nextmode)) {
#ifdef DEBUGSERIAL
        Serial.print(nextmode);
        Serial.println(" was rewinding, so we'll advance again");
#endif /* DEBUGSERIAL */
        nextmode = (nextmode + 1) % gNumModes;
    }

#ifdef DEBUGSERIAL
    Serial.print("Returning next mode ");
    Serial.println(nextmode);
    Serial.println("");
#endif /* DEBUGSERIAL */
    return nextmode;
}
