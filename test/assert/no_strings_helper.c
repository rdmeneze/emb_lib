#include "emblib_assert.h"

bool no_strings_trigger(void) {
    int value = 1;
    return EMBLIB_ASSERT(value == 2);
}
