#include "queue.h"
#include "stack.h"
#include <string.h>
#include <assert.h>

bool    queue_init( queue_t* queue, const void* array, const size_t buffer_len, const size_t size_elem )
{
    assert(queue);
    assert(array);
    assert(buffer_len);
    assert(size_elem);

    return circ_buffer_init((circ_buffer_t*)queue, array, buffer_len, size_elem );
}

/*****************************************************************************/

size_t  queue_size( queue_t* queue )
{
    assert(queue);

    return circ_buffer_size( (circ_buffer_t*)queue );
}

/*****************************************************************************/

size_t  queue_count( queue_t* queue )
{
    assert(queue);

    return circ_buffer_count( (circ_buffer_t*)queue );
}

/*****************************************************************************/

bool    queue_enqueue( queue_t* queue, void* data )
{
    assert(queue);
    assert(data);

    return circ_buffer_insert((circ_buffer_t*)queue, data);
}

/*****************************************************************************/

bool    queue_push(queue_t* queue, void* data)
{
    assert(queue);
    assert(data);

    return circ_buffer_insert((circ_buffer_t*)queue, data);
}

/*****************************************************************************/

bool    queue_dequeue( queue_t* queue, void* data )
{
    assert(queue);
    assert(data);

    bool bRet = false;

    if( data && queue )
    {
        unsigned char* ptr = (unsigned char*)data;

        if (queue->items)
        {
            memcpy((unsigned char*)ptr, (unsigned char*)queue->array+queue->head, queue->size_elem);
            queue->head += queue->size_elem;
            queue->head %= queue->size;
            queue->items--;            

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool queue_pop(queue_t* queue)
{
    assert(queue);

    return stack_pop((stack_t*)queue, NULL);
}

/*****************************************************************************/

bool    queue_back(queue_t* queue, void* data)
{
    assert(queue);
    assert(data);

    bool bret = stack_pop( (stack_t*)queue, data );
    if (bret)
    {
        bret = stack_push( (circ_buffer_t*)queue, data );
    }
    return bret;
}

/*****************************************************************************/

bool    queue_is_empty( queue_t* queue )
{
    assert(queue);

    return circ_buffer_is_empty((circ_buffer_t*)queue);
}

/*****************************************************************************/

bool    queue_is_full( queue_t* queue )
{
    assert(queue);

    return circ_buffer_is_full((circ_buffer_t*)queue);
}

/*****************************************************************************/

bool    queue_will_full( queue_t* queue, const int size )
{
    assert(queue);
    assert(size);

    return circ_buffer_will_full( (circ_buffer_t*)queue, size );
}

/*****************************************************************************/

bool    queue_flush( queue_t* queue )
{
    assert(queue);

    return circ_buffer_flush((circ_buffer_t*)queue);
}

/*****************************************************************************/