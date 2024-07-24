extern "C" {
#include "emblib_list.h"
#include <inttypes.h>
#include "emblib_util.h"
}

#include "gtest/gtest.h"
#include <sstream>

class ListTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Inicializa a lista e um buffer de dados
        emblib_list_init(&list, buffer, sizeof(buffer), sizeof(buffer[0]));
    }

    // Dados de teste
    emblib_list_t list;
    int buffer[10];
};

TEST_F(ListTest, Initialization) {
    // Testa se a lista foi inicializada corretamente
    EXPECT_TRUE(emblib_list_is_empty(&list));
    EXPECT_FALSE(emblib_list_is_full(&list));
    EXPECT_EQ(emblib_list_count(&list), 0);
}

TEST_F(ListTest, InsertElement) {
    int element = 42;
    EXPECT_TRUE(emblib_list_insert(&list, 0, &element));
    EXPECT_FALSE(emblib_list_is_empty(&list));
    EXPECT_EQ(emblib_list_count(&list), 1);
}

TEST_F(ListTest, InsertMultipleElements) {
    int elements[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(emblib_list_insert(&list, i, &elements[i]));
    }
    EXPECT_EQ(emblib_list_count(&list), 5);

    // Verifica se os elementos foram inseridos corretamente
    for (int i = 0; i < 5; ++i) {
        int value;
        EXPECT_TRUE(emblib_list_get(&list, i, &value));
        EXPECT_EQ(value, elements[i]);
    }
}

TEST_F(ListTest, RemoveElement) {
    int element = 42;
    emblib_list_insert(&list, 0, &element);

    int removed;
    EXPECT_TRUE(emblib_list_remove(&list, 0, &removed));
    EXPECT_EQ(removed, element);
    EXPECT_TRUE(emblib_list_is_empty(&list));
    EXPECT_EQ(emblib_list_count(&list), 0);
}

TEST_F(ListTest, RemoveMultipleElements) {
    int elements[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        emblib_list_insert(&list, i, &elements[i]);
    }

    int removed;
    EXPECT_TRUE(emblib_list_remove(&list, 2, &removed));  // Remove o terceiro elemento
    EXPECT_EQ(removed, 3);
    EXPECT_EQ(emblib_list_count(&list), 4);

    // Verifica os elementos restantes
    int expected[] = {1, 2, 4, 5};
    for (int i = 0; i < 4; ++i) {
        int value;
        EXPECT_TRUE(emblib_list_get(&list, i, &value));
        EXPECT_EQ(value, expected[i]);
    }
}

TEST_F(ListTest, GetElement) {
    int element = 42;
    emblib_list_insert(&list, 0, &element);

    int value;
    EXPECT_TRUE(emblib_list_get(&list, 0, &value));
    EXPECT_EQ(value, element);
}

TEST_F(ListTest, ListIsFull) {
    for (int i = 0; i < 10; ++i) {
        emblib_list_insert(&list, i, &i);
    }
    EXPECT_TRUE(emblib_list_is_full(&list));
    EXPECT_EQ(emblib_list_count(&list), 10);
}

TEST_F(ListTest, InsertAtInvalidIndex) {
    int element = 42;
    EXPECT_FALSE(emblib_list_insert(&list, 1, &element)); // Índice inválido, pois a lista está vazia
}

TEST_F(ListTest, RemoveAtInvalidIndex) {
    int removed;
    EXPECT_FALSE(emblib_list_remove(&list, 0, &removed)); // Índice inválido, pois a lista está vazia
}

TEST_F(ListTest, GetAtInvalidIndex) {
    int value;
    EXPECT_FALSE(emblib_list_get(&list, 0, &value)); // Índice inválido, pois a lista está vazia
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

