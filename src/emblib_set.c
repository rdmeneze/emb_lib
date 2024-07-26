#include "emblib_set.h"
#include <string.h>

bool emblib_set_init(emblib_set_t *set, void *array, size_t buffer_len, size_t size_elem) {
    return emblib_list_init(set, array, buffer_len, size_elem);
}

bool emblib_set_add(emblib_set_t *set, void *data) {
    if (emblib_set_contains(set, data) || emblib_list_is_full(set)) {
        return false;
    }
    return emblib_list_insert(set, emblib_list_count(set), data);
}

bool emblib_set_remove(emblib_set_t *set, void *data) {
    for (size_t i = 0; i < emblib_list_count(set); ++i) {
        char temp[set->elem_size];
        emblib_list_get(set, i, temp);
        if (memcmp(temp, data, set->elem_size) == 0) {
            return emblib_list_remove(set, i, temp);
        }
    }
    return false;
}

bool emblib_set_contains(emblib_set_t *set, void *data) {
    for (size_t i = 0; i < emblib_list_count(set); ++i) {
        char temp[set->elem_size];
        emblib_list_get(set, i, temp);
        if (memcmp(temp, data, set->elem_size) == 0) {
            return true;
        }
    }
    return false;
}

size_t emblib_set_size(emblib_set_t *set) {
    return emblib_list_size(set);
}

void emblib_set_flush(emblib_set_t *set) {
    emblib_list_flush(set);
}

size_t emblib_set_count(emblib_set_t *set) {
    return emblib_list_count(set);
}
