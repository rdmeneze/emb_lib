extern "C" {
#include "emblib_stack.h"
#include <stdio.h>
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>

TEST(stack_test, foo_test) {
    ASSERT_EQ(true, true);
}

TEST(stack_test, stack_init_stack_null) {
    emblib_stack_t *stack = NULL;
    uint8_t buffer[16] = {0};

    bool ret = emblib_stack_init(stack, buffer, sizeof(buffer), sizeof(uint8_t));
    ASSERT_EQ(ret, false);
}

TEST(stack_test, stack_init_buffer_null) {
    emblib_stack_t stack;

    bool ret = emblib_stack_init(&stack, NULL, sizeof(uint8_t), sizeof(uint8_t));
    ASSERT_EQ(ret, false);
}

TEST(stack_test, stack_init_bufferlen_size_elem_incompatible) {
    emblib_stack_t stack;
    uint8_t buffer[16];

    bool ret = emblib_stack_init(&stack, buffer, sizeof(buffer), 3);
    ASSERT_EQ(ret, false);
}

TEST(stack_test, stack_init_bufferlen_size_elem_compatible) {
    emblib_stack_t stack;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint8_t)));
    ASSERT_FALSE(stack.is_full);
    ASSERT_EQ(stack.head, 0);
    ASSERT_EQ(stack.tail, 0);
    ASSERT_EQ(stack.capacity, sizeof(buffer));
    ASSERT_EQ(stack.size_elem, sizeof(uint8_t));
}

TEST(stack_test, stack_flush) {
    emblib_stack_t stack;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint8_t)));
    emblib_stack_flush(&stack);
    ASSERT_EQ(stack.head, 0);
    ASSERT_EQ(stack.tail, 0);
    ASSERT_FALSE(stack.is_full);
    ASSERT_EQ(stack.count, 0);
}

TEST(stack_test, stack_size) {
    emblib_stack_t stack;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t)));
    ASSERT_EQ(emblib_stack_size(&stack), sizeof(buffer) / sizeof(uint16_t));
}

TEST(stack_test, stack_push) {
    emblib_stack_t stack;
    uint16_t buffer[8];
    uint16_t data = 100;

    bool ret = emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_stack_size(&stack), sizeof(buffer) / sizeof(uint16_t));

    ASSERT_TRUE(emblib_stack_push(&stack, &data));
    ASSERT_EQ(emblib_stack_count(&stack), 1);
}

TEST(stack_test, stack_push_is_not_empty) {
    emblib_stack_t stack;
    uint16_t buffer[16];
    uint16_t data = 100;

    bool ret = emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_stack_size(&stack), sizeof(buffer) / sizeof(uint16_t));

    ASSERT_TRUE(emblib_stack_push(&stack, &data));
    ASSERT_EQ(emblib_stack_count(&stack), 1);
    ASSERT_FALSE(emblib_stack_is_empty(&stack));
}

TEST(stack_test, stack_push_is_full) {
    emblib_stack_t stack;
    uint16_t buffer[2];
    uint16_t data[sizeof(buffer) / sizeof(uint16_t)] = {100, 200};

    bool ret = emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_stack_size(&stack), sizeof(buffer) / sizeof(uint16_t));

    for (int i = 0; i < emblib_stack_size(&stack); i++) {
        ASSERT_TRUE(emblib_stack_push(&stack, &data[i]));
        ASSERT_EQ(emblib_stack_count(&stack), i + 1);
        ASSERT_FALSE(emblib_stack_is_empty(&stack));
    }

    ASSERT_TRUE(emblib_stack_is_full(&stack));
}

TEST(stack_test, stack_push_full) {
    emblib_stack_t stack;
    uint16_t buffer[2];
    uint16_t data[3] = {100, 200, 300};

    bool ret = emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_stack_size(&stack), sizeof(buffer) / sizeof(uint16_t));

    for (int i = 0; i < 2; i++) {
        ASSERT_TRUE(emblib_stack_push(&stack, &data[i]));
        ASSERT_EQ(emblib_stack_count(&stack), i + 1);
        ASSERT_FALSE(emblib_stack_is_empty(&stack));
    }

    ASSERT_FALSE(emblib_stack_push(&stack, &data[2]));
}

TEST(stack_test, stack_peek) {
    emblib_stack_t stack;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));

    emblib_stack_push(&stack, &data);
    int peeked_data;
    EXPECT_TRUE(emblib_stack_peek(&stack, &peeked_data));
    EXPECT_EQ(peeked_data, 10);
    EXPECT_EQ(emblib_stack_count(&stack), 1);
}

TEST(stack_test, stack_is_empty) {
    emblib_stack_t stack;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));

    EXPECT_TRUE(emblib_stack_is_empty(&stack));
    emblib_stack_push(&stack, &data);
    EXPECT_FALSE(emblib_stack_is_empty(&stack));
}

TEST(stack_test, stack_is_full) {
    emblib_stack_t stack;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_stack_init(&stack, buffer, sizeof(buffer), sizeof(uint16_t));

    EXPECT_FALSE(emblib_stack_is_full(&stack));
    for (int i = 0; i < 3; i++) {
        emblib_stack_push(&stack, &data);
    }
    EXPECT_TRUE(emblib_stack_is_full(&stack));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

