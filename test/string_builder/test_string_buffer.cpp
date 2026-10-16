extern "C"
{
#include <stdio.h>
#include <inttypes.h>
#include "../../modules/string_builder.h"
#include "../../src/emblib_util.h"
}

#include "gtest/gtest.h"
#include <climits>
#include <cstring>
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

    ASSERT_FALSE(sb_is_truncated(&sb));

    ASSERT_FALSE(sb_append_char_array(&sb, b, sizeof(b) - 1));
    ASSERT_TRUE(sb_is_truncated(&sb));
    std::string sEq = b;
    sEq.append("ABC");
    ASSERT_STREQ(sb_str(&sb), sEq.c_str());
    ASSERT_EQ(sb_get_len(&sb), sizeof(s) - 1);
    ASSERT_EQ(sb_get_remaining(&sb), 0u);
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
    ASSERT_FALSE(sb_append_int(&sb, 100));
    ASSERT_TRUE(sb_is_truncated(&sb));
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

TEST(string_builder_test, append_char_array_at) {
    string_builder_t sb;
    const std::string startStr{"START -"};
    const std::string endStr{"- END"};
    const std::string middleStr{" MIDDLE "};
    char s[128];
    sb_init(&sb, s, sizeof(s));
    sb_append_char_array(&sb, startStr.c_str(), startStr.length());
    sb_append_char_array(&sb, endStr.c_str(), endStr.length());

    ASSERT_TRUE(sb_append_char_array_at(&sb, startStr.length(), middleStr.c_str(), middleStr.length()));
    std::ostringstream oss;
    oss << startStr << middleStr << endStr;
    ASSERT_STREQ(sb_str(&sb), oss.str().c_str());
    ASSERT_EQ(sb_get_len(&sb), oss.str().length());
}

TEST(string_builder_test, append_char_array_at_end) {
    string_builder_t sb;
    const std::string startStr{"START -"};
    const std::string endStr{"- END"};
    const std::string middleStr{" MIDDLE "};

    char s[128];
    std::fill(std::begin(s), std::end(s), 0x55);
    sb_init(&sb, (char *) s, sizeof(s));
    sb_append_char_array(&sb, startStr.c_str(), startStr.length());
    sb_append_char_array(&sb, middleStr.c_str(), middleStr.length());

    ASSERT_TRUE(sb_append_char_array_at(&sb, startStr.length() + middleStr.length(), endStr.c_str(), endStr.length()));
    std::ostringstream oss;
    oss << startStr << middleStr << endStr;
    ASSERT_STREQ(sb_str(&sb), oss.str().c_str());
}

TEST(string_builder_test, init_invalid) {
    string_builder_t sb;
    char s[4];
    ASSERT_FALSE(sb_init(&sb, s, 0));
    ASSERT_FALSE(sb_init(&sb, nullptr, sizeof(s)));
    ASSERT_FALSE(sb_init(nullptr, s, sizeof(s)));
}

TEST(string_builder_test, size_one_holds_only_terminator) {
    string_builder_t sb;
    char s[1] = {'x'};
    ASSERT_TRUE(sb_init(&sb, s, sizeof(s)));
    ASSERT_STREQ(sb_str(&sb), "");
    ASSERT_EQ(sb_get_remaining(&sb), 0u);
    ASSERT_FALSE(sb_append_char(&sb, 'A'));
    ASSERT_STREQ(sb_str(&sb), "");
}

TEST(string_builder_test, reset) {
    string_builder_t sb;
    char s[4];
    sb_init(&sb, s, sizeof(s));
    sb_append_str(&sb, "ABCDEF");
    ASSERT_TRUE(sb_is_truncated(&sb));
    sb_reset(&sb);
    ASSERT_EQ(sb_get_len(&sb), 0u);
    ASSERT_STREQ(sb_str(&sb), "");
    ASSERT_FALSE(sb_is_truncated(&sb));
}

TEST(string_builder_test, append_empty) {
    string_builder_t sb;
    char s[4];
    sb_init(&sb, s, sizeof(s));
    ASSERT_TRUE(sb_append_char_array(&sb, "", 0));
    ASSERT_TRUE(sb_append_str(&sb, ""));
    ASSERT_FALSE(sb_append_str(&sb, nullptr));
    ASSERT_EQ(sb_get_len(&sb), 0u);
}

TEST(string_builder_test, never_writes_past_capacity) {
    string_builder_t sb;
    char s[16];
    std::memset(s, 0x55, sizeof(s));
    sb_init(&sb, s, 8);
    sb_append_str(&sb, "0123456789");
    sb_append_char_array_at(&sb, 2, "XYZXYZXYZ", 9);
    sb_append_long_long(&sb, LLONG_MIN);
    sb_append_hex(&sb, UINT64_MAX, 16);
    ASSERT_EQ(sb_get_len(&sb), 7u);
    for (size_t i = 8; i < sizeof(s); i++) {
        ASSERT_EQ(s[i], 0x55) << "byte " << i << " overwritten";
    }
}

