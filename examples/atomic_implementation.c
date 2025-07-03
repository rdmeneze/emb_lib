/**
 *  @file   atomic_implementation.c
 *  @brief  Implementação usando atomics C11 para thread safety
 *  @author Atomic Thread Safety Implementation
 *  @date   July 2025
 */

#include "thread_safe_version.h"
#include <string.h>
#include <assert.h>

#ifdef USE_C11_ATOMICS

// =============================================================================
// IMPLEMENTAÇÃO COM ATOMICS C11
// =============================================================================

bool emblib_circ_buffer_atomic_init(emblib_circ_buffer_atomic_t *circ_buffer,
                                   const void *array, const size_t buffer_len,
                                   const size_t size_elem,
                                   void (*copy_fn)(void *dest, void *src),
                                   void (*free_fn)(void *data)) {
    
    if (!circ_buffer || !array || !buffer_len || !size_elem || 
        (buffer_len % size_elem != 0) || !copy_fn) {
        return false;
    }

    // Inicialização não-atômica (chamada apenas uma vez antes do uso)
    circ_buffer->array = (void *)array;
    circ_buffer->capacity = buffer_len;
    circ_buffer->elem_size = size_elem;
    circ_buffer->copy_fn = copy_fn;
    circ_buffer->free_fn = free_fn;

    // Inicialização atômica
    atomic_store(&circ_buffer->count, 0);
    atomic_store(&circ_buffer->head, 0);
    atomic_store(&circ_buffer->tail, 0);
    
    // Última operação - marca como inicializado
    atomic_store(&circ_buffer->initialized, true);

    return true;
}

bool emblib_circ_buffer_atomic_insert(emblib_circ_buffer_atomic_t *circ_buffer, void *data) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    const size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    // Tentativa com CAS (Compare-And-Swap)
    while (true) {
        // Lê estado atual
        size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
        
        // Verifica se há espaço
        if (current_count >= capacity) {
            return false; // Buffer cheio
        }
        
        // Lê tail atual
        size_t current_tail = atomic_load_explicit(&circ_buffer->tail, memory_order_acquire);
        size_t next_tail = (current_tail + 1) % capacity;
        
        // Tenta reservar posição no tail
        if (atomic_compare_exchange_weak_explicit(&circ_buffer->tail, 
                                                  &current_tail, next_tail,
                                                  memory_order_release, 
                                                  memory_order_relaxed)) {
            
            // Posição reservada! Agora copia os dados
            void *dest = (char *)circ_buffer->array + (current_tail * circ_buffer->elem_size);
            circ_buffer->copy_fn(dest, data);
            
            // Incrementa contador atomicamente
            atomic_fetch_add_explicit(&circ_buffer->count, 1, memory_order_release);
            
            return true;
        }
        
        // CAS falhou, outra thread modificou tail
        // Em sistemas embarcados, pode adicionar yield ou pequeno delay
        #ifdef EMBARCADO_SINGLE_CORE
            // Em single-core, yield para dar chance a outras threads
            __asm__ volatile("yield" ::: "memory"); // ARM
        #endif
    }
}

bool emblib_circ_buffer_atomic_retrieve(emblib_circ_buffer_atomic_t *circ_buffer, void *data) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    const size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    while (true) {
        // Lê estado atual
        size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
        
        // Verifica se há dados
        if (current_count == 0) {
            return false; // Buffer vazio
        }
        
        // Lê head atual
        size_t current_head = atomic_load_explicit(&circ_buffer->head, memory_order_acquire);
        size_t next_head = (current_head + 1) % capacity;
        
        // Tenta reservar posição no head
        if (atomic_compare_exchange_weak_explicit(&circ_buffer->head, 
                                                  &current_head, next_head,
                                                  memory_order_release, 
                                                  memory_order_relaxed)) {
            
            // Posição reservada! Copia os dados
            void *src = (char *)circ_buffer->array + (current_head * circ_buffer->elem_size);
            circ_buffer->copy_fn(data, src);
            
            // Decrementa contador atomicamente
            atomic_fetch_sub_explicit(&circ_buffer->count, 1, memory_order_release);
            
            return true;
        }
        
        // CAS falhou, tenta novamente
        #ifdef EMBARCADO_SINGLE_CORE
            __asm__ volatile("yield" ::: "memory");
        #endif
    }
}

