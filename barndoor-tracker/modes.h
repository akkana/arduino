// Modes for the Barn-Door Tracker

typedef struct speedMode {
    double multiplier;
    const char* const name;
} speedMode_t;

// Special tracking rate multipliers:
// http://www.ascom-standards.org/Help/Platform/html/T_ASCOM_DeviceInterface_DriveRates.htm
extern speedMode_t gSpeedModes[];

//#define NUM_MODES ((sizeof gSpeedModes) / (sizeof *gSpeedModes))
#define NUM_MODES 5

#define REWIND_SPEED -900

#define REWINDING(m) ((m) == 0 || (m) == NUM_MODES)
