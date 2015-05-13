/******************************************************************************/
/*                                                                            */
/*     Simple serial port I/O routines for the STM32F4 Cortex-M4 ARM MCU      */
/*                                                                            */
/******************************************************************************/

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

#include <stdlib.h>
#include <string.h>

#include <cpu.h>

#define MAX_SERIAL_PORTS	6	// com1: through com6:
#define BUFSIZE			64	// Must be power of 2!

typedef struct
{
  uint8_t data[BUFSIZE];
  unsigned head;
  unsigned tail;
  unsigned count;
} ringbuffer_t;

typedef struct
{
  volatile ringbuffer_t *rxbuffer;	// receiver buffer structure
  volatile ringbuffer_t *txbuffer;	// transmit buffer structure
  volatile unsigned * const TXEIE;	// TXEIE bit band address
} uart_t;

static uart_t UARTS[MAX_SERIAL_PORTS] =
{
  { NULL, NULL, (volatile unsigned *) 0x4222019C},
  { NULL, NULL, (volatile unsigned *) 0x4208819C},
  { NULL, NULL, (volatile unsigned *) 0x4209019C},
  { NULL, NULL, (volatile unsigned *) 0x4209819C},
  { NULL, NULL, (volatile unsigned *) 0x420A019C},
  { NULL, NULL, (volatile unsigned *) 0x4222819C},
};

static int InitializeRingBuffers(unsigned port)
{
  errno_r = 0;

// Allocate buffer structures

  if (UARTS[port].rxbuffer == NULL)
  {
    UARTS[port].rxbuffer = malloc(sizeof(ringbuffer_t));
    if (UARTS[port].rxbuffer == NULL)
    {
      errno_r = ENOMEM;
      return -1;
    }
  }

  if (UARTS[port].txbuffer == NULL)
  {
    UARTS[port].txbuffer = malloc(sizeof(ringbuffer_t));
    if (UARTS[port].txbuffer == NULL)
    {
      errno_r = ENOMEM;
      return -1;
    }
  }

  memset((void *) UARTS[port].rxbuffer, 0, sizeof(ringbuffer_t));
  memset((void *) UARTS[port].txbuffer, 0, sizeof(ringbuffer_t));
  return 0;
}

/* Map serial port device name to port number */

int serial_name_to_port(char *name)
{
  errno_r = 0;

  if (!strncasecmp(name, "com1:", 5))
    return 0;
  else if (!strncasecmp(name, "com2:", 5))
    return 1;
  else if (!strncasecmp(name, "com3:", 5))
    return 2;
  else if (!strncasecmp(name, "com4:", 5))
    return 3;
  else if (!strncasecmp(name, "com5:", 5))
    return 4;
  else if (!strncasecmp(name, "com6:", 5))
    return 5;
  else
  {
    errno_r = ENODEV;
    return -1;
  }
}

/* Initialize serial port */

int serial_open(char *name, unsigned *subdevice)
{
  unsigned port;
  unsigned baudrate;
  USART_InitTypeDef USART_config;
  GPIO_InitTypeDef GPIO_config;
  NVIC_InitTypeDef NVIC_config;

  errno_r = 0;

// Look up serial port number

  port = serial_name_to_port(name);
  if (port < 0) return port;

// Pass up port number, if requested

  if (subdevice != NULL)
    *subdevice = port;

// Extract baud rate from device name

  baudrate = atoi(name+5);
  if (baudrate == 0)
  {
    errno_r = EINVAL;
    return -1;
  }

// Configure the serial port

  switch (port)
  {
#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 0 : // USART1

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on USART1 peripheral clock

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

// Configure TX pin on PA9

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_9;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure RX pin PA10

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_10;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure USART1

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(USART1, &USART_config);

// Enable USART1

      USART_Cmd(USART1, ENABLE);

// Enable USART1 interrupts

      NVIC_config.NVIC_IRQChannel = USART1_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      USART1->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 1 : // USART2

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on USART2 peripheral clock

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

// Configure TX pin on PA2

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_2;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure RX pin on PA3

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_3;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure USART2

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(USART2, &USART_config);

// Enable USART2

      USART_Cmd(USART2, ENABLE);

// Enable USART2 interrupts

      NVIC_config.NVIC_IRQChannel = USART2_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      USART2->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 2 : // USART3

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on USART3 peripheral clock

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

// Configure TX pin on PB10

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

      GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_10;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOB, &GPIO_config);

// Configure RX pin on PB11

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

      GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_11;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOB, &GPIO_config);

// Configure USART3

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(USART3, &USART_config);

// Enable USART3

      USART_Cmd(USART3, ENABLE);

// Enable USART3 interrupts

      NVIC_config.NVIC_IRQChannel = USART3_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      USART3->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 3 : // UART4

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on UART4 peripheral clock

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

// Configure TX pin on PA0

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_0;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure RX pin on PA1

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_1;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure UART4

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(UART4, &USART_config);

// Enable UART4

      USART_Cmd(UART4, ENABLE);

