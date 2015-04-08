# Makefile definitions for building GNAT Ada application programs for the
# STM32F4 microcontroller family

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

# Don't delete intermediate files

.SECONDARY:

# Toolchain definitions

GNATARCH		= arm-eabi
GNATRELEASE		= 2014
GNATDIR			?= /usr/local/gnat-gpl-$(GNATRELEASE)-$(GNATARCH)
GNATBIN			= $(GNATDIR)/bin
GNATRTS			= $(GNATDIR)/lib/gnat/$(GNATARCH)/ravenscar-sfp-stm32f4
GNATMAKE		= $(GNATBIN)/$(GNATARCH)-gnatmake
GNATMAKEFLAGS		= -p --RTS=$(GNATRTS) -P
GPRBUILD		= env PATH=$(GNATBIN) gprbuild
GPRBUILDFLAGS		= -p --RTS=$(GNATRTS) --target=$(GNATARCH)
OBJCOPY			= $(GNATBIN)/$(GNATARCH)-objcopy

GNATPP			= gnatpp
GNATPPFLAGS		= -i2 -aU -kU -rnb $(ADA_INCLUDES)

# Define a pattern rule for building an Ada program from a project file

%.elf: %.gpr
	$(GPRBUILD) $(GPRBUILDFLAGS) $<

# Convert ELF to binary

%.bin: %.elf
	$(OBJCOPY) -S -O binary --gap-fill=0 $< $@

# Convert ELF to Intel hex

%.hex: %.elf
	$(OBJCOPY) -S -O ihex --change-addresses=$(FLASHWRITEADDR) --gap-fill=0 $< $@

# Define pattern rules for formatting Ada source files

%.adb.prettyprint: %.adb
	$(GNATPP) $(GNATPPFLAGS) $^ ; sed -i -r '/(^ *[-][-])|(^ *\()|( IS \()/! s/ \(/(/g' $^

%.ads.prettyprint: %.ads
	$(GNATPP) $(GNATPPFLAGS) $^ ; sed -i -r '/(^ *[-][-])|(^ *\()|( IS \()/! s/ \(/(/g' $^

# Default make target

ada_mk_default:
	@echo ERROR: You must explicitly specify a make target!
	@exit 1

# Clean out working files

ada_mk_clean:
	-rm -rf *.bin *.elf *.hex *.log *.stackdump obj