// Versão "relaxed" para casos onde performance é crítica
bool emblib_circ_buffer_atomic_insert_relaxed(emblib_circ_buffer_atomic_t *circ_buffer, void *data) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    const size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    // Usando memory_order_relaxed para máxima performance
    while (true) {
        size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_relaxed);
        
        if (current_count >= capacity) {
            return false;
        }
        
        size_t current_tail = atomic_load_explicit(&circ_buffer->tail, memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % capacity;
        
        if (atomic_compare_exchange_weak_explicit(&circ_buffer->tail, 
                                                  &current_tail, next_tail,
                                                  memory_order_relaxed, 
                                                  memory_order_relaxed)) {
            
            void *dest = (char *)circ_buffer->array + (current_tail * circ_buffer->elem_size);
            circ_buffer->copy_fn(dest, data);
            
            atomic_fetch_add_explicit(&circ_buffer->count, 1, memory_order_relaxed);
            return true;
        }
    }
}

bool emblib_circ_buffer_atomic_peek(emblib_circ_buffer_atomic_t *circ_buffer, void *data) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    // Peek é mais simples - não modifica head/tail
    size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
    
    if (current_count == 0) {
        return false;
    }
    
    // Lê head sem modificar
    size_t current_head = atomic_load_explicit(&circ_buffer->head, memory_order_acquire);
    
    void *src = (char *)circ_buffer->array + (current_head * circ_buffer->elem_size);
    circ_buffer->copy_fn(data, src);
    
    return true;
}

size_t emblib_circ_buffer_atomic_count(emblib_circ_buffer_atomic_t *circ_buffer) {
    if (!circ_buffer || !atomic_load(&circ_buffer->initialized)) {
        return 0;
    }
    
    return atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
}

bool emblib_circ_buffer_atomic_is_empty(emblib_circ_buffer_atomic_t *circ_buffer) {
    return emblib_circ_buffer_atomic_count(circ_buffer) == 0;
}

bool emblib_circ_buffer_atomic_is_full(emblib_circ_buffer_atomic_t *circ_buffer) {
    if (!circ_buffer || !atomic_load(&circ_buffer->initialized)) {
        return true; // Safe side
    }
    
    size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    return atomic_load_explicit(&circ_buffer->count, memory_order_acquire) >= capacity;
}

// Versão com retry limit para evitar loops infinitos
bool emblib_circ_buffer_atomic_try_insert(emblib_circ_buffer_atomic_t *circ_buffer, 
                                         void *data, int max_retries) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    const size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    int retries = 0;
    
    while (retries < max_retries) {
        size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
        
        if (current_count >= capacity) {
            return false;
        }
        
        size_t current_tail = atomic_load_explicit(&circ_buffer->tail, memory_order_acquire);
        size_t next_tail = (current_tail + 1) % capacity;
        
        if (atomic_compare_exchange_weak_explicit(&circ_buffer->tail, 
                                                  &current_tail, next_tail,
                                                  memory_order_release, 
                                                  memory_order_relaxed)) {
            
            void *dest = (char *)circ_buffer->array + (current_tail * circ_buffer->elem_size);
            circ_buffer->copy_fn(dest, data);
            
            atomic_fetch_add_explicit(&circ_buffer->count, 1, memory_order_release);
            return true;
        }
        
        retries++;
        
        // Backoff exponencial simples para reduzir contenção
        for (int i = 0; i < (1 << (retries % 4)); i++) {
            #ifdef EMBARCADO_SINGLE_CORE
                __asm__ volatile("yield" ::: "memory");
            #else
                __asm__ volatile("pause" ::: "memory"); // x86
            #endif
        }
    }
    
    return false; // Falhou após max_retries tentativas
}

void emblib_circ_buffer_atomic_flush(emblib_circ_buffer_atomic_t *circ_buffer) {
    if (!circ_buffer || !atomic_load(&circ_buffer->initialized)) {
        return;
    }

    // Flush atômico - reseta todos os ponteiros
    atomic_store_explicit(&circ_buffer->head, 0, memory_order_release);
    atomic_store_explicit(&circ_buffer->tail, 0, memory_order_release);
    atomic_store_explicit(&circ_buffer->count, 0, memory_order_release);
    
    // Se há função de free, chama para cada elemento
    // ATENÇÃO: Esta operação pode não ser totalmente thread-safe
    // se outras threads estiverem acessando simultaneamente
    if (circ_buffer->free_fn) {
        // TODO: Implementar flush seguro com coordination entre threads
        // Para agora, apenas reseta os ponteiros
    }
}

