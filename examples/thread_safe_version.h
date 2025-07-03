/**
 *  @file   thread_safe_version.h
 *  @brief  Exemplo de como implementar thread safety no emb_lib
 *  @author Análise de Thread Safety
 *  @date   July 2025
 */

#ifndef __THREAD_SAFE_VERSION_H__
#define __THREAD_SAFE_VERSION_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>  // Para sistemas POSIX
// #include <windows.h>  // Para Windows
// #include "freertos/FreeRTOS.h"  // Para FreeRTOS

/**
 * ESTRATÉGIAS DE THREAD SAFETY PARA emb_lib
 * ==========================================
 *
 * 1. MUTEX POR ESTRUTURA (Recomendado)
 * 2. LOCK-FREE ALGORITHMS (Avançado)
 * 3. READER-WRITER LOCKS (Para casos específicos)
 * 4. ATOMIC OPERATIONS (Limitado)
 */

// =============================================================================
// OPÇÃO 1: MUTEX POR ESTRUTURA
// =============================================================================

typedef struct emblib_circ_buffer_threadsafe_t {
    void *array;        //!< array pointer elements
    size_t count;       //!< total count
    size_t capacity;    //!< capacity of circ_buffer array
    size_t head;        //!< head element of the circ_buffer
    size_t tail;        //!< tail element of the circ_buffer
    size_t elem_size;   //!< size of each element
    void (*copy_fn)(void *dest, void *src); //! copy function
    void (*free_fn)(void *data);      //! free function

    // Thread safety components
    pthread_mutex_t mutex;  //!< Mutex para sincronização
    bool thread_safe_enabled; //!< Flag para habilitar/desabilitar thread safety
} emblib_circ_buffer_threadsafe_t;

// Macros para diferentes sistemas
#ifdef USE_PTHREAD
    #define EMBLIB_MUTEX_T pthread_mutex_t
    #define EMBLIB_MUTEX_INIT(m) pthread_mutex_init(m, NULL)
    #define EMBLIB_MUTEX_DESTROY(m) pthread_mutex_destroy(m)
    #define EMBLIB_MUTEX_LOCK(m) pthread_mutex_lock(m)
    #define EMBLIB_MUTEX_UNLOCK(m) pthread_mutex_unlock(m)
#elif defined(USE_FREERTOS)
    #define EMBLIB_MUTEX_T SemaphoreHandle_t
    #define EMBLIB_MUTEX_INIT(m) *(m) = xSemaphoreCreateMutex()
    #define EMBLIB_MUTEX_DESTROY(m) vSemaphoreDelete(*(m))
    #define EMBLIB_MUTEX_LOCK(m) xSemaphoreTake(*(m), portMAX_DELAY)
    #define EMBLIB_MUTEX_UNLOCK(m) xSemaphoreGive(*(m))
#elif defined(USE_WINDOWS)
    #define EMBLIB_MUTEX_T CRITICAL_SECTION
    #define EMBLIB_MUTEX_INIT(m) InitializeCriticalSection(m)
    #define EMBLIB_MUTEX_DESTROY(m) DeleteCriticalSection(m)
    #define EMBLIB_MUTEX_LOCK(m) EnterCriticalSection(m)
    #define EMBLIB_MUTEX_UNLOCK(m) LeaveCriticalSection(m)
#else
    // No thread safety - dummy macros
    #define EMBLIB_MUTEX_T int
    #define EMBLIB_MUTEX_INIT(m) do {} while(0)
    #define EMBLIB_MUTEX_DESTROY(m) do {} while(0)
    #define EMBLIB_MUTEX_LOCK(m) do {} while(0)
    #define EMBLIB_MUTEX_UNLOCK(m) do {} while(0)
#endif

// =============================================================================
// FUNÇÕES COM THREAD SAFETY
// =============================================================================

/**
 * @brief Initialize thread-safe circular buffer
 */
bool emblib_circ_buffer_threadsafe_init(emblib_circ_buffer_threadsafe_t *circ_buffer,
                                        const void *array, const size_t buffer_len,
                                        const size_t size_elem,
                                        void (*copy_fn)(void *dest, void *src),
                                        void (*free_fn)(void *data),
                                        bool enable_thread_safety);

/**
 * @brief Thread-safe insert operation
 */
bool emblib_circ_buffer_threadsafe_insert(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data);

/**
 * @brief Thread-safe retrieve operation
 */
bool emblib_circ_buffer_threadsafe_retrieve(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data);

/**
 * @brief Thread-safe peek operation (multiple readers safe)
 */
bool emblib_circ_buffer_threadsafe_peek(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data);

/**
 * @brief Thread-safe count operation
 */
size_t emblib_circ_buffer_threadsafe_count(emblib_circ_buffer_threadsafe_t *circ_buffer);

