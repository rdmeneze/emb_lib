extern "C" {
#include <stdio.h>
#include <inttypes.h>
#include "../modules/string_builder.h"
#include "../../src/emblib_util.h"

}

#include "gtest/gtest.h"
#include <sstream>

TEST(string_builder_test, init) {
    string_builder_t sb;
    char s[64];
    ASSERT_TRUE(sb_init(&sb, s, sizeof(s)));
    ASSERT_EQ(sb_get_capacity(&sb), ARRAY_LEN(s));
}

TEST(string_builder_test, empty_append_char_array) {
    string_builder_t sb;
    char s[32] = {0};
    ASSERT_TRUE(sb_init(&sb, s, sizeof(s)));
    char b[] = "ABCDEF";
    ASSERT_TRUE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    ASSERT_EQ(sb_get_len(&sb), sizeof(b) - 1);
}

TEST(string_builder_test, withdata_append_char_array) {
    string_builder_t sb;
    char s[13] = {0};
    ASSERT_TRUE(sb_init(&sb, s, sizeof(s)));
    char b[] = "ABCDEF";
    ASSERT_TRUE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    ASSERT_EQ(sb_get_len(&sb), sizeof(b) - 1);

    ASSERT_TRUE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    std::string sEq = b;
    sEq.append(b);
    ASSERT_STREQ(sb_str(&sb), sEq.c_str());
}

TEST(string_builder_test, withdata_overflow_append_char_array) {
    string_builder_t sb;
    char s[10] = {0};
    ASSERT_TRUE(sb_init(&sb, s, sizeof(s)));
    char b[] = "ABCDEF";
    ASSERT_TRUE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    ASSERT_EQ(sb_get_len(&sb), sizeof(b) - 1);

    ASSERT_TRUE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    std::string sEq = b;
    sEq.append("ABC");
    ASSERT_STREQ(sb_str(&sb), sEq.c_str());
}

TEST(string_builder_test, append_int) {
    string_builder_t sb;
    char s[10];
    sb_init(&sb, s, sizeof(s));
    sb_append_char_array(&sb, "INT: ", sizeof("INT: ") - 1);
    ASSERT_TRUE(sb_append_int(&sb, 100));
    ASSERT_STREQ(sb_str(&sb), "INT: 100");
}

TEST(string_builder_test, append_int_ov) {
    string_builder_t sb;
    char s[7];
    sb_init(&sb, s, sizeof(s));
    sb_append_char_array(&sb, "INT: ", sizeof("INT: ") - 1);
    ASSERT_TRUE(sb_append_int(&sb, 100));
    ASSERT_STREQ(sb_str(&sb), "INT: 1");
}

TEST(string_builder_test, append_byte) {
    string_builder_t sb;
    char s[8];
    sb_init(&sb, s, sizeof(s));
    sb_append_char_array(&sb, "BYTE: ", sizeof("BYTE: ") - 1);
    ASSERT_TRUE(sb_append_byte(&sb, 5));
    uint8_t buffer[7] = "BYTE: ";
    memset(&buffer[6], 5, 1);
    ASSERT_EQ(memcmp(sb_str(&sb), buffer, sizeof(buffer)), 0);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

