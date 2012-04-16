/* ARM microcontroller device specific definitions and header files */

// $Id$

#ifndef _CPU_H
#define _CPU_H

#include <_ansi.h>

_BEGIN_STD_C

#include <arm.h>
#include <device.h>
#include <gpiopins.h>
#include <leds.h>
#include <serial.h>
#include <spi.h>
#include <stm32f10x.h>

#define CPUFREQ			SystemCoreClock

#ifndef DEFAULT_CPU_FREQ
#ifdef stm32f100rb
#define DEFAULT_CPU_FREQ	24000000
#endif

#ifdef stm32f103c8
#define DEFAULT_CPU_FREQ	72000000
#endif

#ifdef stm32f103rb
#define DEFAULT_CPU_FREQ	72000000
#endif

#ifdef stm32f107rb
#define DEFAULT_CPU_FREQ	72000000
#endif
#endif

#ifndef CONSOLE_PORT
#ifdef OLIMEX_STM32_P103
#define CONSOLE_PORT		"com2:115200,n,8,1"
#endif

#ifdef OLIMEX_STM32_P107
#define CONSOLE_PORT		"com3:115200,n,8,1"
#endif

#ifdef STM32_VALUE_LINE_DISCOVERY
#define CONSOLE_PORT		"com1:115200,n,8,1"
#endif
#endif

#ifdef W5200E01_M3
#define CONSOLE_PORT		"com1:115200,n,8,1"
#endif

extern void cpu_init(unsigned long int frequency);

_END_STD_C
#endif
