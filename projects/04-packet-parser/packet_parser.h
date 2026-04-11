#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t version;
    uint8_t ihl;
    uint8_t tos;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} ipv4_header_t;

int parse_ipv4_header(const uint8_t *buffer, size_t len, ipv4_header_t *header);
void print_ipv4_header(const ipv4_header_t *header);

#endif
