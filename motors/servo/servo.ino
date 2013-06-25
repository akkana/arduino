/* -*- c -*- */
#line 3

#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
 
void setup()
{
    myservo.attach(7);

    delay(1000);
    myservo.write(10);
    delay(1000);
    myservo.write(170);
    delay(1000);
    myservo.write(90);
}
 
 
void loop()
{
    delay(1000);
    return;

    for (pos = 0; pos < 180; pos += 1)
    {
        myservo.write(pos);
        delay(15);
    }
    for (pos = 180; pos>=1; pos-=1)
    {                                
        myservo.write(pos);
        delay(15);
    }
}

