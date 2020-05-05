#include <list.h>
#include <stdio.h>
#include <inttypes.h>

#define BUFFER_LEN 128

uint32_t dwBuffer[BUFFER_LEN];

int main(void)
{
    list_t list_uint32;

    list_init( &list_uint32, dwBuffer, sizeof(dwBuffer)/sizeof(dwBuffer[0]),sizeof(dwBuffer[0]) );

    return 0;
}