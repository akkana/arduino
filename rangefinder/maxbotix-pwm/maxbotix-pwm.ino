/* -*- c-mode -*- */
/*
 * Print the output of a sonar rangefinder by timing the PWM pulses
 * coming from the rangefinder, on digital pin 7.
 * A machine on the other end can read the serial output
 * and do something with it.
 *
 * In practice, the rangefinder returns values between about 30 and 50.
 *
 * by Akkana Peck <http://shallowsky.com>
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License.
 */

int triggerPin = 7;
int sensorPin = 8;

void setup()
{
    pinMode(triggerPin, OUTPUT);
    pinMode(sensorPin, INPUT);

    Serial.begin(9600);
}

void loop()
{
    // Wait for the line to go high, so we'll see a transition to low.
    while (! digitalRead(sensorPin))
        ;
    // Now it's high. Wait for it to go low.
    while (digitalRead(sensorPin))
        ;
    long start_time = millis();
    // Wait for it to go high again:
    while (! digitalRead(sensorPin))
        ;
    long elapsed_time = millis() - start_time;
    Serial.println(elapsed_time);

    delay(100);
}


