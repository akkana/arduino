/* -*- c -*- */

/*
 * Control an Air Swimmers shark from an external computer,
 * by sending commands to an Arduino over USB.
 * Copyright 2011 by Akkana Peck: share and enjoy under the GPL v2 or later.
 *
 * Protocol for controlling the shark:
 *
 * C nnnn
 * where C is a letter for the direction (U, D, L, R)
 * and nnnn is an integer indicating how many milliseconds
 * to hold that direction before releasing it.
 * (hint: 80 is a reasonable short pulse, e.g. for the tail;
 * 300 is a longer pulse suitable for controlling attitude.)
 *
 */

#define RIGHT_PIN 2
#define LEFT_PIN  3
#define DOWN_PIN  4
#define UP_PIN    5

#define NOTANUM   -9999

#define SERIAL 1
#undef DEBUG

void setup()
{
    pinMode(LEFT_PIN,  OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);
    pinMode(UP_PIN,    OUTPUT);
    pinMode(DOWN_PIN,  OUTPUT);

#ifdef SERIAL
    Serial.begin(9600);
#endif

    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
    digitalWrite(UP_PIN, LOW);
    digitalWrite(DOWN_PIN, LOW);

#if defined(SERIAL)
    Serial.println("Sharkduino initialized");
#endif
}

void pulse(int pin, int interval)
{
    digitalWrite(pin, HIGH);
    delay(interval);
    digitalWrite(pin, LOW);
#if defined(SERIAL) && defined(DEBUG)
    Serial.print("OK ");
    Serial.print(pin);
    Serial.print(" ");
    Serial.println(interval);
#endif
}

#ifdef SERIAL
char* readline()
{
#define BUFSIZ 128
    static char line[BUFSIZ];
    char* lp = line - 1;

    while (1) {
        int nchars = Serial.available();
        if (nchars <= 0) {
            delay(10);
            continue;
        }
        /* Now we know there are some chars available */
        *(++lp) = Serial.read();
        if (*lp == '\n' || *lp == '\r' || lp >= line + sizeof line - 1) {
            *lp = '\0';
            /*
#ifdef SERIAL
            int num = int(lp - line);
            Serial.print(num);
            Serial.print(" chars read; ");
#ifdef SERIAL
            */
            return line;
        }
    }
    /* notreached */
    *lp = '\0';
    return line;
}

/* That's right, the Serial library has no functions for reading
 * lines or integers.
 * Return NOTANUM on errors.
 */
int read_int(const char* line)
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

void print_error(const char* e, const char* line)
{
    Serial.print("Sharkduino error: ");
    Serial.print(e);
    Serial.print(": '");
    Serial.print(line);
    Serial.println("'");
}
#endif

void loop()
{
#ifdef SERIAL
    char* line = readline();
    char cmd = line[0];
    int delay = read_int(line+1);
    if (delay < 0 && cmd != '?') {
        print_error("Couldn't read delay", line);
        return;
    }
#ifdef DEBUG
    Serial.print("cmd was "); Serial.print(cmd);
#endif

    switch (cmd) {
      case 'U': case 'u':
        pulse(UP_PIN, delay);
        return;
      case 'D': case 'd':
        pulse(DOWN_PIN, delay);
        return;
      case 'L': case 'l':
        pulse(LEFT_PIN, delay);
        return;
      case 'R': case 'r':
        pulse(RIGHT_PIN, delay);
        return;
      case '?':
        Serial.println("Sharkduino, awaiting commands!");
        return;
    }
    print_error("Couldn't parse", line);
#else
    pulse(DOWN_PIN, 50);
    delay(1000);
    pulse(UP_PIN, 50);
    delay(1000);
#endif
}

