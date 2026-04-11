#ifndef RELIABLE_UDP_PROTOCOL_H
#define RELIABLE_UDP_PROTOCOL_H

#include <stdint.h>

#define RUDP_TYPE_DATA 1
#define RUDP_TYPE_ACK 2
#define RUDP_MAX_PAYLOAD 256

typedef struct {
    uint8_t type;
    uint8_t seq;
    uint16_t length;
    uint8_t payload[RUDP_MAX_PAYLOAD];
} rudp_packet_t;

#endif
