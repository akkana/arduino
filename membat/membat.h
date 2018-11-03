/*
 * Global definitions for membat.
 */

#ifndef membat_h
#define membat_h

#define btnNONE      0
#define btnSTARTSTOP 1
#define btnSELECT    2
#define btnCLEAR     3
#define btnUP        4
#define btnDOWN      5

// Which button to use for which function:
// Let btnSELECT select models, btnUP and btnDOWN adjust alarm time.

// The important routine: check whether a button is pressed and
// return the code for it.
extern unsigned int read_buttons();

#endif /* membat.h */

