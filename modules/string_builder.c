#include "string_builder.h"
#include <string.h>

bool sb_init(string_builder_t* sb, const char* s, const size_t size){
    bool bRet = false;
    if(sb && s && size){
        sb->s = (char*)s;
        sb->capacity = size;
        sb->len = 0;
        sb->s[0] = 0;
        bRet = true;
    }

    return bRet;
}

static size_t sb_get_remainning_bytes(const string_builder_t* sb){
    return sb ?  sb->capacity - sb->len - 1 : 0;
}

size_t sb_get_len(const string_builder_t* sb){
    return sb ? sb->len : 0;
}

size_t sb_get_capacity(const string_builder_t* sb) {
    return sb ? sb->capacity : 0;
}

const char* sb_str(const string_builder_t* sb){
    return sb ? sb->s : NULL;
}


bool sb_append_char_array(string_builder_t* sb, const char* buffer, const size_t len ){
    bool bRet = false;

    if(sb && buffer && len) {
        if(sb_get_remainning_bytes(sb) >= len) {
            memcpy( sb->s + sb->len, buffer, len );
            sb->len += len;
            bRet = true;
        }
    }

    return bRet;
}
