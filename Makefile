# Makefile purely for recursively cleaning out this directory.

DIRS := . $(shell find * -type d)

GARBAGE_PATTERNS := build-* applet *.o *~ core

GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

clean:
	rm -rf $(GARBAGE)

