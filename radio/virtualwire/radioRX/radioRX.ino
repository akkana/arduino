/* -*- c -*- */

// Adapted from
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Wireless/General/RFASK_Example_Code.pde

#include <VirtualWire.h>

void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(9600);	// Debugging only
    Serial.println("Radio receiver");

    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(68);	 // Bits per sec
    //vw_set_rx_pin(3);
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (! vw_get_message(buf, &buflen)) // Non-blocking
        return;

    digitalWrite(13, true); // Flash a light to show received good message
    Serial.println("Got something!");

    // Message with a good checksum received, dump it.
    Serial.print("buflen was ");
    Serial.println(buflen);

    char* str = (char*)buf;
    str[buflen] = '\0';
    Serial.print(str);
    Serial.print(" = ");
	
    for (int i = 0; i < buflen; i++)
        Serial.print(buf[i]);
    Serial.println("-- that's all");
    digitalWrite(13, false);
}

