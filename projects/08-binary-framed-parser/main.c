#include <stdint.h>
#include <stdio.h>

#include "../01-ring-buffer/ring_buffer.h"
#include "framed_parser.h"

static void feed_stream(framed_parser_t *parser, const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (framed_parser_feed_byte(parser, data[i]) != 0) {
            printf("buffer full while feeding byte index %zu\n", i);
            return;
        }
    }
}

static void print_frame(const frame_t *frame)
{
    printf("frame type=0x%02X length=%u payload=", frame->type, frame->length);
    for (uint8_t i = 0; i < frame->length; i++) {
        printf("%02X ", frame->payload[i]);
    }
    printf("checksum=0x%02X\n", frame->checksum);
}

int main(void)
{
    uint8_t storage[128];
    ring_buffer_t rb;
    framed_parser_t parser;
    frame_t frame;

    if (ring_buffer_init(&rb, storage, sizeof(storage)) != 0) {
        fprintf(stderr, "ring_buffer_init failed\n");
        return 1;
    }

    if (framed_parser_init(&parser, &rb) != 0) {
        fprintf(stderr, "framed_parser_init failed\n");
        return 1;
    }

    const uint8_t valid_frame_1[] = {0xAA, 0x10, 0x03, 0x01, 0x02, 0x03, 0x13};
    const uint8_t noise_and_valid[] = {0x00, 0xFF, 0xAA, 0x20, 0x02, 0x05, 0x06, 0x21};
    const uint8_t invalid_checksum[] = {0xAA, 0x30, 0x01, 0x09, 0x00};

    feed_stream(&parser, valid_frame_1, sizeof(valid_frame_1));
    if (framed_parser_next_frame(&parser, &frame) == 1) {
        print_frame(&frame);
    }

    feed_stream(&parser, noise_and_valid, sizeof(noise_and_valid));
    if (framed_parser_next_frame(&parser, &frame) == 1) {
        print_frame(&frame);
    }

    feed_stream(&parser, invalid_checksum, sizeof(invalid_checksum));
    if (framed_parser_next_frame(&parser, &frame) == -1) {
        printf("detected invalid checksum as expected\n");
    }

    return 0;
}
