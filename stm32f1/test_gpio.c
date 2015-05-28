/* Simple LED test program */

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

int main(void)
{
  unsigned long int i;

  cpu_init(DEFAULT_CPU_FREQ);

#ifdef BUTTON0_PIN
  gpiopin_configure(BUTTON0_PIN, GPIOPIN_INPUT);
#endif

#ifdef BUTTON1_PIN
  gpiopin_configure(BUTTON1_PIN, GPIOPIN_INPUT);
#endif

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

  for (i = 0;; i++)
  {
#ifdef BUTTON0_INPUT
#ifdef BUTTON0_ACTIVELOW
    if (!BUTTON0_INPUT)
#else
    if (BUTTON0_INPUT)
#endif
    {
    // BUTTON0 pressed; flash LED's faster
#ifdef LED0_OUTPUT
#ifdef LED0_ACTIVELOW
      LED0_OUTPUT = ~i >> 17;
#else
      LED0_OUTPUT = i >> 17;
#endif
#endif

#ifdef LED1_OUTPUT
#ifdef LED1_ACTIVELOW
      LED1_OUTPUT = ~i >> 18;
#else
      LED1_OUTPUT = i >> 18;
#endif
#endif

#ifdef LED2_OUTPUT
#ifdef LED2_ACTIVELOW
      LED2_OUTPUT = ~i >> 19;
#else
      LED2_OUTPUT = i >> 19;
#endif
#endif

#ifdef LED3_OUTPUT
#ifdef LED3_ACTIVELOW
      LED3_OUTPUT = ~i >> 20;
#else
      LED3_OUTPUT = i >> 20;
#endif
#endif
    }
    else
#endif

#ifdef BUTTON1_INPUT
#ifdef BUTTON1_ACTIVELOW
    if (!BUTTON1_INPUT)
#else
    if (BUTTON1_INPUT)
#endif
    {
    // BUTTON1 pressed; flash LED's slower
#ifdef LED0_OUTPUT
#ifdef LED0_ACTIVELOW
      LED0_OUTPUT = ~i >> 19;
#else
      LED0_OUTPUT = i >> 19;
#endif
#endif

#ifdef LED1_OUTPUT
#ifdef LED1_ACTIVELOW
      LED1_OUTPUT = ~i >> 20;
#else
      LED1_OUTPUT = i >> 20;
#endif
#endif

#ifdef LED2_OUTPUT
#ifdef LED2_ACTIVELOW
      LED2_OUTPUT = ~i >> 21;
#else
      LED2_OUTPUT = i >> 21;
#endif
#endif

#ifdef LED3_OUTPUT
#ifdef LED3_ACTIVELOW
      LED3_OUTPUT = ~i >> 22;
#else
      LED3_OUTPUT = i >> 22;
#endif
#endif
    }
    else
#endif

    // Flash LED's at normal speed
    {
#ifdef LED0_OUTPUT
#ifdef LED0_ACTIVELOW
      LED0_OUTPUT = ~i >> 18;
#else
      LED0_OUTPUT = i >> 18;
#endif
#endif

#ifdef LED1_OUTPUT
#ifdef LED1_ACTIVELOW
      LED1_OUTPUT = ~i >> 19;
#else
      LED1_OUTPUT = i >> 19;
#endif
#endif

#ifdef LED2_OUTPUT
#ifdef LED2_ACTIVELOW
      LED2_OUTPUT = ~i >> 20;
#else
      LED2_OUTPUT = i >> 20;
#endif
#endif

#ifdef LED3_OUTPUT
#ifdef LED3_ACTIVELOW
      LED3_OUTPUT = ~i >> 21;
#else
      LED3_OUTPUT = i >> 21;
#endif
#endif
    }
  }
}
