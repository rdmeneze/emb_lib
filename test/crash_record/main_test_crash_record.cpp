extern "C"
{
#include "emblib_crash_record.h"
}

#include "gtest/gtest.h"
#include <cstring>

namespace {

emblib_crash_record_t make_fault(uint32_t pc) {
    emblib_crash_record_t data{};
    data.reason = EMBLIB_CRASH_FAULT;
    data.pc = pc;
    data.lr = 0x08000F01u;
    data.sp = 0x2000FFC0u;
    data.psr = 0x21000000u;
    data.fault_status = 0x02000000u;
    data.fault_status2 = 0x40000000u;
    std::strcpy(data.msg, "DIVBYZERO");
    return data;
}

}  // namespace

TEST(crash_record_test, random_ram_is_not_valid) {
    emblib_crash_record_t rec;
    std::memset(&rec, 0xA5, sizeof(rec));
    EXPECT_FALSE(emblib_crash_record_is_valid(&rec));

    // right magic, garbage elsewhere
    rec.magic = EMBLIB_CRASH_RECORD_MAGIC;
    EXPECT_FALSE(emblib_crash_record_is_valid(&rec));
    EXPECT_FALSE(emblib_crash_record_is_valid(nullptr));
}

TEST(crash_record_test, save_then_valid) {
    emblib_crash_record_t rec;
    std::memset(&rec, 0xA5, sizeof(rec));
    const emblib_crash_record_t data = make_fault(0x08001234u);

    ASSERT_TRUE(emblib_crash_record_save(&rec, &data));
    EXPECT_TRUE(emblib_crash_record_is_valid(&rec));
    EXPECT_EQ(rec.pc, 0x08001234u);
    EXPECT_STREQ(rec.msg, "DIVBYZERO");
}

TEST(crash_record_test, first_crash_wins) {
    emblib_crash_record_t rec{};
    const emblib_crash_record_t first = make_fault(0x08001111u);
    const emblib_crash_record_t second = make_fault(0x08002222u);

    ASSERT_TRUE(emblib_crash_record_save(&rec, &first));
    EXPECT_FALSE(emblib_crash_record_save(&rec, &second));
    EXPECT_EQ(rec.pc, 0x08001111u);

    emblib_crash_record_clear(&rec);
    EXPECT_FALSE(emblib_crash_record_is_valid(&rec));
    EXPECT_TRUE(emblib_crash_record_save(&rec, &second));
    EXPECT_EQ(rec.pc, 0x08002222u);
}

TEST(crash_record_test, corruption_detected) {
    emblib_crash_record_t rec{};
    const emblib_crash_record_t data = make_fault(0x08001234u);
    ASSERT_TRUE(emblib_crash_record_save(&rec, &data));

    rec.pc ^= 1;
    EXPECT_FALSE(emblib_crash_record_is_valid(&rec));
}

TEST(crash_record_test, msg_always_terminated) {
    emblib_crash_record_t rec{};
    emblib_crash_record_t data = make_fault(0);
    std::memset(data.msg, 'x', sizeof(data.msg));
    ASSERT_TRUE(emblib_crash_record_save(&rec, &data));
    EXPECT_EQ(std::strlen(rec.msg), sizeof(rec.msg) - 1);
}

TEST(crash_record_test, format) {
    emblib_crash_record_t rec{};
    const emblib_crash_record_t data = make_fault(0x08001234u);
    ASSERT_TRUE(emblib_crash_record_save(&rec, &data));

    char s[160];
    string_builder_t sb;
    sb_init(&sb, s, sizeof(s));
    EXPECT_TRUE(emblib_crash_record_format(&rec, &sb));
    EXPECT_STREQ(sb_str(&sb),
                 "FAULT pc=08001234 lr=08000F01 sp=2000FFC0 psr=21000000 "
                 "fs=02000000 fs2=40000000 addr=00000000 \"DIVBYZERO\"");
}

TEST(crash_record_test, format_invalid) {
    emblib_crash_record_t rec{};
    char s[16];
    string_builder_t sb;
    sb_init(&sb, s, sizeof(s));
    EXPECT_FALSE(emblib_crash_record_format(&rec, &sb));
    EXPECT_STREQ(sb_str(&sb), "");
}

TEST(crash_record_test, reason_str) {
    EXPECT_STREQ(emblib_crash_reason_str(EMBLIB_CRASH_ASSERT), "ASSERT");
    EXPECT_STREQ(emblib_crash_reason_str(EMBLIB_CRASH_STACK_OVERFLOW), "STACK_OVERFLOW");
    EXPECT_STREQ(emblib_crash_reason_str(0xFFu), "UNKNOWN");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
