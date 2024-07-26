#ifndef __EMB_LIB_EMBLIB_SET_H__
#define __EMB_LIB_EMBLIB_SET_H__

#include "emblib_list.h"

/**
 * @brief Alias for list structure to represent a set.
 */
//typedef emblib_list_t emblib_set_t;
typedef struct _emblib_set_t {
    emblib_list_t list;

    int (*cmp_fn)(void *right, void *left);
} emblib_set_t;

/**
 * @brief Initializes the set.
 *
 * @param set Pointer to the set structure.
 * @param array Pointer to the memory where elements will be stored.
 * @param buffer_len Total number of elements that the set can store.
 * @param size_elem Size of each element in bytes.
 * @return true if initialization is successful, false otherwise.
 */
bool emblib_set_init(emblib_set_t *set, void *array, size_t buffer_len, size_t size_elem,
                     void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data),
                     int (*cmp_fn)(void *right, void *left));

/**
 * @brief Adds an element to the set.
 *
 * @param set Pointer to the set structure.
 * @param data Pointer to the element to be added.
 * @return true if the add is successful, false if the element already exists or the set is full.
 */
bool emblib_set_add(emblib_set_t *set, void *data);


/**
 * @brief Removes an element from the set.
 *
 * @param set Pointer to the set structure.
 * @param data Pointer to the element to be removed.
 * @return true if the remove is successful, false otherwise.
 */
bool emblib_set_remove(emblib_set_t *set, void *data);

/**
 * @brief Checks if the set contains a specific element.
 *
 * @param set Pointer to the set structure.
 * @param data Pointer to the element to check.
 * @return true if the set contains the element, false otherwise.
 */
bool emblib_set_contains(emblib_set_t *set, void *data);

/**
 * @brief Returns the number of elements currently stored in the set.
 *
 * @param set Pointer to the set structure.
 * @return Number of elements in the set.
 */
size_t emblib_set_size(emblib_set_t *set);

/**
 * @brief Returns the number of elements currently stored in the set.
 *
 * @param set Pointer to the set structure.
 * @return Number of elements in the set.
 */
size_t emblib_set_count(emblib_set_t *set);

/**
 * @brief Clears all elements from the set.
 *
 * @param set Pointer to the set structure.
 */
void emblib_set_flush(emblib_set_t *set);

bool emblib_set_is_full(emblib_set_t *set);

bool emblib_set_is_empty(emblib_set_t *set);

#endif //__EMB_LIB_EMBLIB_SET_H__
