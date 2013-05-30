//
// Detect whether our radio signal is present, using the FreqCounter library.
// Based on the Frequency Counter Lib example.
// should give 6 or 7 when signal is present, 0 otherwise.

// Note: another way to do it might use pulsein,
// http://www.arduino.cc/en/Reference/PulseIn

#include <FreqCounter.h>

unsigned long frq;
int cnt;
int pinLED=13;

void setup() {
  pinMode(pinLED, OUTPUT);

  Serial.begin(115200);        // connect to the serial port

  Serial.println("Frequency Counter");
}

void loop() {

  // wait if any serial is going on
  FreqCounter::f_comp=10;   // Cal Value / Calibrate with professional Freq Counter
  FreqCounter::start(100);  // 100 ms Gate Time

  while (FreqCounter::f_ready == 0) 

  frq=FreqCounter::f_freq;
  Serial.print(cnt++);
  Serial.print("  Freq: ");
  Serial.println(frq);
  if (frq == 6 || frq == 7)
      digitalWrite(pinLED, 0);
  else
      digitalWrite(pinLED, 1);
  delay(20);
  //digitalWrite(pinLed,!digitalRead(pinLED));  // blink Led

}  

