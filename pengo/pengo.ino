#include <Servo.h>

/* WIRING:
 *
 * Rangefinder: left to right, red=VCC, yellow=TRIG, green=ECHO, black=Gnd
 * TRIG (yellow) goes to triggerPin (2), ECHO (green) goes to echoPin (3).
 *
 * GWS Servos:
 * brown=Gnd red=V+ orange=signal, the two signal pins are 7 and 8.
 * Airtronics servos:
 * red=v+, middle black=Gnd, far black=signal
 */

Servo right_servo;
Servo  left_servo;

// The servo limits:
int DOWN = 10;
int UP   = 170;

// The positions of the arms:
int arm = DOWN;

// Should we move the arms up, or down? By how much each time?
int amount = 20;
int direction = amount;

// For the rangefinder sensor
int triggerPin = 2;
int echoPin = 3 ;

// For the servos
int leftServoPin = 7;
int rightServoPin = 8;

#define DEBUG 1

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif
    right_servo.attach(rightServoPin);
    right_servo.write(DOWN);

    left_servo.attach(leftServoPin);
    left_servo.write(DOWN);

#ifdef DEBUG
    Serial.println("Arms Down!");
#endif

    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);

    // Initial delay of 2 seconds
    delay(5000);
}

void loop() {
    // Send the trigger high for 10 microseconds or more
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    int duration = pulseIn(echoPin, HIGH);
    //Serial.print("Duration: ");
    //Serial.print(duration);
    //Serial.print(" usec");

    float inches = duration/148;
#ifdef DEBUG
    Serial.print(inches);
    Serial.print(" inches: ");
#endif

    // If we detected someone, then move the arms up and down.
    // 25 is about the maximum this sensor can detect, and it
    // isn't really 25", more like half that.
    if (inches < 25) {
        Serial.println("Waving!");
        if (arm >= UP)
            direction = -amount;
        else if (arm <= DOWN)
            direction = amount;
        // Otherwise we're somewhere in between, so leave direction unchanged.

        arm += direction;
#ifdef DEBUG
        Serial.print("Waving! ");
        Serial.println(arm);
#endif
        right_servo.write(arm);
        left_servo.write(arm);

        // Tune the amount of delay for how fast you want the arms to move.
    }
#ifdef DEBUG
    else {
        Serial.println("Arms back down.");
        left_servo.write(DOWN);
        right_servo.write(DOWN);
    }
#endif

    delay(80);
}

