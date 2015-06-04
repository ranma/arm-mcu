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
  unsigned i;

  cpu_init(DEFAULT_CPU_FREQ);
  systick_init(100);
  LEDS_initialize();

#ifdef BUTTON0_PIN
  gpiopin_configure(BUTTON0_PIN, GPIOPIN_INPUT);
#endif

#ifdef BUTTON1_PIN
  gpiopin_configure(BUTTON1_PIN, GPIOPIN_INPUT);
#endif

  for (i = 0;; i++)
  {
    LEDS_set(i);

#ifdef BUTTON0_INPUT
#ifdef BUTTON0_ACTIVELOW
    if (!BUTTON0_INPUT)
#else
    if (BUTTON0_INPUT)
#endif
      // BUTTON0 pressed; flash LED's faster
      millisleep(250);
    else
#endif

#ifdef BUTTON1_INPUT
#ifdef BUTTON1_ACTIVELOW
    if (!BUTTON1_INPUT)
#else
    if (BUTTON1_INPUT)
#endif
      // BUTTON1 pressed; flash LED's slower
      millisleep(1000);
    else
#endif

      // Flash LED's at normal speed
      millisleep(500);
  }
}
