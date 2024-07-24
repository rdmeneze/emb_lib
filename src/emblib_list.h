#ifndef __EMBLIB_LIST__
#define __EMBLIB_LIST__

#include <stdbool.h>
#include <stddef.h>
#include "emblib_circ_buffer.h" // Inclui a definição de emblib_circ_buffer_t

typedef emblib_circ_buffer_t emblib_list_t;

bool emblib_list_init(emblib_list_t *list, void *array, size_t buffer_len, size_t size_elem);

bool emblib_list_insert(emblib_list_t *list, size_t index, void *data);

bool emblib_list_remove(emblib_list_t *list, size_t index, void *data);

bool emblib_list_get(emblib_list_t *list, size_t index, void *data);

bool emblib_list_is_empty(emblib_list_t *list);

bool emblib_list_is_full(emblib_list_t *list);

size_t emblib_list_size(emblib_list_t *list);

size_t emblib_list_count(emblib_list_t *list);


#endif
