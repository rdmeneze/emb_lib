#ifndef __EMBLIB_LIST__
#define __EMBLIB_LIST__

#include <stdbool.h>
#include <stddef.h>
#include "emblib_circ_buffer.h"

/**
 * @brief Alias for circular buffer structure to represent a list.
 */
typedef emblib_circ_buffer_t emblib_list_t;

/**
 * @brief Initializes the list.
 *
 * @param[in,out] list Pointer to the list structure.
 * @param[in] array Pointer to the memory where elements will be stored.
 * @param[in] buffer_len Total number of elements that the list can store.
 * @param[in] size_elem Size of each element in bytes.
 * @return true if initialization is successful, false otherwise.
 */
bool emblib_list_init(emblib_list_t *list, void *array, size_t buffer_len, size_t size_elem);

/**
 * @brief Clears all elements from the list.
 *
 * @param list Pointer to the list structure.
 */
void emblib_list_flush(emblib_list_t *list);

/**
 * @brief Inserts an element at a specific index in the list.
 *
 * @param[in,out] list Pointer to the list structure.
 * @param[in] index Index at which to insert the element.
 * @param[in] data Pointer to the element to be inserted.
 * @return true if the insert is successful, false otherwise.
 */
bool emblib_list_insert(emblib_list_t *list, size_t index, void *data);

/**
 * @brief Removes an element from a specific index in the list.
 *
 * @param[in,out] list Pointer to the list structure.
 * @param[in] index Index from which to remove the element.
 * @param[out] data Pointer to the memory where the removed element will be stored.
 * @return true if the remove is successful, false otherwise.
 */
bool emblib_list_remove(emblib_list_t *list, size_t index, void *data);

/**
 * @brief Gets an element from a specific index in the list without removing it.
 *
 * @param[in,out] list Pointer to the list structure.
 * @param[in] index Index from which to get the element.
 * @param[out] data Pointer to the memory where the element will be stored.
 * @return true if the get is successful, false otherwise.
 */
bool emblib_list_get(emblib_list_t *list, size_t index, void *data);

/**
 * @brief Checks if the list is empty.
 *
 * @param[in,out] list Pointer to the list structure.
 * @return true if the list is empty, false otherwise.
 */
bool emblib_list_is_empty(emblib_list_t *list);

/**
 * @brief Checks if the list is full.
 *
 * @param[in,out] list Pointer to the list structure.
 * @return true if the list is full, false otherwise.
 */
bool emblib_list_is_full(emblib_list_t *list);


/**
 * @brief Returns the number maximum of elements of the list.
 *
 * @param[in,out] list Pointer to the list structure.
 * @return Number maximum of elements of the list.
 */
size_t emblib_list_size(emblib_list_t *list);

/**
 * @brief Returns the number of elements currently stored in the list.
 *
 * @param[in,out] list Pointer to the list structure.
 * @return Number of elements in the list.
 */
size_t emblib_list_count(emblib_list_t *list);


#endif
