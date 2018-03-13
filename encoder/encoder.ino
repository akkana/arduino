#include "rotary-encoder.h"

int BTN_PIN = 7;

void setup()
{
    Serial.begin(115200); // start the serial monitor link

    pinMode(BTN_PIN, INPUT_PULLUP);

    initRotaryEncoder();

    Serial.println("Starting ...");
}

void loop() {
    static int oldEncPos = 0;
    static int oldBtn = 0;

    int encoderPos = getRotaryPosition();
    int btn = digitalRead(BTN_PIN);

    if (oldEncPos != encoderPos) {
        Serial.print("encoder: ");
        Serial.println(encoderPos);
        oldEncPos = encoderPos;
    }
    if (btn != oldBtn) {
        Serial.print("button: ");
        Serial.println(btn);
        oldBtn = btn;
    }
}
