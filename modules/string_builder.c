#include "string_builder.h"
#include <string.h>
#include <stdio.h>


bool sb_init(string_builder_t *sb, const char *s, const size_t size) {
    bool bRet = false;
    if (sb && s && size) {
        sb->s = (char *) s;
        sb->capacity = size;
        sb->len = 0;
        sb->s[0] = 0;
        bRet = true;
    }

    return bRet;
}

static size_t sb_get_remainning_bytes(const string_builder_t *sb) {
    return sb ? sb->capacity - sb->len - 1 : 0;
}

static size_t sb_get_nbytes2copy(const string_builder_t *sb, const size_t len) {
    return sb ?
           sb_get_remainning_bytes(sb) >= len ? len : sb_get_capacity(sb) - sb_get_len(sb) - 1 :
           0;
}

size_t sb_get_len(const string_builder_t *sb) {
    return sb ? sb->len : 0;
}

size_t sb_get_capacity(const string_builder_t *sb) {
    return sb ? sb->capacity : 0;
}

const char *sb_str(const string_builder_t *sb) {
    return sb ? sb->s : NULL;
}


bool sb_append_char_array(string_builder_t *sb, const char *buffer, const size_t len) {
    bool bRet = false;

    if (sb && buffer && len) {
        const size_t szBytes2Copy = sb_get_nbytes2copy(sb, len);
        memcpy(sb->s + sb->len, buffer, szBytes2Copy);
        sb->len += szBytes2Copy;
        sb->s[sb->len] = 0;
        bRet = true;
    }

    return bRet;
}

bool sb_append_char_array_at(string_builder_t *sb, const size_t position, const char *buffer, const size_t len) {
    bool bRet = false;

    if (sb && buffer && len) {
        if (position < (sb_get_len(sb) - 1)) {
            const size_t szBytes2Copy = sb_get_nbytes2copy(sb, len);
            memmove(sb->s + position + len, sb->s + position, szBytes2Copy);
            memcpy(sb->s + position, buffer, szBytes2Copy);
            bRet = true;
        } else if (position == (sb_get_len(sb))) {
            bRet = sb_append_char_array(sb, buffer, len);
        }
    }

    return bRet;
}

bool sb_append_int(string_builder_t *sb, int val) {
    bool bRet = false;

    if (sb) {
        char val_str[16];
        const uint8_t len = sprintf(val_str, "%d", val);
        bRet = sb_append_char_array(sb, val_str, len);
    }

    return bRet;
}

bool sb_append_byte(string_builder_t *sb, uint8_t val) {
    return sb ? sb_append_char_array(sb, &val, 1) : false;
}
