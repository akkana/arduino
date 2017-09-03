/* -*- c -*- */
#line 3

/*
 * Control a DSLR camera such as a Canon EOS Rebel
 * through its 3-wire remote.
 *
 * http://forum.arduino.cc/index.php/topic,10207.0.html
 * http://www.paulodowd.com/2013/02/arduino-remote-trigger-for-dslr-stop.html
 * http://www.instructables.com/id/How-to-create-an-Arduino-remote-shutter/?ALLSTEPS
 * Optocoupler: http://www.vishay.com/docs/83725/4n25.pdf
 *  from http://www.instructables.com/id/Turn-on-and-off-your-things-with-a-laser/?ALLSTEPS
 * Use a resistor around 380 ohm, exact value not critical:
 * http://forum.arduino.cc/index.php?topic=56485.0
 */

int focusPin = 6;
int shutterPin = 7;

int focusDelay = 50;
int shutterOpen = 100;
int betweenPictures = 10000;

void setup()
{
    pinMode(focusPin, OUTPUT);
    pinMode(shutterPin, OUTPUT);
}

void snapPhoto()
{
    digitalWrite(focusPin, HIGH);
    delay(focusDelay);
    digitalWrite(shutterPin, HIGH);
    delay(shutterOpen);
    digitalWrite(shutterPin, LOW);
    digitalWrite(focusPin, LOW);
}

void loop()
{
    delay(betweenPictures);
    snapPhoto();
}

