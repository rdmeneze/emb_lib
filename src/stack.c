#include "stack.h"
#include <string.h>
#include <assert.h>

bool stack_init(stack_t* stack, const void* array, const size_t buffer_len, const size_t size_elem )
{
    assert( stack );
    assert( array );
    assert( buffer_len );
    assert( size_elem );

    return circ_buffer_init((circ_buffer_t*)stack, array, buffer_len, size_elem);
}

/*****************************************************************************/

size_t stack_size(stack_t* stack)
{
    assert( stack );
    return circ_buffer_size((circ_buffer_t*)stack);
}

/*****************************************************************************/

size_t stack_count(stack_t* stack)
{
    assert(stack);
    return circ_buffer_count((circ_buffer_t*)stack);
}

/*****************************************************************************/

bool stack_push(stack_t* stack, void* data)
{
    assert(stack);
    assert(data);

    return circ_buffer_insert( (circ_buffer_t*)stack, data );
}

/*****************************************************************************/

bool stack_pop(stack_t* stack, void* data)
{
    assert(stack);
    assert(data);

    bool bRet = false;

    if (stack && data)
    {

        if (circ_buffer_is_empty((circ_buffer_t*)stack))
        {
            const size_t head = stack->head - stack->size_elem;
            const unsigned char* ptr = (unsigned char*)stack->array + head;
            memcpy(data, (unsigned char*)ptr, stack->size_elem);

            stack->head = head;
            stack->isfull = false;

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool stack_peek(stack_t* stack, void* data)
{
    assert(stack);
    assert(data);

    bool bRet = false;

    if (data && stack)
    {
        if (stack_pop(stack, data))
        {
            stack_push(stack, data);
        }
    }

    return bRet;
}

/*****************************************************************************/

bool stack_is_full(stack_t* stack)
{
    assert(stack);

    return circ_buffer_is_full((circ_buffer_t*)stack);
}

/*****************************************************************************/

bool stack_is_empty(stack_t* stack)
{
    assert(stack);

    return circ_buffer_is_empty((circ_buffer_t*)stack);
}

/*****************************************************************************/

bool stack_flush(stack_t* stack)
{
    assert(stack);

    return circ_buffer_flush((circ_buffer_t*)stack);
}

/*****************************************************************************/

bool stack_will_full(stack_t* stack)
{
    assert(stack);

    return circ_buffer_will_full((circ_buffer_t*)stack, stack->size_elem);
}

/*****************************************************************************/