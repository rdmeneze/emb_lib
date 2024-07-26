#include "emblib_set.h"
#include <string.h>

bool emblib_set_init(emblib_set_t *set, void *array, size_t buffer_len, size_t size_elem,
                     void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data),
                     int (*cmp_fn)(void *right, void *left)) {
    if (!cmp_fn) return false;
    set->cmp_fn = cmp_fn;

    return emblib_list_init(&set->list, array, buffer_len, size_elem, copy_fn, free_fn);
}

bool emblib_set_add(emblib_set_t *set, void *data) {
    if (emblib_set_contains(set, data) || emblib_set_is_full(set)) {
        return false;
    }
    return emblib_list_insert(&set->list, emblib_set_count(set), data);
}

bool emblib_set_remove(emblib_set_t *set, void *data) {
    for (size_t i = 0; i < emblib_set_count(set); ++i) {
        char temp[set->list.elem_size];
        emblib_list_get(&set->list, i, temp);
        if (set->cmp_fn(temp, data) == 0) {
            return emblib_list_remove(&set->list, i, temp);
        }
    }
    return false;
}

bool emblib_set_contains(emblib_set_t *set, void *data) {
    for (size_t i = 0; i < emblib_set_count(set); i++) {
        char temp[set->list.elem_size];
        emblib_list_get(&set->list, i, temp);
        if (set->cmp_fn(temp, data) == 0) {
            return true;
        }
    }
    return false;
}

size_t emblib_set_size(emblib_set_t *set) {
    return emblib_list_size(&set->list);
}

void emblib_set_flush(emblib_set_t *set) {
    emblib_list_flush(&set->list);
}

size_t emblib_set_count(emblib_set_t *set) {
    return emblib_list_count(&set->list);
}

bool emblib_set_is_full(emblib_set_t *set) {
    return emblib_list_is_full(&set->list);
}

bool emblib_set_is_empty(emblib_set_t *set) {
    return emblib_list_is_empty(&set->list);
}
