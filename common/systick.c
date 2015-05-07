/* Abstract services for controlling and using the system tick timer */

// $Id$

// Copyright (C)2015, Philip Munts, President, Munts AM Corp.
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

#include <cpu.h>

static volatile unsigned SleepCounter = 0;

void SysTick_Handler(void)
{
  if (SleepCounter) SleepCounter--;
}

static unsigned systick_rate;

int systick_init(unsigned rate)
{
  errno_r = 0;

  // Validate parameters

  if (SystemCoreClock/rate > 16777216)
  {
    errno_r = EINVAL;
    return -1;
  }

  // Save the systick rate

  systick_rate = rate;

  SysTick_Config(SystemCoreClock / systick_rate);
  return 0;
}

void millisleep(unsigned milliseconds)
{
  SleepCounter = systick_rate*milliseconds/1000;
#ifdef __WFI
  while (SleepCounter) __WFI();
#else
  while (SleepCounter);
#endif
}

unsigned sleep(unsigned seconds)
{
  millisleep(seconds*1000);
  return 0;
}
