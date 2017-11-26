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

// A "just" scale:
float middle_c = 262.626;
float just[] = { 1., 9./8, 5./4, 4./3, 3./2, 5./3, 15./8 };
#define NUMNOTES (sizeof(just)/sizeof(*just))
float cur_octave = 1.;

int whichnote;

void loop()
{
    unsigned int rand;

    // Chose a note, making it somewhat likely that it will be one note
    // up from the last.
    rand = random(0, 6);
    if (rand == 0)
        whichnote = (whichnote + 1) % NUMNOTES;
    else if (rand == 1)
        whichnote = (whichnote + 1) % NUMNOTES;
    else
        whichnote = random(0, NUMNOTES);

    float freq = middle_c * just[whichnote];

    // Change octave?
    rand = random(0, 10);
    if (rand == 1 && cur_octave <= 3) {
        cur_octave *= 2.;
    } else if (rand == 2 && cur_octave >= 1) {
        cur_octave /= 2.;
    }
    freq *= cur_octave;

    long len = 60;
    rand = random(1, 7);

    len *= 4;
    if (rand < 4)
        len *= rand;

    tone(SPEAKER, (unsigned long)freq, len);

    delay(len);
}

