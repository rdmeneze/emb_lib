#include "circ_buffer.h"
#include "stack.h"
#include "queue.h"
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <util.h>

#define BUFFER_LEN 10

uint32_t buffer_circ_buffer[BUFFER_LEN];
uint32_t buffer_queue[BUFFER_LEN];

void printhex(const uint8_t* data, const size_t len)
{
    for (int i = 0; i < len; i++)
    {
        printf( "Ox%X ", data[i] );
    }
}

int main(void)
{
    uint32_t data;
    circ_buffer_t circ_buffer_uint32;

    circ_buffer_init( &circ_buffer_uint32, buffer_circ_buffer, sizeof(buffer_circ_buffer),sizeof(buffer_circ_buffer[0]) );

    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  1000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  2000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  3000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  4000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  5000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  6000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  7000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  8000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){  9000 }) == true );
    assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){ 10000 }) == true );
    // assert( circ_buffer_insert(&circ_buffer_uint32,&(uint32_t){11000}) == true );

    size_t num_elem = circ_buffer_count(&circ_buffer_uint32);

    printf( "elements %ld\n", num_elem);

    for(size_t count = 0; count < num_elem; count++)
    {
        circ_buffer_retrieve(&circ_buffer_uint32, (uint32_t*)&data, 1);

        printf( "elem[%ld] : %u\n", count, data );
    }

    //circ_buffer_flush(&circ_buffer_uint32);

    for (size_t count = 0; count < num_elem+1; count++)
    {
        uint32_t dwvalue = 200;

        if (circ_buffer_will_full( &circ_buffer_uint32, sizeof(dwvalue)) == false )
        {
            (void)circ_buffer_insert( &circ_buffer_uint32, &dwvalue );
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

    printf("tests with lobyte, hibyte, loword, hiword, makeword and makedword\n");
    assert( 0xAABB == MAKEWORD(0xBB, 0xAA) );
    assert( 0xAABBCCDD == MAKEDWORD( 0xCCDD, 0xAABB ) );

    assert( 0xBB    == LOBYTE( 0xAABB )     );
    assert( 0xAA    == HIBYTE( 0xAABB )     );
    assert( 0xCCDD  == LOWORD( 0xAABBCCDD ) );
    assert( 0xAABB  == HIWORD( 0xAABBCCDD ) );

    printf("tests memrev\n");
    {
        printf("test 1: \n");
        printf("input : 0xAA, 0xBB, 0xCC, 0xDD, 0xEE\n");
        uint8_t data[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
        memrev( data, ARRAY_LEN(data) );

        printf("output: ");
        printhex(data, ARRAY_LEN(data));
        printf("\n\n");
    }

    {
        printf("test 2: \n");
        printf("input : 0xAA, 0xBB, 0xCC, 0xDD\n");
        uint8_t data[] = { 0xAA, 0xBB, 0xCC, 0xDD };
        memrev( data, ARRAY_LEN(data) );

        printf("output: ");
        printhex(data, ARRAY_LEN(data));
        printf("\n\n");
    }

    {
        printf("test 3: \n");
        printf("input : 0xAA\n");
        uint8_t data[] = { 0xAA };
        memrev( data, ARRAY_LEN(data) );

        printf("output: ");
        printhex(data, ARRAY_LEN(data));
        printf("\n\n");
    }

    printf("tests strrev\n");
    {
        printf("test 1: \n");
        printf("input : ABCDE\n");
        char data[] = "ABCDE";
        strrev( data );
        printf("output: %s\n\n", data );
    }

    {
        printf("test 2: \n");
        printf("input : ABCD\n");
        char data[] = "ABCD";
        strrev( data );
        printf("output: %s\n\n", data );
    }

    {
        printf("test 3: \n");
        printf("input : A\n");
        char data[] = "A";
        strrev( data );
        printf("output: %s\n\n", data );
    }

    return 0;
}
