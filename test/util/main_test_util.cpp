extern "C" {
#include <inttypes.h>
#include <assert.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <algorithm>

TEST(util_test, array_len) {
    uint8_t tmp[10];

    ASSERT_EQ(ARRAY_LEN(tmp), 10);
}

TEST(util_test, util_hibyte) {
    uint16_t t = 0xAA55;

    ASSERT_EQ(HIBYTE(t), 0xAA);
}

TEST(util_test, util_lobyte) {
    uint16_t t = 0xAA55;

    ASSERT_EQ(LOBYTE(t), 0x55);
}

TEST(util_test, util_hiword) {
    uint32_t t = 0xAA5555AA;

    ASSERT_EQ(HIWORD(t), 0xAA55);
}

TEST(util_test, util_loword) {
    uint32_t t = 0xAA5555AA;

    ASSERT_EQ(LOWORD(t), 0x55AA);
}

TEST(util_test, util_makeword) {
    constexpr uint16_t t = 0xAABB;
    uint16_t b = MAKEWORD(0xBB, 0xAA);

    ASSERT_EQ(t, b);
}

TEST(util_test, util_makedword) {
    constexpr uint32_t t = 0xAABBCCDD;
    auto b = MAKEDWORD(0xCCDD, 0xAABB);

    ASSERT_EQ(t, b);
}

TEST(util_test, util_memrev_buffer_lenght_odd) {
    uint8_t t[] = {1, 2, 3, 4, 5, 6};
    uint8_t b[] = {6, 5, 4, 3, 2, 1};

    auto ret = memrev(t, ARRAY_LEN(t));

    ASSERT_TRUE(std::equal(std::begin(t), std::end(t), std::begin(b)));
    ASSERT_EQ(ret, t);
}

TEST(util_test, util_memrev_buffer_lenght_even) {
    uint8_t t[] = {1, 2, 3, 4, 5};
    uint8_t b[] = {5, 4, 3, 2, 1};

    auto ret = memrev(t, ARRAY_LEN(t));

    ASSERT_TRUE(std::equal(std::begin(t), std::end(t), std::begin(b)));
    ASSERT_EQ(ret, t);
}

TEST(util_test, util_memrev_null) {

    auto ret = memrev(NULL, 100);

    ASSERT_EQ(ret, (void *) NULL);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
