/*
 * A green-yellow-red timer for short speeches, for instance, in Toastmasters.
 *
 * MAX7219 8-digit 7-segment LED display Wiring:
 *   Vcc to Arduino 5V Pin
 *   GND to Arduino GND
 *   DIN to Arduino pin 12
 *   CS to Arduino pin 10
 *   CLK to Arduino pin 13
 *
 * Neopixel ring on pin 5
 *
 * start/stop button on 7, rotary encoder on 2 and 3.
 * Buttons are inverted: normally high, pressed is low,
 * using internal pullups.
 * If using a rotary encoder, normally there's a path from the
 * center pin through both outside pins
 */

#include "SpeechTimer.h"

LedControl* dpy = 0;

SpeechTimer* speechtimer = 0;

#define LED_PIN     5
#define NUM_LEDS 7
CRGB lightstring[NUM_LEDS];

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");

    dpy = new LedControl(12, 13, 10, 2);

    // The MAX72XX is in power-saving mode on startup, so wake it up.
    dpy->shutdown(0, false);

    // Set the LCD display's brightness to a medium values
    dpy->setIntensity(0, 8);

    // clear the display
    dpy->clearDisplay(0);

    // Initialize the neopixel ring
    delay(2000); // sanity delay
    FastLED.addLeds<NEOPIXEL, LED_PIN>(lightstring, NUM_LEDS);

    // SpeechTimer with up/down buttons:
    // speechtimer = new SpeechTimer(dpy, 5, 7, 8, 9, lightstring);
    // SpeechTimer with rotary encoder for up/down:
    speechtimer = new SpeechTimer(dpy, 5, 7, lightstring);
}

void loop() {
    speechtimer->tick();
    delay(100);
}
