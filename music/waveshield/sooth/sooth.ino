/*
 * Play (probably soothing) sounds found on the SD card, in a loop.
 * Sort the files in alphabetical order, and play starting with the first.
 * (You can give files names like 01_foo.wav to control which plays first.)
 * When a button is pressed, move on to the next sound file.
 *
 * Adapted from the Adafruit Wave shield example, daphc.pde.
 */
#include <WaveHC.h>
#include <WaveUtil.h>

#define ButtonPin 12  // Pressing the button will switch to the next file

SdReader card;        // This object holds the information for the card
FatVolume vol;        // information for the partition on the card
FatReader root;       // information for the volumes root directory
WaveHC wave;          // This is the only wave (audio) object
FatReader file;

dir_t dirBuf;         // buffer for directory reads

char* curPlaying = 0; // the file currently playing

#define DEBUG 1
#ifdef DEBUG
// F(s), and possibly also PSTR(s), tells the compiler to use program
// memory (in flash) rather than copying into RAM.
# define PRINT(s)    Serial.print(F(s))
# define PRINTln(s)  Serial.println(F(s))
// But gcc has a bug that makes it spew warnings for F(s) and PSTR(s).
// http://avr.2057.n7.nabble.com/bug-28837-using-PSTR-with-c-produces-warnings-td14486.html
// Here's how Adafruit does it, but it doesn't help the warnings.
//# define PRINT(s)    SerialPrint_P(PSTR(s))
//# define PRINTln(s)  SerialPrintln_P(PSTR(x))
#else
# define PRINT(s)    
# define PRINTln(s)  
#endif

// forward
extern void playNext();

//////////////////////////////////// SETUP
void setup()
{
#if DEBUG
  Serial.begin(9600);

  PRINT("\n================ Wave test!");
  
  PRINT("Free RAM (k): ");
  Serial.println(FreeRam());
#endif

  if (!card.init()) {                  // play with 8 MHz spi (default faster!)
      PRINTln("Card init. failed!");
  }
  
  // Enable optimize read - some cards may time out.
  // Disable if you're having problems
  card.partialBlockRead(true);
  
  // Look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, lets bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
#if DEBUG
    PRINTln("No valid FAT partition!");   // Something went wrong
#endif
  }

#if DEBUG
  // Lets tell the user about what we found
  PRINT("Using partition ");
  Serial.print(part, DEC);
  PRINT(", type is FAT");
  Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
#endif

  // Try to open the root directory
  if (root.openRoot(vol)) {
      playNext();
  }
#if DEBUG
  else {
    PRINTln("Can't open root dir!");      // Something went wrong,
  }

  Serial.print(F("Free RAM (k): "));
  Serial.println(FreeRam());
#endif
}

//////////////////////////////////// LOOP
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void loop() {
    // Check whether the button has been pressed.
    if (digitalRead(ButtonPin)) {
        sdErrorCheck();                   // Everything still okay?
        playNext();
    }

    else if (wave.isplaying) {
        //Serial.print(curPlaying);
        //PRINTln("is still playing");
        PRINT(".");
    }
    else {
        PRINTln("\nDone -- replaying!");
        wave.play();
#if DEBUG
        PRINT("Free RAM (k): ");
        Serial.println(FreeRam());
#endif
    }

    delay(300);
}

/////////////////////////////////// HELPERS

/* Play the next file on the disk after the given filename.
 * If curfile == 0, play the first file.
 * If curfile is the last file, play the first file.
 */
void playNext()
{
    curPlaying = 0;
    int rewound = false;

    while (1) {
        int rv = root.readDir(dirBuf);
#if DEBUG
        PRINT("next readDir: ");
        Serial.println(rv);
#endif
        if (rv <= 0) {
            if (rewound) {
                // We've already rewound but we still can't read.
                // Something is wrong.
                PRINTln("Can't read SD card directory!");
                return;
            }
            PRINTln("Rewinding back to first file.");
            root.rewind();
            rewound = true;
            continue;
        }

        // The read was successful. But was it a valid WAV file?
        if (DIR_IS_SUBDIR(dirBuf)
            || strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) {
#if DEBUG
            PRINT("Skipping non WAV file ");
            Serial.println((char*)dirBuf.name);
#endif
            continue;
        }

        // We read it AND it's a wav file. Try to play it.
        if (!file.open(vol, dirBuf)) {        // open the file in the directory
            PRINTln("file.open failed");      // something went wrong
            return;
        }

        if (!wave.create(file)) {             // is it a WAV proper?
            PRINTln(" Not a valid WAV");
            return;
        }

        // Okay, we think we can play this.
        wave.play();
        curPlaying = (char*)(dirBuf.name);
#if DEBUG
        PRINT("Currently playing: ");
        Serial.println(curPlaying);
#endif
        return;
    }
}

