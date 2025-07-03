/**
 *  @file   emblib_thread_safety.h
 *  @brief  Thread safety abstraction layer for emb_lib
 *  @author emb_lib contributors
 *  @date   July 2025
 */

#ifndef __EMBLIB_THREAD_SAFETY_H__
#define __EMBLIB_THREAD_SAFETY_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// COMPILE-TIME CONFIGURATION
// =============================================================================

/**
 * Thread safety can be configured at compile time:
 *
 * -DEMBLIB_THREAD_SAFETY_ENABLED=1  : Enable thread safety
 * -DEMBLIB_USE_ATOMICS=1            : Use C11 atomics (preferred)
 * -DEMBLIB_USE_PTHREAD=1            : Use pthread mutex
 * -DEMBLIB_USE_FREERTOS=1           : Use FreeRTOS semaphores
 * -DEMBLIB_USE_WINDOWS=1            : Use Windows critical sections
 */

// =============================================================================
// THREADING ABSTRACTIONS
// =============================================================================

#ifdef EMBLIB_THREAD_SAFETY_ENABLED

    #ifdef EMBLIB_USE_ATOMICS
        // C11 Atomics - preferred for performance
        #include <stdatomic.h>

        typedef atomic_size_t emblib_atomic_size_t;
        typedef atomic_bool emblib_atomic_bool_t;

        #define EMBLIB_ATOMIC_LOAD(ptr) atomic_load(ptr)
        #define EMBLIB_ATOMIC_STORE(ptr, val) atomic_store(ptr, val)
        #define EMBLIB_ATOMIC_LOAD_EXPLICIT(ptr, order) atomic_load_explicit(ptr, order)
        #define EMBLIB_ATOMIC_STORE_EXPLICIT(ptr, val, order) atomic_store_explicit(ptr, val, order)
        #define EMBLIB_ATOMIC_FETCH_ADD(ptr, val) atomic_fetch_add(ptr, val)
        #define EMBLIB_ATOMIC_FETCH_SUB(ptr, val) atomic_fetch_sub(ptr, val)
        #define EMBLIB_ATOMIC_COMPARE_EXCHANGE_WEAK(ptr, expected, desired) \
            atomic_compare_exchange_weak(ptr, expected, desired)
        #define EMBLIB_ATOMIC_COMPARE_EXCHANGE_WEAK_EXPLICIT(ptr, expected, desired, success_order, failure_order) \
            atomic_compare_exchange_weak_explicit(ptr, expected, desired, success_order, failure_order)

        // Memory ordering aliases
        #define EMBLIB_MEMORY_ORDER_RELAXED memory_order_relaxed
        #define EMBLIB_MEMORY_ORDER_ACQUIRE memory_order_acquire
        #define EMBLIB_MEMORY_ORDER_RELEASE memory_order_release
        #define EMBLIB_MEMORY_ORDER_SEQ_CST memory_order_seq_cst

        // No mutex needed for atomics
        typedef int emblib_mutex_t;
        #define EMBLIB_MUTEX_INIT(m) (0)
        #define EMBLIB_MUTEX_DESTROY(m) do {} while(0)
        #define EMBLIB_MUTEX_LOCK(m) do {} while(0)
        #define EMBLIB_MUTEX_UNLOCK(m) do {} while(0)

    #elif defined(EMBLIB_USE_PTHREAD)
        // POSIX Threads
        #include <pthread.h>

        typedef size_t emblib_atomic_size_t;
        typedef bool emblib_atomic_bool_t;
        typedef pthread_mutex_t emblib_mutex_t;

        #define EMBLIB_MUTEX_INIT(m) pthread_mutex_init(m, NULL)
        #define EMBLIB_MUTEX_DESTROY(m) pthread_mutex_destroy(m)
        #define EMBLIB_MUTEX_LOCK(m) pthread_mutex_lock(m)
        #define EMBLIB_MUTEX_UNLOCK(m) pthread_mutex_unlock(m)

        // Atomic operations via mutex (slower but compatible)
        #define EMBLIB_ATOMIC_LOAD(ptr) (*(ptr))
        #define EMBLIB_ATOMIC_STORE(ptr, val) (*(ptr) = (val))

    #elif defined(EMBLIB_USE_FREERTOS)
        // FreeRTOS
        #include "FreeRTOS.h"
        #include "semphr.h"

        typedef size_t emblib_atomic_size_t;
        typedef bool emblib_atomic_bool_t;
        typedef SemaphoreHandle_t emblib_mutex_t;

        #define EMBLIB_MUTEX_INIT(m) (*(m) = xSemaphoreCreateMutex())
        #define EMBLIB_MUTEX_DESTROY(m) vSemaphoreDelete(*(m))
        #define EMBLIB_MUTEX_LOCK(m) xSemaphoreTake(*(m), portMAX_DELAY)
        #define EMBLIB_MUTEX_UNLOCK(m) xSemaphoreGive(*(m))

        #define EMBLIB_ATOMIC_LOAD(ptr) (*(ptr))
        #define EMBLIB_ATOMIC_STORE(ptr, val) (*(ptr) = (val))

    #elif defined(EMBLIB_USE_WINDOWS)
        // Windows
        #include <windows.h>

        typedef size_t emblib_atomic_size_t;
        typedef bool emblib_atomic_bool_t;
        typedef CRITICAL_SECTION emblib_mutex_t;

        #define EMBLIB_MUTEX_INIT(m) InitializeCriticalSection(m)
        #define EMBLIB_MUTEX_DESTROY(m) DeleteCriticalSection(m)
        #define EMBLIB_MUTEX_LOCK(m) EnterCriticalSection(m)
        #define EMBLIB_MUTEX_UNLOCK(m) LeaveCriticalSection(m)

        #define EMBLIB_ATOMIC_LOAD(ptr) (*(ptr))
        #define EMBLIB_ATOMIC_STORE(ptr, val) (*(ptr) = (val))

    #else
        #error "Thread safety enabled but no backend specified"
    #endif

