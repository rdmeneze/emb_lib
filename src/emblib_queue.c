#include "emblib_queue.h"
#include <assert.h>

bool emblib_queue_init(emblib_queue_t *queue, const void *array, const size_t buffer_len, const size_t size_elem,
                       void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data)) {
    return emblib_circ_buffer_init((emblib_circ_buffer_t *) queue, array, buffer_len, size_elem, copy_fn, free_fn);
}

void emblib_queue_flush(emblib_queue_t *queue) {
    emblib_circ_buffer_flush(queue);
}

size_t emblib_queue_size(emblib_queue_t *queue) {
    return emblib_circ_buffer_size((emblib_circ_buffer_t *) queue);
}

size_t emblib_queue_count(emblib_queue_t *queue) {
    return emblib_circ_buffer_count((emblib_circ_buffer_t *) queue);
}

bool emblib_queue_enqueue(emblib_queue_t *queue, void *data) {
    return emblib_circ_buffer_insert((emblib_circ_buffer_t *) queue, data);
}

bool emblib_queue_dequeue(emblib_queue_t *queue, void *data) {
    return emblib_circ_buffer_retrieve(queue, data);
}

bool emblib_queue_peek(emblib_queue_t *queue, void *data) {
    return emblib_circ_buffer_peek(queue, data);
}

bool emblib_queue_is_empty(emblib_queue_t *queue) {
    return emblib_circ_buffer_is_empty((emblib_circ_buffer_t *) queue);
}

bool emblib_queue_is_full(emblib_queue_t *queue) {
    return emblib_circ_buffer_is_full((emblib_circ_buffer_t *) queue);
}

bool emblib_queue_will_full(emblib_queue_t *queue, const int size) {
    return emblib_circ_buffer_will_full((emblib_circ_buffer_t *) queue, size);
}
