/**
 *  @file   test_atomic_implementation.cpp
 *  @brief  Testes específicos para implementação com atomics C11
 *  @author Atomic Testing Suite
 *  @date   July 2025
 */

extern "C" {
#define USE_C11_ATOMICS
#include "../examples/thread_safe_version.h"
#include "../examples/atomic_implementation.c"
}

#include "gtest/gtest.h"
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>

class AtomicCircBufferTest : public ::testing::Test {
protected:
    emblib_circ_buffer_atomic_t buffer;
    int buffer_array[100];

    static void int_copy(void *dest, void *src) {
        if (dest && src) {
            memcpy(dest, src, sizeof(int));
        }
    }

    static void int_free(void *data) {
        // No-op para inteiros
    }

    void SetUp() override {
        ASSERT_TRUE(emblib_circ_buffer_atomic_init(
            &buffer, buffer_array, sizeof(buffer_array),
            sizeof(int), int_copy, int_free));
    }
};

// =============================================================================
// TESTES BÁSICOS DE ATOMICS
// =============================================================================

TEST_F(AtomicCircBufferTest, BasicAtomicOperations) {
    // Verifica inicialização
    ASSERT_TRUE(atomic_load(&buffer.initialized));
    ASSERT_EQ(emblib_circ_buffer_atomic_count(&buffer), 0);
    ASSERT_TRUE(emblib_circ_buffer_atomic_is_empty(&buffer));
    ASSERT_FALSE(emblib_circ_buffer_atomic_is_full(&buffer));

    // Teste de inserção
    int data = 42;
    ASSERT_TRUE(emblib_circ_buffer_atomic_insert(&buffer, &data));
    ASSERT_EQ(emblib_circ_buffer_atomic_count(&buffer), 1);
    ASSERT_FALSE(emblib_circ_buffer_atomic_is_empty(&buffer));

    // Teste de peek
    int peeked;
    ASSERT_TRUE(emblib_circ_buffer_atomic_peek(&buffer, &peeked));
    ASSERT_EQ(peeked, 42);
    ASSERT_EQ(emblib_circ_buffer_atomic_count(&buffer), 1); // Peek não remove

    // Teste de retrieve
    int retrieved;
    ASSERT_TRUE(emblib_circ_buffer_atomic_retrieve(&buffer, &retrieved));
    ASSERT_EQ(retrieved, 42);
    ASSERT_EQ(emblib_circ_buffer_atomic_count(&buffer), 0);
    ASSERT_TRUE(emblib_circ_buffer_atomic_is_empty(&buffer));
}

TEST_F(AtomicCircBufferTest, MemoryOrderingTest) {
    // Preenche buffer com padrão conhecido
    std::vector<int> test_data = {1, 2, 3, 4, 5};

    for (int data : test_data) {
        ASSERT_TRUE(emblib_circ_buffer_atomic_insert(&buffer, &data));
    }

    // Verifica ordem de saída
    for (size_t i = 0; i < test_data.size(); i++) {
        int retrieved;
        ASSERT_TRUE(emblib_circ_buffer_atomic_retrieve(&buffer, &retrieved));
        ASSERT_EQ(retrieved, test_data[i]);
    }
}

TEST_F(AtomicCircBufferTest, TryInsertWithRetries) {
    // Preenche buffer até ficar cheio
    int capacity = sizeof(buffer_array) / sizeof(int);

    for (int i = 0; i < capacity; i++) {
        ASSERT_TRUE(emblib_circ_buffer_atomic_try_insert(&buffer, &i, 10));
    }

    ASSERT_TRUE(emblib_circ_buffer_atomic_is_full(&buffer));

    // Tentativa de inserir em buffer cheio deve falhar
    int extra_data = 999;
    ASSERT_FALSE(emblib_circ_buffer_atomic_try_insert(&buffer, &extra_data, 5));
}

// =============================================================================
// TESTES DE PERFORMANCE E CONTENÇÃO
// =============================================================================

TEST_F(AtomicCircBufferTest, HighContentionTest) {
    const int NUM_THREADS = 8;
    const int OPERATIONS_PER_THREAD = 1000;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    std::atomic<int> total_produced{0};
    std::atomic<int> total_consumed{0};
    std::atomic<bool> stop_flag{false};

    // Produtores
    for (int t = 0; t < NUM_THREADS / 2; t++) {
        producers.emplace_back([this, t, OPERATIONS_PER_THREAD, &total_produced]() {
            for (int i = 0; i < OPERATIONS_PER_THREAD; i++) {
                int data = t * OPERATIONS_PER_THREAD + i;

                // Tenta inserir com retry limitado
                if (emblib_circ_buffer_atomic_try_insert(&buffer, &data, 100)) {
                    total_produced.fetch_add(1, std::memory_order_relaxed);
                } else {
                    // Se falhar, tenta novamente após pequeno delay
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                    i--; // Retry
                }
            }
        });
    }

    // Consumidores
    for (int t = 0; t < NUM_THREADS / 2; t++) {
        consumers.emplace_back([this, OPERATIONS_PER_THREAD, &total_consumed, &stop_flag]() {
            int consumed = 0;
            int data;

            while (consumed < OPERATIONS_PER_THREAD && !stop_flag.load()) {
                if (emblib_circ_buffer_atomic_retrieve(&buffer, &data)) {
                    consumed++;
                    total_consumed.fetch_add(1, std::memory_order_relaxed);
                } else {
                    // Buffer vazio, pequeno delay
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
        });
    }

    // Aguarda produtores terminarem
    for (auto& t : producers) {
        t.join();
    }

    // Aguarda um pouco para consumidores processarem
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    stop_flag.store(true);

    // Aguarda consumidores terminarem
    for (auto& t : consumers) {
        t.join();
    }

    std::cout << "Produced: " << total_produced.load()
              << ", Consumed: " << total_consumed.load() << std::endl;

    // Deve ter produzido e consumido a mesma quantidade
    ASSERT_EQ(total_produced.load(), (NUM_THREADS / 2) * OPERATIONS_PER_THREAD);
    ASSERT_EQ(total_consumed.load(), total_produced.load());
}

TEST_F(AtomicCircBufferTest, RelaxedVsStrictOrderingPerformance) {
    const int NUM_OPERATIONS = 10000;

    // Teste com memory ordering strict
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        emblib_circ_buffer_atomic_insert(&buffer, &i);
    }

    int data;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        emblib_circ_buffer_atomic_retrieve(&buffer, &data);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto strict_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Reset buffer
    emblib_circ_buffer_atomic_flush(&buffer);

    // Teste com memory ordering relaxed
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        emblib_circ_buffer_atomic_insert_relaxed(&buffer, &i);
    }

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        // Note: não temos retrieve_relaxed, mas seria similar
        emblib_circ_buffer_atomic_retrieve(&buffer, &data);
    }

    end = std::chrono::high_resolution_clock::now();
    auto relaxed_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Strict ordering: " << strict_duration.count() << " μs" << std::endl;
    std::cout << "Relaxed ordering: " << relaxed_duration.count() << " μs" << std::endl;

    // Versão relaxed deve ser igual ou mais rápida
    ASSERT_LE(relaxed_duration.count(), strict_duration.count() + 1000); // +1ms tolerance
}

