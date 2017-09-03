
#include <Servo.h>

#define TWO

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
#ifdef TWO
Servo myservo2;
#endif

int pos = 0;    // variable to store the servo position

void setup()
{
    myservo.attach(7);
    myservo.write(0);
#ifdef TWO
    myservo2.attach(8);
    myservo2.write(0);
#endif
    delay(3000);

    /*
    delay(1000);
    myservo.write(10);
    delay(1000);
    myservo.write(170);
    delay(1000);
    myservo.write(90);
    */
}


void loop()
{
    for (pos = 0; pos <= 180; pos += 1) {
        myservo.write(pos);
#ifdef TWO
        myservo2.write(pos);
#endif
        delay(15);
    }
    delay(1000);

    for (pos = 180; pos >= 0; pos -= 1) {
        myservo.write(pos);
#ifdef TWO
        myservo2.write(pos);
#endif
        delay(15);
    }
    delay(3000);
}
