/*
 * A green-yellow-red timer for short speeches, for instance, in Toastmasters.
 * Uses a MAX7219 8-digit 7-segment LED display.
 *
 * MAX7219 Wiring:
 *   Vcc to Arduino 5V Pin
 *   GND to Arduino GND
 *   DIN to Arduino pin 12
 *   CS to Arduino pin 10
 *   CLK to Arduino pin 13
 */

#include "SpeechTimer.h"

LedControl* dpy = 0;

SpeechTimer* speechtimer = 0;

#define LED_PIN     5
//#define BRIGHTNESS  200
#define NUM_LEDS 7
CRGB lightstring[NUM_LEDS];

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");

    dpy = new LedControl(12, 13, 10, 2);

    // The MAX72XX is in power-saving mode on startup, so wake it up.
    dpy->shutdown(0, false);

    // Set the brightness to a medium values
    dpy->setIntensity(0, 8);

    // clear the display
    dpy->clearDisplay(0);

    // Initialize the neopixel ring
    delay(2000); // sanity delay
    FastLED.addLeds<NEOPIXEL, LED_PIN>(lightstring, NUM_LEDS);

    // start/stop button on 7, up on 8, down on 9.
    speechtimer = new SpeechTimer(dpy, 5, 7, 8, 9, lightstring);
}

void loop() {
    speechtimer->tick();
    delay(100);
}
