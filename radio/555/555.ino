/* -*- c -*- */
/*
 * Announce when we see anything on an incoming pin.
 * The idea is to watch the radio receiver pin and see when it's transmitting.
 * (Then use a 555 to drive the transmitter.)
 * Unfortunately these radios don't work like that and can only send
 * very short logic pulses, not pulses long enough to see.
 */

void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(9600);	// Debugging only
    Serial.println("Radio receiver");
}

void loop()
{
    int on = analogRead(1);
    digitalWrite(13, on);

    Serial.println(on);

    delay(100);
}

