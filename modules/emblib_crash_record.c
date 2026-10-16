#include "emblib_crash_record.h"
#include <stddef.h>
#include <string.h>

// CRC-32 (IEEE 802.3), bitwise to avoid a 1 KB table in flash
static uint32_t crash_record_crc32(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *) data;
    uint32_t crc = 0xFFFFFFFFu;

    while (len--) {
        crc ^= *p++;
        for (int bit = 0; bit < 8; bit++) {
            crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1u)));
        }
    }

    return ~crc;
}

static uint32_t crash_record_compute_crc(const emblib_crash_record_t *rec) {
    return crash_record_crc32(rec, offsetof(emblib_crash_record_t, crc));
}

bool emblib_crash_record_is_valid(const emblib_crash_record_t *rec) {
    return rec && rec->magic == EMBLIB_CRASH_RECORD_MAGIC && rec->crc == crash_record_compute_crc(rec);
}

bool emblib_crash_record_save(emblib_crash_record_t *rec, const emblib_crash_record_t *data) {
    if (!rec || !data || emblib_crash_record_is_valid(rec)) {
        return false;
    }

    // invalidate first: if we are interrupted halfway the record reads as empty
    rec->magic = 0;
    memcpy(rec, data, offsetof(emblib_crash_record_t, crc));
    rec->msg[EMBLIB_CRASH_RECORD_MSG_LEN - 1] = 0;
    rec->magic = EMBLIB_CRASH_RECORD_MAGIC;
    rec->crc = crash_record_compute_crc(rec);

    return true;
}

void emblib_crash_record_clear(emblib_crash_record_t *rec) {
    if (rec) {
        memset(rec, 0, sizeof(*rec));
    }
}

const char *emblib_crash_reason_str(uint32_t reason) {
    switch (reason) {
        case EMBLIB_CRASH_NONE:
            return "NONE";
        case EMBLIB_CRASH_ASSERT:
            return "ASSERT";
        case EMBLIB_CRASH_FAULT:
            return "FAULT";
        case EMBLIB_CRASH_STACK_OVERFLOW:
            return "STACK_OVERFLOW";
        case EMBLIB_CRASH_WATCHDOG:
            return "WATCHDOG";
        case EMBLIB_CRASH_USER:
            return "USER";
        default:
            return "UNKNOWN";
    }
}

static void crash_record_append_reg(string_builder_t *sb, const char *name, uint32_t val) {
    sb_append_char(sb, ' ');
    sb_append_str(sb, name);
    sb_append_char(sb, '=');
    sb_append_hex(sb, val, 8);
}

bool emblib_crash_record_format(const emblib_crash_record_t *rec, string_builder_t *sb) {
    if (!sb || !emblib_crash_record_is_valid(rec)) {
        return false;
    }

    sb_append_str(sb, emblib_crash_reason_str(rec->reason));
    crash_record_append_reg(sb, "pc", rec->pc);
    crash_record_append_reg(sb, "lr", rec->lr);
    crash_record_append_reg(sb, "sp", rec->sp);
    crash_record_append_reg(sb, "psr", rec->psr);
    crash_record_append_reg(sb, "fs", rec->fault_status);
    crash_record_append_reg(sb, "fs2", rec->fault_status2);
    crash_record_append_reg(sb, "addr", rec->fault_address);

    if (rec->msg[0]) {
        sb_append_str(sb, " \"");
        sb_append_str(sb, rec->msg);  // terminated by save() and protected by the CRC
        sb_append_char(sb, '"');
    }

    return !sb_is_truncated(sb);
}
