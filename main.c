#include <list.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#define BUFFER_LEN 10

uint32_t dwBuffer[BUFFER_LEN];

int main(void)
{
    list_t list_uint32;

    list_init( &list_uint32, dwBuffer, sizeof(dwBuffer),sizeof(dwBuffer[0]) );

    assert( list_insert(&list_uint32,&(uint32_t){ 1000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 2000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 3000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 4000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 5000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 6000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 7000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 8000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 9000}) == true );
    assert( list_insert(&list_uint32,&(uint32_t){10000}) == true );
    // assert( list_insert(&list_uint32,&(uint32_t){11000}) == true );

    size_t num_elem = list_count(&list_uint32);

    printf( "elements %ld\n", num_elem);

    for(size_t count = 0; count < num_elem; count++)
    {
        uint32_t elem;
        list_retrieve(&list_uint32, (uint32_t*)&elem, 1);

        printf( "elem[%ld] : %u\n", count, elem );
    }

    list_flush(&list_uint32);

    for (size_t count = 0; count < num_elem+1; count++)
    {
        uint32_t dwvalue = 200;

        if (list_will_full( &list_uint32, sizeof(dwvalue)) == false )
        {
            (void)list_insert( &list_uint32, &dwvalue );
        }
    }
    


    return 0;
}