#include <Servo.h>
 
Servo right_servo;
Servo  left_servo;

// The servo limits:
int DOWN = 10;
int UP   = 170;

// The positions of the arms:
int arm = DOWN;

// Should we move the arms up, or down?
int direction = 5;

void setup() {
    Serial.begin(9600);
    right_servo.attach(8);
    right_servo.write(DOWN);

    left_servo.attach(7);
    left_servo.write(DOWN);

    Serial.println("Arms Down!");
    delay(2000);
}

void loop() {
    int waving = digitalRead(2);
    //Serial.println(x);

    // If we detected someone, then move the arms up and down:
    if (waving) {
        Serial.println("Waving!");
        if (arm >= UP)
            direction = -5;
        else if (arm <= DOWN)
            direction = 5;
        // Otherwise we're somewhere in between, so leave direction unchanged.

        arm += direction;
        Serial.print("Waving! ");
        Serial.println(arm);
        right_servo.write(arm);
        left_servo.write(arm);
    }
    else
        Serial.println("Stopping.");
}

