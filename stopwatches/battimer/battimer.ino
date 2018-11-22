/* -*- c-mode -*- */

/*
 * BatTimer
 * A Timer for model airplane batteries: sound an alarm after a fixed time.
 * by Akkana <http://shallowsky.com>
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License.
 */

/* Times in milliseconds */
#define SHORTBLINK   90
#define ALARMBLINK  300
#define AFTERBLINK 1000

#define DELAYTIME  1500

#define SHORTBEEP    70
#define ALARMBEEP   400

int ledPin = 13;          // select the pin for the LED
int buzzerPin = 8;

unsigned long startTime = 0;        // # milliseconds when the start button was released
unsigned int curMins = 0;

void setup()
{
    pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
    pinMode(buzzerPin, OUTPUT);
#ifdef DEBUG
    Serial.begin(9600);
#endif
}

/* beep or flash the LED several times for duration milliseconds */
void beepOrFlashN(int num, int duration, int pin)
{
    while (num-- > 0) {
        digitalWrite(pin, HIGH);
        delay(duration);
        digitalWrite(pin, LOW);
        delay(duration);
    }
}

void loop()
{
    if (startTime == 0) {
        digitalWrite(ledPin, LOW);
        beepOrFlashN(2, SHORTBEEP, buzzerPin);
        beepOrFlashN(3, SHORTBLINK, ledPin);
        startTime = millis();
#ifdef DEBUG
        Serial.println((int)startTime);
#endif
    }

    /* Running ... the normal case */
    else {
        unsigned long mils = millis() - startTime;
        unsigned long mins = mils / 1000 / 60;
 #ifdef DEBUG
        Serial.println(mils);
 #endif
        if (mins > curMins) {
            if (mins == 9)
                beepOrFlashN(2, SHORTBEEP, buzzerPin);
            else if (mins == 10)
                beepOrFlashN(1, ALARMBEEP, buzzerPin);
            else if (mins > 10)
                beepOrFlashN(mins%10, SHORTBEEP, buzzerPin);
            //else
            //    beepN(1, SHORTBEEP);
        }
        if (mins > 0)
            beepOrFlashN(mins%5, SHORTBLINK, ledPin);

        curMins = mins;
        delay(DELAYTIME);        // sleep
    }
}

