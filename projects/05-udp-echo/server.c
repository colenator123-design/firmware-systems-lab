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

    printf("UDP echo server listening on port %d\n", port);

    while (1) {
        char buffer[BUFFER_SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ssize_t received =
            recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client_addr, &client_len);
        if (received == -1) {
            perror("recvfrom");
            continue;
        }

        buffer[received] = '\0';
        printf("Received from %s:%d -> %s", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port), buffer);

        if (sendto(sock_fd, buffer, (size_t)received, 0,
                   (struct sockaddr *)&client_addr, client_len) == -1) {
            perror("sendto");
        }
    }

    close(sock_fd);
    return EXIT_SUCCESS;
}
