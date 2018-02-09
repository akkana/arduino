// Stacked CD4021 Shift Registers.
// Based on Carlyn Maw's Arduino ShiftIn example at
// https://www.arduino.cc/en/Tutorial/ShiftIn

// The three pins used by the CD4021:
int latchPin = 8;
int dataPin = 9;
int clockPin = 7;

// We can store up to three bytes, one for each shift register:
byte switchVar1;
byte switchVar2;
byte switchVar3;

void setup()
{
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
}

// Print a byte as binary with leading zeros:
void printBits(byte myByte)
{
    for(byte mask = 0x80; mask; mask >>= 1) {
        if (mask  & myByte)
            Serial.print('1');
        else
            Serial.print('0');
    }
}

void loop()
{
    // Pulse the latch pin:
    digitalWrite(latchPin, 1);
    delayMicroseconds(20);
    digitalWrite(latchPin, 0);

    // While the shift register is in serial mode,
    // collect each shift register into a byte.
    // The register attached to the chip comes in first.
    switchVar1 = shiftIn(dataPin, clockPin, MSBFIRST);
    switchVar2 = shiftIn(dataPin, clockPin, MSBFIRST);
    switchVar3 = shiftIn(dataPin, clockPin, MSBFIRST);

    // Print all three:
    printBits(switchVar1);
    Serial.print("   ");
    printBits(switchVar2);
    Serial.print("   ");
    printBits(switchVar3);

    // and add a final newline.
    Serial.println();

    delay(1000);
}

////// ----------------------------------------shiftIn function
///// just needs the location of the data pin and the clock pin
///// it returns a byte with each bit in the byte corresponding
///// to a pin on the shift register. leftBit 7 = Pin 7 / Bit 0= Pin 0
byte shiftIn(int myDataPin, int myClockPin) {
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);
//we will be holding the clock pin high 8 times (0,..,7) at the
//end of each time through the for loop

//at the begining of each loop when we set the clock low, it will
//be doing the necessary low to high drop to cause the shift
//register's DataPin to change state based on the value
//of the next bit in its serial information flow.
//The register transmits the information about the pins from pin 7 to pin 0
//so that is why our function counts down
  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    else {
      //turn it off -- only necessary for debuging
     //print statement since myDataIn starts as 0
      pinState = 0;
    }

    //Debuging print statements
    //Serial.print(pinState);
    //Serial.print("     ");
    //Serial.println (dataIn, BIN);

    digitalWrite(myClockPin, 1);

  }
  //debuging print statements whitespace
  //Serial.println();
  //Serial.println(myDataIn, BIN);
  return myDataIn;
}
