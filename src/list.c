/** 
 *  @file   list.c
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#include "list.h"
#include <string.h>

/******************************************************************************/

bool	list_init(list_t* list, const void* array, const size_t buffer_len, const size_t size_elem )
{
	bool bRet = false;
	if (list)
	{
        list->array     = (void*)array;
        list->size      = buffer_len;
        list->size_elem = size_elem;
		list->head      = 0;
		list->tail      = 0;
        list->items     = 0;
		bRet = true;
	}

	return bRet;
}

/******************************************************************************/

size_t list_size(list_t* list)
{
    size_t ret = 0;
    if ( list )
    {
        ret = list->size;
    }
    return ret;
}

/******************************************************************************/

size_t   list_count( list_t* list )
{
    int ret = 0;
    if ( list )
    {
        ret = list->items;
    }
    return ret;
}

/******************************************************************************/

bool    list_insert( list_t* list, void* data )
{
    bool bRet = false;
    if ( list && data )
    {
        unsigned char* ptr = (unsigned char*)data;

        if ( list->items < list->size / list->size_elem )
        {
            memcpy( (unsigned char*)list->array+list->head, (unsigned char*)ptr, list->size_elem );
            list->head += list->size_elem;
            list->head %= list->size;
            list->items++;
            
            bRet = true;
        }
    }
    return bRet;
}

/******************************************************************************/

size_t list_retrieve( list_t* list, void* data, const size_t nelem )
{
    size_t count = 0;
    
    if ( ( NULL != list ) && (NULL != data) && (nelem) && (list->items))
    {
        size_t szItens2Read = nelem;
        
        if ( nelem > list->items )
        {
            szItens2Read = list->items;
        }
        
        size_t szBytes2Read = szItens2Read * list->size_elem;
        
        memcpy((unsigned char*)data, (unsigned char*)list->array + list->tail, szBytes2Read);
        list->tail += szBytes2Read;
        list->tail %= list->size;
        list->items -= szItens2Read;
        
        count = szItens2Read;
    }
    
    return count;
}

/******************************************************************************/

bool    list_empty( list_t* list )
{
    bool bRet = false;
    if ( list )
    {
        bRet = (list->tail == list->head) ? true : false;
    }
        
    return bRet;
}

/******************************************************************************/

bool    list_full( list_t* list )
{
    bool bRet = false;
    if ( list )
    {
        bRet = (list->items == list->size )? true : false;
    }
    return bRet;
}

/******************************************************************************/

bool    list_will_full( list_t* list, const int size )
{
    bool bRet = false;
    
    if ( list )
    {
        bRet = (((list->items*list->size_elem) + size ) >= list->size)? true : false;
    }
    
    return bRet;
}

/******************************************************************************/

bool    list_flush( list_t* list )
{
    bool bRet = false;
    
    if ( list )
    {
        list->items = list->tail = list->head = 0;
        bRet = true;
    }
    
    return bRet;
}

/******************************************************************************/

