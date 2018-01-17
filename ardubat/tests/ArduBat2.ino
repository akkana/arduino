/////////////////////////////////////////////////////////////////////////////////////////
//
//        Tony's ArduBat Logging Program -- AdaFruit 1141 Datalogger shield
//        and ArduBat shield are used -- Date and time functions using a 
//        DS1307 RTC connected via I2C and Wire lib -- SD card also used
//
//        This version supports analog data ...
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <RTClib.h>
#include <SD.h>

RTC_DS1307 rtc;

const int chipSelect = 10;
 
const int Bats = 2;          // Bat Pulses
const int RLY1 = 3;          // Relay ON
const int RLY0 = 4;          // Relay Off
const int LedA = 5;          // Green LED
const int LedB = 6;          // Yellow LED
const int LedC = 7;          // Red LED
const int PBF1 = 8;          // F1 PButton
const int PBF2 = 9;          // F2 PButton

unsigned long BatPulse = 0;  // holds the value of PulseIn
unsigned long MaxTBC = 0;    // Maximum TBC holder

unsigned long sensorValue;   // Result of Analog Read
unsigned long anaLog;        // Calc Value

int Control = 0;             // Processing control variable

int Freq = 0;                // current frequency
int AvFrq = 0;               // Averaged frequency
int Samples = 0;             // # of samples

int CallRly = 0;             // Used to timeout the Bat Call Relay
int LoopCntr = 0;            // Misc Loop Counter

int Events = 0;              // Used to count bat call events
int AIline = 0;              // Activity in line
int AIhour = 0;              // Activity in one hour
int AIdiv = 0;               // Hourly AI divisor

int LastDay = 0;             // Triggers new header line
int LastHour = 0;            // Triggers new data block
int LastMin = 0;             // Triggers new data line
int LastSec = 0;             // Triggers timeouts

String dataString = "New";   // Used to accumulate data file text

boolean Activity = false;
boolean RlyOn = false;

void setup () {
  
  pinMode ( Bats, INPUT );     // Div 32 pulse input
  pinMode ( RLY1, OUTPUT );    
  pinMode ( RLY0, OUTPUT );
  pinMode ( LedA, OUTPUT );
  pinMode ( LedB, OUTPUT );
  pinMode ( LedC, OUTPUT );
  pinMode ( PBF1, INPUT );
  pinMode ( PBF2, INPUT );
  
  pinMode(10, OUTPUT);
  
  analogReference( DEFAULT );
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  
  
  digitalWrite ( RLY1, LOW );  // Initialize Relay
  digitalWrite ( RLY0, HIGH ); // Turn OFF
  delay ( 250 );               // ( Pulsed !! )
  digitalWrite ( RLY0, LOW );  // relay is now off
  
  digitalWrite ( LedA, LOW );  // initialize the
  digitalWrite ( LedB, LOW );  // LEDS
  digitalWrite ( LedC, HIGH ); // Red is ON
  
  Serial.begin(57600);

#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  rtc.begin();

  while (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    digitalWrite ( LedC, HIGH );
    delay ( 500 );
    digitalWrite ( LedC, LOW );
    delay ( 500 );
  }

///////////////////////////////////////////////////////////////////////////////////
//
//    Intialize the SD card ... if the card con't be intialized ( missing )
//    then blink the Red LED until it can.
//
///////////////////////////////////////////////////////////////////////////////////

  while (!SD.begin(chipSelect)) {    
    digitalWrite ( LedC, LOW );                         // Blink the Red led
    delay ( 1000 );
    digitalWrite ( LedC, HIGH );
    delay ( 1000 );
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);  
  if (dataFile) { 
    dataFile.println();
    dataFile.println("Powered Up...");
    dataFile.println();
    dataFile.close();
  }  
}

////////////////////////////////////////////////////////////////////////////////////////
//
//                                MAIN PROGRAM LOOP
//
////////////////////////////////////////////////////////////////////////////////////////

