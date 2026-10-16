extern "C"
{
#include "emblib_assert.h"

bool no_strings_trigger(void);
}

#include "gtest/gtest.h"

namespace {

int handler_calls;
emblib_assert_info_t last_info;

void recording_handler(const emblib_assert_info_t *info) {
    handler_calls++;
    last_info = *info;
}

void nested_handler(const emblib_assert_info_t *info) {
    handler_calls++;
    last_info = *info;
    EMBLIB_ASSERT(info == nullptr);  // must not recurse into this handler
}

class AssertTest : public ::testing::Test {
protected:
    void SetUp() override {
        handler_calls = 0;
        last_info = {};
        emblib_assert_set_handler(recording_handler);
    }

    void TearDown() override {
        emblib_assert_set_handler(nullptr);
    }
};

}  // namespace

TEST_F(AssertTest, passing_assert_returns_false_and_does_not_report) {
    int x = 1;
    EXPECT_FALSE(EMBLIB_ASSERT(x == 1));
    EXPECT_EQ(handler_calls, 0);
}

TEST_F(AssertTest, failing_assert_returns_true_and_reports) {
    int x = 1;
    const int expected_line = __LINE__ + 1;
    EXPECT_TRUE(EMBLIB_ASSERT(x == 2));

    ASSERT_EQ(handler_calls, 1);
    EXPECT_FALSE(last_info.fatal);
    EXPECT_EQ(last_info.line, (uint32_t) expected_line);
    EXPECT_STREQ(emblib_assert_basename(last_info.file), "main_test_assert.cpp");
    EXPECT_STREQ(last_info.condition, "x == 2");
    EXPECT_STREQ(last_info.function, "TestBody");  // gtest wraps the test body in TestBody()
}

TEST_F(AssertTest, condition_evaluated_once) {
    int evaluations = 0;
    EMBLIB_ASSERT(++evaluations == 0);
    EXPECT_EQ(evaluations, 1);
    EMBLIB_ASSERT(++evaluations == 2);
    EXPECT_EQ(evaluations, 2);
}

TEST_F(AssertTest, bail_out_pattern) {
    auto checked_div = [](int a, int b, int *out) -> bool {
        if (EMBLIB_ASSERT(b != 0)) return false;
        *out = a / b;
        return true;
    };

    int out = 0;
    EXPECT_TRUE(checked_div(10, 2, &out));
    EXPECT_EQ(out, 5);
    EXPECT_FALSE(checked_div(10, 0, &out));
    EXPECT_EQ(handler_calls, 1);
}

TEST_F(AssertTest, no_handler_recoverable_is_silent) {
    emblib_assert_set_handler(nullptr);
    EXPECT_TRUE(EMBLIB_ASSERT(1 == 2));
}

TEST_F(AssertTest, assert_inside_handler_does_not_recurse) {
    emblib_assert_set_handler(nested_handler);
    EXPECT_TRUE(EMBLIB_ASSERT(1 == 2));
    EXPECT_EQ(handler_calls, 1);
}

TEST_F(AssertTest, no_strings_keeps_file_and_line_only) {
    EXPECT_TRUE(no_strings_trigger());
    ASSERT_EQ(handler_calls, 1);
    EXPECT_STREQ(emblib_assert_basename(last_info.file), "no_strings_helper.c");
    EXPECT_EQ(last_info.function, nullptr);
    EXPECT_EQ(last_info.condition, nullptr);
}

using AssertDeathTest = AssertTest;

TEST_F(AssertDeathTest, fatal_aborts_after_reporting) {
    int x = 0;
    EXPECT_FALSE(handler_calls);
    EMBLIB_ASSERT_FATAL(x == 0);  // passes, must not abort
    EXPECT_DEATH(EMBLIB_ASSERT_FATAL(x == 1), "");
}

TEST_F(AssertDeathTest, fatal_aborts_without_handler) {
    emblib_assert_set_handler(nullptr);
    EXPECT_DEATH(EMBLIB_ASSERT_FATAL(1 == 2), "");
}

TEST(assert_format_test, full_info) {
    const emblib_assert_info_t info = {"/src/app/main.c", "app_init", "ptr != NULL", 42, false};
    char s[64];
    string_builder_t sb;
    sb_init(&sb, s, sizeof(s));
    EXPECT_TRUE(emblib_assert_format(&info, &sb));
    EXPECT_STREQ(sb_str(&sb), "main.c:42 app_init: ptr != NULL");
}

TEST(assert_format_test, missing_fields) {
    const emblib_assert_info_t info = {nullptr, nullptr, nullptr, 7, true};
    char s[16];
    string_builder_t sb;
    sb_init(&sb, s, sizeof(s));
    EXPECT_TRUE(emblib_assert_format(&info, &sb));
    EXPECT_STREQ(sb_str(&sb), "7");
}

TEST(assert_format_test, truncated) {
    const emblib_assert_info_t info = {"main.c", "app_init", "ptr != NULL", 42, false};
    char s[12];
    string_builder_t sb;
    sb_init(&sb, s, sizeof(s));
    EXPECT_FALSE(emblib_assert_format(&info, &sb));
    EXPECT_STREQ(sb_str(&sb), "main.c:42 a");
}

TEST(assert_format_test, basename) {
    EXPECT_STREQ(emblib_assert_basename("a/b/c.c"), "c.c");
    EXPECT_STREQ(emblib_assert_basename("C:\\src\\c.c"), "c.c");
    EXPECT_STREQ(emblib_assert_basename("c.c"), "c.c");
    EXPECT_EQ(emblib_assert_basename(nullptr), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
