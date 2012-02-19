
/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * Arduino general purpose I/O.
 * Make the Arduino read or write to its output pins under USB-serial control,
 * so any PC with a USB serial connection can use the Arduino as a
 * general-purpose I/O device.
 *
 * On Linux, connect to it with something like: screen /dev/ttyUSB0 9600
 *
 * Copyright 2010, 2011 by Akkana Peck.
 * Share and enjoy under the terms of the GPL v.2 or later.
 */

// include the library code:
//#include <SoftwareSerial.h>

/* Pin modes */
#define NO_MODE       0
#define INPUT_MODE    1
#define OUTPUT_MODE   2

/* Error codes */
#define NOTANUM        -9999

/* Arrays to tell us which pins are in use */
int digitalPins[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#define NUM_DIGITAL_PINS ((sizeof digitalPins) / (sizeof *digitalPins))

int analogPins[6] = { 0, 0, 0, 0, 0, 0 };
#define NUM_ANALOG_PINS ((sizeof analogPins) / (sizeof *analogPins))

/* Timeout in milliseconds */
int timeout = 1000;

void setup()
{
    Serial.begin(9600);

    Serial.println("genio: Arduino General-Purpose I/O");
}

/* Eventually we might want to buffer line reads here,
 * so we can do lookahead and detect CRLF.
 */
#define BUFSIZ 128
char* readline()
{
    static char line[BUFSIZ];
    char* lp = line - 1;
    int nchars = Serial.available();
    while (nchars-- > 0 && lp < line + sizeof line - 1) {
        *(++lp) = Serial.read();
        if (*lp == '\n' || *lp == '\r') {
            break;
        }
    }
    *lp = '\0';
    return line;
}

/* That's right, the Serial library has no functions for reading
 * lines or integers.
 * Return NOTANUM on errors.
 */
int read_int(char* line)
{
    int num = 0;
    int sign = 1;

    /* Skip over non-numerics */
    while (*line && (*line < '0' || *line > '9') && (*line != '-')) {
        ++line;
    }
    if (! *line)
        return NOTANUM;

    if (*line == '-') {
        sign = -1;
        ++line;
    }
    else if (*line == '-')
        ++line;

    if (! *line || *line < '0' || *line > '9')
        return NOTANUM;

    while (*line >= '0' && *line <= '9') {
        num = num * 10 + *(line++) - '0';
    }

    if (sign < 0)
        num = -num;
    return num;
}

void printres(const char* const cmd, int pin_no, const char* const arg)
{
    Serial.print("OK ");
    Serial.print(cmd);
    Serial.print(pin_no);
    Serial.print(" ");
    Serial.println(arg);
}

/*
  Protocol:
  Commands have the following structure:
  D1 N - set digital pin N ON
  D0 N - set digital pin N OFF
  DI N - monitor digital pin N for input
  DO N - set digital pin N to output mode
  AI N - monitor analog pin N for input
  D- N - ignore digital pin N
  A- N - ignore digital pin N
  T N  - set timeout to N milliseconds (default 1000)
  Commands must end with a newline.

  Responses:
  Commands with no output will respond OK or ERROR
    (with optional additional information after that).
  Read commands will return 1 or 0 (read digital)
    or a floating point number (read analog)
    or ERROR.
 */
void readcmd()
{
    char* line = readline();
    char ch = line[0];
    char cmd = line[1];

    if (ch == 'D' || ch == 'd') {
        int i = read_int(line+2);
        if (i > NUM_DIGITAL_PINS) {
            Serial.print("ERROR No digital pin #");
            Serial.println(i);
            return;
        }
        switch (cmd) {
        case '1':
            pinMode(i, OUTPUT);
            digitalPins[i] = OUTPUT_MODE;
            digitalWrite(i, HIGH);
            printres("set DIGITAL pin", i, "ON");
            break;
        case '0':
            pinMode(i, OUTPUT);
            digitalPins[i] = OUTPUT_MODE;
            digitalWrite(i, LOW);
            printres("set digital pin", i, "OFF");
            break;
        case 'I': case 'i':
            digitalPins[i] = INPUT_MODE;
            printres("digital pin", i, "INPUT mode");
            break;
        case 'O': case 'o':
            digitalPins[i] = OUTPUT_MODE;
            printres("digital pin", i, "OUTPUT mode");
            break;
        case '-':
            digitalPins[i] = NO_MODE;
            printres("digital pin", i, "IGNORED");
            break;
        default:
            Serial.print("ERROR Unknown command ");
            Serial.print(ch);
            Serial.print(" ");
            Serial.println(cmd);
            break;
        }
    }
    else if (ch == 'A' || ch == 'a') {
        int i = read_int(line+2);
        if (i > NUM_ANALOG_PINS) {
            Serial.print("ERROR No analog pin #");
            Serial.println(i);
            return;
        }
        switch (cmd) {
        case 'I': case 'i':
            analogPins[i] = INPUT_MODE;
            printres("analog pin", i, "INPUT mode");
            break;
        case '-':
            analogPins[i] = NO_MODE;
            printres("analog pin", i, "IGNORED");
            break;
        default:
            Serial.print("ERROR Unknown command ");
            Serial.print(ch);
            Serial.print(" ");
            Serial.println(cmd);
            break;
        }
    } else if (ch == 'T' || ch == 't') {
        int t = read_int(line+1);
        if (t > 0) {
            timeout = t;
            Serial.print("OK timeout = ");
            Serial.println(timeout);
        } else {
            Serial.print("ERROR bad timeout ");
            Serial.println(line);
        }
    }

    Serial.flush();
}

void loop()
{
    int i, byte;

    if (Serial.available() > 0) {
        readcmd();
    }

    /* Print values of all input pins */
    for (i=0; i < NUM_DIGITAL_PINS; ++i)
        if (digitalPins[i] == INPUT_MODE) {
            int val = digitalRead(i);
            Serial.print("D");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(val);
        }
    for (i=0; i < NUM_ANALOG_PINS; ++i)
        if (analogPins[i] == INPUT_MODE) {
            int val = analogRead(i);
            Serial.print("A ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(val);
        }

    /* For real use we might not want to delay, but for testing we do */
    delay(timeout);
}

