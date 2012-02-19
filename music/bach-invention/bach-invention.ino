/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// Part of Bach 2-part invention #4 in D minor
// http://www.8notes.com/scores/2791.asp

/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/* start at A below middle C */
#define NOTE_Ab 207.652
#define NOTE_A  220.000
#define NOTE_As 233.082
#define NOTE_Bb NOTE_As
#define NOTE_B  246.942
#define NOTE_C  261.626
#define NOTE_Cs 277.183
#define NOTE_Db NOTE_Cs
#define NOTE_D  293.665
#define NOTE_Ds 311.127
#define NOTE_Eb NOTE_Ds
#define NOTE_E  329.628
#define NOTE_F  349.228
#define NOTE_Fs 369.994
#define NOTE_Gb NOTE_Fs
#define NOTE_G  391.995
#define NOTE_Gs 415.305

#define NOTE_REST     0.0
#define NOTE_SUSTAIN -1.0

// what digital pin is the speaker on? (other speaker wire is Gnd.)
uint8_t SPEAKER = 8;

int tempo = 150;

float composition[] = {
    NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A*2, NOTE_As*2,
    NOTE_Db, NOTE_As*2, NOTE_A*2, NOTE_G, NOTE_F, NOTE_E,
    NOTE_F, NOTE_REST, NOTE_A*2, NOTE_REST, NOTE_D*2, NOTE_REST,
    NOTE_G, NOTE_REST, NOTE_Cs*2, NOTE_REST, NOTE_E*2, NOTE_REST,

    NOTE_D*2, NOTE_E*2, NOTE_F*2, NOTE_G*2, NOTE_A*4, NOTE_As*4,
    NOTE_Db*2, NOTE_As*4, NOTE_A*4, NOTE_G*2, NOTE_F*2, NOTE_E*2,
};

void setup()
{
    pinMode(SPEAKER, OUTPUT);
}

unsigned int i = 0;

void loop()
{
    unsigned long note = composition[i++];
    if (i >= (sizeof composition / sizeof *composition)) {
        noTone(SPEAKER);
        delay(8000);
        i = 0;
        return;
    }
    if (note == NOTE_REST)
        noTone(SPEAKER);
    else if (note == NOTE_SUSTAIN)
        ;      // Don't do anything, just let the current tone continue
    else
        tone(SPEAKER, note);
    // If we want space between the notes, pass tempo as a third arg
    // to tone(), then do noTone() afterward.

    delay(tempo);
}
