/** 
 *  @file   emblib_circ_buffer.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#ifndef __EMBLIB_CIRC_BUFFER_H__
#define    __EMBLIB_CIRC_BUFFER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//! @struct emblib_circ_buffer_t
typedef struct emblib_circ_buffer_t {
    void *array;      //!< array pointer elements
    size_t items;      //!< total items
    size_t size;       //!< size of circ_buffer array
    size_t head;       //!< head element of the circ_buffer
    size_t tail;       //!< tail element of the circ_buffer
    size_t size_elem;  //!< size of each element
    bool isfull;
} emblib_circ_buffer_t;

/**
 * @brief   initialize the circ_buffer
 * @param   circ_buffer pointer to the circ_buffer object
 * @returns  true on success 
 * @returns  false on fail
 */
bool emblib_circ_buffer_init(emblib_circ_buffer_t *circ_buffer, const void *array, const size_t buffer_len,
                             const size_t size_elem);

/**
 * @brief   size of the circ_buffer
 * @param   circ_buffer pointer to the circ_buffer object
 * @return size of the circ_buffer
 */
size_t emblib_circ_buffer_size(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief   count the elements of the circ_buffer
 * @param   circ_buffer pointer to the circ_buffer object
 * @return  elements saved on the circ_buffer
 */
size_t emblib_circ_buffer_count(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief       insert a element into the circ_buffer. If the buffer is full, do nothing
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @param[in]   data pointer to data to be added to circ_buffer
 * @returns     true on success 
 * @returns     false on fail
 */
bool emblib_circ_buffer_insert(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief       insert a element into the circ_buffer. If the buffer is full, overwrite the beginning of the queue
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @param[in]   data pointer to data to be added to circ_buffer
 * @returns     true on success
 * @returns     false on fail
 */
bool emblib_circ_buffer_insert_overwrite(emblib_circ_buffer_t *circ_buffer, void *data);


/**
 * @brief           get a element from the circ_buffer
 * @param[in]       circ_buffer    pointer to the circ_buffer object
 * @param[in,out]   data    pointer to data to be retrieve from circ_buffer
 * @returns         number of elements retrieved from circ_buffer
 */
size_t emblib_circ_buffer_retrieve(emblib_circ_buffer_t *circ_buffer, void *data);

/**
 * @brief       return if the circ_buffer is empty
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @returns     TRUE if the circ_buffer is empty
 * @returns     FALSE if the circ_buffer is not empty
 */
bool emblib_circ_buffer_is_empty(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief       return if the circ_buffer is full
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @returns     TRUE if the circ_buffer is full
 * @returns     FALSE if the circ_buffer is not full
 */
bool emblib_circ_buffer_is_full(emblib_circ_buffer_t *circ_buffer);

/**
 * @brief       return if the circ_buffer will be full
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @param[in]   size of data to be inserted 
 * @returns     TRUE if the circ_buffer will be full
 * @returns     FALSE if the circ_buffer will be not full
 */
bool emblib_circ_buffer_will_full(emblib_circ_buffer_t *circ_buffer, const size_t size);

/**
 * @brief       flush the circ_buffer
 * @param[in]   circ_buffer pointer to the circ_buffer object
 * @returns     TRUE on success
 * @returns     FALSE on fail
 */
bool emblib_circ_buffer_flush(emblib_circ_buffer_t *circ_buffer);

#endif    /* EMBLIB_CIRC_BUFFER_H */

