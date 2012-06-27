/*
  Analog input, analog output, serial output from a dual-axis joystick.
  Adapted from AnalogInOutSerial in the Arduino examples.

  The circuit:
  Joystick outputs connected to analog pins 0 and 1,
  with the + and gnd pins connected appropriately.
  LEDs connected to digital pins 9 and 10,
  then to ground through appropriate resistors.
 */

// The pins used:
const int analogInPin0 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin1 = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin0 = 9; // Analog output pin that the LED is attached to
const int analogOutPin1 = 10; // Analog output pin that the LED is attached to

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  int sensorValue0 = analogRead(analogInPin0);
  // map it to the range of the analog out:
  int outputValue0 = map(sensorValue0, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin0, outputValue0);

  // do the same for the other axis:
  int sensorValue1 = analogRead(analogInPin1);
  int outputValue1 = map(sensorValue1, 0, 1023, 0, 255);
  analogWrite(analogOutPin1, outputValue1);

  // print the results to the serial monitor:
  Serial.print("sensor0 = " );
  Serial.print(sensorValue0);
  Serial.print("\t output0 = ");
  Serial.print(outputValue0);
  Serial.print("\tsensor1 = " );
  Serial.print(sensorValue1);
  Serial.print("\t output1 = ");
  Serial.println(outputValue1);

  delay(50);    
}
