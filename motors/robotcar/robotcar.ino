/* -*- c -*- */
/*
 * Drive a little car around, using the HalfBridge library.
 * Take input from an HC-SR04 rangefinder, and if we get too close
 * to a wall or anything, stop, then spin around.
 *
 * by Akkana Peck, http://shallowsky.com
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License v2 or later.
 */

#include <HalfBridge.h>

#define DEBUG 1

// Motor 0 has a speed control on pin 9, direction controls on pins 2 and 3.
// Motor 1 uses 10 for speed control, 4 and 5 for direction.
Motor motors[2] = { Motor(9, 2, 3), Motor(10, 4, 5) };

int FASTER = 170;
int FAST = 80;
int SLOW = 60;

// For the rangefinder:
int triggerPin = 7;
int echoPin = 8;

int CLOSE = 20;
int TOOCLOSE = 15;

// Read the rangefinder several times, in case of noise.
// Not too many, though, in case our car is moving fast.
int num_reads = 1;

int read_inches()
{
    int distance = 0;
    for (int i=0; i < num_reads; ++i) {
        // Send the trigger high for 10 microseconds or more
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);

        // Now read how many microseconds the echo pin stays high:
        distance += pulseIn(echoPin, HIGH);    // microseconds
    }
    // Take the average, then divide by 140 to convert to inches:
    distance /= (num_reads * 140);

#ifdef DEBUG
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" inches");
#endif

    return distance;
}

void setup()
{
    // Initialize the rangefinder:
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);

    // Initialize the motors:
    motors[0].init();
    motors[1].init();

#ifdef DEBUG
    Serial.begin(9600);
#endif

    // Long delay at the beginning, to allow time to unplug USB.
    delay(8000);
}

void loop()
{
    // Is there anything in front of us? Read the rangefinder.
    int inches = read_inches();

    // If everything is comfortably far away, go normal speed.
    if (inches > CLOSE) {
        motors[0].setSpeed(FAST);
        motors[1].setSpeed(FAST);
    }

    // If we're getting close, slow down.
    else if (inches > TOOCLOSE) {
        motors[0].setSpeed(SLOW);
        motors[1].setSpeed(SLOW);
    }

    // Otherwise, we must be really close to something.
    // Take evasive action.
    else {
        // First, stop.
        motors[0].setSpeed(0);
        motors[1].setSpeed(0);
        delay(500);

        // Back up a little.
        motors[0].setSpeed(-SLOW);
        motors[1].setSpeed(-SLOW);
        delay(1000);

        // Stop again
        motors[0].setSpeed(0);
        motors[1].setSpeed(0);
        delay(300);

        // Now spin around:
        motors[0].setSpeed(-FASTER);
        motors[1].setSpeed(FASTER);
        delay(2000);

        // Stop again.
        motors[0].setSpeed(0);
        motors[1].setSpeed(0);
        delay(500);

        // Now don't start up immediately -- when we call loop() again
        // we'll check the rangefinder again and make sure our spinning
        // around hasn't put us pointing at another obstacle.
    }
}
