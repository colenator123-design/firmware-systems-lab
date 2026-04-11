#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"

static int parse_port(const char *text)
{
    char *end = NULL;
    long port = strtol(text, &end, 10);
    if (end == text || *end != '\0' || port < 1 || port > 65535) {
        return -1;
    }
    return (int)port;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = parse_port(argv[1]);
    if (port == -1) {
        fprintf(stderr, "Invalid port: %s\n", argv[1]);
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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)port);

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    uint8_t expected_seq = 1;
    printf("Reliable UDP server listening on port %d\n", port);

    while (1) {
        rudp_packet_t packet;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ssize_t received =
            recvfrom(sock_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr,
                     &client_len);
        if (received == -1) {
            perror("recvfrom");
            continue;
        }

        if (packet.type != RUDP_TYPE_DATA) {
            continue;
        }

        printf("server received seq=%u len=%u\n", packet.seq, packet.length);

        rudp_packet_t ack;
        memset(&ack, 0, sizeof(ack));
        ack.type = RUDP_TYPE_ACK;
        ack.seq = packet.seq;

        if (sendto(sock_fd, &ack, sizeof(ack.type) + sizeof(ack.seq) + sizeof(ack.length), 0,
                   (struct sockaddr *)&client_addr, client_len) == -1) {
            perror("sendto");
            continue;
        }

        if (packet.seq == expected_seq) {
            if (packet.length <= RUDP_MAX_PAYLOAD) {
                packet.payload[packet.length] = '\0';
                printf("accepted data: %s\n", packet.payload);
            }
            expected_seq++;
        } else {
            printf("out-of-order packet: expected=%u got=%u\n", expected_seq, packet.seq);
        }
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}
