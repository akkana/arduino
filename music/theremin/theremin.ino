/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 8;

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

unsigned int freq = 20;
void loop()
{
    // Set the frequency according to the light value read off analog pin 0.
    // Typical night/indoor Light sensor values:
    // 110-290 (low light), 490 (room light), 980 (direct flashlight).
    // Typical for daytime with window:
    // 325 (hand over sensor), 790 (sunlight through window)
    // Want frequency between 100 and 10000 (Hz).
#define MAX_SIGNAL 800  /* 1024 */
#define MAX_FREQ  5000
#define MIN_SIGNAL  380 /* 100 */
#define MIN_FREQ    20
    int lightsensor = analogRead(LIGHTSENSOR);
    freq = (lightsensor - MIN_SIGNAL) * (float)(MAX_FREQ - MIN_FREQ) / (MAX_SIGNAL - MIN_SIGNAL) + MIN_FREQ;

    tone(SPEAKER, freq);

#ifdef SERIAL_DEBUG
    Serial.print("Read ");
    Serial.print(lightsensor);
    Serial.print(" -> ");
    Serial.println(freq);
    delay(200);
#endif
}

