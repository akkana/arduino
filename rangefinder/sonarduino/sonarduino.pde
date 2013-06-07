
/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * Use a sonar rnagefinder and turn it into clicking sounds:
 * interval between sounds corresponds to rangefinder distance.
 *
 * Copyright 2011 by Akkana Peck.
 * Share and enjoy under the terms of the GPL v.2 or later.
 *
 * Earbud Wiring:
 *  D PIN 0 --- 10k resistor ---- + 47 uF cap - --- earbud leads
 * The cap doesn't seem to make any audible difference; suggested by
 * http://www.thinkcreate.org/index.php/experience-%E2%80%9Coctave-illusion%E2%80%9D-with-arduino/
 * Square waves may not be good for earbuds:
 * use cheap ones, not your best set.
 *
 * Rangefinder widing:
 *  A PIN 0 --- 
 */

/* Calibration:
 * Higher factor means fewer clicks for the same object distance.
 * Higher MINRANGE means a greater distance when clicks are almost continuous.
 * These aren't in any useful units; they correspond to the voltage
 * readings coming from the rangefinder.
 * The rangefinder reads around 35-40 for something a couple of feet away,
 * 150-250 across a small room.
 */
#define FACTOR       1
#define MINRANGE     20

/* How close together (in milliseconds) can clicks be? */
#define MINTICK       5

/* We'll tick at least this often */
#define MAXTICK    4000

#define FREQ       8000

/* Pins assignments */
#define EARBUD       5
#define RANGEFINDER  1

/* length of a click in milliseconds */
#define CLICKLEN    10

void setup()
{
    pinMode(EARBUD,      OUTPUT);
    pinMode(RANGEFINDER, INPUT);
    Serial.begin(9600);
}

void buzz(int pin, long frequency, long length)
{
    long delayValue = 1000000/frequency/2;
    /* calculate the delay value between transitions:
     * 1 second's worth of microseconds, divided by the frequency,
     * then split in half since there are two phases to each cycle.
     */
    long numCycles = frequency * length / 1000;

    for (long i=0; i < numCycles; i++) { // for the calculated length of time...
        digitalWrite(pin,HIGH);
        delayMicroseconds(delayValue);
        digitalWrite(pin,LOW);
        delayMicroseconds(delayValue);
    }
}

static int nextTick = MAXTICK;
void loop()
{
    float range = analogRead(RANGEFINDER);
    if (range < MINRANGE)
        range = MINRANGE;

    int timeout = FACTOR * (range-MINRANGE);
    if (timeout < nextTick)
        nextTick = timeout;

    Serial.print(range);
    Serial.print(" -> ");
    Serial.print(timeout);
    Serial.print(" -> ");
    Serial.println(nextTick);

    if (nextTick <= 0) {
        buzz(EARBUD, FREQ, 15);
        nextTick = MAXTICK;
    }

    nextTick -= MINTICK;
    delay(MINTICK);
}

/* Various sites say to add this to get around the missing symbol when
 * including Serial.println. No idea why it's an infinite loop.
 * The error otherwise is
applet/core.a(Print.o):(.data+0x6): undefined reference to `__cxa_pure_virtual'
 * and comes from Print.h:    virtual void write(uint8_t) = 0;
extern "C" void __cxa_pure_virtual() { while (1) ; }
 */

