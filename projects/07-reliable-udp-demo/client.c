#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
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

static int wait_for_ack(int sock_fd, uint8_t seq)
{
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int ready = select(sock_fd + 1, &read_fds, NULL, NULL, &timeout);
    if (ready <= 0) {
        return -1;
    }

    rudp_packet_t ack;
    ssize_t received = recvfrom(sock_fd, &ack, sizeof(ack), 0, NULL, NULL);
    if (received == -1) {
        return -1;
    }

    if (ack.type == RUDP_TYPE_ACK && ack.seq == seq) {
        return 0;
    }

    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-ip> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = parse_port(argv[2]);
    if (port == -1) {
        fprintf(stderr, "Invalid port: %s\n", argv[2]);
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
    server_addr.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4 address: %s\n", argv[1]);
        close(sock_fd);
        return EXIT_FAILURE;
    }

    uint8_t seq = 1;
    char input[RUDP_MAX_PAYLOAD];

    printf("Type a message and press Enter. Press Ctrl+D to quit.\n");

    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }

        rudp_packet_t packet;
        memset(&packet, 0, sizeof(packet));
        packet.type = RUDP_TYPE_DATA;
        packet.seq = seq;
        packet.length = (uint16_t)len;
        memcpy(packet.payload, input, len);

        int delivered = 0;
        for (int attempt = 1; attempt <= 3; attempt++) {
            printf("sending seq=%u attempt=%d\n", seq, attempt);

            size_t packet_size = sizeof(packet.type) + sizeof(packet.seq) +
                                 sizeof(packet.length) + len;
            if (sendto(sock_fd, &packet, packet_size, 0, (struct sockaddr *)&server_addr,
                       sizeof(server_addr)) == -1) {
                perror("sendto");
                close(sock_fd);
                return EXIT_FAILURE;
            }

            if (wait_for_ack(sock_fd, seq) == 0) {
                printf("ack received for seq=%u\n", seq);
                delivered = 1;
                break;
            }

            printf("ack timeout for seq=%u\n", seq);
        }

        if (!delivered) {
            fprintf(stderr, "failed to deliver seq=%u\n", seq);
            close(sock_fd);
            return EXIT_FAILURE;
        }

        seq++;
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}
