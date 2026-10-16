#include "string_builder.h"
#include <string.h>

// enough for the 20 digits of UINT64_MAX plus the sign
#define SB_DEC_BUFFER_LEN 21

bool sb_init(string_builder_t *sb, char *s, const size_t size) {
    bool bRet = false;
    if (sb && s && size) {
        sb->s = s;
        sb->capacity = size;
        sb_reset(sb);
        bRet = true;
    }

    return bRet;
}

void sb_reset(string_builder_t *sb) {
    if (sb && sb->s) {
        sb->len = 0;
        sb->s[0] = 0;
        sb->truncated = false;
    }
}

size_t sb_get_len(const string_builder_t *sb) {
    return sb ? sb->len : 0;
}

size_t sb_get_capacity(const string_builder_t *sb) {
    return sb ? sb->capacity : 0;
}

size_t sb_get_remaining(const string_builder_t *sb) {
    return sb ? sb->capacity - sb->len - 1 : 0;
}

bool sb_is_truncated(const string_builder_t *sb) {
    return sb ? sb->truncated : false;
}

const char *sb_str(const string_builder_t *sb) {
    return sb ? sb->s : NULL;
}

bool sb_append_char_array(string_builder_t *sb, const char *buffer, const size_t len) {
    return sb ? sb_append_char_array_at(sb, sb->len, buffer, len) : false;
}

bool sb_append_char_array_at(string_builder_t *sb, const size_t position, const char *buffer, const size_t len) {
    if (!sb || !buffer || position > sb->len) {
        return false;
    }

    const size_t tail_len = sb->len - position;
    const size_t room = sb->capacity - 1 - position;

    const size_t n_insert = len < room ? len : room;
    const size_t n_tail = tail_len < room - n_insert ? tail_len : room - n_insert;

    memmove(sb->s + position + n_insert, sb->s + position, n_tail);
    memcpy(sb->s + position, buffer, n_insert);
    sb->len = position + n_insert + n_tail;
    sb->s[sb->len] = 0;

    const bool fits = (n_insert == len) && (n_tail == tail_len);
    if (!fits) {
        sb->truncated = true;
    }

    return fits;
}

bool sb_append_str(string_builder_t *sb, const char *str) {
    return str ? sb_append_char_array(sb, str, strlen(str)) : false;
}

bool sb_append_char(string_builder_t *sb, char c) {
    return sb_append_char_array(sb, &c, 1);
}

bool sb_append_byte(string_builder_t *sb, uint8_t val) {
    return sb_append_char(sb, (char) val);
}

static bool sb_append_dec(string_builder_t *sb, unsigned long long magnitude, bool negative) {
    char buf[SB_DEC_BUFFER_LEN];
    size_t pos = sizeof(buf);

    do {
        buf[--pos] = (char) ('0' + (magnitude % 10));
        magnitude /= 10;
    } while (magnitude);

    if (negative) {
        buf[--pos] = '-';
    }

    return sb_append_char_array(sb, &buf[pos], sizeof(buf) - pos);
}

// magnitude of a signed value, without overflowing on LLONG_MIN
static unsigned long long sb_magnitude(long long val) {
    return val < 0 ? (unsigned long long) (-(val + 1)) + 1 : (unsigned long long) val;
}

bool sb_append_int(string_builder_t *sb, int val) {
    return sb_append_dec(sb, sb_magnitude(val), val < 0);
}

bool sb_append_uint(string_builder_t *sb, unsigned int val) {
    return sb_append_dec(sb, val, false);
}

bool sb_append_long_long(string_builder_t *sb, long long val) {
    return sb_append_dec(sb, sb_magnitude(val), val < 0);
}

bool sb_append_ulong_long(string_builder_t *sb, unsigned long long val) {
    return sb_append_dec(sb, val, false);
}

bool sb_append_hex(string_builder_t *sb, uint64_t val, uint8_t digits) {
    static const char hex_digits[] = "0123456789ABCDEF";
    char buf[16];

    if (digits == 0 || digits > sizeof(buf)) {
        return false;
    }

    for (size_t i = digits; i > 0; i--) {
        buf[i - 1] = hex_digits[val & 0xF];
        val >>= 4;
    }

    return sb_append_char_array(sb, buf, digits);
}
