/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/* Random music  */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 8;

void setup()
{
    pinMode(SPEAKER, OUTPUT);

    // Seed the random number generator from floating analog pin 0:
    randomSeed(analogRead(0));
}

void loop()
{
    // Random frequency between 100 and 10000 (Hz).
    unsigned long freq = random(20, 1400);
    long len = random(5, 50);

    tone(SPEAKER, freq, len);
    delay(random(100, 300));
}