// Enable UART4 interrupts

      NVIC_config.NVIC_IRQChannel = UART4_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      UART4->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 4 : // UART5

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on UART5 peripheral clock

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

// Configure TX pin on PC12

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

      GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_12;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOC, &GPIO_config);

// Configure RX pin on PD2

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

      GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_2;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOD, &GPIO_config);

// Configure UART5

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(UART5, &USART_config);

// Enable UART5

      USART_Cmd(UART5, ENABLE);

// Enable UART5 interrupts

      NVIC_config.NVIC_IRQChannel = UART5_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      UART5->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
    case 5 : //  USART6

// Initialize ring buffers

      if (InitializeRingBuffers(port))
        return -1;

// Turn on USART6 peripheral clock

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

#ifdef NUCLEO_F411RE
// Configure TX pin on PA11 (aka Arduino D1, wire wrap CN10 pin 35 to CN10 pin 14)

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_USART6);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_11;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_config);

// Configure RX pin on PA12 (aka Arduino D0, wire wrap CN10 pin 37 to CN 10 pin 12)

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

      GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_USART6);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_12;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOA, &GPIO_config);
#else
// Configure TX pin on PC6

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

      GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_6;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_config.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOC, &GPIO_config);

// Configure RX pin on PC7

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

      GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

      GPIO_StructInit(&GPIO_config);
      GPIO_config.GPIO_Pin = GPIO_Pin_7;
      GPIO_config.GPIO_Mode = GPIO_Mode_AF;
      GPIO_config.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(GPIOC, &GPIO_config);
#endif

// Configure USART6

      USART_StructInit(&USART_config);
      USART_config.USART_BaudRate = baudrate;
      USART_Init(USART6, &USART_config);

// Enable USART6

      USART_Cmd(USART6, ENABLE);

// Enable USART6 interrupts

      NVIC_config.NVIC_IRQChannel = USART6_IRQn;
      NVIC_config.NVIC_IRQChannelPreemptionPriority = 8;
      NVIC_config.NVIC_IRQChannelSubPriority = 8;
      NVIC_config.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_config);

      USART6->CR1 |= USART_CR1_RXNEIE;
      break;
#endif

    default :
      errno_r = ENODEV;
      return -1;
  }


  return 0;
}

/* Register serial port for standard I/O */

int serial_stdio(char *name)
{
  unsigned subdevice;

  if (serial_open(name, &subdevice))
    return -1;

  // Nuke existing stdin, stdout, stderr

  device_unregister(0);
  device_unregister(1);
  device_unregister(2);

  // Register new stdin, stdout, stderr

  device_register_char_fd(0, subdevice, NULL, serial_read, NULL, serial_rxready, NULL);
  device_register_char_fd(1, subdevice, serial_write, NULL, serial_txready, NULL, NULL);
  device_register_char_fd(2, subdevice, serial_write, NULL, serial_txready, NULL, NULL);

  return 0;
}

/* Register a serial port device */

int serial_register(char *name)
{
  unsigned port;

// Look up serial port number

  port = serial_name_to_port(name);
  if (port < 0) return port;

// Register the serial port driver

  return device_register_char(name, port, serial_open, NULL, serial_write, serial_read, serial_txready, serial_rxready, NULL);
}

/* Return true if receive data is available */

