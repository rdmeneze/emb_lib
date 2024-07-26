extern "C" {
#include "emblib_set.h"
#include <stdio.h>
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>
#include <string.h>

void int_copy(void *dest, void *src) {
    if (dest && src) {
        memcpy(dest, src, sizeof(int));
    }
}

void int_free(void *data) {
    return;
}

int int_cmp(void *left, void *right) {
    if (left && right) {
        int l = *(int *) left;
        int r = *(int *) right;
        return r - l;
    }

    return 1;
}


TEST(SetTest, Initialization) {
    emblib_set_t set;
    int array[10];
    ASSERT_TRUE(emblib_set_init(&set, array, sizeof(array), sizeof(int), int_copy, int_free, int_cmp));
    ASSERT_TRUE(emblib_set_is_empty(&set));
    ASSERT_FALSE(emblib_set_is_full(&set));
    ASSERT_EQ(emblib_set_size(&set), ARRAY_LEN(array));
    ASSERT_EQ(emblib_set_count(&set), 0);
}

TEST(SetTest, AddElement) {
    emblib_set_t set;
    int array[10];
    emblib_set_init(&set, array, sizeof(array), sizeof(int), int_copy, int_free, int_cmp);

    int elem = 42;
    ASSERT_TRUE(emblib_set_add(&set, &elem));
    ASSERT_TRUE(emblib_set_contains(&set, &elem));
    ASSERT_EQ(emblib_set_count(&set), 1);

    // Adding the same element should fail
    ASSERT_FALSE(emblib_set_add(&set, &elem));
    ASSERT_EQ(emblib_set_count(&set), 1);
}

TEST(SetTest, RemoveElement) {
    emblib_set_t set;
    int array[10];
    emblib_set_init(&set, array, sizeof(array), sizeof(int), int_copy, int_free, int_cmp);

    int elem = 42;
    emblib_set_add(&set, &elem);

    ASSERT_TRUE(emblib_set_remove(&set, &elem));
    ASSERT_FALSE(emblib_set_contains(&set, &elem));
    ASSERT_EQ(emblib_set_count(&set), 0);

    // Removing a non-existing element should fail
    ASSERT_FALSE(emblib_set_remove(&set, &elem));
}

TEST(SetTest, ClearSet) {
    emblib_set_t set;
    int array[10];
    emblib_set_init(&set, array, sizeof(array), sizeof(int), int_copy, int_free, int_cmp);

    int elem1 = 42;
    int elem2 = 43;
    emblib_set_add(&set, &elem1);
    emblib_set_add(&set, &elem2);

    ASSERT_EQ(emblib_set_count(&set), 2);
    emblib_set_flush(&set);
    ASSERT_EQ(emblib_set_count(&set), 0);
    ASSERT_FALSE(emblib_set_contains(&set, &elem1));
    ASSERT_FALSE(emblib_set_contains(&set, &elem2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

