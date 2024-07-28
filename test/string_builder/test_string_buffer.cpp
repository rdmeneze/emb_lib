extern "C" {
#include <stdio.h>
#include <inttypes.h>
#include "../modules/string_builder.h"
#include "../../src/emblib_util.h"

}

#include "gtest/gtest.h"
#include <sstream>


TEST(string_builder_test, foo_test) {
    ASSERT_EQ(true, true);
}

TEST(string_builder_init, foo_test) {
    string_builder_t sb;
    char s[64];
    ASSERT_TRUE(sb_init( &sb,s, sizeof(s)  ));
    ASSERT_EQ(sb_get_capacity(&sb), ARRAY_LEN(s));
}

TEST(string_builder_append_char_array, foo_test) {
    string_builder_t sb;
    char s[32] = {0};
    ASSERT_TRUE(sb_init( &sb,s, sizeof(s)));
    char b[] = "ABCDEF";
    ASSERT_TRUE(sb_append_char_array( &sb,b, sizeof (b) - 1));
    ASSERT_EQ(sb_get_len( &sb), sizeof(b) - 1);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

