#include "queue.h"
#include <string.h>

bool queue_init(queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem )
{
    return list_init((list_t*)queue, array, buffer_len, size_elem);
}

size_t queue_size(queue_t* queue)
{
    return list_size((list_t*)queue);
}

size_t queue_count(queue_t* queue)
{
    return list_count((list_t*)queue);
}

bool queue_push(queue_t* queue, void* data)
{
    bool bRet = false;

    if (queue && data)
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

bool queue_pop(queue_t* queue, void* data)
{
    bool bRet = false;

    if (queue && data)
    {
        if (queue->items)
        {
            queue->tail -= queue->size_elem;
            queue->items--;
            memcpy( data, (unsigned char*)queue->array+queue->tail, queue->size_elem );

            bRet = true;       
        }
    }

    return bRet;
}

bool queue_full(queue_t* queue)
{
    return list_full((list_t*)queue);
}

bool queue_empty(queue_t* queue)
{
    return list_empty((list_t*)queue);
}

bool queue_flush(queue_t* queue)
{
    return list_flush((list_t*)queue);
}

bool queue_will_full(queue_t* queue)
{
    return list_will_full((list_t*)queue, queue->size_elem);
}