/* -*- c -*- */

// Test an electronic speed controller (ESC) to find out its arming sequence.
// Wouldn't it be nice if these were standard?
//
// How to use:
// Connect Arduino digital pin 8 to the input of the ESC.
// wire up a motor (with some load on it).
// Connect the Arduino to USB, but can be powered either from USB
// or from the ESC battery.
// Switch on, and connect to usb-serial at 9600 baud.
// Watch the numbers as the Arduino counts up, and see where the
// motor first beeps.
// If nothing has happened by the time you get to 180, you should
// probably give up (the Servo library uses angles from 0 to 180).

#include <Servo.h>

// Pin 8 goes to the input of the ESC.
#define MOTOR_PIN        8

Servo motorServo;

int counter = 0;

void setup()
{
    motorServo.attach(MOTOR_PIN);
    Serial.begin(9600);
    motorServo.write(0);
    delay(1000);
    motorServo.write(70);
    delay(1000);
    motorServo.write(0);
    delay(1000);
    Serial.println("Counting up");
    counter = 60;
}

void loop()
{ 
    Serial.println(counter);
    
    digitalWrite(13, counter%5);
    motorServo.write(counter++);

    delay(200);

    if (counter > 180) {
        Serial.println("Hit 180 -- waiting 2 seconds");
        delay(1800);
        counter = 0;
        Serial.println("Zero -- waiting 2 seconds");
        motorServo.write(counter);
        delay(2000);
        Serial.println("Restarting count.");
    }

    return;
}

