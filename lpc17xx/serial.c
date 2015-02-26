/******************************************************************************/
/*                                                                            */
/*          Simple serial port I/O routines for the LPC17xx ARM MCU           */
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

#define MAX_SERIAL_PORTS	4

static LPC_UART_TypeDef * const UARTS[MAX_SERIAL_PORTS] =
{
  (LPC_UART_TypeDef *) LPC_UART0,
  (LPC_UART_TypeDef *) LPC_UART1,
  (LPC_UART_TypeDef *) LPC_UART2,
  (LPC_UART_TypeDef *) LPC_UART3,
};

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
  else
  {
    errno_r = ENODEV;
    return -1;
  }
}

/* Initialize serial port */

int serial_open(char *name, unsigned int *subdevice)
{
  unsigned int port;
  unsigned int baudrate;
  unsigned short int divisor;

  errno_r = 0;

// Look up serial port number

  port = serial_name_to_port(name);
  if (port < 0) return port;

// Pass up port number, if requested

  if (subdevice != NULL)
    *subdevice = port;

// Extract baud rate from device name

  baudrate = atoi(name+5);

  switch (port)
  {
    case 0 :
      LPC_SC->PCONP |= (1 << 3);		// Power on UART 0
      LPC_SC->PCLKSEL0 &= 0xFFFFFF3F;		// CCLK/4
      LPC_PINCON->PINSEL0 &= 0xFFFFFF0F;	// Enable UART 0 I/O pins
      LPC_PINCON->PINSEL0 |= 0x00000050;
      break;

    case 1 :
      LPC_SC->PCONP |= (1 << 4);		// Power on UART 1
      LPC_SC->PCLKSEL0 &= 0xFFFFFCFF;		// CCLK/4
      LPC_PINCON->PINSEL0 &= 0x3FFFFFFF;	// Enable UART 1 I/O pins
      LPC_PINCON->PINSEL0 |= 0x40000000;
      LPC_PINCON->PINSEL1 &= 0xFFFFFFFC;
      LPC_PINCON->PINSEL1 |= 0x00000001;
      break;

    case 2 :
      LPC_SC->PCONP |= (1 << 24);		// Power on UART 2
      LPC_SC->PCLKSEL1 &= 0xFFFCFFFF;		// CCLK/4
      LPC_PINCON->PINSEL0 &= 0xFF0FFFFF;	// Enable UART 2 I/O pins
      LPC_PINCON->PINSEL0 |= 0x00500000;
      break;

    case 3 :
      LPC_SC->PCONP |= (1 << 25);		// Power on UART 3
      LPC_SC->PCLKSEL1 &= 0xFFF3FFFF;		// CCLK/4
      LPC_PINCON->PINSEL0 &= 0xFFFFFFF0;	// Enable UART 3 I/O pins
      LPC_PINCON->PINSEL0 |= 0x0000000A;
      break;

    default :
      errno_r = ENODEV;
      return -1;
  }

// Configure the serial port

  divisor = SystemCoreClock/108/baudrate;	// Calculate baud rate divisor

  UARTS[port]->FDR = 0xD9;			// Set fractional divider=1 9/13
  UARTS[port]->LCR = 0x83;			// Enable access to DLL and DLM
  UARTS[port]->DLM = divisor / 256;		// Set baud rate
  UARTS[port]->DLL = divisor % 256;
  UARTS[port]->LCR = 0x03;			// Always 8 bits no parity 1 stop
  UARTS[port]->FCR = 0x07;			// Enable and clear FIFO's
  UARTS[port]->IER = 0x00;			// Disable UART interrupts
  UARTS[port]->ACR = 0x00;			// Disable autobaud
  UARTS[port]->TER = 0x80;			// Enable transmitter

  return 0;
}

/* Register serial port for standard I/O */

int serial_stdio(char *name)
{
  unsigned int subdevice;

  if (serial_open(name, &subdevice))
    return -1;

  // Nuke existing stdin, stdout, stderr

  device_unregister(0);
  device_unregister(1);
  device_unregister(2);

  // Register new stdin, stdout, stderr

  device_register_char_fd(0, subdevice, NULL, serial_read, NULL, serial_rxready);
  device_register_char_fd(1, subdevice, serial_write, NULL, serial_txready, NULL);
  device_register_char_fd(2, subdevice, serial_write, NULL, serial_txready, NULL);

  return 0;
}

/* Register a serial port device */

int serial_register(char *name)
{
  unsigned int port;

// Look up serial port number

  port = serial_name_to_port(name);
  if (port < 0) return port;

// Register the serial port driver

  return device_register_char(name, port, serial_open, NULL, serial_write, serial_read, serial_txready, serial_rxready);
}

/* Return true if transmitter is ready to accept data */

int serial_txready(unsigned int port)
{
  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (UARTS[port]->LSR & 0x20)
    return 1;
  else
    return 0;
}

/* Send a buffer to the serial port */

int serial_write(unsigned int port, char *buf, unsigned int count)
{
  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (serial_txready(port))
  {
    UARTS[port]->THR = *buf++;
    return 1;
  }

  return 0;
}

/* Return true if receive data is available */

int serial_rxready(unsigned int port)
{
  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (UARTS[port]->LSR & 0x01)
    return 1;
  else
    return 0;
}

/* Read buffer from the serial port */

int serial_read(unsigned int port, char *buf, unsigned int count)
{
  errno_r = 0;

  if (port >= MAX_SERIAL_PORTS)
  {
    errno_r = ENODEV;
    return -1;
  }

  if (serial_rxready(port))
  {
    *buf++ = UARTS[port]->RBR;
    return 1;
  }

  return 0;
}
