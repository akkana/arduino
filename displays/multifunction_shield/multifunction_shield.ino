/*
  http://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/
  http://arduinolearning.com/code/multi-function-shield-examples.php
 */

#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library

  MFS.write("12:34");
  delay(2000);
}

int counter = 0;

void loop() {
     counter = (counter + 1) % 10000;

     if (counter % 10 == 0)
         MFS.write("Hiya");
     else
         MFS.write((int)counter);

     delay(200);
}
