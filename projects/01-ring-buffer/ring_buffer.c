#include "ring_buffer.h"

int ring_buffer_init(ring_buffer_t *rb, uint8_t *storage, size_t capacity)
{
    if (rb == 0 || storage == 0 || capacity == 0) {
        return -1;
    }

    rb->buffer = storage;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    return 0;
}

int ring_buffer_push(ring_buffer_t *rb, uint8_t value)
{
    if (rb == 0 || ring_buffer_is_full(rb)) {
        return -1;
    }

    rb->buffer[rb->head] = value;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return 0;
}

int ring_buffer_pop(ring_buffer_t *rb, uint8_t *value)
{
    if (rb == 0 || value == 0 || ring_buffer_is_empty(rb)) {
        return -1;
    }

    *value = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return 0;
}

int ring_buffer_is_empty(const ring_buffer_t *rb)
{
    return rb == 0 || rb->count == 0;
}

int ring_buffer_is_full(const ring_buffer_t *rb)
{
    return rb != 0 && rb->count == rb->capacity;
}

size_t ring_buffer_size(const ring_buffer_t *rb)
{
    if (rb == 0) {
        return 0;
    }
    return rb->count;
}

size_t ring_buffer_capacity(const ring_buffer_t *rb)
{
    if (rb == 0) {
        return 0;
    }
    return rb->capacity;
}
