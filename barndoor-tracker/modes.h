// Modes for the Barn-Door Tracker

typedef struct speedMode {
    double multiplier;    // The multiplier vs. basic Sidereal rate
    int speed;            // Actual motor speed
    const char* const name;
} speedMode_t;

extern speedMode_t gSpeedModes[];
extern unsigned int gNumModes;

// Initialize the speeds in gSpeedModes, so we don't have to
// multiply them out every time we change speeds:
extern void initSpeeds(int siderealSpeed, int rewindSpeed);

// Does this mode mean we're rewinding, either forward or backward?
extern bool isRewindMode(unsigned short mode);

// Go to the next mode, skipping over rewind modes:
extern unsigned int nextMode(unsigned int mode);

// What index specifies rewinding, forward or backward?
extern unsigned int rewindMode(unsigned int curmode);

// Don't define DEBUGSERIAL if you're using pins 0 and 1 for anything.
#define DEBUGSERIAL
