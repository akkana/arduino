# Arduino Makefile. Refer to https://github.com/sudar/Arduino-Makefile

# Arduino core from https://github.com/damellis/attiny/

# This works, but delay times are about a factor of 14 off.
# A delay(100) actually lasts about 1.4 seconds.

ALTERNATE_CORE = attiny-damellis
BOARD_TAG = ATtinyX5.menu.cpu.attiny85
BOARD_SUB = attiny85
F_CPU = 1000000L
# If you make it 16000000L, delays will run 16x too long,
# e.g. delay(100) will take 1.6 seconds.
ISP_PROG = usbtiny

include /usr/share/arduino/Arduino.mk

