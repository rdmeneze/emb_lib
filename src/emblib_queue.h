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
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      array pointer to array buffer
 *  @param[in]      buffer_len buffer lenght
 *  @param[in]      elem_size capacity of a unique element of the queue
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_init(emblib_queue_t *queue, const void *array, const size_t buffer_len, const size_t size_elem);

/**
 *  @brief          return the queue capacity in elem_size units
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue capacity
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
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_enqueue(emblib_queue_t *queue, void *data);

/**
 *  @brief          put a element into the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_push(emblib_queue_t *queue, void *data);

/**
 *  @brief          get a element from the top of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_dequeue(emblib_queue_t *queue, void *data);

/**
 *  @brief          remove oldest element on the queue
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_pop(emblib_queue_t *queue);

/**
 *  @brief          get the last element on the top of queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_queue_back(emblib_queue_t *queue, void *data);

/**
 *  @brief          get information about the queue is empty or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on empty queue
 *  @returns        false on not empty queue
 */
bool emblib_queue_is_empty(emblib_queue_t *queue);

/**
 *  @brief          get information about the queue is full or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on full queue
 *  @returns        false on not full queue
 */
bool emblib_queue_is_full(emblib_queue_t *queue);

/**
 *  @brief          get information about the queue will full if I want to put a new data
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true when queue will be full
 *  @returns        false when queue will not be full
 */
bool emblib_queue_will_full(emblib_queue_t *queue, const int size);

/**
 *  @brief          clean the queue contents
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on success
 */
void emblib_queue_flush(emblib_queue_t *queue);

#endif
