/* -*- c -*- */

// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Motor test!");

    // turn on motors
    motor1.setSpeed(200);
    motor2.setSpeed(200);
 
    motor1.run(RELEASE);
    motor2.run(RELEASE);
}

#define FAST 255

void loop() {
    uint8_t i;

    delay(5000);

    // Go forward
    Serial.print("forward");
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    for (i=0; i<FAST; i++) {
        motor1.setSpeed(i);  
        motor2.setSpeed(i);  
        delay(10);
    }
    delay(200);

    // Stop
    Serial.print("stopping");
    for (i=FAST; i!=0; i--) {
        motor1.setSpeed(i);  
        motor2.setSpeed(i);  
        delay(10);
    }
    delay(3000);
  
    // Go backward
    Serial.print("backward");
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    for (i=0; i<FAST; i++) {
        motor1.setSpeed(i);  
        motor2.setSpeed(i);  
        delay(10);
    }

    // and stop
    Serial.print("stopping");
    for (i=FAST; i!=0; i--) {
        motor1.setSpeed(i);  
        motor2.setSpeed(i);  
        delay(10);
    }
    delay(3000);

    // Spin around
    Serial.print("spinning");
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    for (i=0; i<FAST; i++) {
        motor1.setSpeed(i);
        motor2.setSpeed(i);
        delay(10);
    }

    // and stop
    Serial.print("stopping");
    for (i=FAST; i!=0; i--) {
        motor1.setSpeed(i);  
        motor2.setSpeed(i);  
        delay(10);
    }

    Serial.print("tech");
    motor1.run(RELEASE);
    motor2.run(RELEASE);
}
