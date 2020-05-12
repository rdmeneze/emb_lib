/** 
 *  @file   queue.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   may-2020
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "list.h"

//! @struct queue_t
typedef list_t queue_t;

bool queue_init(queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem );

size_t queue_size(queue_t* queue);

size_t queue_count(queue_t* queue);

bool queue_push(queue_t* queue, void* data);

bool queue_pop(queue_t* queue, void* data);

bool queue_full(queue_t* queue);

bool queue_empty(queue_t* queue);

bool queue_flush(queue_t* queue);

bool queue_will_full(queue_t* queue);

#endif
