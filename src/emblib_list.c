#include "emblib_list.h"
#include <string.h>

bool emblib_list_init(emblib_list_t *list, void *array, size_t buffer_len, size_t size_elem,
                      void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data)) {
    return emblib_circ_buffer_init(list, array, buffer_len, size_elem, copy_fn, free_fn);
}

void emblib_list_flush(emblib_list_t *list) {
    emblib_circ_buffer_flush(list);
}

bool emblib_list_insert(emblib_list_t *list, size_t index, void *data) {
    if (emblib_list_is_full(list) || index > list->count) return false;

    const size_t size = emblib_list_size(list);
    const size_t insert_pos = (list->head + index) % size;
    const size_t move_count = list->count - index;
    void *src = (char *) list->array + insert_pos * list->elem_size;
    void *dest = (char *) list->array + ((insert_pos + 1) % size) * list->elem_size;

    if (move_count > 0) {
        memmove(dest, src, move_count * list->elem_size);
    }

    list->copy_fn((char *) list->array + insert_pos * list->elem_size, data);
    list->tail = (list->tail + 1) % size;
    list->count++;
    return true;
}

bool emblib_list_remove(emblib_list_t *list, size_t index, void *data) {
    if (emblib_list_is_empty(list) || index >= list->count) return false;

    const size_t size = emblib_list_size(list);

    const size_t remove_pos = (list->head + index) % size;
    list->copy_fn(data, (char *) list->array + remove_pos * list->elem_size);

    size_t move_count = list->count - index - 1;
    void *src = (char *) list->array + ((remove_pos + 1) % size) * list->elem_size;
    void *dest = (char *) list->array + remove_pos * list->elem_size;

    if (move_count > 0) {
        memmove(dest, src, move_count * list->elem_size);
    }

    list->tail = (list->tail - 1 + size) % size;
    list->count--;
    return true;
}

bool emblib_list_get(emblib_list_t *list, size_t index, void *data) {
    if (index >= list->count) return false;

    const size_t size = emblib_list_size(list);

    const size_t pos = (list->head + index) % size;
    list->copy_fn(data, (char *) list->array + pos * list->elem_size);
    return true;
}

bool emblib_list_is_empty(emblib_list_t *list) {
    return emblib_circ_buffer_is_empty(list);
}

bool emblib_list_is_full(emblib_list_t *list) {
    return emblib_circ_buffer_is_full(list);
}

size_t emblib_list_size(emblib_list_t *list) {
    return emblib_circ_buffer_size(list);
}

size_t emblib_list_count(emblib_list_t *list) {
    return emblib_circ_buffer_count(list);
}
