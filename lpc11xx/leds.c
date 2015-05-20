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
  gpio_configure(LED0_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED1_PIN
  gpio_configure(LED1_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED2_PIN
  gpio_configure(LED2_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED3_PIN
  gpio_configure(LED3_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED4_PIN
  gpio_configure(LED4_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED5_PIN
  gpio_configure(LED5_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED6_PIN
  gpio_configure(LED6_PIN, GPIO_MODE_OUTPUT);
#endif

#ifdef LED7_PIN
  gpio_configure(LED7_PIN, GPIO_MODE_OUTPUT);
#endif

  LEDS_set(0);	// Turn off all LEDs at startup
}

// Get LED states.  A 1 bit in the result indicates the LED is on.
// A 0 bit in the result indicates the LED is off.

unsigned long int LEDS_get(void)
{
  unsigned long int result = 0;

#ifdef LED0_PIN
#ifdef LED0_ACTIVELOW
  result = !gpio_read(LED0_PIN);
#else
  result = gpio_read(LED0_PIN);
#endif
#endif

#ifdef LED1_PIN
#ifdef LED1_ACTIVELOW
  result = !gpio_read(LED1_PIN) << 1;
#else
  result = gpio_read(LED1_PIN) << 1;
#endif
#endif

#ifdef LED2_PIN
#ifdef LED2_ACTIVELOW
  result = !gpio_read(LED2_PIN) << 2;
#else
  result = gpio_read(LED2_PIN) << 2;
#endif
#endif

#ifdef LED3_PIN
#ifdef LED3_ACTIVELOW
  result = !gpio_read(LED3_PIN) << 3;
#else
  result = gpio_read(LED3_PIN) << 3;
#endif
#endif

#ifdef LED4_PIN
#ifdef LED4_ACTIVELOW
  result = !gpio_read(LED4_PIN) << 4;
#else
  result = gpio_read(LED4_PIN) << 4;
#endif
#endif

#ifdef LED5_PIN
#ifdef LED5_ACTIVELOW
  result = !gpio_read(LED5_PIN) << 5;
#else
  result = gpio_read(LED5_PIN) << 5;
#endif
#endif

#ifdef LED6_PIN
#ifdef LED6_ACTIVELOW
  result = !gpio_read(LED6_PIN) << 6;
#else
  result = gpio_read(LED6_PIN) << 6;
#endif
#endif

#ifdef LED7_PIN
#ifdef LED7_ACTIVELOW
  result = !gpio_read(LED7_PIN) << 7;
#else
  result = gpio_read(LED7_PIN) << 7;
#endif
#endif

  return result;
}

// Set LED states.  A 1 bit in the mask indicates the LED should be turned on.
// A 0 bit in the mask indicates the LED should be turned off.

void LEDS_set(unsigned long int mask)
{
#ifdef LED0_PIN
#ifdef LED0_ACTIVELOW
  gpio_write(LED0_PIN, ~mask & 0x01);
#else
  gpio_write(LED0_PIN, mask & 0x01);
#endif
#endif

#ifdef LED1_PIN
#ifdef LED1_ACTIVELOW
  gpio_write(LED1_PIN, (~mask >> 1) & 0x01);
#else
  gpio_write(LED1_PIN, (mask >> 1) & 0x01);
#endif
#endif

#ifdef LED2_PIN
#ifdef LED2_ACTIVELOW
  gpio_write(LED2_PIN, (~mask >> 2) & 0x01);
#else
  gpio_write(LED2_PIN, (mask >> 2) & 0x01);
#endif
#endif

#ifdef LED3_PIN
#ifdef LED3_ACTIVELOW
  gpio_write(LED3_PIN, (~mask >> 3) & 0x01);
#else
  gpio_write(LED3_PIN, (mask >> 3) & 0x01);
#endif
#endif

#ifdef LED4_PIN
#ifdef LED4_ACTIVELOW
  gpio_write(LED4_PIN, (~mask >> 4) & 0x01);
#else
  gpio_write(LED4_PIN, (mask >> 4) & 0x01);
#endif
#endif

#ifdef LED5_PIN
#ifdef LED5_ACTIVELOW
  gpio_write(LED5_PIN, (~mask >> 5) & 0x01);
#else
  gpio_write(LED6_PIN, (mask >> 5) & 0x01);
#endif
#endif

#ifdef LED6_PIN
#ifdef LED6_ACTIVELOW
  gpio_write(LED6_PIN, (~mask >> 6) & 0x01);
#else
  gpio_write(LED6_PIN, (mask >> 6) & 0x01);
#endif
#endif

#ifdef LED7_PIN
#ifdef LED7_ACTIVELOW
  gpio_write(LED7_PIN, (~mask >> 7) & 0x01);
#else
  gpio_write(LED7_PIN, (mask >> 7) & 0x01);
#endif
#endif
}
