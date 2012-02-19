/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 8;

// which analog pin is the light sensor on?
uint8_t LIGHTSENSOR = 0;

void buzz(long frequency, long length)
{
    long delayValue = 1000000/frequency/2;
    /* calculate the delay value between transitions:
     * 1 second's worth of microseconds, divided by the frequency,
     * then split in half since there are two phases to each cycle.
     */
    long numCycles = frequency * length / 1000;

    for (long i=0; i < numCycles; i++) { // for the calculated length of time...
        digitalWrite(SPEAKER, HIGH);
        delayMicroseconds(delayValue);
        digitalWrite(SPEAKER, LOW);
        delayMicroseconds(delayValue);
    }
}

void setup()
{
    pinMode(SPEAKER, OUTPUT);

    //Serial.begin(9600);
}

unsigned int freq = 20;
void loop()
{
    /*
    */
    if (freq > 20000) {
        delay(2000);
        freq = 20;
    }

    // Set the frequency according to the light value read off analog pin 0.
    // Typical Light sensor values:
    // 110-290 (low light), 490 (room light), 980 (direct flashlight).
    // Want frequency between 100 and 10000 (Hz).
    int lightsensor = analogRead(LIGHTSENSOR);
    if (lightsensor < 150) lightsensor = 150;
    freq = (lightsensor - 150) * 13000.0 / 400;

    /*
    Serial.print("Read ");
    Serial.print(lightsensor);
    Serial.print(" -> ");
    Serial.println(freq);
    */

    //buzz(freq, 20);
    tone(SPEAKER, freq);
    //delay(200);

    //freq = (int)(freq * 1.05);
}

