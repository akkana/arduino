/* -*- c-mode -*- */
/*
 * Print the output of a sonar rangefinder on analog input 1.
 * A machine on the other end can read the serial output
 * and do something with it.
 *
 * by Akkana Peck <http://shallowsky.com>
 * This is Free Software: share and enjoy under the terms of the
 * GNU Public License.
 */

int sensorPin = 1;

void setup()
{
    pinMode(sensorPin, INPUT);

    Serial.begin(9600);
}

void loop()
{
    while (1) {
        /* Read 8 values and average them, in case of spurious readings */
        float val = 0;
        for (int i=0; i<8; i++) {
            val += analogRead(sensorPin);
            delay(50);
        }
        val /= 8;
        Serial.println(val);
        delay(250);
    }
}

