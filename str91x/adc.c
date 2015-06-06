/* Abstract services for reading analog inputs */

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

#define MAX_ADC_CHANNELS	8

// Initialize an A/D input pin
//   Returns 0 on success or nonzero on failure and sets errno

int adc_init(void *subsystem, unsigned int channel)
{
  ADC_InitTypeDef adcinit;

// Validate parameters

  if (channel >= MAX_ADC_CHANNELS)
  {
    errno_r = ENODEV;
    return -1;
  }

// Turn on peripheral clocks

  SCU_APBPeriphClockConfig(__ADC, ENABLE);
  SCU_APBPeriphClockConfig(__GPIO4, ENABLE);

// Configure GPIO pin as analog input

  GPIO_ANAPinConfig(1 << channel, ENABLE);

// Configure A/D converter

  ADC_DeInit();			// Reset A/D converter
  ADC_Cmd(ENABLE);		// Power on A/D converter
  ADC_PrescalerConfig(0x2);	// Conversion clock is 24 MHz

  ADC_StructInit(&adcinit);
  adcinit.ADC_Channel_0_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_1_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_2_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_3_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_4_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_5_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_6_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Channel_7_Mode = ADC_NoThreshold_Conversion;
  adcinit.ADC_Scan_Mode = DISABLE;
  adcinit.ADC_Conversion_Mode = ADC_Single_Mode;
  ADC_Init(&adcinit);

  return 0;
}

// Read an A/D input
//   Returns unsigned 16-bit result, right-justified, or zero on error

uint16_t adc_read(void *subsystem, unsigned int channel)
{

// Validate parameters

  if (channel >= MAX_ADC_CHANNELS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (!(SCU->GPIOANA & (1 << channel)))
  {
    errno_r = ENODEV;
    return -1;
  }

// Select A/D channel

  ADC->CR &= 0xFE3F;
  ADC->CR |= channel << 0x6;

// Start A/D conversion

  ADC_ConversionCmd(ADC_Conversion_Start);
  while (!ADC_GetFlagStatus(ADC_FLAG_ECV));

// Read result

  return ADC_GetConversionValue(channel);
}
