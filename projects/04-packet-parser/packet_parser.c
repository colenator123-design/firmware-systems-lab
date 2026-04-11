#include "packet_parser.h"

#include <stdio.h>

static uint16_t read_u16_be(const uint8_t *buffer)
{
    return (uint16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
}

static uint32_t read_u32_be(const uint8_t *buffer)
{
    return ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) |
           ((uint32_t)buffer[2] << 8) | buffer[3];
}

int parse_ipv4_header(const uint8_t *buffer, size_t len, ipv4_header_t *header)
{
    if (buffer == 0 || header == 0 || len < 20) {
        return -1;
    }

    header->version = (uint8_t)(buffer[0] >> 4);
    header->ihl = (uint8_t)(buffer[0] & 0x0F);
    header->tos = buffer[1];
    header->total_length = read_u16_be(&buffer[2]);
    header->identification = read_u16_be(&buffer[4]);
    header->flags_fragment_offset = read_u16_be(&buffer[6]);
    header->ttl = buffer[8];
    header->protocol = buffer[9];
    header->header_checksum = read_u16_be(&buffer[10]);
    header->src_ip = read_u32_be(&buffer[12]);
    header->dst_ip = read_u32_be(&buffer[16]);
    return 0;
}

static void print_ip(uint32_t ip)
{
    printf("%u.%u.%u.%u", (unsigned int)((ip >> 24) & 0xFF),
           (unsigned int)((ip >> 16) & 0xFF), (unsigned int)((ip >> 8) & 0xFF),
           (unsigned int)(ip & 0xFF));
}

void print_ipv4_header(const ipv4_header_t *header)
{
    if (header == 0) {
        return;
    }

    printf("version: %u\n", header->version);
    printf("ihl: %u\n", header->ihl);
    printf("tos: %u\n", header->tos);
    printf("total_length: %u\n", header->total_length);
    printf("identification: %u\n", header->identification);
    printf("flags_fragment_offset: %u\n", header->flags_fragment_offset);
    printf("ttl: %u\n", header->ttl);
    printf("protocol: %u\n", header->protocol);
    printf("header_checksum: %u\n", header->header_checksum);
    printf("src_ip: ");
    print_ip(header->src_ip);
    printf("\n");
    printf("dst_ip: ");
    print_ip(header->dst_ip);
    printf("\n");
}
