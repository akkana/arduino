/* Warble from a low frequency to a high one and back down,
 * but only while the overspeed condition is true.
 */

/*
 * CONSTANTS
 */

// What pin is the buzzer/speaker on?
#define SPEAKER_PIN 9

// What pin will be used to decide when to warble?
#define SPEED_PIN 7

/*
 * GLOBAL VARIABLES
 */

// The high and low frequency limits, in Hz.
int lowFreq = 400;
int highFreq = 2000;

// How long should each tone last? (milliseconds)
int duration = 250;

// A delay (msec) that controls how fast the warble changes.
// Values between 5 and 20 make it sound like different sirens.
int warbleSpeed = 10;

// Initialize the frequency to the lowest frequency the first time,
// but use a static variable so the value is remembered
// and it will pick up wherever it left off.
int freq = lowFreq;

// Will the tone change up or down?
// Initially it will increase, by 250 Hz each time,
// but when freq gets to highFreq, the direction will change
// and it will go down instead.
int freqchange = 50;

/*
 * FUNCTIONS
 */

//
// A function to decide when to warble.
// For now, warble whenever the input pin is high.
// Eventually this should check the status of the speedometer instead.
//
// This function could also change variables like duration, warbleSpeed
// and highFreq/lowFreq to give different-sounding warnings
// depending on how high the speed is.
//
bool overspeed()
{
    //Serial.println(digitalRead(SPEED_PIN));

    return digitalRead(SPEED_PIN);
}

//
// Called once when the Arduino initializes.
//
void setup()
{
    pinMode(SPEED_PIN, INPUT);
    pinMode(SPEAKER_PIN, OUTPUT);

    //Serial.begin(9600);
}

//
// Called repeatedly.
//
void loop()
{
    delay(warbleSpeed);

    // If not speeding, do nothing:
    if (! overspeed()) {
        noTone(SPEAKER_PIN);
        return;
    }

    // Must be speeding. Start or continue a warble,
    // from whatever frequency is current.
    tone(SPEAKER_PIN, freq, duration);

    // Adjust freq -- make it either bigger or smaller:
    freq += freqchange;

    // Is freq now too high?
    if (freq > highFreq) {
        freqchange = -freqchange;    // freqchange is now negative
        freq = highFreq + freqchange;
    }
    // Or is it too low?
    else if (freq < lowFreq) {
        freqchange = -freqchange;    // freqchange is now positive
        freq = lowFreq + freqchange;
    }
}

