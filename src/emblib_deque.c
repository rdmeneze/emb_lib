#include "emblib_deque.h"
#include "emblib_circ_buffer.h"
#include <string.h>

bool emblib_deque_init(emblib_deque_t *deque, void *array, size_t buffer_len, size_t size_elem,
                       void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data)) {
    return emblib_circ_buffer_init(deque, array, buffer_len, size_elem, copy_fn, free_fn);
}

void emblib_deque_flush(emblib_deque_t *deque) {
    emblib_circ_buffer_flush(deque);
}

bool emblib_deque_push_front(emblib_deque_t *deque, void *data) {
    bool bRet = false;
    if (!emblib_deque_is_full(deque)) {
        const size_t size = emblib_deque_size(deque);
        deque->head = (deque->head - 1 + size) % size;
        deque->copy_fn((char *) deque->array + deque->head * deque->elem_size, data);
        deque->count++;

        bRet = true;
    }
    return bRet;
}

bool emblib_deque_push_back(emblib_deque_t *deque, void *data) {
    return emblib_circ_buffer_insert(deque, data);
}

bool emblib_deque_pop_front(emblib_deque_t *deque, void *data) {
    return emblib_circ_buffer_retrieve(deque, data);
}

bool emblib_deque_pop_back(emblib_deque_t *deque, void *data) {
    bool bRet = false;
    if (!emblib_deque_is_empty(deque)) {
        const size_t size = emblib_deque_size(deque);
        deque->tail = (deque->tail - 1 + size) % size;
        memcpy(data, (char *) deque->array + deque->tail * deque->elem_size, deque->elem_size);
        deque->count--;
        bRet = true;
    }
    return bRet;
}

bool emblib_deque_peek_front(emblib_deque_t *deque, void *data) {
    bool bRet = false;
    if (!emblib_deque_is_empty(deque)) {
        memcpy(data, (char *) deque->array + deque->head * deque->elem_size, deque->elem_size);
        bRet = true;

    }
    return bRet;
}

bool emblib_deque_peek_back(emblib_deque_t *deque, void *data) {
    bool bRet = false;
    if (!emblib_deque_is_empty(deque)) {
        const size_t size = emblib_deque_size(deque);
        size_t back_index = (deque->tail - 1 + size) % size;
        memcpy(data, (char *) deque->array + back_index * deque->elem_size, deque->elem_size);
        bRet = true;
    }
    return bRet;
}

bool emblib_deque_is_empty(emblib_deque_t *deque) {
    return emblib_circ_buffer_is_empty(deque);
}

bool emblib_deque_is_full(emblib_deque_t *deque) {
    return emblib_circ_buffer_is_full(deque);
}

size_t emblib_deque_size(emblib_deque_t *deque) {
    return emblib_circ_buffer_size(deque);
}

size_t emblib_deque_count(emblib_deque_t *deque) {
    return emblib_circ_buffer_count(deque);
}
