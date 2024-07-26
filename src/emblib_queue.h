/**
 *  @file   emblib_queue.h
 *  @brief  interface for queue module
 *  @author Rafael Dias <rdmeneze@gmail.com>
 */

#ifndef __EMBLIB_QUEUE_H__
#define __EMBLIB_QUEUE_H__

#include "emblib_circ_buffer.h"

typedef emblib_circ_buffer_t emblib_queue_t; //!< emblib_queue_t declaration

/**
 *  @brief          initialize the queue
 *  @param[in,out]   queue pointer to the queue object
 *  @param[in]      array pointer to array buffer
 *  @param[in]      buffer_len buffer lenght
 *  @param[in]      elem_size capacity of a unique element of the queue
 * @return     true on success, false on fail
 */
bool emblib_queue_init(emblib_queue_t *queue, const void *array, const size_t buffer_len, const size_t size_elem,
                       void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data));

/**
 * @brief Clears all elements from the queue.
 *
 * @param queue Pointer to the deque structure.
 */
void emblib_queue_flush(emblib_queue_t *queue);

/**
 *  @brief          return the queue capacity in elem_size units
 *  @param[in,out]   queue pointer to the queue object
 *  @return         queue size
 */
size_t emblib_queue_size(emblib_queue_t *queue);

/**
 *  @brief          get the number of elements saved into the queue
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue number of elements into the queue
 */
size_t emblib_queue_count(emblib_queue_t *queue);

/**
 *  @brief          put a element into the end of the queue
 *  @param[in,out]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 * @return     true on success, false on fail
 */
bool emblib_queue_enqueue(emblib_queue_t *queue, void *data);

/**
 *  @brief          get a element from the top of the queue
 *  @param[in,out]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 * @return     true on success, false on fail
 */
bool emblib_queue_dequeue(emblib_queue_t *queue, void *data);

/**
 *  @brief          get a element from the top of the queue without remove it
 *  @param[in,out]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 * @return     true on success, false on fail
 */
bool emblib_queue_peek(emblib_queue_t *queue, void *data);

/**
 *  @brief          get information about the queue is empty or not
 *  @param[in,out]   queue pointer to the queue object
 *  @return     true if empty, false if not
 */
bool emblib_queue_is_empty(emblib_queue_t *queue);

/**
 *  @brief          get information about the queue is full or not
 *  @param[in,out]   queue pointer to the queue object
 * @return     true if full, false if not
 */
bool emblib_queue_is_full(emblib_queue_t *queue);

/**
 *  @brief          get information about the queue will full if I want to put a new data
 *  @param[in,out]   queue pointer to the queue object
 *  @return        true when queue will be full, false when queue will not be full
 */
bool emblib_queue_will_full(emblib_queue_t *queue, const int size);

#endif
