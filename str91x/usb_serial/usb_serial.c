/* USB serial port library encapsulation routines */

// $Id$

// Copyright (C)2013-2014, Philip Munts, President, Munts AM Corp.
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
#include <string.h>
#include <91x_lib.h>
#include <usb_lib.h>
#include <usb_desc.h>
#include <usb_pwr.h>
#include <usb_serial.h>

extern void USB_Istr(void);

#define RINGBUFFER_SIZE 256

static volatile boolean txready = TRUE;

// Define a ring buffer data structure

typedef struct
{
  int head;
  int tail;
  int count;
  char data[RINGBUFFER_SIZE];
} ringbuffer_t;

static volatile ringbuffer_t rxbuf;

// Enqueue data into a ring buffer

static int ringbuffer_enqueue(char *src, ringbuffer_t *dst, int count)
{
  int result = 0;

  while ((dst->count < RINGBUFFER_SIZE) && count--)
  {
    dst->data[dst->tail++] = *src++;
    if (dst->tail == RINGBUFFER_SIZE) dst->tail = 0;
    dst->count++;
    result++;
  }

  return result;
}

// Dequeue data from a ring buffer

static int ringbuffer_dequeue(ringbuffer_t *src, char *dst, int count)
{
  int result = 0;

  while ((src->count > 0) && count--)
  {
    *dst++ = src->data[src->head++];
    if (src->head == RINGBUFFER_SIZE) src->head = 0;
    DISABLE_INTERRUPTS(IRQ);
    src->count--;
    ENABLE_INTERRUPTS(IRQ);
    result++;
  }

  return result;
}

// Initialize USB subsystem

int usb_serial_open(char *name, unsigned int *subdevice)
{
  GPIO_InitTypeDef config_gpio;

// Initialize transmit done flag

  txready = TRUE;

// Initialize the receive ring buffer

  memset((ringbuffer_t *) &rxbuf, 0, sizeof(ringbuffer_t));

// Enable VIC subsystem

  SCU_AHBPeriphClockConfig(__VIC,ENABLE);
  SCU_AHBPeriphReset(__VIC,DISABLE);

// Enable GPIO0 subsystem

  SCU_APBPeriphClockConfig(__GPIO0, ENABLE);
  SCU_APBPeriphReset(__GPIO0, DISABLE);

// Configure P0.1 to control USB D+ pullup resistor

  GPIO_DeInit(GPIO0);
  config_gpio.GPIO_Direction = GPIO_PinOutput;
  config_gpio.GPIO_Pin = GPIO_Pin_1;
  config_gpio.GPIO_Type = GPIO_Type_PushPull;
  config_gpio.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
  config_gpio.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init(GPIO0, &config_gpio);

// Enable USB subsystem

  SCU_AHBPeriphClockConfig(__USB,ENABLE);
  SCU_AHBPeriphReset(__USB,DISABLE);
  SCU_AHBPeriphClockConfig(__USB48M,ENABLE);

// Configure USB interrupt

  DISABLE_INTERRUPTS(IRQ);

  VIC_Config(USBLP_ITLine, VIC_IRQ, 1);
  VIC_ITCmd(USBLP_ITLine, ENABLE);

  ENABLE_INTERRUPTS(IRQ);

// Initialize USB subsystem

  USB_Init();
  return 0;
}

// Register USB serial port device for standard I/O

int usb_serial_stdio(char *name)
{
  unsigned int subdevice;

  errno_r = 0;

  if (usb_serial_open(name, &subdevice))
    return -1;

  // Nuke existing stdin, stdout, stderr

  device_unregister(0);
  device_unregister(1);
  device_unregister(2);

  // Register new stdin, stdout, stderr

  device_register_char_fd(0, subdevice, NULL, usb_serial_read, NULL, usb_serial_rxready);
  device_register_char_fd(1, subdevice, usb_serial_write, NULL, usb_serial_txready, NULL);
  device_register_char_fd(2, subdevice, usb_serial_write, NULL, usb_serial_txready, NULL);

  return 0;
}

// Register USB serial port device

int usb_serial_register(char *name)
{
  return device_register_char(name, 0, usb_serial_open, NULL, usb_serial_write, usb_serial_read, usb_serial_txready, usb_serial_rxready);
}

// Return TRUE if USB system is ready to accept another transmit message

int usb_serial_txready(unsigned subdevice)
{
  if (!(GetDADDR() & DADDR_EF)) return FALSE;
  if (!(GetDADDR() & DADDR_ADD)) return FALSE;
  return txready;
}

// Return TRUE if USB subsystem has receive data available

int usb_serial_rxready(unsigned subdevice)
{
  if (!(GetDADDR() & DADDR_EF)) return FALSE;
  if (!(GetDADDR() & DADDR_ADD)) return FALSE;
  return (rxbuf.count > 0);
}

// Send data to USB host

int usb_serial_write(unsigned subdevice, char *buf, unsigned int count)
{
  if (!usb_serial_txready(subdevice)) return 0;

  if (count > VIRTUAL_COM_PORT_DATA_SIZE)
    count = VIRTUAL_COM_PORT_DATA_SIZE;

  UserToPMABufferCopy((unsigned char *) buf, ENDP1_TXADDR, count);
  SetEPTxCount(ENDP1, count);
  SetEPTxValid(ENDP1);

  txready = FALSE;
  return count;
}

// Receive data from USB host

int usb_serial_read(unsigned subdevice, char *buf, unsigned int count)
{
  return ringbuffer_dequeue((ringbuffer_t *) &rxbuf, buf, count);
}

// Connect or disconnect D+ pullup resistor

void USB_Cable_Config(FunctionalState NewState)
{
  if (NewState == ENABLE)
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_RESET);
  else
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_SET);
}

// USB Low Priority Interrupt Service Routine

__attribute__ ((__interrupt__)) void USBLP_IRQHandler(void)
{
  USB_Istr();		// Call USB ISR
  VIC0->VAR = 0;	// End of interrupt
}

// This function is called from the USB ISR when data has been received from the USB host

void EP3_OUT_Callback(void)
{
  ringbuffer_enqueue((char *) (PMAAddr + ENDP3_RXADDR), (ringbuffer_t *) &rxbuf, GetEPRxCount(ENDP3));
  SetEPRxValid(ENDP3);
}

// This function is called from the USB ISR when data has been sent to the USB host

void EP1_IN_Callback(void)
{
  txready = TRUE;
}

void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}