// =============================================================================
// IMPLEMENTAÇÃO HÍBRIDA (ATOMICS + MUTEX PARA OPERAÇÕES COMPLEXAS)
// =============================================================================

#ifdef USE_HYBRID_APPROACH

bool emblib_circ_buffer_hybrid_init(emblib_circ_buffer_hybrid_t *circ_buffer,
                                   const void *array, const size_t buffer_len,
                                   const size_t size_elem,
                                   void (*copy_fn)(void *dest, void *src),
                                   void (*free_fn)(void *data)) {
    
    if (!circ_buffer || !array || !buffer_len || !size_elem || 
        (buffer_len % size_elem != 0) || !copy_fn) {
        return false;
    }

    // Inicializa mutex primeiro
    if (pthread_mutex_init(&circ_buffer->complex_ops_mutex, NULL) != 0) {
        return false;
    }

    // Campos não-atômicos
    circ_buffer->array = (void *)array;
    circ_buffer->capacity = buffer_len;
    circ_buffer->elem_size = size_elem;
    circ_buffer->copy_fn = copy_fn;
    circ_buffer->free_fn = free_fn;

    // Campos atômicos
    atomic_store(&circ_buffer->count, 0);
    atomic_store(&circ_buffer->head, 0);
    atomic_store(&circ_buffer->tail, 0);
    atomic_store(&circ_buffer->initialized, true);

    return true;
}

// Fast path - apenas atomics (mesmo código que antes)
bool emblib_circ_buffer_hybrid_insert_fast(emblib_circ_buffer_hybrid_t *circ_buffer, void *data) {
    if (!circ_buffer || !data || !atomic_load(&circ_buffer->initialized)) {
        return false;
    }

    const size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    // Implementação idêntica à versão atomic_insert
    while (true) {
        size_t current_count = atomic_load_explicit(&circ_buffer->count, memory_order_acquire);
        
        if (current_count >= capacity) {
            return false;
        }
        
        size_t current_tail = atomic_load_explicit(&circ_buffer->tail, memory_order_acquire);
        size_t next_tail = (current_tail + 1) % capacity;
        
        if (atomic_compare_exchange_weak_explicit(&circ_buffer->tail, 
                                                  &current_tail, next_tail,
                                                  memory_order_release, 
                                                  memory_order_relaxed)) {
            
            void *dest = (char *)circ_buffer->array + (current_tail * circ_buffer->elem_size);
            circ_buffer->copy_fn(dest, data);
            
            atomic_fetch_add_explicit(&circ_buffer->count, 1, memory_order_release);
            return true;
        }
    }
}

// Slow path - mutex para operações que requerem consistência
void emblib_circ_buffer_hybrid_flush_safe(emblib_circ_buffer_hybrid_t *circ_buffer) {
    if (!circ_buffer || !atomic_load(&circ_buffer->initialized)) {
        return;
    }

    pthread_mutex_lock(&circ_buffer->complex_ops_mutex);
    
    // Com mutex, podemos fazer operações mais complexas seguramente
    size_t count = atomic_load(&circ_buffer->count);
    
    if (circ_buffer->free_fn && count > 0) {
        // Chama free_fn para cada elemento
        size_t head = atomic_load(&circ_buffer->head);
        size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
        
        for (size_t i = 0; i < count; i++) {
            size_t index = (head + i) % capacity;
            void *element = (char *)circ_buffer->array + (index * circ_buffer->elem_size);
            circ_buffer->free_fn(element);
        }
    }
    
    // Reseta ponteiros atomicamente
    atomic_store(&circ_buffer->head, 0);
    atomic_store(&circ_buffer->tail, 0);
    atomic_store(&circ_buffer->count, 0);
    
    pthread_mutex_unlock(&circ_buffer->complex_ops_mutex);
}

#endif // USE_HYBRID_APPROACH

#endif // USE_C11_ATOMICS
