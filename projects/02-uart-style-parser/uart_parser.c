#include "uart_parser.h"

static int is_end_of_command(uint8_t byte)
{
    return byte == '\n';
}

int uart_parser_init(uart_parser_t *parser, ring_buffer_t *rb, char *command_buffer,
                     size_t command_capacity)
{
    if (parser == 0 || rb == 0 || command_buffer == 0 || command_capacity == 0) {
        return -1;
    }

    parser->rb = rb;
    parser->command_buffer = command_buffer;
    parser->command_capacity = command_capacity;
    parser->command_length = 0;
    return 0;
}

int uart_parser_feed_byte(uart_parser_t *parser, uint8_t byte)
{
    if (parser == 0) {
        return -1;
    }

    return ring_buffer_push(parser->rb, byte);
}

int uart_parser_get_command(uart_parser_t *parser, char *out, size_t out_size)
{
    if (parser == 0 || out == 0 || out_size == 0) {
        return -1;
    }

    while (!ring_buffer_is_empty(parser->rb)) {
        uint8_t byte = 0;
        if (ring_buffer_pop(parser->rb, &byte) != 0) {
            return -1;
        }

        if (byte == '\r') {
            continue;
        }

        if (is_end_of_command(byte)) {
            if (parser->command_length >= out_size) {
                return -1;
            }

            parser->command_buffer[parser->command_length] = '\0';
            for (size_t i = 0; i <= parser->command_length; i++) {
                out[i] = parser->command_buffer[i];
            }
            parser->command_length = 0;
            return 1;
        }

        if (parser->command_length + 1 >= parser->command_capacity) {
            parser->command_length = 0;
            return -1;
        }

        parser->command_buffer[parser->command_length] = (char)byte;
        parser->command_length++;
    }

    return 0;
}