/**
 * @brief Cleanup resources
 */
void emblib_circ_buffer_threadsafe_destroy(emblib_circ_buffer_threadsafe_t *circ_buffer);

// =============================================================================
// OPÇÃO 2: LOCK-FREE COM ATOMIC OPERATIONS
// =============================================================================

#ifdef USE_ATOMICS
#include <stdatomic.h>

typedef struct emblib_circ_buffer_lockfree_t {
    void *array;
    atomic_size_t count;
    size_t capacity;
    atomic_size_t head;
    atomic_size_t tail;
    size_t elem_size;
    void (*copy_fn)(void *dest, void *src);
    void (*free_fn)(void *data);
} emblib_circ_buffer_lockfree_t;

// Funções lock-free (mais complexas de implementar corretamente)
bool emblib_circ_buffer_lockfree_insert(emblib_circ_buffer_lockfree_t *circ_buffer, void *data);
bool emblib_circ_buffer_lockfree_retrieve(emblib_circ_buffer_lockfree_t *circ_buffer, void *data);

#endif

// =============================================================================
// OPÇÃO 3: READER-WRITER LOCKS
// =============================================================================

#ifdef USE_RWLOCK
typedef struct emblib_circ_buffer_rwlock_t {
    void *array;
    size_t count;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t elem_size;
    void (*copy_fn)(void *dest, void *src);
    void (*free_fn)(void *data);

    pthread_rwlock_t rwlock;  // Permite múltiplos leitores, um escritor
} emblib_circ_buffer_rwlock_t;

// Funções com RW locks
bool emblib_circ_buffer_rwlock_insert(emblib_circ_buffer_rwlock_t *circ_buffer, void *data);
bool emblib_circ_buffer_rwlock_retrieve(emblib_circ_buffer_rwlock_t *circ_buffer, void *data);
bool emblib_circ_buffer_rwlock_peek(emblib_circ_buffer_rwlock_t *circ_buffer, void *data);

#endif

// =============================================================================
// OPÇÃO 4: ATOMICS C11 (RECOMENDADO PARA EMBARCADOS)
// =============================================================================

#ifdef USE_C11_ATOMICS
#include <stdatomic.h>

/**
 * VANTAGENS DOS ATOMICS C11:
 * - Padrão C11, amplamente suportado
 * - Sem overhead de locks para operações simples
 * - Memory ordering explícito e controlado
 * - Ideal para sistemas embarcados
 * - Funciona bem em single-core com preemption
 */

typedef struct emblib_circ_buffer_atomic_t {
    void *array;                    //!< array pointer elements (readonly após init)
    atomic_size_t count;            //!< total count (atomic)
    size_t capacity;                //!< capacity (readonly após init)
    atomic_size_t head;             //!< head element (atomic)
    atomic_size_t tail;             //!< tail element (atomic)
    size_t elem_size;               //!< size of each element (readonly após init)
    void (*copy_fn)(void *dest, void *src); //!< copy function (readonly após init)
    void (*free_fn)(void *data);    //!< free function (readonly após init)

    // Estado de inicialização
    atomic_bool initialized;        //!< Flag de inicialização
} emblib_circ_buffer_atomic_t;

// Memory ordering policies para diferentes cenários
typedef enum {
    EMBLIB_MEMORY_RELAXED,    // Melhor performance, sem garantias de ordering
    EMBLIB_MEMORY_ACQUIRE,    // Para operações de leitura
    EMBLIB_MEMORY_RELEASE,    // Para operações de escrita
    EMBLIB_MEMORY_SEQ_CST     // Máxima segurança, sequential consistency
} emblib_memory_order_t;

// Funções com atomics C11
bool emblib_circ_buffer_atomic_init(emblib_circ_buffer_atomic_t *circ_buffer,
                                   const void *array, const size_t buffer_len,
                                   const size_t size_elem,
                                   void (*copy_fn)(void *dest, void *src),
                                   void (*free_fn)(void *data));

bool emblib_circ_buffer_atomic_insert(emblib_circ_buffer_atomic_t *circ_buffer, void *data);
bool emblib_circ_buffer_atomic_insert_relaxed(emblib_circ_buffer_atomic_t *circ_buffer, void *data);

bool emblib_circ_buffer_atomic_retrieve(emblib_circ_buffer_atomic_t *circ_buffer, void *data);
bool emblib_circ_buffer_atomic_retrieve_relaxed(emblib_circ_buffer_atomic_t *circ_buffer, void *data);

// Operações de leitura (não modificam estado)
bool emblib_circ_buffer_atomic_peek(emblib_circ_buffer_atomic_t *circ_buffer, void *data);
size_t emblib_circ_buffer_atomic_count(emblib_circ_buffer_atomic_t *circ_buffer);
bool emblib_circ_buffer_atomic_is_empty(emblib_circ_buffer_atomic_t *circ_buffer);
bool emblib_circ_buffer_atomic_is_full(emblib_circ_buffer_atomic_t *circ_buffer);

