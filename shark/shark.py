#! /usr/bin/env python

# Send commands to the Air Swimmers shark via an Arduino.

import sys, serial

class Sharkduino() :
    def __init__(self) :
        """Locate the Arduino port and open it."""
        # Port may vary, so look for it:
        baseports = ['/dev/ttyACM', '/dev/ttyUSB']
        self.ser = None
        self.debug = True
        for baseport in baseports :
            for i in xrange(0, 9) :
                try :
                    port = baseport + str(i)
                    if self.debug :
                        print "Trying", port
                    self.ser = serial.Serial(port, 9600)
                    print "Opened", port
                    print "Initialized:", self.ser.readline()
                    return
                except :
                    self.ser = None
                    pass

        # If we get here, we're in trouble and didn't initialize
        print "Couldn't open a serial port"
        sys.exit(1)

    def report(self) :
        self.ser.write("?\n");
        print "Result:", self.ser.readline()

    def send(self, str) :
        """Send any string to the shark"""
        if self.debug :
            print "Sending:", str
        self.ser.write(str + '\n')
        #if self.debug :
        #    print "Reading result"
        #    print "Result:", self.ser.read()

    def pulse(self, direc, millisec) :
        """Pulse U, D, L or R for a fixed number of milliseconds."""
        if debug :
            print "Pulse %s %d msec" % (direc, millisec)
        self.ser.write('%s %d\n' % (direc, millisec))
        if debug :
            print "Result:", self.ser.readline()

    def swim(self, beats) :
        """Swim a set number of full tail-beats."""
        pulse('L', 80)
        pulse('R', 160)
        pulse('L', 80)

    def up(self, amount) :
        pulse('U', amount)

    def down(self, amount) :
        pulse('U', amount)

if __name__ == '__main__':
    shark = Sharkduino()

    # try :
    #     arduino.run()
    # except serial.SerialException :
    #     print "Disconnected (Serial exception)"
    # except IOError :
    #     print "Disconnected (I/O Error)"
    # except KeyboardInterrupt :
    #     print "Interrupt"
