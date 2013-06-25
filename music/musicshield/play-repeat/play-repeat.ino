#include "debug.h"

#include <avr/io.h>
#include "config.h"
#include "filesys.h"
#include "vs10xx.h"
#include "record.h"
#include "player.h"

#define DEBUG 1

void setup()
{
    Serial.begin(9600);
#ifdef DEBUG
    PRINTln("Test Player");
#endif
  
    InitSPI();
    InitIOForVs10xx();
    InitIOForKeys();
    InitIOForLEDs();
    InitFileSystem();

    // issues:
    // tracks 4 and 5 are too soft, so are 8 and 9, 11, 12, 13, 15
    // at 16, loses place, doesn't wrap around. But another push
    // and it works.

    //VsSineTest();

    Mp3Reset();
    analogWrite(9, 10);    // make the red LED (pin 9) really dim
    GREEN_LED_OFF();       // no brightness adjustment on the green LED

    currentFile = 1;
    int lastFile = -1;
    while (1) {
        int rv;

        // Are we playing a new file? Then we need to reset the MP3,
        // otherwise it'll get confused and stop playing anything.
        // It would be nice to do that in CheckButtons where the
        // file is actually changed, but for some reason Mp3SoftReset()
        // doesn't work from there, only from here.
        // All I can guess is that this must have something to do with
        // things the player code is doing after passing control back from
        // AvailableProcessorTime().
        if (currentFile != lastFile)
            Mp3SoftReset();
        lastFile = currentFile;

#ifdef DEBUG
        PRINT("setup loop: currentFile = ");
        Serial.println(currentFile);
#endif
        while ((rv = OpenFile(currentFile))) {
            if (rv == 0x0c) // "File not found" which actually means end of dir
            {
#ifdef DEBUG
                PRINT("End of directory -- cycling back from ");
                Serial.println(currentFile);
                PRINTln("=========================");
#endif
                if (currentFile == 0) {
#ifdef DEBUG
                    PRINTln("Can't read any files -- bailing");
#endif
                    delay(500000);
                }
                // I'm not sure if this is actually needed:
                //unsigned int initFS = (unsigned int)(InitFileSystem());
#ifdef DEBUG_NOT
                PRINT("================\nInitFileSystem() returned ");
                Serial.println(initFS, HEX);
#endif
                currentFile = 1;
                continue;
            }

#ifdef DEBUG
            PRINT("Oh no! Couldn't open file ");
            Serial.println(currentFile);
#endif
            ++currentFile;
        }

#ifdef DEBUG
        PRINT("Setting volume to ");
        Serial.println(g_volume);
        Mp3SetVolume(g_volume, g_volume);
        PRINT("Playing file ");
        Serial.println(currentFile);
#endif
        PlayCurrentFile();
    }
}

// The normal Arduino loop() is never called
// since playing consumes the CPU forever.
// If it does get called, we're confused and not doing anything useful,
// so delay a bit.
void loop() {
#ifdef DEBUG
    PRINT("Loop");
#endif
    delay(500);
}

void CheckButtons()
{
    static unsigned int lastButtonPress = 0;

    // How long has it been? Don't check buttons if one was pressed recently.
    int curTime = millis();
    if (curTime - lastButtonPress < 200)
        return;

    if (0 == PSKey || 0 == NTKey || 0 == BKKey)
    {
#ifdef DEBUG
        PRINTln("\nButton pressed!");
#endif
        lastButtonPress = curTime;

        // Try to signal the player that we want to stop
        playingState = PS_END_OF_SONG;
#ifdef DEBUG
        PRINT("Incrementing currentFile to ");
        Serial.println(currentFile);
#endif
        if (0 == BKKey)
            --currentFile;
        else
            ++currentFile;
        delay(500);       // button debounce, don't advance more than 1 song
        // I hate to do a debounce delay in AvailableProcessorTime(),
        // but it doesn't work if we wait until we get back to setup() --
        // we'll get several more button presses first.

        // For some reason, Mp3SoftReset() here doesn't work.
        // It only works if it's called from the main loop --
        // right after we return from this function!
        //Serial.println("Soft resetting");
        //Mp3SoftReset();
    }

    // The combo button can also go forward or back.
    // To do something with it, use these:
    else if (0 == NTKey) {
#ifdef DEBUG
        PRINTln("\nNext song button");
#endif
        lastButtonPress = curTime;
    }
    else if (0 == BKKey) {
#ifdef DEBUG
        PRINTln("\nPrevious song button");
#endif
        lastButtonPress = curTime;
    }

    // Volume controls. Mp3SetVolume(leftchannel, rightchannel)
    // apparently takes values between 0 and 0xff.
    // Code apparently increases g_volume for vd, decreases for vu.
    // Go figure!
    // Also: the volume increment here is quite small.
    // But because there's no button debouncing, in practice
    // one button press creates quite a large volume difference.
    // Would probably be good to check millis().
    else if (0 == VUKey || 0 == VDKey)    // volume up or down
    {
        lastButtonPress = curTime;
        if (0 == VUKey && g_volume-- < 0)
            g_volume = 0;     // limit to 0 (maximum volume)
        else if (0 == VDKey && g_volume++ == 254)
            g_volume = 254;   // limit to 254 (minimum vol)

        Mp3SetVolume(g_volume, g_volume);
#ifdef DEBUG
        PRINT("Setting volume to ");
        Serial.println(g_volume);
#endif
    }
}

void AvailableProcessorTime()
{
    // See if any buttons are pressed, and react accordingly
    CheckButtons();

    // Do mysterious and non visually obvious things related to the LEDs.
    //ControlLed();
}


