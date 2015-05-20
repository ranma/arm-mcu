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
#ifdef LED0_ACTIVELOW
  result += !LED0_INPUT;
#else
  result += LED0_INPUT;
#endif
#endif

#ifdef LED1_INPUT
#ifdef LED1_ACTIVELOW
  result += !LED1_INPUT << 1;
#else
  result += LED1_INPUT << 1;
#endif
#endif

#ifdef LED2_INPUT
#ifdef LED2_ACTIVELOW
  result += !LED2_INPUT << 2;
#else
  result += LED2_INPUT << 2;
#endif
#endif

#ifdef LED3_INPUT
#ifdef LED3_ACTIVELOW
  result += !LED3_INPUT << 3;
#else
  result += LED3_INPUT << 3;
#endif
#endif

#ifdef LED4_INPUT
#ifdef LED4_ACTIVELOW
  result += !LED4_INPUT << 4;
#else
  result += LED4_INPUT << 4;
#endif
#endif

#ifdef LED5_INPUT
#ifdef LED5_ACTIVELOW
  result += !LED5_INPUT << 5;
#else
  result += LED5_INPUT << 5;
#endif
#endif

#ifdef LED6_INPUT
#ifdef LED6_ACTIVELOW
  result += !LED6_INPUT << 6;
#else
  result += LED6_INPUT << 6;
#endif
#endif

#ifdef LED7_INPUT
#ifdef LED7_ACTIVELOW
  result += !LED7_INPUT << 7;
#else
  result += LED7_INPUT << 7;
#endif
#endif

  return result;
}

// Set LED states.  A 1 bit in the mask indicates the LED should be turned on.
// A 0 bit in the mask indicates the LED should be turned off.

void LEDS_set(unsigned long int mask)
{
#ifdef LED0_OUTPUT
#ifdef LED0_ACTIVELOW
  LED0_OUTPUT = ~mask;
#else
  LED0_OUTPUT = mask;
#endif
#endif

#ifdef LED1_OUTPUT
#ifdef LED1_ACTIVELOW
  LED1_OUTPUT = ~mask >> 1;
#else
  LED1_OUTPUT = mask >> 1;
#endif
#endif

#ifdef LED2_OUTPUT
#ifdef LED2_ACTIVELOW
  LED2_OUTPUT = ~mask >> 2;
#else
  LED2_OUTPUT = mask >> 2;
#endif
#endif

#ifdef LED3_OUTPUT
#ifdef LED3_ACTIVELOW
  LED3_OUTPUT = ~mask >> 3;
#else
  LED3_OUTPUT = mask >> 3;
#endif
#endif

#ifdef LED4_OUTPUT
#ifdef LED4_ACTIVELOW
  LED4_OUTPUT = ~mask >> 4;
#else
  LED4_OUTPUT = mask >> 4;
#endif
#endif

#ifdef LED5_OUTPUT
#ifdef LED5_ACTIVELOW
  LED5_OUTPUT = ~mask >> 5;
#else
  LED5_OUTPUT = mask >> 5;
#endif
#endif

#ifdef LED6_OUTPUT
#ifdef LED6_ACTIVELOW
  LED6_OUTPUT = ~mask >> 6;
#else
  LED6_OUTPUT = mask >> 6;
#endif
#endif

#ifdef LED7_OUTPUT
#ifdef LED7_ACTIVELOW
  LED7_OUTPUT = ~mask >> 7;
#else
  LED7_OUTPUT = mask >> 7;
#endif
#endif
}
