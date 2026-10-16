#include "emblib_assert.h"
#include <stdlib.h>
#include <string.h>

static emblib_assert_handler_t assert_handler = NULL;

// set while the handler runs, so an assert inside the handler does not recurse
static volatile bool in_handler = false;

void emblib_assert_set_handler(emblib_assert_handler_t handler) {
    assert_handler = handler;
}

bool emblib_assert_fail(bool fatal, const char *file, uint32_t line, const char *function,
                        const char *condition) {
    const emblib_assert_info_t info = {
        .file = file,
        .function = function,
        .condition = condition,
        .line = line,
        .fatal = fatal,
    };

    if (assert_handler && !in_handler) {
        in_handler = true;
        assert_handler(&info);
        in_handler = false;
    }

    if (fatal) {
        abort();
    }

    return true;
}

const char *emblib_assert_basename(const char *path) {
    if (!path) {
        return NULL;
    }

    const char *base = path;
    for (const char *p = path; *p; p++) {
        if (*p == '/' || *p == '\\') {
            base = p + 1;
        }
    }

    return base;
}

bool emblib_assert_format(const emblib_assert_info_t *info, string_builder_t *sb) {
    if (!info || !sb) {
        return false;
    }

    if (info->file) {
        sb_append_str(sb, emblib_assert_basename(info->file));
        sb_append_char(sb, ':');
    }
    sb_append_uint(sb, (unsigned int) info->line);

    if (info->function) {
        sb_append_char(sb, ' ');
        sb_append_str(sb, info->function);
    }

    if (info->condition) {
        sb_append_str(sb, ": ");
        sb_append_str(sb, info->condition);
    }

    return !sb_is_truncated(sb);
}
