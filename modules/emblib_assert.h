#ifndef __EMBLIB_ASSERT_H__
#define __EMBLIB_ASSERT_H__

#include <stdbool.h>
#include <stdint.h>
#include "string_builder.h"

/**
 * Assert that reports through a platform handler and returns whether it failed.
 *
 * - EMBLIB_ASSERT(cond): recoverable. Reports and returns true when cond is false,
 *   so the caller can bail out:  if (EMBLIB_ASSERT(p != NULL)) return false;
 * - EMBLIB_ASSERT_FATAL(cond): reports and never returns when cond is false.
 *
 * cond is evaluated exactly once. On success the cost is a single compare.
 *
 * The platform decides what a report does (log, save a crash record, break into
 * the debugger, reset...) by registering a handler with emblib_assert_set_handler().
 * A handler for fatal asserts must not return; if it does, or no handler is
 * registered, abort() is called.
 *
 * Define EMBLIB_ASSERT_NO_STRINGS to drop the condition and function name strings
 * from flash. The file name and line are kept.
 */

typedef struct _emblib_assert_info_t {
    const char *file;       // may be NULL
    const char *function;   // may be NULL
    const char *condition;  // may be NULL
    uint32_t line;
    bool fatal;
} emblib_assert_info_t;

typedef void (*emblib_assert_handler_t)(const emblib_assert_info_t *info);

/**
 *  @brief      register the platform handler, NULL restores the default (no report)
 */
void emblib_assert_set_handler(emblib_assert_handler_t handler);

/**
 *  @brief      report a failed assert. Use the macros instead of calling it directly.
 *  @return     always true (only returns for recoverable asserts)
 */
bool emblib_assert_fail(bool fatal, const char *file, uint32_t line, const char *function,
                        const char *condition);

/**
 *  @brief      write "file:line function: condition" into sb, skipping missing fields
 *  @return     false if the text was truncated
 */
bool emblib_assert_format(const emblib_assert_info_t *info, string_builder_t *sb);

/**
 *  @brief      file name without its directories
 */
const char *emblib_assert_basename(const char *path);

#if defined(__FILE_NAME__)
#define EMBLIB_ASSERT_FILE_ __FILE_NAME__
#else
#define EMBLIB_ASSERT_FILE_ __FILE__
#endif

#ifdef EMBLIB_ASSERT_NO_STRINGS
#define EMBLIB_ASSERT_FUNC_ NULL
#define EMBLIB_ASSERT_COND_(cond) NULL
#else
#define EMBLIB_ASSERT_FUNC_ __func__
#define EMBLIB_ASSERT_COND_(cond) #cond
#endif

#define EMBLIB_ASSERT_IMPL_(cond, fatal)                                                  \
    ((cond) ? false                                                                       \
            : emblib_assert_fail((fatal), EMBLIB_ASSERT_FILE_, __LINE__, EMBLIB_ASSERT_FUNC_, \
                                 EMBLIB_ASSERT_COND_(cond)))

#define EMBLIB_ASSERT(cond) EMBLIB_ASSERT_IMPL_(cond, false)
#define EMBLIB_ASSERT_FATAL(cond) ((void) EMBLIB_ASSERT_IMPL_(cond, true))

#endif //__EMBLIB_ASSERT_H__
