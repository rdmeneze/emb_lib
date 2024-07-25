/**
 *  @file   emblib_circ_buffer.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#ifndef __EMBLIB_CIRC_BUFFER_H__
#define __EMBLIB_CIRC_BUFFER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//! @struct emblib_circ_buffer_t
typedef struct emblib_circ_buffer_t {
    void *array;        //!< array pointer elements
    size_t count;       //!< total count
    size_t capacity;    //!< capacity of circ_buffer array
    size_t head;        //!< head element of the circ_buffer
    size_t tail;        //!< tail element of the circ_buffer
    size_t elem_size;   //!< size of each element
} emblib_circ_buffer_t;

/**
 * @brief   initialize the circ_buffer
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @returns  true on success, false other else
 */
bool emblib_circ_buffer_init(emblib_circ_buffer_t *circ_buffer, const void *array, const size_t buffer_len,
                             const size_t size_elem);

/**
 * @brief   size in elements of circ_buffer
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @return capacity of the circ_buffer in elements
 */
size_t emblib_circ_buffer_size(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief maximum buffer capacity (circular buffer maximum size in bytes)
 * @param[in,out] circ_buffer pointer to the circ_buffer object
 * @return circular buffer capacity
 * */
size_t emblib_circ_buffer_capacity(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief   read the elements of circ_buffer
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @return  elements saved into circ_buffer
 */
size_t emblib_circ_buffer_count(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief       insert a element into the circ_buffer. If the buffer is full, do nothing
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @param[in]   data pointer to data to be added to circ_buffer
 * @return     true on success, false on fail
 */
bool emblib_circ_buffer_insert(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief       insert a element into the circ_buffer. If the buffer is full, overwrite the beginning of the queue
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @param[in]   data pointer to data to be added to circ_buffer
 * @return     true on success, false on fail
 */
bool emblib_circ_buffer_insert_overwrite(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief           get a element from the circ_buffer
 * @param[in,out]   circ_buffer    pointer to the circ_buffer object
 * @param[out]   data    pointer to data to be retrieved from circ_buffer
 * @return          true on success, false on fail
 */
bool emblib_circ_buffer_retrieve(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief           get a element from the circ_buffer without remove it from the buffer
 * @param[in,out]   circ_buffer    pointer to the circ_buffer object
 * @param[out]      data    pointer to data to be read from circ_buffer
 * @return          true on success, false on fail
 */
bool emblib_circ_buffer_peek(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief           return if the circ_buffer is empty
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @return          true for empty, false for not empty
 */
bool emblib_circ_buffer_is_empty(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief           return if the circ_buffer is full
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @return          true for full, false for not full
 */
bool emblib_circ_buffer_is_full(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief           return if the circ_buffer will be full
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @param[in]       size of data to be inserted
 * @return          true for will full, false for will not full
 */
bool emblib_circ_buffer_will_full(emblib_circ_buffer_t *circ_buffer, const size_t size);

/**
 * @brief           flush the circ_buffer
 * @param[in,out]   circ_buffer pointer to the circ_buffer object
 * @return          true on success, false on fail
 */
void emblib_circ_buffer_flush(emblib_circ_buffer_t *circ_buffer);

#endif    /* EMBLIB_CIRC_BUFFER_H */

