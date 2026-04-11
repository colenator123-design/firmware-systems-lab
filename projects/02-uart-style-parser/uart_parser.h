#ifndef UART_PARSER_H
#define UART_PARSER_H

#include <stddef.h>
#include <stdint.h>

#include "../01-ring-buffer/ring_buffer.h"

typedef struct {
    ring_buffer_t *rb;
    char *command_buffer;
    size_t command_capacity;
    size_t command_length;
} uart_parser_t;

int uart_parser_init(uart_parser_t *parser, ring_buffer_t *rb, char *command_buffer,
                     size_t command_capacity);
int uart_parser_feed_byte(uart_parser_t *parser, uint8_t byte);
int uart_parser_get_command(uart_parser_t *parser, char *out, size_t out_size);

#endif
