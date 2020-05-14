/** 
 *  @file   stack.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   may-2020
 */

#ifndef STACK_H
#define	STACK_H

#include "list.h"

//! @struct stack_t
typedef list_t stack_t;

/**
 *  @brief          initialize the stack
 *  @param[inout]   stack pointer to the queue object
 *  @param[in]      array pointer to array buffer
 *  @param[in]      buffer_len buffer lenght
 *  @param[in]      elem_size size of a unique element of the queue
 *  @returns        true on success
 *  @returns        false on fail
 */
bool stack_init(stack_t* stack, const void* array, const size_t buffer_len, const size_t size_elem );

/**
 *  @brief          return the stack size in elem_size units
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue size
 */
size_t stack_size(stack_t* stack);

/**
 *  @brief          get the number of elements saved into the stack
 *  @param[inout]   queue pointer to the queue object
 *  @return         queue number of elements into the queue 
 */
size_t stack_count(stack_t* stack);

/**
 *  @brief          put a element into the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[in]      data pointer to the element to be saved
 *  @returns        true on success
 *  @returns        false on fail
 */
bool stack_push(stack_t* stack, void* data);

/**
 *  @brief          get a element from the end of the queue
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool stack_pop(stack_t* stack, void* data);

/**
 *  @brief          get a element from the end of the queue, but do not remove them
 *  @param[inout]   queue pointer to the queue object
 *  @param[out]     data pointer to the element to be get
 *  @returns        true on success
 *  @returns        false on fail
 */
bool stack_peek(stack_t* stack, void* data);

/**
 *  @brief          get information about the queue is full or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on full queue
 *  @returns        false on not full queue
 */
bool stack_is_full(stack_t* stack);

/**
 *  @brief          get information about the queue is empty or not
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on empty queue
 *  @returns        false on not empty queue
 */
bool stack_is_empty(stack_t* stack);

/**
 *  @brief          clean the queue contents
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true on success
 *  @returns        false on fail
 */
bool stack_flush(stack_t* stack);

/**
 *  @brief          get information about the queue will full if I want to put a new data
 *  @param[inout]   queue pointer to the queue object
 *  @returns        true when queue will be full
 *  @returns        false when queue will not be full
 */
bool stack_will_full(stack_t* stack);

#endif