#else
    // No thread safety - dummy definitions
    typedef size_t emblib_atomic_size_t;
    typedef bool emblib_atomic_bool_t;
    typedef int emblib_mutex_t;

    #define EMBLIB_MUTEX_INIT(m) (0)
    #define EMBLIB_MUTEX_DESTROY(m) do {} while(0)
    #define EMBLIB_MUTEX_LOCK(m) do {} while(0)
    #define EMBLIB_MUTEX_UNLOCK(m) do {} while(0)

    #define EMBLIB_ATOMIC_LOAD(ptr) (*(ptr))
    #define EMBLIB_ATOMIC_STORE(ptr, val) (*(ptr) = (val))

#endif // EMBLIB_THREAD_SAFETY_ENABLED

// =============================================================================
// HELPER MACROS
// =============================================================================

/**
 * @brief Execute code in thread-safe context
 *
 * Usage:
 * EMBLIB_THREAD_SAFE_SECTION(mutex) {
 *     // Critical section code here
 * }
 */
#ifdef EMBLIB_USE_ATOMICS
    #define EMBLIB_THREAD_SAFE_SECTION(mutex) \
        do
#else
    #define EMBLIB_THREAD_SAFE_SECTION(mutex) \
        for (int _emblib_lock = (EMBLIB_MUTEX_LOCK(&(mutex)), 1); \
             _emblib_lock; \
             _emblib_lock = 0, EMBLIB_MUTEX_UNLOCK(&(mutex)))
#endif

/**
 * @brief Check if thread safety is enabled at runtime
 */
static inline bool emblib_is_thread_safe_enabled(void) {
#ifdef EMBLIB_THREAD_SAFETY_ENABLED
    return true;
#else
    return false;
#endif
}

/**
 * @brief Get thread safety backend information
 */
static inline const char* emblib_thread_backend(void) {
#ifdef EMBLIB_USE_ATOMICS
    return "C11_ATOMICS";
#elif defined(EMBLIB_USE_PTHREAD)
    return "PTHREAD";
#elif defined(EMBLIB_USE_FREERTOS)
    return "FREERTOS";
#elif defined(EMBLIB_USE_WINDOWS)
    return "WINDOWS";
#else
    return "NONE";
#endif
}

// =============================================================================
// FUTURE EXTENSIONS
// =============================================================================

/**
 * Reserved for future thread safety extensions:
 * - Reader-writer locks
 * - Condition variables
 * - Thread-local storage
 * - Memory barriers
 */

#ifdef __cplusplus
}
#endif

#endif // __EMBLIB_THREAD_SAFETY_H__
