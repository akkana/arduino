// ArduBat Test / Demo Program -- ArduBat Shield and Arduino UNO

const int Bats = 2;       // Bat Pulses - Pin 2
const int LedA = 5;       // Green LED - Pin 5
const int LedB = 6;       // Yellow LED - Pin 6
const int LedC = 7;       // Red LED - Pin 7
const int PBF1 = 8;       // F1 PButton - Pin 8
const int PBF2 = 9;       // F2 PButton - Pin 9

unsigned long BatPulse = 0;        // holds the value of PulseIn

int Freq = 0;                      // calculated frequency

void setup () {
  
  pinMode ( Bats, INPUT );         // Div 32 pulse input
  
  pinMode ( LedA, OUTPUT );
  pinMode ( LedB, OUTPUT );
  pinMode ( LedC, OUTPUT );
  pinMode ( PBF1, INPUT );
  pinMode ( PBF2, INPUT );
   
  digitalWrite ( LedA, LOW );  // initialize the
  digitalWrite ( LedB, LOW );  // LEDS
  digitalWrite ( LedC, LOW );  // All are off
  
  Serial.begin(57600);

}

void loop () {

    if ( ( digitalRead ( PBF2 ) == LOW )) {   // Test Button / Red LED
        digitalWrite ( LedC, HIGH );          // Turn on Red LED
        digitalWrite ( LedA, LOW );           // Turn off Green LED
    }

    if ( ( digitalRead ( PBF1 ) == LOW )) {   // Test Button / Green LED
      digitalWrite ( LedA, HIGH );            // Turn on Green
      digitalWrite ( LedC, LOW );             // Turn off Red LED
    }

   ////////////////////////////////////////////////////////////////////////////////
   //
   //                  Heart of program - process bat pulses !!
   //
   ////////////////////////////////////////////////////////////////////////////////
            
    
    BatPulse = pulseIn( Bats, HIGH );         // Get a bat pulse
    
    Serial.print ( "Pulse: ");                // Show the Raw pulse width
    Serial.print  ( BatPulse );               
    Serial.print (" uSeconds " );
    
    if ( BatPulse < 1000 ) {                  // Must be > 16kHz
      if ( BatPulse > 200 ) {                 // Must be < 64kHz

        Freq = ( 16000 / BatPulse );          // Calculate Frequency
        Serial.print ( " : Frequency is " );  // Show frequency
        Serial.print ( Freq );
        Serial.print ( " kHz " );

        digitalWrite ( LedB, HIGH );          // Turn on Yellow LED

        }
      }
      
    delay(500);                               // short delay
    digitalWrite ( LedB, LOW );               // Turn Yellow LED off
 
    Serial.println();                         // finish line
} 
