/* Simple C++ exception test program */

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

#include <cstdio>
#include <iostream>
#include <exception>

using namespace std;

#include <cpu.h>

#define CLEARSCREEN	"\033[H\033[2J"
#define SYSTICKRATE	100

_BEGIN_STD_C

static volatile unsigned SleepCounter = 0;

void SysTick_Handler(void)
{
  if (SleepCounter) SleepCounter--;
}

_END_STD_C

static void millisleep(unsigned milliseconds)
{
  SleepCounter = SYSTICKRATE*milliseconds/1000;
  while (SleepCounter) __WFI();
}

// Define an exception object

class finger_exception: public exception
{
  virtual const char* what() const throw()
  {
    return "You ran out of fingers!";
  }
} finger_error;

int main(void)
{
  int counter;

  cpu_init(DEFAULT_CPU_FREQ);

// Bind standard I/O to hardware

#ifdef CONSOLE_SERIAL
  serial_stdio((char *) CONSOLE_PORT);
#endif

#ifdef CONSOLE_SEMIHOSTING
  semihosting_stdio((char *) CONSOLE_PORT)
#endif

#ifdef CONSOLE_USB
  usb_serial_stdio((char *) NULL);
  getch();
#endif

// Display startup banner

  cout << CLEARSCREEN; 
  cout << MCUFAMILYNAME;
  cout << " C++ Exception Test ";
  cout << "(" << __DATE__ << " " <<  __TIME__ << ")";
  cout << "\n\n";

  cout << revision;
  cout << "\n\n";

  cout << "Board:    " << BOARDNAME << "\n";
  cout << "CPU Freq: " << SystemCoreClock/1000000 << " MHz\n";
  cout << "Compiler: " << __COMPILER__ << " " << __VERSION__ << " " << __ABI__ << "\n\n";

// Initialize System Tick

  SysTick_Config(SystemCoreClock / SYSTICKRATE);

  counter = 1;

// Trivial main program

  for (;;)
  {
    try
    {
      cout << "Hello, world " << counter << "\n";
      millisleep(1000);

      counter += 1;
      if (counter > 10) throw finger_error;
    }
    catch(exception& e)
    {
      cout << e.what() << "\n";
      counter = 1;
    }
  }
}
