/* -*- c -*- */

/*
 * Arduino basic blink
 */

/* Times in milliseconds */
#define DELAYTIME   500

int ledPin = 13;

void setup()
{
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    digitalWrite(ledPin, HIGH);
    delay(DELAYTIME);
    digitalWrite(ledPin, LOW);
    delay(DELAYTIME);
}

