/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
int SPEAKER = 8;

// Which digital pin is the light sensor on?
// (The other speaker wire should connect to Gnd.)
int LIGHTSENSOR = 0;

// Typical photocell values for daytime with window and 1k resistor:
// min 15, max 100.
#define MIN_SIGNAL   15  /* 100 */
#define MAX_SIGNAL  100  /* 1024 */

// What frequencies should we allow (in Hertz)?
// Typical human hearing is 20 Hz to 20,000 Hz,
// but our piezo buzzers can't reproduce such a wide range.
// 100 to 10k or so is usually okay.
#define MIN_FREQ    100
#define MAX_FREQ   8000

// How many samples should we read from the sensor each time?
// The sensor is noisy, so there will be a lot of spurious readings.
#define NUM_SAMPLES  15

// To monitor the output from the sensor (so you can calibrate MAX_SIGNAL
// and MIN_SIGNAL), uncomment SERIAL_DEBUG:
//#define SERIAL_DEBUG 1

void setup()
{
    pinMode(SPEAKER, OUTPUT);

#ifdef SERIAL_DEBUG
    Serial.begin(9600);
#endif
}

void loop()
{
    // Set the frequency according to the light value read off analog pin 0.

    // Average 5 reads
    int lightsensor = 0;
    for (int i=1; i < NUM_SAMPLES; ++i)
        lightsensor += analogRead(LIGHTSENSOR);
    lightsensor /= NUM_SAMPLES;

    unsigned int freq = map(lightsensor,
                            MIN_SIGNAL, MAX_SIGNAL,
                            MIN_FREQ, MAX_FREQ);

    tone(SPEAKER, freq);

#ifdef SERIAL_DEBUG
    Serial.print("Read ");
    Serial.print(lightsensor);
    Serial.print(" -> ");
    Serial.println(freq);
    delay(500);
#endif
}

