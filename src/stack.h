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

bool stack_init(stack_t* stack, const void* array, const size_t buffer_len, const size_t size_elem );

size_t stack_size(stack_t* stack);

size_t stack_count(stack_t* stack);

bool stack_push(stack_t* stack, void* data);

bool stack_pop(stack_t* stack, void* data);

bool stack_peek(stack_t* stack, void* data);

bool stack_is_full(stack_t* stack);

bool stack_is_empty(stack_t* stack);

bool stack_flush(stack_t* stack);

bool stack_will_full(stack_t* stack);

#endif
