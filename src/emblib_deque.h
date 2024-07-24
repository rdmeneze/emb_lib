#ifndef __DEQUE_H__
#define __DEQUE_H__

#include "emblib_circ_buffer.h"

typedef emblib_circ_buffer_t emblib_deque_t;

bool emblib_deque_init(emblib_deque_t *deque, void *array, size_t buffer_len, size_t size_elem);

bool emblib_deque_push_front(emblib_deque_t *deque, void *data);

bool emblib_deque_push_back(emblib_deque_t *deque, void *data);

bool emblib_deque_pop_front(emblib_deque_t *deque, void *data);

bool emblib_deque_pop_back(emblib_deque_t *deque, void *data);

bool emblib_deque_peek_front(emblib_deque_t *deque, void *data);

bool emblib_deque_peek_back(emblib_deque_t *deque, void *data);

bool emblib_deque_is_empty(emblib_deque_t *deque);

bool emblib_deque_is_full(emblib_deque_t *deque);

size_t emblib_deque_size(emblib_deque_t *deque);

size_t emblib_deque_count(emblib_deque_t *deque);

#endif //~__DEQUE_H__
