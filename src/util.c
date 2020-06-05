#include "util.h"
#include <string.h>

uint8_t HIBYTE( const uint16_t b )
{
    return (uint8_t)(b >> 8);
}

uint8_t LOBYTE( const uint16_t b )
{
    return (uint8_t)b; 
}

uint16_t HIWORD( const uint32_t b )
{
    return (uint16_t)(b >> 16);
}

uint16_t LOWORD( const uint32_t b )
{
    return (uint16_t)(b);
}

uint16_t MAKEWORD( const uint8_t lo, const uint8_t hi )
{
    return ((hi << 8) | lo);
}

uint32_t MAKEDWORD( const uint16_t lo, const uint16_t hi )
{
    return ((hi << 16) | lo);
}

void memrev(uint8_t* p, const size_t len)
{
    for (size_t i = 0; (i < (len >> 1)) && p;i++)
    {
        uint8_t c = p[i];
        p[i] = p[len-i-1];
        p[len-i-1] = c; 
    }
}

void strrev(char* s)
{
    size_t len = strlen(s);

    memrev((uint8_t*)s, len);
}

