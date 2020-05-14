#include "stack.h"
#include <string.h>

bool stack_init(stack_t* stack, const void* array, const size_t buffer_len, const size_t size_elem )
{
    return list_init((list_t*)stack, array, buffer_len, size_elem);
}

/*****************************************************************************/

size_t stack_size(stack_t* stack)
{
    return list_size((list_t*)stack);
}

/*****************************************************************************/

size_t stack_count(stack_t* stack)
{
    return list_count((list_t*)stack);
}

/*****************************************************************************/

bool stack_push(stack_t* stack, void* data)
{
    bool bRet = false;

    if (stack && data)
    {
        unsigned char* ptr = (unsigned char*)data;
        const size_t items_max = stack->size / stack->size_elem; 

        if (stack->items < items_max)
        {
            memcpy((unsigned char*)stack->array+stack->tail, (unsigned char*)ptr, stack->size_elem);
            stack->tail += stack->size_elem;
            stack->items++;            

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool stack_pop(stack_t* stack, void* data)
{
    bool bRet = false;

    if (stack && data)
    {
        if (stack->items)
        {
            stack->tail -= stack->size_elem;
            stack->items--;
            const unsigned char* ptr = (unsigned char*)stack->array + stack->tail;

            memcpy( data, (unsigned char*)ptr, stack->size_elem );

            bRet = true;       
        }
    }

    return bRet;
}

/*****************************************************************************/

bool stack_peek(stack_t* stack, void* data)
{
    bool bRet = false;

    if (data && stack)
    {
        if (stack->items)
        {
            const unsigned char* ptr = ((unsigned char*)stack->array + stack->tail) - stack->size_elem;

            memcpy((unsigned char*)data, (unsigned char*)ptr, stack->size_elem );
            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool stack_is_full(stack_t* stack)
{
    return list_is_full((list_t*)stack);
}

/*****************************************************************************/

bool stack_is_empty(stack_t* stack)
{
    return list_is_empty((list_t*)stack);
}

/*****************************************************************************/

bool stack_flush(stack_t* stack)
{
    return list_flush((list_t*)stack);
}

/*****************************************************************************/

bool stack_will_full(stack_t* stack)
{
    return list_will_full((list_t*)stack, stack->size_elem);
}

/*****************************************************************************/