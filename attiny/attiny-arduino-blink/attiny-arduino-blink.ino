/*
 * Arduino basic blink
 */

/* Times in milliseconds */
#define DELAYTIME   500

int ledPin = 3;
int ledPin2 = 2;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
}

void loop()
{
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
    delay(DELAYTIME);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(DELAYTIME);
}

