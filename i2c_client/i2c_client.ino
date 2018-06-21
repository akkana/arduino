// A simple I2C client, for instance, to talk to a Raspberry Pi.

#include <Wire.h>            // For I2C

#define SLAVE_ADDRESS 0x04

int number = 0;

// callback for received data
void receiveData(int byteCount)
{
    while(Wire.available())
    {
        number = Wire.read();
        Serial.print("data received: ");
        Serial.println(number);
    }
}

// Send a single byte
void sendByte()
{
    Wire.write(number);
}

// Send an unsigned int (2 bytes, on Arduino)
void sendWord()
{
    // analogRead reads from 0 to 1023
    int sensorValue = analogRead(A0);

    Serial.print("sending ");
    Serial.println(sensorValue);

    Wire.write((byte*)&sensorValue, sizeof(unsigned int));
}

void setup()
{
    Serial.begin(9600); // start serial for output

    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendWord);

    Serial.println("Ready!");
}

void loop() {
    delay(10);
}
