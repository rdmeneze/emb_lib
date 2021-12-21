#ifndef _UTIL_H_
#define _UTIL_H_

#include <inttypes.h>
#include <stddef.h>


#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

/**
 *  @brief      get the MSB from the input word
 *  @param[in]  b input word
 *  @return     MSB from b
 */
uint8_t HIBYTE( const uint16_t b );

/**
 *  @brief      get the LSB from the input word
 *  @param[in]  b input word
 *  @return     LSB from b
 */
uint8_t LOBYTE( const uint16_t b );

/**
 *  @brief      get the MSB from the input word
 *  @param[in]  b input word
 *  @return     MSB from b
 */
uint16_t HIWORD( const uint32_t b );

/**
 *  @brief      get the LSB from the input word
 *  @param[in]  b input word
 *  @return     LSB from b
 */
uint16_t LOWORD( const uint32_t b );

/**
 *  @brief      build a word using two bytes
 *  @param[in]  lo lsb byte
 *  @param[in]  hi msb byte
 *  @return     word build from hi and lo bytes
 */
uint16_t MAKEWORD( const uint8_t lo, const uint8_t hi );

/**
 *  @brief      build a dword using two words
 *  @param[in]  lo lsb word
 *  @param[in]  hi msb word
 *  @return     dword build from hi and lo words
 */
uint32_t MAKEDWORD( const uint16_t lo, const uint16_t hi );

///**
// *  @brief      reverse the memory content
// *  @param[in]  p pointer to the memory area
// *  @param[in]  len length of memory area to be reversed 
// */
//void memrev(uint8_t** p, const size_t len);
//
///**
// *  @brief      reverse string
// *  @param[in]  s string to be reversed 
// */
//void strrev(char** s);

#endif
