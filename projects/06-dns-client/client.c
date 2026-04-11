#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DNS_PORT 53
#define BUFFER_SIZE 512

static size_t encode_dns_name(const char *domain, uint8_t *out)
{
    size_t out_index = 0;
    const char *label = domain;
    const char *cursor = domain;

    while (1) {
        if (*cursor == '.' || *cursor == '\0') {
            size_t label_len = (size_t)(cursor - label);
            out[out_index++] = (uint8_t)label_len;
            for (size_t i = 0; i < label_len; i++) {
                out[out_index++] = (uint8_t)label[i];
            }

            if (*cursor == '\0') {
                out[out_index++] = 0;
                break;
            }

            cursor++;
            label = cursor;
            continue;
        }
        cursor++;
    }

    return out_index;
}

static uint16_t read_u16_be(const uint8_t *buffer)
{
    return (uint16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
}

static uint32_t read_u32_be(const uint8_t *buffer)
{
    return ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) |
           ((uint32_t)buffer[2] << 8) | buffer[3];
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dns-server-ip> <domain>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid DNS server IPv4 address: %s\n", argv[1]);
        close(sock_fd);
        return EXIT_FAILURE;
    }

    uint8_t query[BUFFER_SIZE];
    memset(query, 0, sizeof(query));

    query[0] = 0x12;
    query[1] = 0x34;
    query[2] = 0x01;
    query[3] = 0x00;
    query[4] = 0x00;
    query[5] = 0x01;

    size_t index = 12;
    index += encode_dns_name(argv[2], &query[index]);
    query[index++] = 0x00;
    query[index++] = 0x01;
    query[index++] = 0x00;
    query[index++] = 0x01;

    if (sendto(sock_fd, query, index, 0, (struct sockaddr *)&server_addr,
               sizeof(server_addr)) == -1) {
        perror("sendto");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    uint8_t response[BUFFER_SIZE];
    ssize_t received = recvfrom(sock_fd, response, sizeof(response), 0, NULL, NULL);
    if (received == -1) {
        perror("recvfrom");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    if (received < 12) {
        fprintf(stderr, "DNS response too short\n");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    uint16_t answer_count = read_u16_be(&response[6]);
    printf("DNS answers: %u\n", answer_count);

    size_t offset = index;
    for (uint16_t i = 0; i < answer_count; i++) {
        if (offset + 12 > (size_t)received) {
            break;
        }

        if ((response[offset] & 0xC0) == 0xC0) {
            offset += 2;
        } else {
            while (offset < (size_t)received && response[offset] != 0) {
                offset += (size_t)response[offset] + 1;
            }
            offset++;
        }

        uint16_t type = read_u16_be(&response[offset]);
        offset += 2;
        uint16_t class_code = read_u16_be(&response[offset]);
        offset += 2;
        uint32_t ttl = read_u32_be(&response[offset]);
        offset += 4;
        uint16_t rdlength = read_u16_be(&response[offset]);
        offset += 2;

        if (offset + rdlength > (size_t)received) {
            break;
        }

        if (type == 1 && class_code == 1 && rdlength == 4) {
            char ip_text[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &response[offset], ip_text, sizeof(ip_text)) != NULL) {
                printf("A record: %s (ttl=%u)\n", ip_text, ttl);
            }
        }

        offset += rdlength;
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}
