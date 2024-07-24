extern "C" {
#include "emblib_deque.h"
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>

class DequeTest : public ::testing::Test {
protected:
    emblib_deque_t deque;
    int deque_array[5];

    virtual void SetUp() {
        emblib_deque_init(&deque, deque_array, sizeof(deque_array), sizeof(int));
    }

    virtual void TearDown() {
    }
};

TEST_F(DequeTest, Initialization) {
    EXPECT_EQ(emblib_deque_size(&deque), 5);
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    EXPECT_TRUE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, PushFront) {
    int data = 10;
    EXPECT_TRUE(emblib_deque_push_front(&deque, &data));
    EXPECT_EQ(emblib_deque_count(&deque), 1);
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    EXPECT_FALSE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, PushBack) {
    int data = 20;
    EXPECT_TRUE(emblib_deque_push_back(&deque, &data));
    EXPECT_EQ(emblib_deque_count(&deque), 1);
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    EXPECT_FALSE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, PopFront) {
    int data = 10;
    emblib_deque_push_front(&deque, &data);
    int retrieved_data;
    EXPECT_TRUE(emblib_deque_pop_front(&deque, &retrieved_data));
    EXPECT_EQ(retrieved_data, 10);
    EXPECT_EQ(emblib_deque_count(&deque), 0);
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    EXPECT_TRUE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, PopBack) {
    int data = 20;
    emblib_deque_push_back(&deque, &data);
    int retrieved_data;
    EXPECT_TRUE(emblib_deque_pop_back(&deque, &retrieved_data));
    EXPECT_EQ(retrieved_data, 20);
    EXPECT_EQ(emblib_deque_count(&deque), 0);
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    EXPECT_TRUE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, PeekFront) {
    int data = 10;
    emblib_deque_push_front(&deque, &data);
    int peeked_data;
    EXPECT_TRUE(emblib_deque_peek_front(&deque, &peeked_data));
    EXPECT_EQ(peeked_data, 10);
    EXPECT_EQ(emblib_deque_count(&deque), 1);
}

TEST_F(DequeTest, PeekBack) {
    int data = 20;
    emblib_deque_push_back(&deque, &data);
    int peeked_data;
    EXPECT_TRUE(emblib_deque_peek_back(&deque, &peeked_data));
    EXPECT_EQ(peeked_data, 20);
    EXPECT_EQ(emblib_deque_count(&deque), 1);
}

TEST_F(DequeTest, IsEmpty) {
    EXPECT_TRUE(emblib_deque_is_empty(&deque));
    int data = 10;
    emblib_deque_push_front(&deque, &data);
    EXPECT_FALSE(emblib_deque_is_empty(&deque));
}

TEST_F(DequeTest, IsFull) {
    EXPECT_FALSE(emblib_deque_is_full(&deque));
    int data = 10;
    for (int i = 0; i < 5; i++) {
        emblib_deque_push_back(&deque, &data);
    }
    EXPECT_TRUE(emblib_deque_is_full(&deque));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