TEST(string_builder_test, append_str_and_char) {
    string_builder_t sb;
    char s[16];
    sb_init(&sb, s, sizeof(s));
    ASSERT_TRUE(sb_append_str(&sb, "PC"));
    ASSERT_TRUE(sb_append_char(&sb, '='));
    ASSERT_STREQ(sb_str(&sb), "PC=");
}

TEST(string_builder_test, append_numbers) {
    string_builder_t sb;
    char s[128];
    sb_init(&sb, s, sizeof(s));
    sb_append_int(&sb, 0);
    sb_append_char(&sb, ' ');
    sb_append_int(&sb, -42);
    sb_append_char(&sb, ' ');
    sb_append_int(&sb, INT_MIN);
    sb_append_char(&sb, ' ');
    sb_append_uint(&sb, UINT_MAX);
    sb_append_char(&sb, ' ');
    sb_append_long_long(&sb, LLONG_MIN);
    sb_append_char(&sb, ' ');
    sb_append_long_long(&sb, LLONG_MAX);
    sb_append_char(&sb, ' ');
    ASSERT_TRUE(sb_append_ulong_long(&sb, ULLONG_MAX));

    std::ostringstream oss;
    oss << 0 << ' ' << -42 << ' ' << INT_MIN << ' ' << UINT_MAX << ' '
        << LLONG_MIN << ' ' << LLONG_MAX << ' ' << ULLONG_MAX;
    ASSERT_STREQ(sb_str(&sb), oss.str().c_str());
    ASSERT_FALSE(sb_is_truncated(&sb));
}

TEST(string_builder_test, append_hex) {
    string_builder_t sb;
    char s[64];
    sb_init(&sb, s, sizeof(s));
    ASSERT_TRUE(sb_append_hex(&sb, 0x2000AB0Cu, 8));
    sb_append_char(&sb, ':');
    ASSERT_TRUE(sb_append_hex(&sb, 0x5, 2));
    sb_append_char(&sb, ':');
    ASSERT_TRUE(sb_append_hex(&sb, 0x1234, 2));  // higher digits dropped
    sb_append_char(&sb, ':');
    ASSERT_TRUE(sb_append_hex(&sb, UINT64_MAX, 16));
    ASSERT_STREQ(sb_str(&sb), "2000AB0C:05:34:FFFFFFFFFFFFFFFF");

    ASSERT_FALSE(sb_append_hex(&sb, 1, 0));
    ASSERT_FALSE(sb_append_hex(&sb, 1, 17));
}

TEST(string_builder_test, insert_tail_longer_than_insert) {
    string_builder_t sb;
    char s[64];
    sb_init(&sb, s, sizeof(s));
    sb_append_str(&sb, "AB-0123456789abcdef");
    ASSERT_TRUE(sb_append_char_array_at(&sb, 2, "XY", 2));
    ASSERT_STREQ(sb_str(&sb), "ABXY-0123456789abcdef");
    ASSERT_EQ(sb_get_len(&sb), 21u);
}

TEST(string_builder_test, insert_at_start_and_before_last) {
    string_builder_t sb;
    char s[16];
    sb_init(&sb, s, sizeof(s));
    sb_append_str(&sb, "ABC");
    ASSERT_TRUE(sb_append_char_array_at(&sb, 2, "X", 1));
    ASSERT_STREQ(sb_str(&sb), "ABXC");
    ASSERT_TRUE(sb_append_char_array_at(&sb, 0, ">", 1));
    ASSERT_STREQ(sb_str(&sb), ">ABXC");
}

TEST(string_builder_test, insert_past_end_rejected) {
    string_builder_t sb;
    char s[16];
    sb_init(&sb, s, sizeof(s));
    ASSERT_FALSE(sb_append_char_array_at(&sb, 5, "X", 1));
    ASSERT_STREQ(sb_str(&sb), "");
    sb_append_str(&sb, "AB");
    ASSERT_FALSE(sb_append_char_array_at(&sb, 3, "X", 1));
    ASSERT_STREQ(sb_str(&sb), "AB");
}

TEST(string_builder_test, insert_truncates_tail_first) {
    string_builder_t sb;
    char s[8];
    sb_init(&sb, s, sizeof(s));
    sb_append_str(&sb, "ABCDE");
    ASSERT_FALSE(sb_append_char_array_at(&sb, 1, "xyz", 3));
    ASSERT_STREQ(sb_str(&sb), "AxyzBCD");
    ASSERT_TRUE(sb_is_truncated(&sb));

    sb_reset(&sb);
    sb_append_str(&sb, "AB");
    ASSERT_FALSE(sb_append_char_array_at(&sb, 1, "0123456789", 10));
    ASSERT_STREQ(sb_str(&sb), "A012345");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
