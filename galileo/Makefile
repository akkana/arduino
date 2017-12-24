# Makefile For Intel Galileo Gen2 board.

# First install Intel Galileo in the Arduino IDE.
# This will fail (at least on Linux), but it will at least download the files.
# Now edit ~/.arduino15/packages/Intel/tools/i586-poky-linux-uclibc/1.6.2+1.0/i586/install_script.sh
# look for the line beginning "executable_files=$($SUDO_EXEC find"
# and change -perm +111 to -perm /111.
# Then run ~/.arduino15/packages/Intel/tools/i586-poky-linux-uclibc/1.6.2+1.0/i586/install_script.sh
# and hopefully you'll have an installed Galileo environment.

# Where your Arduino IDE is installed:
ARDUINO_DIR=$(HOME)/outsrc/arduino64/arduino-1.8.5

# Where the Intel package ended up after installing it in the IDE:
INTEL_DIR=$(HOME)/.arduino15/packages/Intel

blink.ino.elf: blink.ino
	mkdir -p $(CURDIR)/build
	mkdir -p $(CURDIR)/cache
	$(ARDUINO_DIR)/arduino-builder -dump-prefs -logger=machine -hardware $(ARDUINO_DIR)/hardware -hardware $(HOME)/.arduino15/packages -hardware $(HOME)/sketchbook/hardware -tools $(HOME)/outsrc/arduino64/arduino-1.8.5/tools-builder -tools $(HOME)/outsrc/arduino64/arduino-1.8.5/hardware/tools/avr -tools $(HOME)/.arduino15/packages -built-in-libraries $(ARDUINO_DIR)/libraries -libraries $(HOME)/sketchbook/libraries -fqbn=Intel:i586:izmir_fg -vid-pid=0X8086_0XBABE -ide-version=10805 -build-path $(CURDIR)/build -warnings=all -build-cache $(CURDIR)/cache -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.sketchUploader.path=$(INTEL_DIR)/tools/sketchUploader/1.6.2+1.0 -prefs=runtime.tools.i586-poky-linux-uclibc.path=$(INTEL_DIR)/tools/i586-poky-linux-uclibc/1.6.2+1.0 -verbose blink.ino
	$(ARDUINO_DIR)/arduino-builder -compile -logger=machine -hardware $(ARDUINO_DIR)/hardware -hardware $(HOME)/.arduino15/packages -hardware $(HOME)/sketchbook/hardware -tools $(ARDUINO_DIR)/tools-builder -tools $(ARDUINO_DIR)/hardware/tools/avr -tools $(HOME)/.arduino15/packages -built-in-libraries $(ARDUINO_DIR)/libraries -libraries $(HOME)/sketchbook/libraries -fqbn=Intel:i586:izmir_fg -vid-pid=0X8086_0XBABE -ide-version=10805 -build-path $(CURDIR)/build -warnings=all -build-cache $(CURDIR)/cache -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.sketchUploader.path=$(INTEL_DIR)/tools/sketchUploader/1.6.2+1.0 -prefs=runtime.tools.i586-poky-linux-uclibc.path=$(INTEL_DIR)/tools/i586-poky-linux-uclibc/1.6.2+1.0 -verbose blink.ino
	$(INTEL_DIR)/tools/i586-poky-linux-uclibc/1.6.2+1.0/i586/sysroots/pokysdk/usr/bin/i586-poky-linux-uclibc/i586-poky-linux-uclibc-strip $(CURDIR)/build/blink.ino.elf

upload:
	/bin/bash --verbose --noprofile $(INTEL_DIR)/tools/sketchUploader/1.6.2+1.0/clupload/cluploadGalileo_linux.sh $(INTEL_DIR)/tools/sketchUploader/1.6.2+1.0/x86/bin $(CURDIR)/build/blink.ino.elf /dev/ttyACM0

clean:
	rm -rf build cache

