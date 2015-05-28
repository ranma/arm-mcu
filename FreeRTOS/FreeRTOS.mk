# FreeRTOS make definitions

# $Id$

# Copyright (C)2013-2015, Philip Munts, President, Munts AM Corp.
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

FREERTOS_SRC	?= $(FREERTOS_DIR)/FreeRTOSV8.2.1/FreeRTOS/Source

ifeq ($(MCUFAMILY), lpc17xx)
FREERTOS_PORT	= $(FREERTOS_SRC)/portable/GCC/ARM_CM3
endif 

ifeq ($(MCUFAMILY), stm32f1)
FREERTOS_PORT	= $(FREERTOS_SRC)/portable/GCC/ARM_CM3
endif 

ifeq ($(MCUFAMILY), stm32f4)
FREERTOS_PORT	= $(FREERTOS_SRC)/portable/GCC/ARM_CM4F
endif 

CFLAGS		+= -DFREERTOS
CFLAGS		+= -I$(FREERTOS_DIR)
CFLAGS		+= -I$(FREERTOS_SRC)/include
CFLAGS		+= -I$(FREERTOS_PORT)

RMAKEFLAGS	+= WITH_FREERTOS=$(WITH_FREERTOS)

.PHONY: freertos_lib freertos_clean

# These are the FreeRTOS object files

FREERTOS_OBJS	= $(FREERTOS_PORT)/port.o			\
		  $(FREERTOS_SRC)/croutine.o			\
		  $(FREERTOS_SRC)/event_groups.o		\
		  $(FREERTOS_SRC)/list.o			\
		  $(FREERTOS_SRC)/queue.o			\
		  $(FREERTOS_SRC)/tasks.o			\
		  $(FREERTOS_SRC)/timers.o			\
		  $(FREERTOS_SRC)/portable/MemMang/heap_3.o

# Add FreeRTOS object files to the MCU library

freertos_lib: $(FREERTOS_OBJS)
	$(AR) crs lib$(MCU).a $(FREERTOS_OBJS)

# Remove FreeRTOS object files

freertos_clean:
	rm -f $(FREERTOS_OBJS)

# Add to target lists

LIBTARGETS	+= freertos_lib
CLEANTARGETS	+= freertos_clean
