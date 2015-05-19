/* Initialize CPU core */

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

#ifdef NUCLEO_F411RE
#define USE_PLL		true
#define FLASH_WAITS	FLASH_ACR_LATENCY_3WS
#define VOS		3
#define HSE_BYPASS	true
#define PLLM		HSE_VALUE/1000000
#define PLLN		400
#define PLLP		4
#define PLLQ		7
#define HPRE		RCC_CFGR_HPRE_DIV1
#define PPRE1		RCC_CFGR_PPRE1_DIV2
#define PPRE2		RCC_CFGR_PPRE2_DIV2
#endif

#ifdef USB_PLL
static void ConfigurePLL(void)
{
  // Configure internal voltage regulator for full speed CPU clock

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;

  PWR->CR |= VOS << 14;

  // Configure flash wait states for full speed CPU clock

  do
  {
    FLASH->ACR = FLASH_WAITS;
  }
  while (FLASH->ACR != FLASH_WAITS);

  // Start the HSE

  RCC->CR |= RCC_CR_HSEON | (HSE_BYPASS ? RCC_CR_HSEBYP : 0);
  while (!(RCC->CR & RCC_CR_HSERDY));

  // Configure bus clock dividers

  RCC->CFGR = HPRE | PPRE1 | PPRE2;

  // Configure the PLL

  RCC->PLLCFGR = (PLLQ << 24)|RCC_PLLCFGR_PLLSRC_HSE|((PLLP/2-1) << 16)|(PLLN << 6)|(PLLM);

  // Start the PLL

  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY));

  // Switch to the PLL

  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

  // Enable flash prefetch and cache

  FLASH->ACR |= FLASH_ACR_PRFTEN|FLASH_ACR_ICEN|FLASH_ACR_DCEN;
}
#endif

void cpu_init(unsigned long int frequency)
{
  // AIRCR.PRIGROUP=3 (4 bits priority, 4 bits subpriority)

  SCB->AIRCR &= SCB_AIRCR_PRIGROUP_Msk;
  SCB->AIRCR |= 3 << SCB_AIRCR_PRIGROUP_Pos;

  // Initialize 16 MHz internal oscillator (HSI)

  SystemInit();

#ifdef USE_PLL
  // For known oscillator configurations, we can use the external oscillator
  // (HSE) and PLL to run full speed

  ConfigurePLL();
#endif

  SystemCoreClockUpdate();
}