int serial_rxready(unsigned port)
{
  volatile ringbuffer_t *bufptr;

  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  bufptr = UARTS[port].rxbuffer;

  if (bufptr == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  return (bufptr->count > 0);
}

/* Read data from the serial port */

int serial_read(unsigned port, char *buf, unsigned count)
{
  volatile ringbuffer_t *bufptr;
  int len = 0;

  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (buf == NULL)
  {
    errno_r = EINVAL;
    return -1;
  }

  bufptr = UARTS[port].rxbuffer;

  if (bufptr == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  while ((bufptr->count > 0) && (count > 0))
  {
    *buf++ = bufptr->data[bufptr->tail];
    count--;
    __disable_irq();
    bufptr->count--;
    __enable_irq();
    bufptr->tail++;
    bufptr->tail &= BUFSIZE-1;
    len++;
  }

  return len;
}

/* Return true if transmitter is ready to accept data */

int serial_txready(unsigned port)
{
  volatile ringbuffer_t *bufptr;

  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  bufptr = UARTS[port].txbuffer;

  if (bufptr == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  return (bufptr->count < BUFSIZE);
}

/* Send data out the serial port */

int serial_write(unsigned port, char *buf, unsigned count)
{
  volatile ringbuffer_t *bufptr;
  int len = 0;

  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (buf == NULL)
  {
    errno_r = EINVAL;
    return -1;
  }

  bufptr = UARTS[port].txbuffer;

  if (bufptr == NULL)
  {
    errno_r = ENODEV;
    return -1;
  }

  while ((bufptr->count < BUFSIZE) && (count > 0))
  {
    bufptr->data[bufptr->head] = *buf++;
    count--;
    __disable_irq();
    bufptr->count++;
    __enable_irq();
    bufptr->head++;
    bufptr->head &= BUFSIZE-1;
    len++;

    // Enable TXEIE interrupt
    *UARTS[port].TXEIE = true;
  }

  return len;
}

//*************** UART interrupt service routines follow ****************

#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define USART1_DR	(*((volatile unsigned *) 0x40011004))
#define USART1_RXNE	(*((volatile unsigned *) 0x42220014))
#define USART1_TXE	(*((volatile unsigned *) 0x4222001C))
#define USART1_RXNEIE	(*((volatile unsigned *) 0x42220194))
#define USART1_TXEIE	(*((volatile unsigned *) 0x4222019C))

void USART1_IRQHandler(void)
{
  if (USART1_RXNEIE && USART1_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[0].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      USART1_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = USART1_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (USART1_TXEIE && USART1_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[0].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      USART1_TXEIE = 0;	
    }
    else
    {
      USART1_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif

#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define USART2_DR	(*((volatile unsigned *) 0x40004404))
#define USART2_RXNE	(*((volatile unsigned *) 0x42088014))
#define USART2_TXE	(*((volatile unsigned *) 0x4208801C))
#define USART2_RXNEIE	(*((volatile unsigned *) 0x42088194))
#define USART2_TXEIE	(*((volatile unsigned *) 0x4208819C))

void USART2_IRQHandler(void)
{
  if (USART2_RXNEIE && USART2_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[1].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      USART2_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = USART2_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (USART2_TXEIE && USART2_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[1].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      USART2_TXEIE = 0;	
    }
    else
    {
      USART2_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define USART3_DR	(*((volatile unsigned *) 0x40004804))
#define USART3_RXNE	(*((volatile unsigned *) 0x42090014))
#define USART3_TXE	(*((volatile unsigned *) 0x4209001C))
#define USART3_RXNEIE	(*((volatile unsigned *) 0x42090194))
#define USART3_TXEIE	(*((volatile unsigned *) 0x4209019C))

void USART3_IRQHandler(void)
{
  if (USART3_RXNEIE && USART3_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[2].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      USART3_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = USART3_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (USART3_TXEIE && USART3_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[2].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      USART3_TXEIE = 0;	
    }
    else
    {
      USART3_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define UART4_DR	(*((volatile unsigned *) 0x40004C04))
#define UART4_RXNE	(*((volatile unsigned *) 0x42098014))
#define UART4_TXE	(*((volatile unsigned *) 0x4209801C))
#define UART4_RXNEIE	(*((volatile unsigned *) 0x42098194))
#define UART4_TXEIE	(*((volatile unsigned *) 0x4209819C))

void UART4_IRQHandler(void)
{
  if (UART4_RXNEIE && UART4_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[3].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      UART4_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = UART4_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (UART4_TXEIE && UART4_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[3].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      UART4_TXEIE = 0;	
    }
    else
    {
      UART4_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define UART5_DR	(*((volatile unsigned *) 0x40005004))
#define UART5_RXNE	(*((volatile unsigned *) 0x420A0014))
#define UART5_TXE	(*((volatile unsigned *) 0x420A001C))
#define UART5_RXNEIE	(*((volatile unsigned *) 0x420A0194))
#define UART5_TXEIE	(*((volatile unsigned *) 0x420A019C))

void UART5_IRQHandler(void)
{
  if (UART5_RXNEIE && UART5_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[4].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      UART5_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = UART5_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (UART5_TXEIE && UART5_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[4].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      UART5_TXEIE = 0;	
    }
    else
    {
      UART5_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif

#if defined(STM32F40_41xxx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define USART6_DR	(*((volatile unsigned *) 0x40011404))
#define USART6_RXNE	(*((volatile unsigned *) 0x42228014))
#define USART6_TXE	(*((volatile unsigned *) 0x4222801C))
#define USART6_RXNEIE	(*((volatile unsigned *) 0x42228194))
#define USART6_TXEIE	(*((volatile unsigned *) 0x4222819C))

void USART6_IRQHandler(void)
{
  if (USART6_RXNEIE && USART6_RXNE)
  {
    volatile ringbuffer_t *bufptr = UARTS[5].rxbuffer;

    if (bufptr->count == BUFSIZE)
    {
      // Receive buffer is full; discard overrun data
      USART6_DR;
    }
    else
    {
      bufptr->data[bufptr->head] = USART6_DR;
      bufptr->count++;
      bufptr->head++;
      bufptr->head &= BUFSIZE-1;
    }
  }

  if (USART6_TXEIE && USART6_TXE)
  {
    volatile ringbuffer_t *bufptr = UARTS[5].txbuffer;

    if (bufptr->count == 0)
    {
      // Transmit buffer is empty; disable interrupt
      USART6_TXEIE = 0;	
    }
    else
    {
      USART6_DR = bufptr->data[bufptr->tail];
      bufptr->count--;
      bufptr->tail++;
      bufptr->tail &= BUFSIZE-1;
    }
  }
}
#endif
