# Makefile for NUCLEO-F411RE Console Test

# $Id$

# Copyright (C)2015, Philip Munts, President, Munts AM Corp.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

GNU_ARM_EMBEDDED_DIR ?= /usr/local/gcc-arm-none-eabi-4_9-2015q1

# Override the following macros to build out of tree

ARMSRC          ?= $(HOME)/src/ARM
MBEDSRC  	?= $(HOME)/src/ARM/mbed

MCUFAMILY       = stm32f4
BOARDNAME       = NUCLEO_F411RE
PROJECTNAME	= test_console

# Compile the program

build: mbed
	$(MAKE) -f main.mk all GCC_BIN="$(GNU_ARM_EMBEDDED_DIR)/bin/" PROJECT="$(PROJECTNAME)"

# Flash the program

install: build
	$(MAKE) $(PROJECTNAME).flashstlink

# Debug the program

debug: build
	$(MAKE) $(PROJECTNAME).debugstlink

# Link to mbed SDK checkout

mbed:
	cd $(MBEDSRC) ; ./GetSDK.sh
ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
	cmd.exe /C mklink /J mbed `cygpath -a -w $(MBEDSRC)/mbed`
else
	ln -s $(MBEDSRC)/mbed
endif

# Remove working files

clean:
	$(MAKE) -f main.mk clean GCC_BIN=$(GNU_ARM_DIR)/bin/
	rm -f mbed *.bin *.d *.elf *.hex *.log *.map *.o

# Include subordinate makefiles

include $(ARMSRC)/include/ARM.mk
