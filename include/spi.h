/* Simple SPI master routines */

// $Id$

#include <stdint.h>

#define SPI_BIGENDIAN		1
#define SPI_LITTLEENDIAN	0

// Initialize SPI port for bidirection master mode

int spimaster_init(uint32_t port,
                   uint32_t clockmode,
                   uint32_t speed,
                   uint32_t bigendian);

// Transmit command and/or receive result in bidirectional master mode

int spimaster_transfer(uint32_t port,
                       uint8_t *txbuf,
                       uint32_t txcount,
                       uint8_t *rxbuf,
                       uint32_t rxcount);