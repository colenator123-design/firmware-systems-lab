#include "framed_parser.h"

static void reset_parser(framed_parser_t *parser)
{
    parser->state = PARSER_WAIT_SOF;
    parser->current.type = 0;
    parser->current.length = 0;
    parser->current.checksum = 0;
    parser->payload_index = 0;
}

uint8_t framed_parser_compute_checksum(uint8_t type, uint8_t length, const uint8_t *payload)
{
    uint8_t checksum = (uint8_t)(type ^ length);

    for (uint8_t i = 0; i < length; i++) {
        checksum ^= payload[i];
    }

    return checksum;
}

int framed_parser_init(framed_parser_t *parser, ring_buffer_t *rb)
{
    if (parser == 0 || rb == 0) {
        return -1;
    }

    parser->rb = rb;
    reset_parser(parser);
    return 0;
}

int framed_parser_feed_byte(framed_parser_t *parser, uint8_t byte)
{
    if (parser == 0) {
        return -1;
    }

    return ring_buffer_push(parser->rb, byte);
}

int framed_parser_next_frame(framed_parser_t *parser, frame_t *out_frame)
{
    if (parser == 0 || out_frame == 0) {
        return -1;
    }

    while (!ring_buffer_is_empty(parser->rb)) {
        uint8_t byte = 0;
        if (ring_buffer_pop(parser->rb, &byte) != 0) {
            return -1;
        }

        switch (parser->state) {
        case PARSER_WAIT_SOF:
            if (byte == FRAME_SOF) {
                parser->state = PARSER_WAIT_TYPE;
            }
            break;

        case PARSER_WAIT_TYPE:
            parser->current.type = byte;
            parser->state = PARSER_WAIT_LENGTH;
            break;

        case PARSER_WAIT_LENGTH:
            if (byte > FRAME_MAX_PAYLOAD) {
                reset_parser(parser);
                return -1;
            }
            parser->current.length = byte;
            parser->payload_index = 0;
            parser->state = (byte == 0) ? PARSER_WAIT_CHECKSUM : PARSER_WAIT_PAYLOAD;
            break;

        case PARSER_WAIT_PAYLOAD:
            parser->current.payload[parser->payload_index++] = byte;
            if (parser->payload_index == parser->current.length) {
                parser->state = PARSER_WAIT_CHECKSUM;
            }
            break;

        case PARSER_WAIT_CHECKSUM: {
            uint8_t expected = framed_parser_compute_checksum(
                parser->current.type, parser->current.length, parser->current.payload);
            if (byte != expected) {
                reset_parser(parser);
                return -1;
            }

            parser->current.checksum = byte;
            *out_frame = parser->current;
            reset_parser(parser);
            return 1;
        }
        }
    }

    return 0;
}
