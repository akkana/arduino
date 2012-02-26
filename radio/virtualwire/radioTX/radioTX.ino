/* -*- c -*- */

// Adapted from
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Wireless/General/RFASK_Example_Code.pde

#include "VirtualWire.h"

void setup()
{
    pinMode(13, OUTPUT);

    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(68);	 // Bits per sec
    //vw_set_tx_pin(3); 
}

void loop()
{
    const char* msg = "Hello, world!";
    digitalWrite(13, true); // light the LED to show we're sending
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();           // Wait until the whole message is gone
    // killer11 on #arduino said to wait 10ms after every transmission
    delay(10);
    digitalWrite(13, false);

    // but actually, let's delay longer:
    delay(1000);
}

