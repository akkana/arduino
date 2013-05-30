/* -*- c -*- */
/*
 * Print the output of a sonar rangefinder by timing the PWM pulses
 * coming from an HC-SR04 rangefinder.
 *
 * by Akkana Peck <http://shallowsky.com>
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License.
 */

int triggerPin = 7;
int echoPin = 8;

void setup()
{
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);

    Serial.begin(9600);
}

void loop()
{
    // Send the trigger high for 10 microseconds or more
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    int duration = pulseIn(echoPin, HIGH);
    Serial.print("Duration: ");
    Serial.print(duration);
    Serial.print(" usec");

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


