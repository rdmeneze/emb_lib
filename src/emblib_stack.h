/**
 *  @file   emblib_stack.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   may-2020
 */

#ifndef __EMBLIB_STACK_H__
#define    __EMBLIB_STACK_H__

#include "emblib_circ_buffer.h"

//! @struct emblib_stack_t
typedef emblib_circ_buffer_t emblib_stack_t;

/**
 *  @brief          initialize the stack
 *  @param[inout]   stack pointer to the queue object
 *  @param[in]      array pointer to array buffer
 *  @param[in]      buffer_len buffer lenght
 *  @param[in]      elem_size capacity of a unique element of the queue
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_stack_init(emblib_stack_t *stack, const void *array, const size_t buffer_len, const size_t size_elem);

/**
 *  @brief          return the stack capacity in elem_size units
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue capacity
 */
size_t emblib_stack_size(emblib_stack_t *stack);

/**
 *  @brief          get the number of elements saved into the stack
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue number of elements into the queue
 */
size_t emblib_stack_count(emblib_stack_t *stack);

/**
 *  @brief          put a element into the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_stack_push(emblib_stack_t *stack, void *data);

/**
 *  @brief          get a element from the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_stack_pop(emblib_stack_t *stack, void *data);

/**
 *  @brief          get a element from the end of the queue, but do not remove them
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool emblib_stack_peek(emblib_stack_t *stack, void *data);

/**
 *  @brief          get information about the queue is full or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on full queue
 *  @returns        false on not full queue
 */
bool emblib_stack_is_full(emblib_stack_t *stack);

/**
 *  @brief          get information about the queue is empty or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on empty queue
 *  @returns        false on not empty queue
 */
bool emblib_stack_is_empty(emblib_stack_t *stack);

/**
 *  @brief          clean the queue contents
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on success
 */
void emblib_stack_flush(emblib_stack_t *stack);

/**
 *  @brief          get information about the queue will full if I want to put a new data
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true when queue will be full
 *  @returns        false when queue will not be full
 */
bool emblib_stack_will_full(emblib_stack_t *stack);

#endif
