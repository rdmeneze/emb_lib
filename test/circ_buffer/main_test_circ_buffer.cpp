extern "C" {
#include "emblib_queue.h"
#include <stdio.h>
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>

class CircBufferTest : public ::testing::Test {
protected:
    emblib_circ_buffer_t buffer;
    int buffer_array[5]{0};

    virtual void SetUp() {
        emblib_circ_buffer_init(&buffer, buffer_array, sizeof(buffer_array), sizeof(int));
    }
};

TEST_F(CircBufferTest, Initialization) {
    EXPECT_EQ(buffer.capacity, 20);
    EXPECT_EQ(buffer.elem_size, sizeof(int));
    EXPECT_EQ(buffer.count, 0);
    EXPECT_EQ(buffer.head, 0);
    EXPECT_EQ(buffer.tail, 0);
}

TEST_F(CircBufferTest, Capacity) {
    EXPECT_EQ(emblib_circ_buffer_capacity(&buffer), 20);
}

TEST_F(CircBufferTest, Insert) {
    int data = 10;
    EXPECT_TRUE(emblib_circ_buffer_insert(&buffer, &data));

    EXPECT_EQ(buffer.count, 1);
    EXPECT_EQ(buffer.head, 0);
    EXPECT_EQ(buffer.tail, 1);
}

TEST_F(CircBufferTest, InsertFull) {
    int data2Insert[]{1, 2, 3, 4, 5};

    for (auto data: data2Insert) {
        EXPECT_TRUE(emblib_circ_buffer_insert(&buffer, &data));
    }
    EXPECT_FALSE(emblib_circ_buffer_insert(&buffer, &data2Insert[0]));
    EXPECT_EQ(buffer.count, 5);
    EXPECT_EQ(buffer.head, 0);
    EXPECT_EQ(buffer.tail, 0);
}

TEST_F(CircBufferTest, InsertOverwrite) {
    int data2Insert[]{1, 2, 3, 4, 5, 6};
    for (auto data: data2Insert) {
        EXPECT_TRUE(emblib_circ_buffer_insert_overwrite(&buffer, &data));
    }
    EXPECT_EQ(buffer.count, 5);
    EXPECT_EQ(buffer.head, 1);
    EXPECT_EQ(buffer.tail, 1);
}

TEST_F(CircBufferTest, Retrieve) {
    int data = 10;
    emblib_circ_buffer_insert(&buffer, &data);
    int retrieved_data;
    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 10);
    EXPECT_EQ(buffer.count, 0);
    EXPECT_EQ(buffer.head, 1);
    EXPECT_EQ(buffer.tail, 1);
}

TEST_F(CircBufferTest, Retrieve2) {
    int data2Insert[]{10, 20, 30, 40, 50};
    for (auto data: data2Insert) {
        emblib_circ_buffer_insert(&buffer, &data);
    }

    for (int i = 0; i < emblib_queue_size(&buffer); i++) {
        int retrieved_data;
        EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
        EXPECT_EQ(retrieved_data, data2Insert[i]);
    }
}

TEST_F(CircBufferTest, Retrieve3) {
    int data = 10;
    int retrieved_data;
    emblib_circ_buffer_insert(&buffer, &data);
    data = 20;
    emblib_circ_buffer_insert(&buffer, &data);
    data = 30;
    emblib_circ_buffer_insert(&buffer, &data);
    emblib_circ_buffer_retrieve(&buffer, &retrieved_data);
    data = 40;
    emblib_circ_buffer_insert(&buffer, &data);
    data = 50;
    emblib_circ_buffer_insert(&buffer, &data);
    data = 60;
    emblib_circ_buffer_insert(&buffer, &data);


    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 20);
    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 30);
    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 40);
    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 50);
    EXPECT_TRUE(emblib_circ_buffer_retrieve(&buffer, &retrieved_data));
    EXPECT_EQ(retrieved_data, 60);
}

TEST_F(CircBufferTest, Peek) {
    int data2Insert[]{10, 20};
    for (auto data: data2Insert) {
        emblib_circ_buffer_insert(&buffer, &data);
    }

    int peeked_data;
    EXPECT_TRUE(emblib_circ_buffer_peek(&buffer, &peeked_data));
    EXPECT_EQ(peeked_data, 10);
    EXPECT_EQ(buffer.count, 2);
    EXPECT_EQ(buffer.head, 0);
    EXPECT_EQ(buffer.tail, 2);
}

TEST_F(CircBufferTest, IsEmpty) {
    EXPECT_TRUE(emblib_circ_buffer_is_empty(&buffer));
    int data = 10;
    emblib_circ_buffer_insert(&buffer, &data);
    EXPECT_FALSE(emblib_circ_buffer_is_empty(&buffer));
}

TEST_F(CircBufferTest, IsFull) {
    EXPECT_FALSE(emblib_circ_buffer_is_full(&buffer));
    int data = 10;
    for (int i = 0; i < emblib_circ_buffer_size(&buffer); i++) {
        emblib_circ_buffer_insert(&buffer, &data);
    }
    EXPECT_TRUE(emblib_circ_buffer_is_full(&buffer));
}

TEST_F(CircBufferTest, WillFull) {
    EXPECT_FALSE(emblib_circ_buffer_will_full(&buffer, 3));
    int data = 10;
    for (int i = 0; i < 3; i++) {
        emblib_circ_buffer_insert(&buffer, &data);
    }
    EXPECT_TRUE(emblib_circ_buffer_will_full(&buffer, 3));
}

TEST_F(CircBufferTest, Flush) {
    int data = 10;
    emblib_circ_buffer_insert(&buffer, &data);
    emblib_circ_buffer_flush(&buffer);
    EXPECT_EQ(buffer.count, 0);
    EXPECT_EQ(buffer.head, 0);
    EXPECT_EQ(buffer.tail, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

