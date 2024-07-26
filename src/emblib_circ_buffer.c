/**
 *  @file   emblib_circ_buffer.c
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#include "emblib_circ_buffer.h"
#include <assert.h>

bool emblib_circ_buffer_init(emblib_circ_buffer_t *circ_buffer, const void *array, const size_t buffer_len,
                             const size_t size_elem, void (*copy_fn)(void *dest, void *src),
                             void (*free_fn)(void *data)) {

    bool bRet = false;

    if (circ_buffer && array && buffer_len && size_elem && (buffer_len % size_elem == 0) && copy_fn) {

        if (circ_buffer) {
            *(circ_buffer) = (emblib_circ_buffer_t) {
                    .array      = (void *) array,
                    .capacity   = buffer_len,
                    .elem_size  = size_elem,
                    .head       = 0,
                    .tail       = 0,
                    .count      = 0,
                    .copy_fn = copy_fn,
                    .free_fn = free_fn
            };

            bRet = true;
        }
    }
    return bRet;
}

size_t emblib_circ_buffer_size(emblib_circ_buffer_t *circ_buffer) {
    return (circ_buffer) ? circ_buffer->capacity / circ_buffer->elem_size : 0;
}

size_t emblib_circ_buffer_capacity(emblib_circ_buffer_t *circ_buffer) {
    return (circ_buffer) ? circ_buffer->capacity : 0;
}

size_t emblib_circ_buffer_count(emblib_circ_buffer_t *circ_buffer) {
    return (circ_buffer) ? circ_buffer->count : 0;
}

bool emblib_circ_buffer_insert(emblib_circ_buffer_t *circ_buffer, void *data) {
    bool bRet = false;
    if (circ_buffer && data && !emblib_circ_buffer_is_full(circ_buffer)) {

        // save data into the list
        circ_buffer->copy_fn((char *) circ_buffer->array + (circ_buffer->tail * circ_buffer->elem_size), data);
        circ_buffer->tail = (circ_buffer->tail + 1) % emblib_circ_buffer_size(circ_buffer);
        circ_buffer->count++;
        bRet = true;
    }
    return bRet;
}

bool emblib_circ_buffer_insert_overwrite(emblib_circ_buffer_t *circ_buffer, void *data) {

    bool bRet = false;
    if (circ_buffer && data) {
        const size_t buff_size = emblib_circ_buffer_size(circ_buffer);
        if (emblib_circ_buffer_is_full(circ_buffer))
            circ_buffer->head = (circ_buffer->head + 1) % buff_size;
        else
            circ_buffer->count++;

        circ_buffer->copy_fn((char *) circ_buffer->array + (circ_buffer->tail * circ_buffer->elem_size), data);
        circ_buffer->tail = (circ_buffer->tail + 1) % buff_size;
        bRet = true;
    }
    return bRet;
}

bool emblib_circ_buffer_retrieve(emblib_circ_buffer_t *circ_buffer, void *data) {
    bool bRet = false;

    if (circ_buffer) {
        if (!emblib_circ_buffer_is_empty(circ_buffer)) {
            circ_buffer->copy_fn(data, (char *) circ_buffer->array + (circ_buffer->head * circ_buffer->elem_size));
            circ_buffer->head = (circ_buffer->head + 1) % emblib_circ_buffer_size(circ_buffer);
            circ_buffer->count--;
            bRet = true;
        }
    }

    return bRet;
}

bool emblib_circ_buffer_peek(emblib_circ_buffer_t *circ_buffer, void *data) {
    bool bRet = false;
    if (circ_buffer && circ_buffer->count) {
        circ_buffer->copy_fn(data, (char *) circ_buffer->array + (circ_buffer->head * circ_buffer->elem_size));
        bRet = true;
    }

    return bRet;
}

bool emblib_circ_buffer_is_empty(emblib_circ_buffer_t *circ_buffer) {
    return circ_buffer ? circ_buffer->count == 0 : false;
}

bool emblib_circ_buffer_is_full(emblib_circ_buffer_t *circ_buffer) {
    return circ_buffer ? circ_buffer->count == emblib_circ_buffer_size(circ_buffer) : false;
}

bool emblib_circ_buffer_will_full(emblib_circ_buffer_t *circ_buffer, const size_t size) {
    return (circ_buffer) ? (emblib_circ_buffer_count(circ_buffer) + size) > emblib_circ_buffer_size(circ_buffer)
                         : false;
}

void emblib_circ_buffer_flush(emblib_circ_buffer_t *circ_buffer) {
    if (circ_buffer) {
        circ_buffer->tail = circ_buffer->head = 0;
        if (circ_buffer->free_fn) {
            for (size_t i = 0; i < circ_buffer->count; i++) {
                circ_buffer->free_fn(circ_buffer->array + (i * circ_buffer->elem_size));
            }
        }
        circ_buffer->count = 0;
    }
}

