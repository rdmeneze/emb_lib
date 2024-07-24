/**
 *  @file   emblib_circ_buffer.c
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#include "emblib_circ_buffer.h"
#include <string.h>
#include <assert.h>


/******************************************************************************/

bool emblib_circ_buffer_init(emblib_circ_buffer_t *circ_buffer, const void *array, const size_t buffer_len,
                             const size_t size_elem) {

    bool bRet = false;

    if ( circ_buffer && array && buffer_len && size_elem && (buffer_len % size_elem == 0)) {

        if (circ_buffer) {
            *(circ_buffer) = (emblib_circ_buffer_t) {
                    .array      = (void *) array,
                    .capacity   = buffer_len,
                    .size_elem  = size_elem,
                    .head       = 0,
                    .tail       = 0,
                    .is_full     = false,
                    .count      = 0
            };

            bRet = true;
        }
    }
    return bRet;
}

/******************************************************************************/

size_t emblib_circ_buffer_size(emblib_circ_buffer_t *circ_buffer) {
    return (circ_buffer) ? circ_buffer->capacity / circ_buffer->size_elem : 0;
}

/******************************************************************************/

size_t emblib_circ_buffer_capacity(emblib_circ_buffer_t *circ_buffer){
    return (circ_buffer) ? circ_buffer->capacity : 0;
}

/******************************************************************************/

size_t emblib_circ_buffer_count(emblib_circ_buffer_t *circ_buffer) {
    return (circ_buffer) ? circ_buffer->count : 0;
}

/******************************************************************************/

bool emblib_circ_buffer_insert(emblib_circ_buffer_t *circ_buffer, void *data) {
    bool bRet = false;
    if (circ_buffer && data && !circ_buffer->is_full) {

        // save data into the list
        memcpy((char *)circ_buffer->array + (circ_buffer->tail * circ_buffer->size_elem), data, circ_buffer->size_elem);

        circ_buffer->tail = (circ_buffer->tail + 1) % emblib_circ_buffer_size(circ_buffer);
        circ_buffer->count++;
        circ_buffer->is_full = circ_buffer->head == circ_buffer->tail;
        bRet = true;
    }
    return bRet;
}

/******************************************************************************/

bool emblib_circ_buffer_insert_overwrite(emblib_circ_buffer_t *circ_buffer, void *data) {

    bool bRet = false;
    if (circ_buffer && data) {
        const size_t buff_size = emblib_circ_buffer_size(circ_buffer);
        if(circ_buffer->is_full)
            circ_buffer->head = (circ_buffer->head + 1) % buff_size;
        else
            circ_buffer->count++;

        memcpy((char*)circ_buffer->array + circ_buffer->tail * circ_buffer->size_elem, data, circ_buffer->size_elem);
        circ_buffer->tail = (circ_buffer->tail + 1) % buff_size;
        circ_buffer->is_full = circ_buffer->head == circ_buffer->tail;
        bRet = true;
    }
    return bRet;
}

/******************************************************************************/

size_t emblib_circ_buffer_retrieve(emblib_circ_buffer_t *circ_buffer, void *data) {

    size_t count = 0;

    if (circ_buffer != NULL) {
        if(circ_buffer->count){
            memcpy(data, (char*)circ_buffer->array + (circ_buffer->head * circ_buffer->size_elem ), circ_buffer->size_elem);
            circ_buffer->head = (circ_buffer->head + 1) % emblib_circ_buffer_size(circ_buffer);
            circ_buffer->count--;
            circ_buffer->is_full = false;
            count = 1;
        }
    }

    return count;
}

bool emblib_circ_buffer_peek(emblib_circ_buffer_t *circ_buffer, void *data){
    bool bRet = false;
    if(circ_buffer && circ_buffer->count){
        memcpy(data, circ_buffer->array + (circ_buffer->head * circ_buffer->size_elem), circ_buffer->size_elem);
        bRet = true;
    }

    return bRet;
}

bool emblib_circ_buffer_is_empty(emblib_circ_buffer_t *circ_buffer) {

    bool bRet = false;
    if (circ_buffer) {
        bRet = (circ_buffer->count == 0);
    }
    return bRet;
}

/******************************************************************************/

bool emblib_circ_buffer_is_full(emblib_circ_buffer_t *circ_buffer) {
    bool bRet = false;
    if (circ_buffer) {
        bRet = circ_buffer->is_full;
    }
    return bRet;
}

/******************************************************************************/

bool emblib_circ_buffer_will_full(emblib_circ_buffer_t *circ_buffer, const size_t size) {
    return (circ_buffer) ? (emblib_circ_buffer_count(circ_buffer) + size)  > emblib_circ_buffer_size(circ_buffer) : false;
}

/******************************************************************************/

void emblib_circ_buffer_flush(emblib_circ_buffer_t *circ_buffer) {
    if (circ_buffer) {
        circ_buffer->tail = circ_buffer->head = 0;
        circ_buffer->is_full = false;
        circ_buffer->count = 0;
    }
}

/******************************************************************************/

