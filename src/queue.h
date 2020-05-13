/**
 *  @file queue.h
 *  @brief  interface for queue module 
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef list_t queue_t; //!< queue_t declaration

bool    queue_init( queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem );

size_t  queue_size( queue_t* queue );

size_t  queue_count( queue_t* queue );

bool    queue_enqueue( queue_t* queue, void* data );

bool    queue_dequeue( queue_t* queue, void* data );

bool    queue_is_empty( queue_t* queue );

bool    queue_is_full( queue_t* queue );

bool    queue_will_full( queue_t* queue, const int size );

bool    queue_flush( queue_t* queue );


#endif
