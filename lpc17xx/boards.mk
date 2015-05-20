# Board dependent make definitions

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

# Board specific macro definitions

ifeq ($(BOARDNAME), MBED_LPC1768)
BOARDFLAGS	+= -DLED0_PIN=GPIOPIN50 -DLED0_INPUT=GPIOPIN50IN -DLED0_OUTPUT=GPIOPIN50OUT
BOARDFLAGS	+= -DLED1_PIN=GPIOPIN52 -DLED1_INPUT=GPIOPIN52IN -DLED1_OUTPUT=GPIOPIN52OUT
BOARDFLAGS	+= -DLED2_PIN=GPIOPIN53 -DLED2_INPUT=GPIOPIN53IN -DLED2_OUTPUT=GPIOPIN53OUT
BOARDFLAGS	+= -DLED3_PIN=GPIOPIN55 -DLED3_INPUT=GPIOPIN55IN -DLED3_OUTPUT=GPIOPIN55OUT
CONSOLEFLAGS	?= -DCONSOLE_SERIAL -DCONSOLE_PORT='"com1:115200,n,8,1"'
endif

ifeq ($(BOARDNAME), BLUEBOARD_LPC1768_H)
BOARDFLAGS	+= -DBUTTON0_PIN=GPIOPIN74 -DBUTTON0_INPUT=GPIOPIN74IN -DBUTTON0_ACTIVELOW
BOARDFLAGS	+= -DLED0_PIN=GPIOPIN61 -DLED0_INPUT=GPIOPIN61IN -DLED0_OUTPUT=GPIOPIN61OUT
CONSOLEFLAGS	?= -DCONSOLE_USB
#CONSOLEFLAGS	?= -DCONSOLE_SERIAL -DCONSOLE_PORT='"com1:115200,n,8,1"'
JLINKMCU	= lpc1768
endif

ifeq ($(BOARDNAME), LPC1768_MINI_DK2)
BOARDFLAGS	+= -DBUTTON0_PIN=GPIOPIN75 -DBUTTON0_INPUT=GPIOPIN75IN -DBUTTON0_ACTIVELOW
BOARDFLAGS	+= -DBUTTON1_PIN=GPIOPIN76 -DBUTTON1_INPUT=GPIOPIN76IN -DBUTTON1_ACTIVELOW
BOARDFLAGS	+= -DLED0_PIN=GPIOPIN121 -DLED0_INPUT=GPIOPIN121IN -DLED0_OUTPUT=GPIOPIN121OUT -DLED0_ACTIVELOW
BOARDFLAGS	+= -DLED1_PIN=GPIOPIN122 -DLED1_INPUT=GPIOPIN122IN -DLED1_OUTPUT=GPIOPIN122OUT -DLED1_ACTIVELOW
CONSOLEFLAGS	?= -DCONSOLE_SERIAL -DCONSOLE_PORT='"com1:115200,n,8,1"'
JLINKMCU	= lpc1768
endif