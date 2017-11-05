# Arduino Makefile. Refer to https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = /usr/local/share/arduino

#-------------- arduino-tiny test:
ALTERNATE_CORE = tiny
BOARD_TAG = attiny85at8

#-------------- attinycore test:
# ALTERNATE_CORE = ATTinyCore
# BOARD_TAG = attinyx5
# BOARD_SUB = 85

#-------------- damellis/hlt test (doesn't work):
# ALTERNATE_CORE = attiny
# BOARD_TAG = attiny
# BOARD_SUB = attiny85

# https://github.com/rexxar-tc/arduino-tiny.core2 (doesn't work):
# ALTERNATE_CORE    = arduino-tiny.core2
# BOARD_TAG         = attiny85at8

F_CPU = 1000000L
ISP_PROG = usbtiny

include /usr/local/share/Arduino-Makefile/Arduino.mk

