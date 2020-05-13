#include "queue.h"
#include <string.h>

bool    queue_init( queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem )
{
    return list_init((list_t*)queue, array, buffer_len, size_elem );
}

/*****************************************************************************/

size_t  queue_size( queue_t* queue )
{
    return list_size( (list_t*)queue );
}

/*****************************************************************************/

size_t  queue_count( queue_t* queue )
{
    return list_count( (list_t*)queue );
}

/*****************************************************************************/

bool    queue_enqueue( queue_t* queue, void* data )
{
    bool bRet = false;

    (void)queue;
    (void)data;

    if( data && queue )
    {
        unsigned char* ptr = (unsigned char*)data;
        const size_t items_max = queue->size / queue->size_elem; 

        if (queue->items < items_max)
        {
            memcpy((unsigned char*)queue->array+queue->tail, (unsigned char*)ptr, queue->size_elem);
            queue->tail += queue->size_elem;
            queue->items++;            

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool    queue_dequeue( queue_t* queue, void* data )
{
    (void)queue;
    (void)data;
    return true;
}

/*****************************************************************************/

bool    queue_is_empty( queue_t* queue )
{
    return list_is_empty((list_t*)queue);
}

/*****************************************************************************/

bool    queue_is_full( queue_t* queue )
{
    return list_is_full((list_t*)queue);
}

/*****************************************************************************/

bool    queue_will_full( queue_t* queue, const int size )
{
    return list_will_full( (list_t*)queue, size );
}

/*****************************************************************************/

bool    queue_flush( queue_t* queue )
{
    return list_flush((list_t*)queue);
}

/*****************************************************************************/