#include "emblib_queue.h"
#include "emblib_stack.h"
#include <string.h>
#include <assert.h>

bool emblib_queue_init(emblib_queue_t *queue, const void *array, const size_t buffer_len, const size_t size_elem) {
    return emblib_circ_buffer_init((emblib_circ_buffer_t *) queue, array, buffer_len, size_elem);
}

/*****************************************************************************/

size_t emblib_queue_size(emblib_queue_t *queue) {
    return emblib_circ_buffer_size((emblib_circ_buffer_t *) queue);
}

/*****************************************************************************/

size_t emblib_queue_count(emblib_queue_t *queue) {
    assert(queue);

    return emblib_circ_buffer_count((emblib_circ_buffer_t *) queue);
}

/*****************************************************************************/

bool emblib_queue_enqueue(emblib_queue_t *queue, void *data) {
    assert(queue);
    assert(data);

    return emblib_circ_buffer_insert((emblib_circ_buffer_t *) queue, data);
}

/*****************************************************************************/

bool emblib_queue_push(emblib_queue_t *queue, void *data) {
    assert(queue);
    assert(data);

    return emblib_circ_buffer_insert((emblib_circ_buffer_t *) queue, data);
}

/*****************************************************************************/

bool emblib_queue_dequeue(emblib_queue_t *queue, void *data) {
    assert(queue);
    assert(data);

    bool bRet = false;

    if (data && queue) {
        unsigned char *ptr = (unsigned char *) data;

        if (queue->items) {
            memcpy((unsigned char *) ptr, (unsigned char *) queue->array + queue->head, queue->size_elem);
            queue->head += queue->size_elem;
            queue->head %= queue->size;
            queue->items--;

            bRet = true;
        }
    }

    return bRet;
}

/*****************************************************************************/

bool emblib_queue_pop(emblib_queue_t *queue) {
    assert(queue);

    return emblib_stack_pop((emblib_stack_t *) queue, NULL);
}

/*****************************************************************************/

bool emblib_queue_back(emblib_queue_t *queue, void *data) {
    assert(queue);
    assert(data);

    bool bret = emblib_stack_pop((emblib_stack_t *) queue, data);
    if (bret) {
        bret = emblib_stack_push((emblib_circ_buffer_t *) queue, data);
    }
    return bret;
}

/*****************************************************************************/

bool emblib_queue_is_empty(emblib_queue_t *queue) {
    assert(queue);

    return emblib_circ_buffer_is_empty((emblib_circ_buffer_t *) queue);
}

/*****************************************************************************/

bool emblib_queue_is_full(emblib_queue_t *queue) {
    assert(queue);

    return emblib_circ_buffer_is_full((emblib_circ_buffer_t *) queue);
}

/*****************************************************************************/

bool emblib_queue_will_full(emblib_queue_t *queue, const int size) {
    assert(queue);
    assert(size);

    return emblib_circ_buffer_will_full((emblib_circ_buffer_t *) queue, size);
}

/*****************************************************************************/

bool emblib_queue_flush(emblib_queue_t *queue) {
    assert(queue);

    return emblib_circ_buffer_flush((emblib_circ_buffer_t *) queue);
}

/*****************************************************************************/
