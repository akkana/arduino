#include "rotary-encoder.h"

void setup()
{
    Serial.begin(115200); // start the serial monitor link

    initRotaryEncoder();

    Serial.println("Starting ...");
}

void loop() {
    static int oldEncPos = 0;

    int encoderPos = getRotaryPosition();

    if (oldEncPos != encoderPos) {
        Serial.println(encoderPos);
        oldEncPos = encoderPos;
    }
}
