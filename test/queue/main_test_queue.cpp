extern "C" {
#include "emblib_queue.h"
#include <stdio.h>
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>

TEST(queue_test, foo_test) {
    ASSERT_EQ(true, true);
}

TEST(queue_test, queue_init_queue_null) {
    emblib_queue_t *queue = NULL;
    uint8_t buffer[16] = {0};

    bool ret = emblib_queue_init(queue, buffer, sizeof(buffer), sizeof(uint8_t));
    ASSERT_EQ(ret, false);
}

TEST(queue_test, queue_init_buffer_null) {
    emblib_queue_t queue;

    bool ret = emblib_queue_init(&queue, NULL, sizeof(uint8_t), sizeof(uint8_t));
    ASSERT_EQ(ret, false);
}

TEST(queue_test, queue_init_bufferlen_size_elem_incompatible) {
    emblib_queue_t queue;
    uint8_t buffer[16];

    bool ret = emblib_queue_init(&queue, buffer, sizeof(buffer), 3);
    ASSERT_EQ(ret, false);
}

TEST(queue_test, queue_init_bufferlen_size_elem_compatible) {
    emblib_queue_t queue;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint8_t)));
    ASSERT_FALSE(queue.isfull);
    ASSERT_EQ(queue.head, 0);
    ASSERT_EQ(queue.tail, 0);
    ASSERT_EQ(queue.capacity, sizeof(buffer));
    ASSERT_EQ(queue.size_elem, sizeof(uint8_t));
}

TEST(queue_test, queue_flush) {
    emblib_queue_t queue;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint8_t)));
    emblib_queue_flush(&queue);
    ASSERT_EQ(queue.head, 0);
    ASSERT_EQ(queue.tail, 0);
    ASSERT_FALSE(queue.isfull);
    ASSERT_EQ(queue.count, 0);
}

TEST(queue_test, queue_size) {
    emblib_queue_t queue;
    uint8_t buffer[16];

    ASSERT_TRUE(emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t)));
    ASSERT_EQ(emblib_queue_size(&queue), sizeof(buffer) / sizeof(uint16_t));
}

TEST(queue_test, queue_enqueue) {
    emblib_queue_t queue;
    uint16_t buffer[8];
    uint16_t data = 100;

    bool ret = emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_queue_size(&queue), sizeof(buffer) / sizeof(uint16_t));

    ASSERT_TRUE(emblib_queue_enqueue(&queue, &data));
    ASSERT_EQ(emblib_queue_count(&queue), 1);
}

TEST(queue_test, queue_enqueue_is_not_empty) {
    emblib_queue_t queue;
    uint16_t buffer[16];
    uint16_t data = 100;

    bool ret = emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_queue_size(&queue), sizeof(buffer) / sizeof(uint16_t));

    ASSERT_TRUE(emblib_queue_enqueue(&queue, &data));
    ASSERT_EQ(emblib_queue_count(&queue), 1);
    ASSERT_FALSE(emblib_queue_is_empty(&queue));
}

TEST(queue_test, queue_enqueue_is_full) {
    emblib_queue_t queue;
    uint16_t buffer[2];
    uint16_t data[sizeof(buffer) / sizeof(uint16_t)] = {100, 200};

    bool ret = emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_queue_size(&queue), sizeof(buffer) / sizeof(uint16_t));

    for (int i = 0; i < emblib_queue_size(&queue); i++) {
        ASSERT_TRUE(emblib_queue_enqueue(&queue, &data[i]));
        ASSERT_EQ(emblib_queue_count(&queue), i + 1);
        ASSERT_FALSE(emblib_queue_is_empty(&queue));
    }

    ASSERT_TRUE(emblib_queue_is_full(&queue));
}

TEST(queue_test, queue_enqueue_full) {
    emblib_queue_t queue;
    uint16_t buffer[2];
    uint16_t data[3] = {100, 200, 300};

    bool ret = emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));
    ASSERT_TRUE(ret);
    ASSERT_EQ(emblib_queue_size(&queue), sizeof(buffer) / sizeof(uint16_t));

    for (int i = 0; i < 2; i++) {
        ASSERT_TRUE(emblib_queue_enqueue(&queue, &data[i]));
        ASSERT_EQ(emblib_queue_count(&queue), i + 1);
        ASSERT_FALSE(emblib_queue_is_empty(&queue));
    }

    ASSERT_FALSE(emblib_queue_enqueue(&queue, &data[2]));
}

TEST(queue_test, dequeue) {
    emblib_queue_t queue;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));

    emblib_queue_enqueue(&queue, &data);
    int retrieved_data;
    EXPECT_TRUE(emblib_queue_dequeue(&queue, &retrieved_data));
    EXPECT_EQ(retrieved_data, 10);
    EXPECT_EQ(emblib_queue_count(&queue), 0);
    EXPECT_FALSE(emblib_queue_is_full(&queue));
    EXPECT_TRUE(emblib_queue_is_empty(&queue));
}

TEST(queue_test, peek) {
    emblib_queue_t queue;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));

    emblib_queue_enqueue(&queue, &data);
    int peeked_data;
    EXPECT_TRUE(emblib_queue_peek(&queue, &peeked_data));
    EXPECT_EQ(peeked_data, 10);
    EXPECT_EQ(emblib_queue_count(&queue), 1);
}

TEST(queue_test, is_empty) {
    emblib_queue_t queue;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));

    EXPECT_TRUE(emblib_queue_is_empty(&queue));
    emblib_queue_enqueue(&queue, &data);
    EXPECT_FALSE(emblib_queue_is_empty(&queue));
}

TEST(queue_test, is_full) {
    emblib_queue_t queue;
    uint16_t buffer[3];
    uint16_t data = 10;

    emblib_queue_init(&queue, buffer, sizeof(buffer), sizeof(uint16_t));

    EXPECT_FALSE(emblib_queue_is_full(&queue));
    for (int i = 0; i < 3; i++) {
        emblib_queue_enqueue(&queue, &data);
    }
    EXPECT_TRUE(emblib_queue_is_full(&queue));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

