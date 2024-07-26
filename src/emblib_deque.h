#ifndef __DEQUE_H__
#define __DEQUE_H__

#include "emblib_circ_buffer.h"

/**
 * @brief Alias for circular buffer structure to represent deque.
 */
typedef emblib_circ_buffer_t emblib_deque_t;

/**
 * @brief Initializes the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[in] array Pointer to the memory where elements will be stored.
 * @param[in] buffer_len Total number of elements that the deque can store.
 * @param[in] size_elem Size of each element in bytes.
 * @return true if initialization is successful, false otherwise.
 */
bool emblib_deque_init(emblib_deque_t *deque, void *array, size_t buffer_len, size_t size_elem,
                       void (*copy_fn)(void *dest, void *src), void (*free_fn)(void *data));

/**
 * @brief Clears all elements from the deque.
 *
 * @param deque Pointer to the deque structure.
 */
void emblib_deque_flush(emblib_deque_t *deque);

/**
 * @brief Pushes an element to the front of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[in] data Pointer to the element to be pushed.
 * @return true if the push is successful, false if the deque is full.
 */
bool emblib_deque_push_front(emblib_deque_t *deque, void *data);

/**
 * @brief Pushes an element to the back of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[in] data Pointer to the element to be pushed.
 * @return true if the push is successful, false if the deque is full.
 */
bool emblib_deque_push_back(emblib_deque_t *deque, void *data);

/**
 * @brief Pops an element from the front of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[out] data Pointer to the memory where the popped element will be stored.
 * @return true if the pop is successful, false if the deque is empty.
 */
bool emblib_deque_pop_front(emblib_deque_t *deque, void *data);

/**
 * @brief Pops an element from the back of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[out] data Pointer to the memory where the popped element will be stored.
 * @return true if the pop is successful, false if the deque is empty.
 */
bool emblib_deque_pop_back(emblib_deque_t *deque, void *data);

/**
 * @brief Peeks an element from the front of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[out] data Pointer to the memory where the peeked element will be stored.
 * @return true if the peek is successful, false if the deque is empty.
 */
bool emblib_deque_peek_front(emblib_deque_t *deque, void *data);

/**
 * @brief Peeks an element from the back of the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @param[out] data Pointer to the memory where the peeked element will be stored.
 * @return true if the peek is successful, false if the deque is empty.
 */
bool emblib_deque_peek_back(emblib_deque_t *deque, void *data);

/**
 * @brief Checks if the deque is empty.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @return true if the deque is empty, false otherwise.
 */
bool emblib_deque_is_empty(emblib_deque_t *deque);

/**
 * @brief Checks if the deque is full.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @return true if the deque is full, false otherwise.
 */
bool emblib_deque_is_full(emblib_deque_t *deque);

/**
 * @brief   size in elements of deque
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @return size of the circ_buffer in elements
 */
size_t emblib_deque_size(emblib_deque_t *deque);

/**
 * @brief Returns the number of elements currently stored in the deque.
 *
 * @param[in,out] deque Pointer to the deque structure.
 * @return Number of elements in the deque.
 */
size_t emblib_deque_count(emblib_deque_t *deque);

#endif //~__DEQUE_H__
