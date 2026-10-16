#ifndef __EMBLIB_CRASH_RECORD_H__
#define __EMBLIB_CRASH_RECORD_H__

#include <stdbool.h>
#include <stdint.h>
#include "string_builder.h"

/**
 * "Black box" record that survives a reset.
 *
 * The platform places one instance in RAM that the startup code does not
 * initialize (e.g. a NOLOAD .noinit section), saves it right before a reset and
 * reads it on the next boot. The magic and the CRC tell a real record from the
 * random content RAM has after power-on, or from a save interrupted halfway.
 *
 * Only the first crash is kept: while a valid record exists, new saves are
 * ignored, so a fault inside the fault handler cannot hide the original cause.
 * Clear the record after reporting it.
 */

#define EMBLIB_CRASH_RECORD_MAGIC 0x48535243u  // "CRSH"
#define EMBLIB_CRASH_RECORD_MSG_LEN 64

typedef enum {
    EMBLIB_CRASH_NONE = 0,
    EMBLIB_CRASH_ASSERT,
    EMBLIB_CRASH_FAULT,
    EMBLIB_CRASH_STACK_OVERFLOW,
    EMBLIB_CRASH_WATCHDOG,
    EMBLIB_CRASH_USER,
} emblib_crash_reason_t;

/*
 * Register fields are architecture specific. On Cortex-M:
 *   fault_status  = CFSR, fault_status2 = HFSR,
 *   fault_address = MMFAR or BFAR (whichever is valid, 0 otherwise)
 */
typedef struct _emblib_crash_record_t {
    uint32_t magic;
    uint32_t reason;
    uint32_t pc;
    uint32_t lr;
    uint32_t sp;
    uint32_t psr;
    uint32_t fault_status;
    uint32_t fault_status2;
    uint32_t fault_address;
    char msg[EMBLIB_CRASH_RECORD_MSG_LEN];
    uint32_t crc;
} emblib_crash_record_t;

/**
 *  @brief      true if rec holds a complete record (magic and CRC match)
 */
bool emblib_crash_record_is_valid(const emblib_crash_record_t *rec);

/**
 *  @brief      store data into rec, unless rec already holds a valid record
 *  @param[out] rec record in persistent RAM
 *  @param[in]  data filled by the caller, magic and crc are ignored
 *  @return     true if saved, false if a previous record was kept or on invalid arguments
 */
bool emblib_crash_record_save(emblib_crash_record_t *rec, const emblib_crash_record_t *data);

void emblib_crash_record_clear(emblib_crash_record_t *rec);

const char *emblib_crash_reason_str(uint32_t reason);

/**
 *  @brief      write a one-line human readable report into sb
 *  @return     false if rec is not valid or the text was truncated
 */
bool emblib_crash_record_format(const emblib_crash_record_t *rec, string_builder_t *sb);

#endif //__EMBLIB_CRASH_RECORD_H__
