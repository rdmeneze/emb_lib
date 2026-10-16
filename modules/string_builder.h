#ifndef __STRING_BUILDER_H__
#define __STRING_BUILDER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * String builder over a caller-provided buffer.
 *
 * - No heap: the buffer is owned by the caller (stack, static, no-init RAM...).
 * - The string is always NUL-terminated, so at most capacity - 1 chars are stored.
 * - When an append does not fit, the part that fits is kept, the builder is
 *   marked as truncated and the function returns false.
 * - Number conversions do not use printf, so they are safe to call from fault
 *   handlers and ISRs.
 */
typedef struct _string_builder_t {
    char *s;
    size_t len;
    size_t capacity;
    bool truncated;
} string_builder_t;

/**
 *  @brief      initialize the string builder with an empty string
 *  @param[in]  sb string builder
 *  @param[in]  s buffer that will hold the string
 *  @param[in]  size size of s in bytes (must be at least 1)
 *  @return     true on success
 */
bool sb_init(string_builder_t *sb, char *s, const size_t size);

/**
 *  @brief      clear the string and the truncated flag
 *  @param[in]  sb string builder
 */
void sb_reset(string_builder_t *sb);

size_t sb_get_len(const string_builder_t *sb);

size_t sb_get_capacity(const string_builder_t *sb);

/**
 *  @brief      number of chars that can still be appended
 */
size_t sb_get_remaining(const string_builder_t *sb);

/**
 *  @brief      true if any append or insert since the last init/reset did not fit
 */
bool sb_is_truncated(const string_builder_t *sb);

const char *sb_str(const string_builder_t *sb);

/**
 *  @brief      append len bytes from buffer
 *  @return     true if everything was appended, false if truncated or on invalid arguments
 */
bool sb_append_char_array(string_builder_t *sb, const char *buffer, const size_t len);

/**
 *  @brief      insert len bytes from buffer at position, shifting the rest of the string right
 *  @param[in]  position insert position, from 0 to sb_get_len(sb)
 *  @return     true if everything was kept, false if truncated or on invalid arguments.
 *              On truncation the inserted bytes have priority over the shifted tail.
 */
bool sb_append_char_array_at(string_builder_t *sb, const size_t position, const char *buffer, const size_t len);

/**
 *  @brief      append a NUL-terminated string
 */
bool sb_append_str(string_builder_t *sb, const char *str);

bool sb_append_char(string_builder_t *sb, char c);

/**
 *  @brief      append a raw byte (not its decimal representation)
 */
bool sb_append_byte(string_builder_t *sb, uint8_t val);

/**
 *  @brief      append a number in decimal
 */
bool sb_append_int(string_builder_t *sb, int val);

bool sb_append_uint(string_builder_t *sb, unsigned int val);

bool sb_append_long_long(string_builder_t *sb, long long val);

bool sb_append_ulong_long(string_builder_t *sb, unsigned long long val);

/**
 *  @brief      append a number in uppercase hexadecimal, without prefix
 *  @param[in]  val value
 *  @param[in]  digits number of digits, from 1 to 16, left padded with zeros.
 *              Higher digits of val that do not fit are dropped.
 */
bool sb_append_hex(string_builder_t *sb, uint64_t val, uint8_t digits);

#endif //__STRING_BUILDER_H__
