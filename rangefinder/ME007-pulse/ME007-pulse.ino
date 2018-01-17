/* -*- c -*- */
/*
 * Print the output of a sonar rangefinder by timing the PWM pulses
 * coming from an HC-SR04 rangefinder.
 * Simpler alternative to the NewPing library,
 * http://code.google.com/p/arduino-new-ping/
 *
 * by Akkana Peck, http://shallowsky.com
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License v2 or later.
 */

int triggerPin = 4;
int echoPin = 3;

void setup()
{
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, HIGH);
    pinMode(echoPin, INPUT);

    Serial.begin(9600);
}

void loop()
{
    // Send the trigger high for 10 microseconds or more
    digitalWrite(triggerPin, LOW);

    int duration = pulseIn(echoPin, HIGH);
    Serial.print("Duration: ");
    Serial.print(duration);
    Serial.print(" usec");
    digitalWrite(triggerPin, HIGH);

    float inches = duration/148;
    float cm     = duration / 58;
        //(duration/2) / 29.1;
    Serial.print("     Distance: ");
    Serial.print(inches);
    Serial.print("inches, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println("");

    delay(500);
}


