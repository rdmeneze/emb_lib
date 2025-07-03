/**
 *  @file   test_thread_safety.cpp
 *  @brief  Testes de thread safety para emb_lib
 *  @author Thread Safety Analysis
 *  @date   July 2025
 */

extern "C" {
#include "../examples/thread_safe_version.h"
#include "../examples/thread_safe_implementation.c"
}

#include "gtest/gtest.h"
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

class ThreadSafetyTest : public ::testing::Test {
protected:
    emblib_circ_buffer_threadsafe_t buffer;
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
        // Inicializa com thread safety habilitada
        ASSERT_TRUE(emblib_circ_buffer_threadsafe_init(
            &buffer, buffer_array, sizeof(buffer_array),
            sizeof(int), int_copy, int_free, true));
    }

    void TearDown() override {
        emblib_circ_buffer_threadsafe_destroy(&buffer);
    }
};

// =============================================================================
// TESTES BÁSICOS DE FUNCIONALIDADE
// =============================================================================

TEST_F(ThreadSafetyTest, BasicOperations) {
    int data = 42;
    int retrieved;

    // Teste de inserção
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_insert(&buffer, &data));
    ASSERT_EQ(emblib_circ_buffer_threadsafe_count(&buffer), 1);

    // Teste de peek
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_peek(&buffer, &retrieved));
    ASSERT_EQ(retrieved, 42);
    ASSERT_EQ(emblib_circ_buffer_threadsafe_count(&buffer), 1); // Peek não remove

    // Teste de retrieve
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_retrieve(&buffer, &retrieved));
    ASSERT_EQ(retrieved, 42);
    ASSERT_EQ(emblib_circ_buffer_threadsafe_count(&buffer), 0);
}

// =============================================================================
// TESTES DE CONCORRÊNCIA
// =============================================================================

TEST_F(ThreadSafetyTest, ConcurrentProducerConsumer) {
    const int NUM_OPERATIONS = 1000;
    const int NUM_PRODUCERS = 4;
    const int NUM_CONSUMERS = 4;

    std::atomic<int> produced_count{0};
    std::atomic<int> consumed_count{0};
    std::vector<std::thread> threads;

    // Produtores
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        threads.emplace_back([this, i, NUM_OPERATIONS, &produced_count]() {
            for (int j = 0; j < NUM_OPERATIONS; ++j) {
                int data = i * NUM_OPERATIONS + j;

                // Tenta inserir até conseguir
                while (!emblib_circ_buffer_threadsafe_insert(&buffer, &data)) {
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }

                produced_count.fetch_add(1);
            }
        });
    }

    // Consumidores
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        threads.emplace_back([this, NUM_OPERATIONS, &consumed_count]() {
            int data;
            int local_consumed = 0;

            while (local_consumed < NUM_OPERATIONS) {
                if (emblib_circ_buffer_threadsafe_retrieve(&buffer, &data)) {
                    local_consumed++;
                    consumed_count.fetch_add(1);
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
        });
    }

    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }

    // Verifica se todos os dados foram produzidos e consumidos
    ASSERT_EQ(produced_count.load(), NUM_PRODUCERS * NUM_OPERATIONS);
    ASSERT_EQ(consumed_count.load(), NUM_CONSUMERS * NUM_OPERATIONS);
    ASSERT_EQ(emblib_circ_buffer_threadsafe_count(&buffer), 0);
}

TEST_F(ThreadSafetyTest, MultipleReaders) {
    // Preenche o buffer
    for (int i = 0; i < 50; ++i) {
        ASSERT_TRUE(emblib_circ_buffer_threadsafe_insert(&buffer, &i));
    }

    const int NUM_READERS = 8;
    std::vector<std::thread> readers;
    std::atomic<int> successful_reads{0};

    // Múltiplos leitores fazendo peek
    for (int i = 0; i < NUM_READERS; ++i) {
        readers.emplace_back([this, &successful_reads]() {
            int data;
            for (int j = 0; j < 100; ++j) {
                if (emblib_circ_buffer_threadsafe_peek(&buffer, &data)) {
                    successful_reads.fetch_add(1);
                }
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        });
    }

    for (auto& t : readers) {
        t.join();
    }

    // Deve ter sucesso na maioria das leituras
    ASSERT_GT(successful_reads.load(), NUM_READERS * 50);

    // Buffer deve permanecer inalterado
    ASSERT_EQ(emblib_circ_buffer_threadsafe_count(&buffer), 50);
}

// =============================================================================
// TESTE DE STRESS
// =============================================================================

TEST_F(ThreadSafetyTest, StressTest) {
    const int NUM_THREADS = 16;
    const int OPERATIONS_PER_THREAD = 500;

    std::vector<std::thread> threads;
    std::atomic<int> total_inserted{0};
    std::atomic<int> total_retrieved{0};

    // Metade das threads inserindo, metade removendo
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (i % 2 == 0) {
            // Thread produtora
            threads.emplace_back([this, i, OPERATIONS_PER_THREAD, &total_inserted]() {
                for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
                    int data = i * OPERATIONS_PER_THREAD + j;

                    while (!emblib_circ_buffer_threadsafe_insert(&buffer, &data)) {
                        std::this_thread::yield();
                    }

                    total_inserted.fetch_add(1);
                }
            });
        } else {
            // Thread consumidora
            threads.emplace_back([this, OPERATIONS_PER_THREAD, &total_retrieved]() {
                int data;
                int retrieved = 0;

                while (retrieved < OPERATIONS_PER_THREAD) {
                    if (emblib_circ_buffer_threadsafe_retrieve(&buffer, &data)) {
                        retrieved++;
                        total_retrieved.fetch_add(1);
                    } else {
                        std::this_thread::yield();
                    }
                }
            });
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verifica consistência
    ASSERT_EQ(total_inserted.load(), (NUM_THREADS / 2) * OPERATIONS_PER_THREAD);
    ASSERT_EQ(total_retrieved.load(), (NUM_THREADS / 2) * OPERATIONS_PER_THREAD);
}

// =============================================================================
// TESTES DE PERFORMANCE
// =============================================================================

TEST_F(ThreadSafetyTest, PerformanceComparison) {
    const int NUM_OPERATIONS = 10000;

    // Testa versão thread-safe
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        emblib_circ_buffer_threadsafe_insert(&buffer, &i);
    }

    int data;
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        emblib_circ_buffer_threadsafe_retrieve(&buffer, &data);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto threadsafe_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Thread-safe operations took: " << threadsafe_duration.count() << " microseconds" << std::endl;

    // Note: Para comparação real, seria necessário também testar a versão não thread-safe
    // Este teste serve principalmente para detectar regressões de performance

    // O overhead não deve ser excessivo para operações simples
    ASSERT_LT(threadsafe_duration.count(), 1000000); // Menos de 1 segundo para 10k ops
}

// =============================================================================
// TESTES DE CONFIGURAÇÃO
// =============================================================================

TEST(ThreadSafetyConfigTest, DisabledThreadSafety) {
    emblib_circ_buffer_threadsafe_t buffer;
    int buffer_array[10];

    // Inicializa sem thread safety
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_init(
        &buffer, buffer_array, sizeof(buffer_array),
        sizeof(int), ThreadSafetyTest::int_copy, ThreadSafetyTest::int_free, false));

    // Deve funcionar normalmente
    int data = 123;
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_insert(&buffer, &data));

    int retrieved;
    ASSERT_TRUE(emblib_circ_buffer_threadsafe_retrieve(&buffer, &retrieved));
    ASSERT_EQ(retrieved, 123);

    emblib_circ_buffer_threadsafe_destroy(&buffer);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
