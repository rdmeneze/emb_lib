#include <list.h>
#include <queue.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#define BUFFER_LEN 10

uint32_t buffer_list[BUFFER_LEN];
uint32_t buffer_queue[BUFFER_LEN];

int main(void)
{
    list_t list_uint32;

    list_init( &list_uint32, buffer_list, sizeof(buffer_list),sizeof(buffer_list[0]) );

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
    
    queue_t queue_uint32;
    queue_init( &queue_uint32, buffer_queue, sizeof(queue_uint32), sizeof(buffer_queue[0]));

    printf("queue tests\r\n");

    printf("push into queue\r\n");
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 1000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 2000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 3000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 4000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 5000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 6000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 7000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 8000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 9000} ));
    assert( true == queue_push( &queue_uint32, &(uint32_t){ 10000} ));

    while(false == queue_empty(&queue_uint32))
    {
        uint32_t data;
        queue_pop( &queue_uint32, &data );
        printf("pop from queue %d\r\n", data);
    }

    return 0;
}