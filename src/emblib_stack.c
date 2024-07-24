#include "emblib_stack.h"
#include <string.h>
#include <assert.h>

bool emblib_stack_init(emblib_stack_t *stack, const void *array, const size_t buffer_len, const size_t size_elem) {
    assert(stack);
    assert(array);
    assert(buffer_len);
    assert(size_elem);

    return emblib_circ_buffer_init((emblib_circ_buffer_t *) stack, array, buffer_len, size_elem);
}

/*****************************************************************************/

size_t emblib_stack_size(emblib_stack_t *stack) {
    assert(stack);
    return emblib_circ_buffer_size((emblib_circ_buffer_t *) stack);
}

/*****************************************************************************/

size_t emblib_stack_count(emblib_stack_t *stack) {
    assert(stack);
    return emblib_circ_buffer_count((emblib_circ_buffer_t *) stack);
}

/*****************************************************************************/

bool emblib_stack_push(emblib_stack_t *stack, void *data) {
    assert(stack);
    assert(data);

    return emblib_circ_buffer_insert((emblib_circ_buffer_t *) stack, data);
}

/*****************************************************************************/

bool emblib_stack_pop(emblib_stack_t *stack, void *data) {
    assert(stack);

    bool bRet = false;

    if (stack) {
        if (!emblib_circ_buffer_is_empty((emblib_circ_buffer_t *) stack)) {
            const size_t head = ((stack->isfull) ? stack->capacity : stack->head) - stack->size_elem;
            const unsigned char *ptr = (unsigned char *) stack->array + head;

            if (data) {
                memcpy(data, (unsigned char *) ptr, stack->size_elem);
            }

            stack->head = head;
            stack->isfull = false;

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool emblib_stack_peek(emblib_stack_t *stack, void *data) {
    assert(stack);
    assert(data);

    bool bRet = false;

    if (data && stack) {
        if (emblib_stack_pop(stack, data)) {
            emblib_stack_push(stack, data);
        }
    }

    return bRet;
}

/*****************************************************************************/

bool emblib_stack_is_full(emblib_stack_t *stack) {
    assert(stack);

    return emblib_circ_buffer_is_full((emblib_circ_buffer_t *) stack);
}

/*****************************************************************************/

bool emblib_stack_is_empty(emblib_stack_t *stack) {
    assert(stack);

    return emblib_circ_buffer_is_empty((emblib_circ_buffer_t *) stack);
}

/*****************************************************************************/

void emblib_stack_flush(emblib_stack_t *stack) {
    emblib_circ_buffer_flush((emblib_circ_buffer_t *) stack);
}

/*****************************************************************************/

bool emblib_stack_will_full(emblib_stack_t *stack) {
    assert(stack);

    return emblib_circ_buffer_will_full((emblib_circ_buffer_t *) stack, stack->size_elem);
}

/*****************************************************************************/
