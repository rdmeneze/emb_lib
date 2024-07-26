#include "emblib_stack.h"
#include <string.h>
#include <assert.h>

bool emblib_stack_init(emblib_stack_t *stack, const void *array, const size_t buffer_len, const size_t size_elem,
                       void (copy_fn)(void *dest, void *src), void (free_fn)(void *data)) {
    return emblib_circ_buffer_init(stack, array, buffer_len, size_elem, copy_fn, free_fn);
}

size_t emblib_stack_size(emblib_stack_t *stack) {
    return emblib_circ_buffer_size(stack);
}

size_t emblib_stack_count(emblib_stack_t *stack) {
    return emblib_circ_buffer_count(stack);
}

bool emblib_stack_push(emblib_stack_t *stack, void *data) {
    return emblib_circ_buffer_insert(stack, data);
}

bool emblib_stack_pop(emblib_stack_t *stack, void *data) {
    return emblib_circ_buffer_retrieve(stack, data);
}

bool emblib_stack_peek(emblib_stack_t *stack, void *data) {
    return emblib_circ_buffer_peek(stack, data);
}

bool emblib_stack_is_full(emblib_stack_t *stack) {
    return emblib_circ_buffer_is_full(stack);
}

bool emblib_stack_is_empty(emblib_stack_t *stack) {
    return emblib_circ_buffer_is_empty(stack);
}

void emblib_stack_flush(emblib_stack_t *stack) {
    emblib_circ_buffer_flush(stack);
}

bool emblib_stack_will_full(emblib_stack_t *stack) {
    return emblib_circ_buffer_will_full(stack, stack->elem_size);
}