// Operações especiais
bool emblib_circ_buffer_atomic_try_insert(emblib_circ_buffer_atomic_t *circ_buffer, void *data, int max_retries);
bool emblib_circ_buffer_atomic_try_retrieve(emblib_circ_buffer_atomic_t *circ_buffer, void *data, int max_retries);

// Flush atômico (para cleanup)
void emblib_circ_buffer_atomic_flush(emblib_circ_buffer_atomic_t *circ_buffer);

#endif // USE_C11_ATOMICS

// =============================================================================
// HÍBRIDO: ATOMICS + MUTEX PARA OPERAÇÕES COMPLEXAS
// =============================================================================

#ifdef USE_HYBRID_APPROACH
typedef struct emblib_circ_buffer_hybrid_t {
    // Campos atômicos para operações rápidas
    atomic_size_t count;
    atomic_size_t head;
    atomic_size_t tail;
    atomic_bool initialized;

    // Campos não-atômicos protegidos por mutex quando necessário
    void *array;
    size_t capacity;
    size_t elem_size;
    void (*copy_fn)(void *dest, void *src);
    void (*free_fn)(void *data);

    // Mutex apenas para operações complexas (flush, resize, etc.)
    pthread_mutex_t complex_ops_mutex;
} emblib_circ_buffer_hybrid_t;

// Fast path (apenas atomics)
bool emblib_circ_buffer_hybrid_insert_fast(emblib_circ_buffer_hybrid_t *circ_buffer, void *data);
bool emblib_circ_buffer_hybrid_retrieve_fast(emblib_circ_buffer_hybrid_t *circ_buffer, void *data);

// Slow path (mutex para operações complexas)
void emblib_circ_buffer_hybrid_flush_safe(emblib_circ_buffer_hybrid_t *circ_buffer);
bool emblib_circ_buffer_hybrid_resize_safe(emblib_circ_buffer_hybrid_t *circ_buffer, size_t new_capacity);

#endif // USE_HYBRID_APPROACH

// =============================================================================
// COMPARAÇÃO DE ABORDAGENS
// =============================================================================

/**
 * COMPARAÇÃO DE PERFORMANCE E CARACTERÍSTICAS:
 *
 * 1. MUTEX (pthread_mutex_t):
 *    - Overhead: ~20-50ns por operação
 *    - Memory: +32 bytes
 *    - Pros: Simples, confiável, funciona sempre
 *    - Cons: Pode bloquear threads, priority inversion
 *
 * 2. ATOMICS C11:
 *    - Overhead: ~2-10ns por operação
 *    - Memory: +0 bytes (apenas change de tipos)
 *    - Pros: Lock-free, baixo overhead, padrão C11
 *    - Cons: Complexo de implementar corretamente, ABA problem
 *
 * 3. HYBRID (Atomics + Mutex):
 *    - Overhead: ~2-10ns (fast path), ~20-50ns (slow path)
 *    - Memory: +32 bytes
 *    - Pros: Melhor de ambos os mundos
 *    - Cons: Complexidade de implementação
 *
 * 4. LOCK-FREE COMPLETO:
 *    - Overhead: ~5-15ns por operação
 *    - Memory: +0 bytes
 *    - Pros: Máxima performance, sem blocking
 *    - Cons: Extremamente complexo, difícil de debugar
 */

// =============================================================================
// CONFIGURAÇÕES PARA DIFERENTES SISTEMAS EMBARCADOS
// =============================================================================

// Para sistemas single-core com preemption
#ifdef EMBARCADO_SINGLE_CORE
    #define EMBLIB_ATOMIC_MEMORY_ORDER memory_order_relaxed
    #define EMBLIB_NEED_CRITICAL_SECTION 1
#endif

// Para sistemas multi-core ARM
#ifdef EMBARCADO_MULTI_CORE_ARM
    #define EMBLIB_ATOMIC_MEMORY_ORDER memory_order_acquire_release
    #define EMBLIB_NEED_MEMORY_BARRIERS 1
#endif

// Para sistemas x86/x64
#ifdef SISTEMA_X86
    #define EMBLIB_ATOMIC_MEMORY_ORDER memory_order_seq_cst
    #define EMBLIB_STRONG_MEMORY_MODEL 1
#endif

// Para sistemas com DMA/cache coherency issues
#ifdef SISTEMA_COM_DMA
    #define EMBLIB_NEED_CACHE_FLUSH 1
    #define EMBLIB_ATOMIC_MEMORY_ORDER memory_order_seq_cst
#endif

#endif // __THREAD_SAFE_VERSION_H__
