/* ARM microcontroller device specific definitions and header files */

// $Id$

extern unsigned long int CPUFREQ;

extern void cpu_init(unsigned long int frequency);

#include <device.h>
#include <serial.h>
#include <stm32f10x.h>

#ifndef DEFAULT_CPU_FREQ
#define DEFAULT_CPU_FREQ	24000000
#endif

#ifndef CONSOLE_PORT
#define CONSOLE_PORT		1
#endif