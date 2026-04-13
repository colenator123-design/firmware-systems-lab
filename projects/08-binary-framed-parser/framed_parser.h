#ifndef FRAMED_PARSER_H
#define FRAMED_PARSER_H

#include <stddef.h>
#include <stdint.h>

#include "../01-ring-buffer/ring_buffer.h"

#define FRAME_SOF 0xAA
#define FRAME_MAX_PAYLOAD 64

typedef enum {
    PARSER_WAIT_SOF = 0,
    PARSER_WAIT_TYPE,
    PARSER_WAIT_LENGTH,
    PARSER_WAIT_PAYLOAD,
    PARSER_WAIT_CHECKSUM
} parser_state_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t payload[FRAME_MAX_PAYLOAD];
    uint8_t checksum;
} frame_t;

typedef struct {
    ring_buffer_t *rb;
    parser_state_t state;
    frame_t current;
    uint8_t payload_index;
} framed_parser_t;

int framed_parser_init(framed_parser_t *parser, ring_buffer_t *rb);
int framed_parser_feed_byte(framed_parser_t *parser, uint8_t byte);
int framed_parser_next_frame(framed_parser_t *parser, frame_t *out_frame);
uint8_t framed_parser_compute_checksum(uint8_t type, uint8_t length, const uint8_t *payload);

#endif
