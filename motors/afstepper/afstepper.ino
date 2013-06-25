#include <AFMotor.h>

AF_Stepper motor(48, 2);

void setup() {
  motor.setSpeed(100);  // 10 rpm   

  motor.release();
  delay(1000);
}

void loop() {
  motor.step(500, FORWARD, SINGLE);    // Go 500 steps forward
  motor.step(100, BACKWARD, SINGLE);   // Go 100 steps back
  delay(1000);                         // Pause for a second
}
