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
#ifdef LED0_PIN
  gpiopin_configure(LED0_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED1_PIN
  gpiopin_configure(LED1_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED2_PIN
  gpiopin_configure(LED2_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED3_PIN
  gpiopin_configure(LED3_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED4_PIN
  gpiopin_configure(LED4_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED5_PIN
  gpiopin_configure(LED5_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED6_PIN
  gpiopin_configure(LED6_PIN, GPIOPIN_OUTPUT);
#endif

#ifdef LED7_PIN
  gpiopin_configure(LED7_PIN, GPIOPIN_OUTPUT);
#endif

  LEDS_set(0);						// Turn off all LEDs at startup
}

// Get LED states.  A 1 bit in the result indicates the LED is on.
// A 0 bit in the result indicates the LED is off.

unsigned long int LEDS_get(void)
{
  unsigned long int result = 0;

#ifdef LED0_INPUT
  result += LED0_INPUT;
#endif

#ifdef LED1_INPUT
  result += LED1_INPUT << 1;
#endif

#ifdef LED2_INPUT
  result += LED2_INPUT << 1;
#endif

#ifdef LED3_INPUT
  result += LED3_INPUT << 1;
#endif

#ifdef LED4_INPUT
  result += LED4_INPUT << 1;
#endif

#ifdef LED5_INPUT
  result += LED5_INPUT << 1;
#endif

#ifdef LED6_INPUT
  result += LED6_INPUT << 1;
#endif

#ifdef LED7_INPUT
  result += LED7_INPUT << 1;
#endif

  return result;
}

// Set LED states.  A 1 bit in the mask indicates the LED should be turned on.
// A 0 bit in the mask indicates the LED should be turned off.

void LEDS_set(unsigned long int mask)
{
#ifdef LED0_OUTPUT
  LED0_OUTPUT = mask;
#endif

#ifdef LED1_OUTPUT
  LED1_OUTPUT = mask >> 1;
#endif

#ifdef LED2_OUTPUT
  LED2_OUTPUT = mask >> 2;
#endif

#ifdef LED3_OUTPUT
  LED3_OUTPUT = mask >> 3;
#endif

#ifdef LED4_OUTPUT
  LED4_OUTPUT = mask >> 4;
#endif

#ifdef LED5_OUTPUT
  LED5_OUTPUT = mask >> 5;
#endif

#ifdef LED6_OUTPUT
  LED6_OUTPUT = mask >> 6;
#endif

#ifdef LED7_OUTPUT
  LED7_OUTPUT = mask >> 7;
#endif
}
