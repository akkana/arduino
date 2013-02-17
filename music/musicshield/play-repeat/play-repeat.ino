#include <avr/io.h>
#include "config.h"
#include "filesys.h"
#include "vs10xx.h"
#include "record.h"
#include "player.h"

// the music library doesn't bother to export these:
extern void PlayCurrentFile();
extern void ControlLed();
extern unsigned char g_volume;
extern int redPwm;             // brightness of red led
extern unsigned int greenFreq; // flash frequency of green led

void PlayIt() {
    // Start the current file playing, maybe a repeat.
    Serial.print("Calling PlayCurrentFile ");
    Serial.println(currentFile);
    PlayCurrentFile();
    Serial.println("Returned from PlayCurrentFile()");
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Test Player");
  
    InitSPI();
    InitIOForVs10xx();
    InitIOForKeys();
    InitIOForLEDs();
    InitFileSystem();

    //VsSineTest();

    Mp3Reset();

    currentFile = 1;
    while (1) {
        Mp3SoftReset();
        Serial.print("setup loop: currentFile = ");
        Serial.println(currentFile);
        while (OpenFile(currentFile)) {
            Serial.print("Oh no! Couldn't open file ");
            Serial.println(currentFile);
            ++currentFile;
        }

        Serial.println("Playing it!");
        PlayIt();
    }
}

// When loop is called, the file should already have been opened.
// But actually, loop is never called since playing consumes the CPU forever.
// If it does get called, we're confused and not doing anything useful,
// so delay a bit.
void loop() {
    Serial.print("Loop");
    delay(500);
}

// This almost works! Need to do button debouncing -- pressing the button
// skips forward many tracks, not just one -- and add the volume buttons
// back in.
void AvailableProcessorTime()
{
    if (0 == PSKey)    // main button was pressed -- switch to the next song.
    {
        Serial.println("\nButton pressed!");

        // Try to signal the player that we want to stop
        playingState = PS_END_OF_SONG;
        Serial.print("Incrementing currentFile to ");
        Serial.println(currentFile);
        ++currentFile;
        delay(500);       // button debounce, don't advance more than 1 song
        // I hate to do a debounce delay in AvailableProcessorTime(),
        // but it doesn't work if we wait until we get back to setup() --
        // we'll get several more button presses first.
    }

    // Volume controls. Mp3SetVolume(leftchannel, rightchannel)
    // apparently takes values between 0 and 0xff.
    // Code apparently increases g_volume for vd, decreases for vu.
    // Go figure!
    else if (0 == VUKey)    // volume up
    {
        Serial.println("VUKey");
        static unsigned int vu_cnt = 1000;  //volume up interval
  	if (--vu_cnt == 0)
	{
            if (g_volume-- == 0)
                g_volume = 0; //Change + limit to 0 (maximum volume)
    
            Mp3SetVolume(g_volume, g_volume);

#if 0
            // Original code set the redPwm here,
            // but I can't see a visible difference

            redPwm = (175-g_volume)*3>>1;
            if (redPwm >255)
            {
                redPwm = 255;
            }
            if (redPwm < 0)
            {
                redPwm = 0;
            }
            //Serial.println(redPwm,DEC);
#endif
            vu_cnt = 1000;
            // debounce
            delay(200);
        }
    }
    else if (0 == VDKey)
    {
        Serial.println("VDKey");
        static unsigned int vd_cnt = 1000;  // volume down interval
        if (--vd_cnt == 0)
	{
            if (g_volume++ == 254)
                g_volume = 254; //Change + limit to 254 (minimum vol)
	
            Mp3SetVolume(g_volume,g_volume);

            redPwm = 305-(g_volume<<1);
            if (redPwm >255)
            {
                redPwm = 255;
            }
            if (redPwm < 0)
            {
                redPwm = 0;
            }
            //Serial.println(redPwm,DEC);
            vd_cnt = 1000;
        }
    }

    // Do mysterious things related to the LEDs.
    ControlLed();
}


