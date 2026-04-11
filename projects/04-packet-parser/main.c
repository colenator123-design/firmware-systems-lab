#include <stdint.h>
#include <stdio.h>

#include "packet_parser.h"

int main(void)
{
    const uint8_t sample_ipv4_packet[] = {
        0x45, 0x00, 0x00, 0x3C, 0x1C, 0x46, 0x40, 0x00, 0x40, 0x06,
        0xB1, 0xE6, 0xC0, 0xA8, 0x01, 0x64, 0x5D, 0xB8, 0xD8, 0x22};

    ipv4_header_t header;
    if (parse_ipv4_header(sample_ipv4_packet, sizeof(sample_ipv4_packet), &header) != 0) {
        fprintf(stderr, "parse_ipv4_header failed\n");
        return 1;
    }

    print_ipv4_header(&header);
    return 0;
}
