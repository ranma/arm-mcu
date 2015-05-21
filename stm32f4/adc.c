/* Abstract services for reading analog inputs */

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

static const char revision[] = "$Id$";

#include <cpu.h>

#define MAX_ADC_CHANNELS	16

// Initialize an A/D input pin
//   Returns 0 on success or nonzero on failure and sets errno

int adc_init(void *subsystem, unsigned int channel)
{
  ADC_TypeDef *ADCsubsystem = NULL;
  errno_r = 0;

// Validate parameters

#ifdef ADC1
  if ((subsystem == NULL) || (subsystem == ADC1))
    ADCsubsystem = ADC1;
#endif

  if (ADCsubsystem == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (channel >= MAX_ADC_CHANNELS)
  {
    errno_r = ENODEV;
    return -1;
  }

// Let ADC subsystem out of reset

  RCC->APB2RSTR &= ~RCC_APB2RSTR_ADCRST;

// Enable ADC peripheral clock

#ifdef ADC1
  if (ADCsubsystem == ADC1)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
#endif

// Configure ADC subsystem

  ADCsubsystem->SR  = 0;
  ADCsubsystem->CR1 = 0;
  ADCsubsystem->CR2 = ADC_CR2_ADON;

  ADC->CCR  = ADC_CCR_TSVREFE;

// Configure the analog input

#ifdef ADC1
  if (ADCsubsystem == ADC1)
    switch (channel)
    {
      case 0 :	// PA0
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 0;
        GPIOA->PUPDR &= ~(3 << 0);
        break;

      case 1 :	// PA1
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 2;
        GPIOA->PUPDR &= ~(3 << 2);
        break;

      case 2 :	// PA2
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 4;
        GPIOA->PUPDR &= ~(3 << 4);
        break;
  
      case 3 :	// PA3
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 6;
        GPIOA->PUPDR &= ~(3 << 6);
        break;

      case 4 :	// PA4
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 8;
        GPIOA->PUPDR &= ~(3 << 8);
        break;

      case 5 :	// PA5
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 10;
        GPIOA->PUPDR &= ~(3 << 10);
        break;

      case 6 :	// PA6
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 12;
        GPIOA->PUPDR &= ~(3 << 12);
        break;

      case 7 :	// PA7
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        GPIOA->MODER |= 3 << 14;
        GPIOA->PUPDR &= ~(3 << 14);
        break;

      case 8 :	// PB0
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIOB->MODER |= 3 << 0;
        GPIOB->PUPDR &= ~(3 << 0);

      case 9 :	// PB1
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        GPIOB->MODER |= 3 << 2;
        GPIOB->PUPDR &= ~(3 << 2);

      case 10 :	// PC0
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 0;
        GPIOC->PUPDR &= ~(3 << 0);

      case 11 :	// PC1
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 2;
        GPIOC->PUPDR &= ~(3 << 2);

      case 12 :	// PC2
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 4;
        GPIOC->PUPDR &= ~(3 << 4);

      case 13 :	// PC3
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 6;
        GPIOC->PUPDR &= ~(3 << 6);

      case 14 :	// PC4
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 8;
        GPIOC->PUPDR &= ~(3 << 8);

      case 15 :	// PC5
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        GPIOC->MODER |= 3 << 10;
        GPIOC->PUPDR &= ~(3 << 10);

      default :
        break;
    }
#endif

  return 0;
}

// Read an A/D input
//   Returns unsigned 16-bit result, right-justified, or zero on error

uint16_t adc_read(void * subsystem, unsigned int channel)
{
  ADC_TypeDef *ADCsubsystem = NULL;
  errno_r = 0;

// Validate parameters

#ifdef ADC1
  if ((subsystem == NULL) || (subsystem == ADC1))
    ADCsubsystem = ADC1;
#endif

  if (ADCsubsystem == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (channel >= MAX_ADC_CHANNELS)
  {
    errno_r = ENODEV;
    return 0;
  }

// Select analog input

  ADCsubsystem->SQR1 = 0;
  ADCsubsystem->SQR2 = 0;
  ADCsubsystem->SQR3 = channel;

// Start conversion

  ADCsubsystem->SR   = 0;
  ADCsubsystem->CR1  = 0;
  ADCsubsystem->CR2  = ADC_CR2_SWSTART|ADC_CR2_ADON;

// Wait for conversion to complete

  while ((ADCsubsystem->SR & ADC_SR_EOC) == 0);

// Retrieve result

  return ADCsubsystem->DR & ADC_DR_DATA;
}
