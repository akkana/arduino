/* -*- c -*- */

// Test of Pololu TB6612FNG Dual Motor Driver Carrier
// http://www.pololu.com/catalog/product/713

int stdby = 7;          // standby pin, must be high for anything to work

int dist_sensor = 0;    // distance sensor on analog pin 0

// And a few other constants:
int driveSpeed = 80;         // 0 to 255, I think
int steerSpeed = 200;
int backingTime = 2500; // msec

////////////////////////////////////////////////////////////
// WIRING (a proto-shield is useful):
// Side 1, input:
//   Gnd to ground
//   STDBY to pin 7 or V+
//   PWMA 9   AIN1 2  AIN2 3
//   PWMB 10  BIN1 4  BIN2 5
//   
// Side 2:
//   wire both GND pins to Gnd
//   VMOT to motor power supply
//   VCC to Arduino V+
//   Motors into AO1, AO2, BO1, BO2
//  

////////////////////////////////////////////////////////////
// Some generic code for handling the motor driver:

// Motor pins
// Each motor has two direction pins (both high: brake, both low: coast)
// plus one PWM speed pin.
struct motor {
    int speed;
    int dir[2];
};

// motors[0] is the drive motor; [1] is the steering motor.
struct motor motors[] = {
    { 9,  { 2, 3 } },
    { 10, { 4, 5 } }
};

void initMotor(int whichmotor)
{
    pinMode(motors[whichmotor].dir[0], OUTPUT);
    pinMode(motors[whichmotor].dir[1], OUTPUT);
    pinMode(motors[whichmotor].speed, OUTPUT);
}

void stop(int whichmotor)
{
    struct motor m = motors[whichmotor];
    digitalWrite(m.speed, 0);
    digitalWrite(m.dir[0], true);
    digitalWrite(m.dir[1], true);
}

// Drive a motor in a direction, 0 or 1
void drive(int whichmotor, int direc, int speed)
{
    if (direc == 0) {
        digitalWrite(motors[whichmotor].dir[1], true);
        digitalWrite(motors[whichmotor].dir[0], false);
    } else {
        digitalWrite(motors[whichmotor].dir[0], true);
        digitalWrite(motors[whichmotor].dir[1], false);
    }
    analogWrite(motors[whichmotor].speed, speed);
}

// End of generic code for handling the motor driver
////////////////////////////////////////////////////////////

void back_and_turn()
{
    // Light the LED to show we're still in this routine
    // (in case we get stuck):
    digitalWrite(13, true);

    // Brake the motor
    stop(0);

    // Turn the steering wheel
    drive(1, 0, steerSpeed);

    // Back up, relatively slowly
    drive(0, 1, driveSpeed);
    delay(backingTime);

    // brake and un-steer:
    stop(0);
    stop(1);

    // and turn off the LED
    digitalWrite(13, false);
}

void setup()
{
    initMotor(0);
    initMotor(1);

    pinMode(stdby, OUTPUT);

    pinMode(13, OUTPUT);

    // Start with both motors braked:
    stop(0);
    stop(1);

    delay(2000);

    // Set the standby pin high so we can drive the motors:
    digitalWrite(stdby, true);
}

void loop()
{
    // Are we about to run into something?
    // The sensor returns around 1023 until we're within about 5-10cm
    // of an object, at which point it goes down under 300.
    int dist = analogRead(dist_sensor);
    if (dist < 500)
        back_and_turn();

    drive(0, 0, driveSpeed);
    delay(10);
}

