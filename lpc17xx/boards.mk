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

ifeq ($(BOARDNAME), LPC1768_MINI_DK2)
BOARDFLAGS	+= -DBUTTON1_PIN=GPIOPIN75 -DBUTTON1_INPUT=GPIOPIN75IN -DBUTTON1_ACTIVELOW
BOARDFLAGS	+= -DBUTTON2_PIN=GPIOPIN76 -DBUTTON2_INPUT=GPIOPIN76IN -DBUTTON2_ACTIVELOW
BOARDFLAGS	+= -DLED1_PIN=GPIOPIN121 -DLED1_OUTPUT=GPIOPIN121OUT -DLED1_ACTIVELOW
BOARDFLAGS	+= -DLED2_PIN=GPIOPIN122 -DLED2_OUTPUT=GPIOPIN122OUT -DLED2_ACTIVELOW
CONSOLEFLAGS	?= -DCONSOLE_SERIAL -DCONSOLE_PORT='"com1:115200,n,8,1"'
JLINKMCU	= lpc1768
endif

ifeq ($(BOARDNAME), MBED_LPC1768)
BOARDFLAGS	+= -DLED1_PIN=GPIOPIN50 -DLED1_OUTPUT=GPIOPIN50OUT
BOARDFLAGS	+= -DLED2_PIN=GPIOPIN52 -DLED2_OUTPUT=GPIOPIN52OUT
BOARDFLAGS	+= -DLED3_PIN=GPIOPIN53 -DLED3_OUTPUT=GPIOPIN53OUT
BOARDFLAGS	+= -DLED4_PIN=GPIOPIN55 -DLED4_OUTPUT=GPIOPIN55OUT
CONSOLEFLAGS	?= -DCONSOLE_SERIAL -DCONSOLE_PORT='"com1:115200,n,8,1"'
JLINKMCU	= lpc1768
endif
