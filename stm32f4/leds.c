/* Abstract services for controlling LED's */

// $Id$

// Copyright (C)2013-2015, Philip Munts, President, Munts AM Corp.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

static const char revision[] = "$Id$";

#include <cpu.h>

// Initialize the LED's (to the off state)

void LEDS_initialize(void)
{
#ifdef STM32F4_DISCOVERY
  gpiopin_configure(GPIOPIN60, GPIOPIN_OUTPUT);		// PD12
  gpiopin_configure(GPIOPIN61, GPIOPIN_OUTPUT);		// PD13
  gpiopin_configure(GPIOPIN62, GPIOPIN_OUTPUT);		// PD14
  gpiopin_configure(GPIOPIN63, GPIOPIN_OUTPUT);		// PD15
#endif

#ifdef NETDUINOPLUS2
  gpiopin_configure(GPIOPIN10, GPIOPIN_OUTPUT);		// PA10
#endif

#ifdef STM32_M4_MINI
  gpiopin_configure(GPIOPIN44, GPIOPIN_OUTPUT);		// PC12, yellow
  gpiopin_configure(GPIOPIN45, GPIOPIN_OUTPUT);		// PC13, red
#endif

#ifdef NUCLEO_F411RE
  gpiopin_configure(GPIOPIN5, GPIOPIN_OUTPUT);		// PA5, green
#endif

  LEDS_set(0);						// Turn off all LEDs at startup
}

// Get LED states.  A 1 bit in the result indicates the LED is on.
// A 0 bit in the result indicates the LED is off.

unsigned long int LEDS_get(void)
{
  unsigned long int result = 0;

#ifdef STM32F4_DISCOVERY
  result += GPIOPIN60IN;
  result += GPIOPIN61IN << 1;
  result += GPIOPIN62IN << 2;
  result += GPIOPIN63IN << 3;
#endif

#ifdef NETDUINOPLUS2
  result += GPIOPIN10IN;
#endif

#ifdef STM32_M4_MINI
  result += GPIOPIN44IN;
  result += GPIOPIN45IN << 1;
#endif

#ifdef NUCLEO_F411RE
  result += GPIOPIN5IN;
#endif

  return result;
}

// Set LED states.  A 1 bit in the mask indicates the LED should be turned on.
// A 0 bit in the mask indicates the LED should be turned off.

void LEDS_set(unsigned long int mask)
{
#ifdef STM32F4_DISCOVERY
  GPIOPIN60OUT = mask;
  GPIOPIN61OUT = mask >> 1;
  GPIOPIN62OUT = mask >> 2;
  GPIOPIN63OUT = mask >> 3;
#endif

#ifdef NETDUINOPLUS2
  GPIOPIN10OUT = mask;
#endif

#ifdef STM32_M4_MINI
  GPIOPIN44OUT = mask;
  GPIOPIN45OUT = mask >> 1;
#endif

#ifdef NUCLEO_F411RE
  GPIOPIN5OUT = mask;
#endif
}
