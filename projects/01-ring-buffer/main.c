#include <stdint.h>
#include <stdio.h>

#include "ring_buffer.h"

static void print_status(const ring_buffer_t *rb, const char *label)
{
    printf("%s: size=%zu capacity=%zu head=%zu tail=%zu\n", label,
           ring_buffer_size(rb), ring_buffer_capacity(rb), rb->head, rb->tail);
}

int main(void)
{
    uint8_t storage[5];
    ring_buffer_t rb;

    if (ring_buffer_init(&rb, storage, sizeof(storage)) != 0) {
        fprintf(stderr, "ring_buffer_init failed\n");
        return 1;
    }

    print_status(&rb, "init");

    for (uint8_t value = 1; value <= 5; value++) {
        if (ring_buffer_push(&rb, value) != 0) {
            fprintf(stderr, "push failed for value %u\n", value);
            return 1;
        }
        print_status(&rb, "after push");
    }

    if (ring_buffer_push(&rb, 99) != 0) {
        printf("push blocked when buffer is full, as expected\n");
    }

    for (int i = 0; i < 3; i++) {
        uint8_t value = 0;
        if (ring_buffer_pop(&rb, &value) != 0) {
            fprintf(stderr, "pop failed\n");
            return 1;
        }
        printf("popped value=%u\n", value);
        print_status(&rb, "after pop");
    }

    for (uint8_t value = 10; value <= 11; value++) {
        if (ring_buffer_push(&rb, value) != 0) {
            fprintf(stderr, "push failed for wrap-around value %u\n", value);
            return 1;
        }
        print_status(&rb, "after wrap push");
    }

    while (!ring_buffer_is_empty(&rb)) {
        uint8_t value = 0;
        if (ring_buffer_pop(&rb, &value) != 0) {
            fprintf(stderr, "final pop failed\n");
            return 1;
        }
        printf("drain value=%u\n", value);
    }

    print_status(&rb, "done");
    return 0;
}
