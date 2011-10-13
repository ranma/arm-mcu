/* WizNet Driver Function Prototypes */

// $Id$

#include <stdint.h>

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

typedef uint8_t macaddress_t[6];		// Ethernet MAC address

typedef uint8_t ipv4address_t[4];		// IPv4 address

void wiznet_tick(void);

int wiznet_initialize(const uint32_t spiportnum,
                      const uint32_t numsockets);

int wiznet_set_hardware_address(const macaddress_t address);

int wiznet_get_hardware_address(macaddress_t address);

int wiznet_configure_network(const ipv4address_t address,
                             const ipv4address_t subnet,
                             const ipv4address_t gateway);

int wiznet_get_ipaddress(ipv4address_t address);

int wiznet_get_linkstate(uint32_t *linkstate);

int wiznet_get_port(const uint32_t socket,
                    uint32_t *port);

int wiznet_get_receive_ready(const uint32_t socket,
                             uint32_t *count);

int wiznet_get_transmit_free(const uint32_t socket,
                             uint32_t *count);

int wiznet_read_receive_ram(const uint32_t socket,
                            uint32_t *rampointer,
                            void *dst,
                            const uint32_t count);

int wiznet_udp_open(const uint32_t socket,
                    const uint32_t port);

int wiznet_udp_receive_from(const uint32_t socket,
                            ipv4address_t srcaddr,
                            uint32_t *srcport,
                            void *buf,
                            uint32_t *count);

int wiznet_udp_send_to(const uint32_t socket,
                       const ipv4address_t destaddr,
                       const uint32_t destport,
                       void *buf,
                       const uint32_t count);

int wiznet_bootp(ipv4address_t address,
                 ipv4address_t subnet,
                 ipv4address_t gateway,
                 ipv4address_t nameserver);