void loop () {
  
    DateTime now = rtc.now();                 // Reads RTC into variables
    
////////////////////////////////////////////////////////////////////////////////////////
//
//          This routine checks the Start Logging Button ( F1 ) and
//          enables logging, and turns off the Red LED ...
//          and turns the Green LED on.  The routine then waits for
//          the top of a ten minute data window ... green blinks.
//
////////////////////////////////////////////////////////////////////////////////////////

    if ( ( digitalRead ( PBF1 ) == LOW )) {
      if ( Control == 0 ) {
        digitalWrite ( LedA, HIGH );          // Turn on Green
        digitalWrite ( LedC, LOW );           // Turn off Red LED
        
        Control = 1;                          // Enable logging
        LoopCntr = 0;                         // Set up Green Blink
        
        LastDay = 0;
        LastHour = now.hour();                
        LastMin = now.minute();    
      
        File dataFile = SD.open("datalog.txt", FILE_WRITE);  
        if (dataFile) { 
          dataFile.println();
          dataFile.println("Logging Started...");
          dataFile.println();
          dataFile.close();
        }
      }
    }
        

////////////////////////////////////////////////////////////////////////////////////////
//
//      This routine checks the Stop Logging Button ( F2 ) and
//      disables logging, and turns on the Red LED ...
//      and turns the Green LED off.
//
////////////////////////////////////////////////////////////////////////////////////////

    if ( ( digitalRead ( PBF2 ) == LOW )) {
      if ( Control > 0 ) {
        digitalWrite ( LedC, HIGH );          // Turn on Red LED
        digitalWrite ( LedA, LOW );           // Turn off Green LED
        digitalWrite ( LedB, LOW );           // Turn off Yellow LED

        Control = 0;                          // Disable logging
        
        digitalWrite ( RLY0, HIGH );          // Turn Off Relay
        delay ( 250 );                        // ( pulsed )
        digitalWrite ( RLY0, LOW );
        RlyOn = false ;
 
      
        File dataFile = SD.open("datalog.txt", FILE_WRITE);  
        if (dataFile) { 
          dataFile.println();
          dataFile.println("Logging Stopped...");
          dataFile.println();
          dataFile.close();
        }
      }
    }
    
//////////////////////////////////////////////////////////////////////////////////
//
//  This routine checks for the top of a ten minute period to enable logging
//  only at the top of the period.  The Green LED blinks while waiting.
//
//////////////////////////////////////////////////////////////////////////////////
      
    if ( Control == 1 ) {
      if ((int(now.minute()/10)*10) != now.minute() ){
      
        LoopCntr = LoopCntr + 1 ;           // LoopCntr used to control
        if  ( LoopCntr < 10 ) {             // blinking Green LED...
          digitalWrite ( LedA, HIGH );      // Turn Green LED On
        }
        else {
          digitalWrite ( LedA, LOW );       // Turn Green LED Off
        }
        if ( LoopCntr > 15 ) {              // Sets the Max Blink Speed
          LoopCntr = 0;
        }
        delay ( 50 );
      }
      else {
        Control = 2;
        digitalWrite ( LedA, HIGH );        // turn Green LED on
        dataString = "NEW";                 // trigger newline
             
        LastMin = now.minute();    
      
      }
    }
    
///////////////////////////////////////////////////////////////////////////////////
//
//       Trap processing if top of 10 minute period not reached !!
//
///////////////////////////////////////////////////////////////////////////////////

    if ( Control < 2 ){
      return;
    }
        
//////////////////////////////////////////////////////////////////////////////////
//
//          This routine sets up a new dataString line if needed
//
//////////////////////////////////////////////////////////////////////////////////

      if ( dataString == "NEW" ) {
        dataString = "";
        if ( now.hour() < 10 ) {
          dataString += '0';
        }
        dataString += now.hour();
        dataString += ':';
        if ( now.minute() < 10 ) {
          dataString += '0';
        }
        dataString += now.minute();
        dataString += ' ';
        
        LastMin = now.minute();    
      
      }     
   
////////////////////////////////////////////////////////////////////////////////
//
//                  Heart of program - process bat pulses !!
//
////////////////////////////////////////////////////////////////////////////////
    
    BatPulse = pulseIn( Bats, HIGH ); // Get a bat pulse
    
    Serial.print ( "Pulse: ");                // test line
    Serial.println ( BatPulse );              // test line
    
    if ( BatPulse < 1000 ) {                  // Must be > 16kHz
      if ( BatPulse > 200 ) {                 // Must be < 80kHz
      
        Freq = ( 16000 / BatPulse );          // Calculate Frequency
        
        AvFrq = (( Samples * AvFrq ) + Freq ) / ( Samples + 1 );

        Samples = Samples + 1;                // hold samples averaged
        if ( Samples > 9 ) {                  // to 10
          Samples = 9;
        }
        
        Events = Events + 2 ;                 // Bump up events
        
        if ( Events > 10 ) {                  // Keep Events at
          Events = 10;                        // 10 MAX
          digitalWrite ( LedB, HIGH );        // Turn on Yellow LED
          
          if ( ! RlyOn ) {
            digitalWrite ( RLY1, HIGH );      // Turn Bat Call Relay
            delay ( 250 );                    // on ( pulsed )
            digitalWrite ( RLY1, LOW );
            RlyOn = true;
          }
          CallRly = 2;                        // set timeout - 2 seconds
          Activity = true;                    // Set Activity flag
        }
      }
    }

    if ( BatPulse > 20000 ) {                // TBC lower limit
      if ( BatPulse < 250000 ) {             // TBC upper limit
        if ( Events > 8 ) {                  // qualify TBC ( was 6 )
          if ( BatPulse > MaxTBC ) {         // Test for new
             MaxTBC = BatPulse;              // MaxTBC
          }
        }
      }
    }
    
    Events = Events - 1 ;                     // erode event counter
    if ( Events < 1 ) {                       // But not below zero
      Events = 0;                             // keep at zero
    }
    
    if ( LastSec != now.second() ) {
      CallRly = CallRly - 1;
      LastSec = now.second() ;
    }

    if ( Events < 9 ) {                       // if few events
      digitalWrite ( LedB, LOW );             // YellowLED off
 
      if ( CallRly < 1 ) {                    // if zero is reached
        CallRly = 0;                          // then turn off the
        if ( RlyOn ){                         // Bat Call Relay
          digitalWrite ( RLY0, HIGH );          
          delay ( 250 );                      // ( pulsed )
          digitalWrite ( RLY0, LOW );
          RlyOn = false ;
        }
      }
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////
//
//    This routine determines if a minute has elapsed, and causes the data line to 
//    be updated.  If it is the last minute of the dataline, then a ten minute
//    summary is appended, and the line is printed to the datafile.
//
////////////////////////////////////////////////////////////////////////////////////////////////

    if ( now.minute() != LastMin ) {          // new minute
      if ( Activity ) {                       // If there was activity
        dataString += '+';                    // append a * 
        AIline = AIline + 1 ;                 // increment the line AI
        AIhour = AIhour + 1 ;                 // increment the hourly AI
      }
      else {                                  // if no activity
        dataString += '-';                    // append a -
      }
      
      Activity = false;                       // reset the Activity flag
      
      
////////////////////////////////////////////////////////////////////////////////////////////
//
//          Log Line wrap-up routines - finishes up the 10 minute data line
//
////////////////////////////////////////////////////////////////////////////////////////////
      
      if( ( int( now.minute() / 10 ) * 10 ) == now.minute() ) { 
        AIline = AIline * 10;
        dataString += "  AI:";

        if ( AIline < 100 ) {
          dataString += ' ';
        }

        if ( AIline < 10 ) {
          dataString += ' ';
        }
        
        dataString += AIline;
        dataString += "% - Avg.Freq: ";

        if ( AvFrq < 10 ){
          dataString += ' ';
        }

        dataString += AvFrq;
        dataString += " kHz - MaxTBC: ";
        MaxTBC = MaxTBC / 1000 ;                 // convert to mSecs
        if ( MaxTBC < 100 ) {
          dataString += ' ';
        }
        if ( MaxTBC < 10 ) {
          dataString += ' ';
        }
        dataString += MaxTBC;         
        dataString += " mSecs";
      
        File dataFile = SD.open("datalog.txt", FILE_WRITE);  
        if (dataFile) {     
          dataFile.println(dataString);
          dataFile.close();            
        }
        
        dataString = "NEW";
        Events = 0;
        MaxTBC = 0;
        AIline = 0;
        AIdiv = AIdiv + 1;
        Freq = 0;
        AvFrq = 0;
        Samples = 0;
      }

////////////////////////////////////////////////////////////////////////////////
//
//          End of hour summary routine
//
////////////////////////////////////////////////////////////////////////////////
    
      if ( now.hour() != LastHour ){        // process end of hour info
        AIhour = (( AIhour * 10 ) / AIdiv );
        File dataFile = SD.open("datalog.txt", FILE_WRITE);  
        if (dataFile) {     
          dataFile.println();
          dataFile.print("Last hour's AI: ");
          dataFile.print( AIhour );
          dataFile.println('%');
          dataFile.println();
          
          //////////////////////////////////////////////////////////////////
          //
          //         Read all analog sensors hourly...
          //
          //////////////////////////////////////////////////////////////////

          delay(1000);                               // stabilize ??

          sensorValue = analogRead(A3);              
          anaLog = (( sensorValue * 100 )/1023 );    // Light Level   
          dataFile.print("Light Sensor: ");
          dataFile.print( anaLog );
          dataFile.println("% ");

          delay(1000);                               // stabilize ??
          
          sensorValue = analogRead(A1);              
          anaLog = (( sensorValue * 500 )/1023 );    // Temperature   
          dataFile.print("Temperature: ");
          dataFile.print( anaLog );
          dataFile.println("°F");

          delay(1000);                               // stabilize ??

          sensorValue = analogRead(A0);
          anaLog = ( ( sensorValue * 50) / 1023 );   // Battery   
          dataFile.print("Battery: ");
          dataFile.print( anaLog );
          dataFile.println(" Volts");
          
          if ( anaLog < 10 ) {
            dataFile.println();
            dataFile.println();
            dataFile.println("Battery discharged ... shutting down... ");
            dataFile.close();
            
            digitalWrite ( LedC, LOW );           // Turn off Red LED
            digitalWrite ( LedA, LOW );           // Turn off Green LED
            digitalWrite ( LedB, LOW );           // Turn off Yellow LED
            
            digitalWrite ( RLY0, HIGH );          // Turn relay off    
            delay ( 250 );                        // ( pulsed )
            digitalWrite ( RLY0, LOW );
            

            while ( -1 ){                         // Go into an infinite
              delay ( 5000 );                     // sleep loop
            }
          }
              
          
          dataFile.println();
          dataFile.close();
          AIhour = 0;
          AIdiv = 0;
        } 
      }
    }
    
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   This routine writes a new date header as needed
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

    if ( LastDay != now.day() ) {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);  
      if (dataFile) {       
        if ( now.month() < 10 ) {
          dataFile.print ('0');
        }
        dataFile.print(now.month(), DEC);
        dataFile.print('/');
        
        if ( now.day() < 10 ) {
          dataFile.print ('0');
        }

        dataFile.print(now.day(), DEC);        
        dataFile.print('/');        
        dataFile.print(now.year(), DEC);
        dataFile.println();
        dataFile.println();
        dataFile.close();
      }
    }
             
      LastDay = now.day();
      LastHour = now.hour();                
      LastMin = now.minute();    
      
} 
