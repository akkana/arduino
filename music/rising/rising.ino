/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* A rising tone, repeated forever */

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 8;

void setup()
{
    pinMode(SPEAKER, OUTPUT);
}

unsigned int freq = 20;

void loop()
{
    /* Reset the frequency if we go too high */
    if (freq > 20000) {
        delay(2000);
        freq = 20;
    }

    tone(SPEAKER, freq);

    freq = (int)(freq * 1.05);
}

