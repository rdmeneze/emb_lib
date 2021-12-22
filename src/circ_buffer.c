/** 
 *  @file   circ_buffer.c
 *  @author Rafael Dias <rdmeneze@gmail.com>
 *  @date   dez-2018
 */

#include "circ_buffer.h"
#include <string.h>
#include <assert.h>


/******************************************************************************/

bool	circ_buffer_init(circ_buffer_t* circ_buffer, const void* array, const size_t buffer_len, const size_t size_elem )
{
    assert( array );
    assert( circ_buffer );
    assert( buffer_len );
    assert( size_elem );

	bool bRet = false;
    if (circ_buffer)
    {
        *(circ_buffer) = (circ_buffer_t){
            .array      = (void*)array,
            .size       = buffer_len,
            .size_elem  = size_elem,
            .head       = 0,
            .tail       = 0,
            .isfull     = false,
        };

		bRet = true;
	}

	return bRet;
}

/******************************************************************************/

size_t circ_buffer_size(circ_buffer_t* circ_buffer)
{
    assert( circ_buffer );

    size_t ret = 0;
    if ( circ_buffer )
    {
        ret = circ_buffer->size/circ_buffer->size_elem;
    }
    return ret;
}

/******************************************************************************/

size_t   circ_buffer_count( circ_buffer_t* circ_buffer )
{
    assert( circ_buffer );

    size_t ret = 0;
    if ( circ_buffer )
    {
        if (circ_buffer->isfull == false)
        {
            if (circ_buffer->head >= circ_buffer->tail)
                ret = (circ_buffer->head - circ_buffer->tail) / circ_buffer->size_elem;
            else
                ret = (circ_buffer->size - (circ_buffer->tail - circ_buffer->head)) / circ_buffer->size_elem;
        }
        else
        {
            ret = circ_buffer_size(circ_buffer);
        }
    }
    return ret;
}

/******************************************************************************/

bool    circ_buffer_insert( circ_buffer_t* circ_buffer, void* data )
{
    assert( circ_buffer );
    assert( data );

    bool bRet = false;
    if ( circ_buffer && data )
    {
        unsigned char* ptr = (unsigned char*)data;

        if (circ_buffer_count(circ_buffer) <  circ_buffer_size(circ_buffer) )
        {   
            size_t next_head = (circ_buffer->head + circ_buffer->size_elem) % circ_buffer->size;

            if (next_head == circ_buffer->tail)
            {
                circ_buffer->isfull = true;
            }

            // save data into the list
            memcpy((unsigned char*)circ_buffer->array + circ_buffer->head, (unsigned char*)ptr, circ_buffer->size_elem);

            // update pointers
            circ_buffer->head = next_head;

            bRet = true;
        }
    }
    return bRet;
}

/******************************************************************************/

bool circ_buffer_insert_overwrite(circ_buffer_t* circ_buffer, void* data)
{
    assert(circ_buffer);
    assert(data);

    bool bRet = false;
    if (circ_buffer && data)
    {
        unsigned char* ptr = (unsigned char*)data;

        circ_buffer->isfull = false;

        if (circ_buffer_count(circ_buffer) < circ_buffer_size(circ_buffer))
        {
            size_t next_head = (circ_buffer->head + circ_buffer->size_elem) % circ_buffer->size;

            // save data into the list
            memcpy((unsigned char*)circ_buffer->array + circ_buffer->head, (unsigned char*)ptr, circ_buffer->size_elem);

            // update pointers
            circ_buffer->head = next_head;

            bRet = true;
        }
    }
    return bRet;
}

/******************************************************************************/

size_t circ_buffer_retrieve( circ_buffer_t* circ_buffer, void* data )
{
    assert( circ_buffer );

    size_t count = 0;
    
    if ( NULL != circ_buffer )
    {
        size_t szItens2Read = 1;
        
        size_t szBytes2Read = szItens2Read * circ_buffer->size_elem;
        
        if (data)
        {
            memcpy((unsigned char*)data, (unsigned char*)circ_buffer->array + circ_buffer->tail, szBytes2Read);
        }

        circ_buffer->tail += szBytes2Read;
        circ_buffer->tail %= circ_buffer->size;

        circ_buffer->isfull = false;

        count = 1;
    }
    
    return count;
}

/******************************************************************************/

bool    circ_buffer_is_empty( circ_buffer_t* circ_buffer )
{
    assert( circ_buffer );

    bool bRet = false;
    if ( circ_buffer )
    {
        bRet = (circ_buffer_count(circ_buffer) == 0);
    }  
    return bRet;
}

/******************************************************************************/

bool    circ_buffer_is_full( circ_buffer_t* circ_buffer )
{
    assert( circ_buffer );

    bool bRet = false;
    if ( circ_buffer )
    {
        bRet = circ_buffer->isfull;
    }
    return bRet;
}

/******************************************************************************/

bool    circ_buffer_will_full( circ_buffer_t* circ_buffer, const size_t size )
{
    assert( circ_buffer );
    assert( size );

    bool bRet = false;
    
    if ( circ_buffer )
    {
        bRet = (((circ_buffer_count(circ_buffer) * circ_buffer->size_elem) + (size_t)size ) > circ_buffer->size)? true : false;
    }
    
    return bRet;
}

/******************************************************************************/

bool    circ_buffer_flush( circ_buffer_t* circ_buffer )
{
    assert( circ_buffer );

    bool bRet = false;
    
    if ( circ_buffer )
    {
        circ_buffer->tail = circ_buffer->head = 0;
        circ_buffer->isfull = false;
        bRet = true;
    }
    
    return bRet;
}

/******************************************************************************/

