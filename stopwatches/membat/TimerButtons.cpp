#include "membat.h"

#include <Arduino.h>

TimerButtons::TimerButtons()
{
}

void TimerButtons::debounce()
{
    long nonetime;
    while (true) {
        if (read_buttons() != btnNONE) {
            nonetime = millis();
            continue;
        }
        // nonetime is the last time we saw something other than btnNONE.
        if (millis() - nonetime > 50)
            return;
    }
}