// =============================================================================
// TESTES DE STRESS PARA DETECTAR RACE CONDITIONS
// =============================================================================

TEST_F(AtomicCircBufferTest, StressTestRaceConditions) {
    const int NUM_ITERATIONS = 100;
    const int NUM_THREADS = 16;
    const int OPS_PER_ITERATION = 100;

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
        // Reset buffer
        emblib_circ_buffer_atomic_flush(&buffer);

        std::vector<std::thread> threads;
        std::atomic<int> errors{0};

        // Mix de operações
        for (int t = 0; t < NUM_THREADS; t++) {
            threads.emplace_back([this, t, OPS_PER_ITERATION, &errors]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 3);

                for (int op = 0; op < OPS_PER_ITERATION; op++) {
                    int operation = dis(gen);
                    int data = t * OPS_PER_ITERATION + op;

                    switch (operation) {
                        case 1: // Insert
                            if (!emblib_circ_buffer_atomic_try_insert(&buffer, &data, 10)) {
                                // Buffer cheio é OK
                            }
                            break;

                        case 2: // Retrieve
                            {
                                int retrieved;
                                emblib_circ_buffer_atomic_retrieve(&buffer, &retrieved);
                            }
                            break;

                        case 3: // Peek
                            {
                                int peeked;
                                emblib_circ_buffer_atomic_peek(&buffer, &peeked);
                            }
                            break;
                    }

                    // Verifica invariantes
                    size_t count = emblib_circ_buffer_atomic_count(&buffer);
                    size_t capacity = sizeof(buffer_array) / sizeof(int);

                    if (count > capacity) {
                        errors.fetch_add(1);
                    }
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        ASSERT_EQ(errors.load(), 0) << "Race condition detected in iteration " << iteration;
    }
}

// =============================================================================
// TESTES DE COMPORTAMENTO EM EDGE CASES
// =============================================================================

TEST_F(AtomicCircBufferTest, EdgeCases) {
    // Buffer de 1 elemento
    emblib_circ_buffer_atomic_t small_buffer;
    int small_array[1];

    ASSERT_TRUE(emblib_circ_buffer_atomic_init(
        &small_buffer, small_array, sizeof(small_array),
        sizeof(int), int_copy, int_free));

    int data1 = 100;
    int data2 = 200;

    // Deve conseguir inserir 1
    ASSERT_TRUE(emblib_circ_buffer_atomic_insert(&small_buffer, &data1));
    ASSERT_TRUE(emblib_circ_buffer_atomic_is_full(&small_buffer));

    // Não deve conseguir inserir o segundo
    ASSERT_FALSE(emblib_circ_buffer_atomic_insert(&small_buffer, &data2));

    // Retrieve deve funcionar
    int retrieved;
    ASSERT_TRUE(emblib_circ_buffer_atomic_retrieve(&small_buffer, &retrieved));
    ASSERT_EQ(retrieved, 100);
    ASSERT_TRUE(emblib_circ_buffer_atomic_is_empty(&small_buffer));
}

TEST_F(AtomicCircBufferTest, WrapAroundTest) {
    // Preenche buffer, esvazia, preenche novamente
    int capacity = sizeof(buffer_array) / sizeof(int);

    // Primeiro ciclo
    for (int i = 0; i < capacity; i++) {
        ASSERT_TRUE(emblib_circ_buffer_atomic_insert(&buffer, &i));
    }

    // Esvazia completamente
    for (int i = 0; i < capacity; i++) {
        int data;
        ASSERT_TRUE(emblib_circ_buffer_atomic_retrieve(&buffer, &data));
        ASSERT_EQ(data, i);
    }

    // Segundo ciclo (testa wrap-around)
    for (int i = capacity; i < capacity * 2; i++) {
        ASSERT_TRUE(emblib_circ_buffer_atomic_insert(&buffer, &i));
    }

    for (int i = capacity; i < capacity * 2; i++) {
        int data;
        ASSERT_TRUE(emblib_circ_buffer_atomic_retrieve(&buffer, &data));
        ASSERT_EQ(data, i);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
