/** 
 *  @file   list.h
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#ifndef LIST_H
#define	LIST_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//! @struct list_t
struct list_t
{
    void*   	array;      //!< array pointer elements
    int   		items;      //!< total items
    size_t  	size;       //!< size of list array
    int   		head;       //!< head element of the list
    int   		tail;       //!< tail element of the list
    uint8_t		size_elem;  //!< size of each element
};

//! @typedef list_t
typedef struct list_t list_t;


/**
 * @brief   initialize the list
 * @param   list pointer to the list object
 * @returns  true on success 
 * @returns  false on fail
 */
bool	list_init( list_t* list, const void* array, const size_t buffer_len, const size_t size_elem );

/**
 * @brief   size of the list
 * @param   list pointer to the list object
 * @return size of the list
 */
size_t list_size( list_t* list );

/**
 * @brief   count the elements of the list
 * @param   list pointer to the list object
 * @return  elements saved on the list
 */
size_t list_count( list_t* list );

/**
 * @brief       insert a element into the list
 * @param[in]   list pointer to the list object
 * @param[in]   data pointer to data to be added to list
 * @returns     true on success 
 * @returns     false on fail
 */
bool list_insert( list_t* list, void* data );

/**
 * @brief           get a element from the list
 * @param[in]       list    pointer to the list object
 * @param[in,out]   data    pointer to data to be retrieve from list
 * @param[in]       nelem   number of elements to be retrieved from list
 * @returns         number of elements retrieved from list
 */
size_t list_retrieve( list_t* list, void* data, const size_t nelem );

/**
 * @brief       return if the list is empty
 * @param[in]   list pointer to the list object
 * @returns     TRUE if the list is empty
 * @returns     FALSE if the list is not empty
 */
bool list_is_empty( list_t* list );

/**
 * @brief       return if the list is full
 * @param[in]   list pointer to the list object
 * @returns     TRUE if the list is full
 * @returns     FALSE if the list is not full
 */
bool list_is_full( list_t* list );

/**
 * @brief       return if the list will be full
 * @param[in]   list pointer to the list object
 * @param[in]   size of data to be inserted 
 * @returns     TRUE if the list will be full
 * @returns     FALSE if the list will be not full
 */
bool list_will_full( list_t* list, const int size );

/**
 * @brief       flush the list
 * @param[in]   list pointer to the list object
 * @returns     TRUE on success
 * @returns     FALSE on fail
 */
bool list_flush( list_t* list );

#endif	/* LIST_H */

