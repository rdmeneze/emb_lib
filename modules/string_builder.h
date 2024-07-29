#ifndef __STRING_BUILDER_H__
#define __STRING_BUILDER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _string_builder_t {
    size_t len;
    size_t capacity;
    char *s;
} string_builder_t;

bool sb_init(string_builder_t *sb, const char *s, const size_t size);

size_t sb_get_len(const string_builder_t *sb);

size_t sb_get_capacity(const string_builder_t *sb);

const char *sb_str(const string_builder_t *sb);

bool sb_append_char_array(string_builder_t *sb, const char *buffer, const size_t len);

bool sb_append_char_array_at(string_builder_t *sb, const size_t position, const char *buffer, const size_t len);

bool sb_append_int(string_builder_t *sb, int val);

bool sb_append_byte(string_builder_t *sb, uint8_t val);

#endif //__STRING_BUILDER_H__
