// Water demo for Arduino LED strings
// Inspired by Fire2012 by Mark Kriegsman,
// since the other four elements don't seem to be publically available.

#include <FastLED.h>

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    7

#define BRIGHTNESS  50   // was 200
#define FRAMES_PER_SECOND 15  // 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

void setup() {
    delay(3000); // sanity delay
    FastLED.addLeds<CHIPSET, LED_PIN,
        COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
    static int pos = 0;
    static int width = 40;

    for( int i = 0; i < NUM_LEDS; i++) {
        //CRGB color = HeatColor( heat[j]);
        double s = sin(i * PI * width / NUM_LEDS / 10.);

        int j = (i + pos) % NUM_LEDS;
        // % in C is a remainder, not modulo, so it can be negative.
        // Make sure it isn't:
        while (j < 0)
            j += NUM_LEDS;

        leds[j].blue = BRIGHTNESS;
        leds[j].red = BRIGHTNESS * s * random(10, 30) / 100.;
        leds[j].green = BRIGHTNESS * s * random(25, 55) / 100.;
    }
    pos += random(-1, 2);
    width += random(-2, 3);

    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}

