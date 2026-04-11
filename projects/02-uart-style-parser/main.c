#include <stdint.h>
#include <stdio.h>

#include "../01-ring-buffer/ring_buffer.h"
#include "uart_parser.h"

static void feed_stream(uart_parser_t *parser, const char *text)
{
    while (*text != '\0') {
        if (uart_parser_feed_byte(parser, (uint8_t)*text) != 0) {
            printf("ring buffer full while feeding byte '%c'\n", *text);
            return;
        }
        text++;
    }
}

int main(void)
{
    uint8_t storage[32];
    ring_buffer_t rb;
    char parser_storage[32];
    char output[32];
    uart_parser_t parser;

    if (ring_buffer_init(&rb, storage, sizeof(storage)) != 0) {
        fprintf(stderr, "ring_buffer_init failed\n");
        return 1;
    }

    if (uart_parser_init(&parser, &rb, parser_storage, sizeof(parser_storage)) != 0) {
        fprintf(stderr, "uart_parser_init failed\n");
        return 1;
    }

    feed_stream(&parser, "LED ON\nTEMP?\nMODE AUTO\n");

    while (1) {
        int status = uart_parser_get_command(&parser, output, sizeof(output));
        if (status == -1) {
            fprintf(stderr, "parser error\n");
            return 1;
        }
        if (status == 0) {
            break;
        }
        printf("command: %s\n", output);
    }

    feed_stream(&parser, "SET RATE 10\r\n");
    if (uart_parser_get_command(&parser, output, sizeof(output)) == 1) {
        printf("command: %s\n", output);
    }

    return 0;
}
