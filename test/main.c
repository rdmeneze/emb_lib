#include "list.h"
#include "stack.h"
#include "queue.h"
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#define BUFFER_LEN 10

uint32_t buffer_list[BUFFER_LEN];
uint32_t buffer_queue[BUFFER_LEN];

int main(void)
{
    uint32_t data;
    list_t list_uint32;

    list_init( &list_uint32, buffer_list, sizeof(buffer_list),sizeof(buffer_list[0]) );

    assert( list_insert(&list_uint32,&(uint32_t){  1000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  2000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  3000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  4000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  5000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  6000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  7000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  8000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){  9000 }) == true );
    assert( list_insert(&list_uint32,&(uint32_t){ 10000 }) == true );
    // assert( list_insert(&list_uint32,&(uint32_t){11000}) == true );

    size_t num_elem = list_count(&list_uint32);

    printf( "elements %ld\n", num_elem);

    for(size_t count = 0; count < num_elem; count++)
    {
        list_retrieve(&list_uint32, (uint32_t*)&data, 1);

        printf( "elem[%ld] : %u\n", count, data );
    }

    //list_flush(&list_uint32);

    for (size_t count = 0; count < num_elem+1; count++)
    {
        uint32_t dwvalue = 200;

        if (list_will_full( &list_uint32, sizeof(dwvalue)) == false )
        {
            (void)list_insert( &list_uint32, &dwvalue );
        }
    }
    
    stack_t stack_uint32;
    stack_init( &stack_uint32, buffer_queue, sizeof(stack_uint32), sizeof(buffer_queue[0]));

    printf("stack tests\r\n");

    printf("push into stack\r\n");
    assert( true == stack_push( &stack_uint32, &(uint32_t){  1000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  2000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  3000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  4000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  5000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  6000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  7000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  8000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){  9000} ));
    assert( true == stack_push( &stack_uint32, &(uint32_t){ 10000} ));

    while(false == stack_is_empty(&stack_uint32))
    {
        stack_pop( &stack_uint32, &data );
        printf("pop from stack %d\r\n", data);
    }

    queue_t queue_uint32;
    queue_init( &queue_uint32, buffer_queue, sizeof(stack_uint32), sizeof(buffer_queue[0]));

    printf("queue tests\r\n");

    printf("push into queue\r\n");
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  1000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  2000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  3000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  4000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  5000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  6000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  7000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  8000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){  9000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 10000} ));
//    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 11000} ));

    while(false == queue_is_empty(&queue_uint32))
    {
        if (queue_dequeue( &queue_uint32, &data ))
        {
            printf("dequeue from queue %d\r\n", data);
        }
    }

    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 11000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 12000} ));
    queue_dequeue( &queue_uint32, &data );
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 13000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 14000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 15000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 16000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 17000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 18000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 19000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 20000} ));
    assert( true == queue_enqueue( &queue_uint32, &(uint32_t){ 21000} ));

    while(false == queue_is_empty(&queue_uint32))
    {
        if (queue_dequeue( &queue_uint32, &data ))
        {
            printf("dequeue from queue %d\r\n", data);
        }
    }

    return 0;
}
