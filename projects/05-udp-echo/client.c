#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

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

    char buffer[BUFFER_SIZE];
    printf("Type a message and press Enter. Press Ctrl+D to quit.\n");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);

        if (sendto(sock_fd, buffer, len, 0, (struct sockaddr *)&server_addr,
                   sizeof(server_addr)) == -1) {
            perror("sendto");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        ssize_t received = recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (received == -1) {
            perror("recvfrom");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        buffer[received] = '\0';
        printf("Echo from server: %s", buffer);
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}
