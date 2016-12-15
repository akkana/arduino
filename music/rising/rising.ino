/* A rising tone, repeated forever */

int SPEAKER = 8;           // What digital pin is the speaker on?

int freq = 50;             // Starting frequency

int TOOHIGH = 6000;

void setup()
{
    pinMode(SPEAKER, OUTPUT);
}

void loop()
{
    // Reset the frequency if we go too high
    if (freq > TOOHIGH) {
        noTone(SPEAKER);
        delay(2000);
        freq = 20;
    }

    tone(SPEAKER, freq);

    freq = (int)(freq * 1.05);
    //freq += 100;

    delay(100);
}

