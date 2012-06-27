/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 10;

// which analog pin is the light sensor on?
uint8_t LIGHTSENSOR = 0;

void setup()
{
    pinMode(SPEAKER, OUTPUT);

//#define SERIAL_DEBUG 1
#ifdef SERIAL_DEBUG
    Serial.begin(9600);
#endif
}

void loop()
{
    // Set the frequency according to the light value read off analog pin 0.
    // Typical night/indoor Light sensor values:
    // 110-290 (low light), 490 (room light), 980 (direct flashlight).
    // Typical for daytime with window:
    // 325 (hand over sensor), 790 (sunlight through window)
    // Want frequency between 100 and 10000 (Hz).
#define MIN_FREQ    100
#define MAX_FREQ   6000
#define MIN_SIGNAL  100  /* 100 */
#define MAX_SIGNAL  512  /* 1024 */

    int lightsensor = analogRead(LIGHTSENSOR);
    unsigned int freq = map(lightsensor,
                            MIN_SIGNAL, MAX_SIGNAL,
                            MIN_FREQ, MAX_FREQ);

    tone(SPEAKER, freq);

#ifdef SERIAL_DEBUG
    Serial.print("Read ");
    Serial.print(lightsensor);
    Serial.print(" -> ");
    Serial.println(freq);
    delay(200);
#endif
}

