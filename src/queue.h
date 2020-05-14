/**
 *  @file   queue.h
 *  @brief  interface for queue module
 *  @author Rafael Dias <rdmeneze@gmail.com>
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef list_t queue_t; //!< queue_t declaration

/**
 *  @brief          initialize the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      array pointer to array buffer
 *  @param[in]      buffer_len buffer lenght
 *  @param[in]      elem_size size of a unique element of the queue
 *  @returns        true on success
 *  @returns        false on fail
 */
bool    queue_init( queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem );

/**
 *  @brief          return the queue size in elem_size units
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue size
 */
size_t  queue_size( queue_t* queue );

/**
 *  @brief          get the number of elements saved into the queue
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue number of elements into the queue 
 */
size_t  queue_count( queue_t* queue );

/**
 *  @brief          put a element into the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 *  @returns        true on success
 *  @returns        false on fail
 */
bool    queue_enqueue( queue_t* queue, void* data );

/**
 *  @brief          get a element from the top of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool    queue_dequeue( queue_t* queue, void* data );

/**
 *  @brief          get information about the queue is empty or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on empty queue
 *  @returns        false on not empty queue
 */
bool    queue_is_empty( queue_t* queue );

/**
 *  @brief          get information about the queue is full or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on full queue
 *  @returns        false on not full queue
 */
bool    queue_is_full( queue_t* queue );

/**
 *  @brief          get information about the queue will full if I want to put a new data
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true when queue will be full
 *  @returns        false when queue will not be full
 */
bool    queue_will_full( queue_t* queue, const int size );

/**
 *  @brief          clean the queue contents
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on success
 *  @returns        false on fail
 */
bool    queue_flush( queue_t* queue );

#endif
