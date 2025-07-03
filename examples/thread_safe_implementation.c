/**
 *  @file   thread_safe_implementation.c
 *  @brief  Exemplo de implementação thread-safe para emb_lib
 *  @author Análise de Thread Safety
 *  @date   July 2025
 */

#include "thread_safe_version.h"
#include <string.h>
#include <assert.h>

// =============================================================================
// IMPLEMENTAÇÃO COM MUTEX (OPÇÃO 1 - RECOMENDADA)
// =============================================================================

bool emblib_circ_buffer_threadsafe_init(emblib_circ_buffer_threadsafe_t *circ_buffer, 
                                        const void *array, const size_t buffer_len,
                                        const size_t size_elem, 
                                        void (*copy_fn)(void *dest, void *src),
                                        void (*free_fn)(void *data),
                                        bool enable_thread_safety) {
    
    bool bRet = false;

    if (circ_buffer && array && buffer_len && size_elem && 
        (buffer_len % size_elem == 0) && copy_fn) {
        
        // Inicialização básica (igual à versão original)
        *circ_buffer = (emblib_circ_buffer_threadsafe_t) {
            .array      = (void *) array,
            .capacity   = buffer_len,
            .elem_size  = size_elem,
            .head       = 0,
            .tail       = 0,
            .count      = 0,
            .copy_fn    = copy_fn,
            .free_fn    = free_fn,
            .thread_safe_enabled = enable_thread_safety
        };

        // Inicialização do mutex se thread safety estiver habilitada
        if (enable_thread_safety) {
            if (EMBLIB_MUTEX_INIT(&circ_buffer->mutex) == 0) {
                bRet = true;
            }
        } else {
            bRet = true;  // Sucesso sem thread safety
        }
    }

    return bRet;
}

bool emblib_circ_buffer_threadsafe_insert(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data) {
    if (!circ_buffer || !data) return false;

    bool bRet = false;
    
    // Lock apenas se thread safety estiver habilitada
    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&circ_buffer->mutex);
    }

    // Operação crítica protegida
    if (!emblib_circ_buffer_threadsafe_is_full_unsafe(circ_buffer)) {
        // Copia dados
        circ_buffer->copy_fn((char *) circ_buffer->array + 
                           (circ_buffer->tail * circ_buffer->elem_size), data);
        
        // Atualiza ponteiros atomicamente
        circ_buffer->tail = (circ_buffer->tail + 1) % 
                           (circ_buffer->capacity / circ_buffer->elem_size);
        circ_buffer->count++;
        
        bRet = true;
    }

    // Unlock
    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&circ_buffer->mutex);
    }

    return bRet;
}

bool emblib_circ_buffer_threadsafe_retrieve(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data) {
    if (!circ_buffer || !data) return false;

    bool bRet = false;

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&circ_buffer->mutex);
    }

    // Operação crítica protegida
    if (!emblib_circ_buffer_threadsafe_is_empty_unsafe(circ_buffer)) {
        // Copia dados
        circ_buffer->copy_fn(data, (char *) circ_buffer->array + 
                           (circ_buffer->head * circ_buffer->elem_size));
        
        // Atualiza ponteiros
        circ_buffer->head = (circ_buffer->head + 1) % 
                           (circ_buffer->capacity / circ_buffer->elem_size);
        circ_buffer->count--;
        
        bRet = true;
    }

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&circ_buffer->mutex);
    }

    return bRet;
}

bool emblib_circ_buffer_threadsafe_peek(emblib_circ_buffer_threadsafe_t *circ_buffer, void *data) {
    if (!circ_buffer || !data) return false;

    bool bRet = false;

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&circ_buffer->mutex);
    }

    // Peek não modifica a estrutura, apenas lê
    if (circ_buffer->count > 0) {
        circ_buffer->copy_fn(data, (char *) circ_buffer->array + 
                           (circ_buffer->head * circ_buffer->elem_size));
        bRet = true;
    }

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&circ_buffer->mutex);
    }

    return bRet;
}

size_t emblib_circ_buffer_threadsafe_count(emblib_circ_buffer_threadsafe_t *circ_buffer) {
    if (!circ_buffer) return 0;

    size_t count;

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&circ_buffer->mutex);
    }

    count = circ_buffer->count;

    if (circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&circ_buffer->mutex);
    }

    return count;
}

void emblib_circ_buffer_threadsafe_destroy(emblib_circ_buffer_threadsafe_t *circ_buffer) {
    if (circ_buffer && circ_buffer->thread_safe_enabled) {
        EMBLIB_MUTEX_DESTROY(&circ_buffer->mutex);
    }
}

// Funções auxiliares não thread-safe (para uso interno)
static inline bool emblib_circ_buffer_threadsafe_is_full_unsafe(emblib_circ_buffer_threadsafe_t *circ_buffer) {
    return circ_buffer->count == (circ_buffer->capacity / circ_buffer->elem_size);
}

static inline bool emblib_circ_buffer_threadsafe_is_empty_unsafe(emblib_circ_buffer_threadsafe_t *circ_buffer) {
    return circ_buffer->count == 0;
}

// =============================================================================
// IMPLEMENTAÇÃO LOCK-FREE (OPÇÃO 2 - AVANÇADA)
// =============================================================================

#ifdef USE_ATOMICS

bool emblib_circ_buffer_lockfree_insert(emblib_circ_buffer_lockfree_t *circ_buffer, void *data) {
    if (!circ_buffer || !data) return false;

    size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    while (true) {
        size_t current_tail = atomic_load(&circ_buffer->tail);
        size_t current_count = atomic_load(&circ_buffer->count);
        
        // Verifica se há espaço
        if (current_count >= capacity) {
            return false; // Buffer cheio
        }
        
        size_t next_tail = (current_tail + 1) % capacity;
        
        // Tenta reservar a posição
        if (atomic_compare_exchange_weak(&circ_buffer->tail, &current_tail, next_tail)) {
            // Posição reservada, agora copia os dados
            circ_buffer->copy_fn((char *) circ_buffer->array + 
                               (current_tail * circ_buffer->elem_size), data);
            
            // Incrementa o contador atomicamente
            atomic_fetch_add(&circ_buffer->count, 1);
            return true;
        }
        
        // CAS falhou, tenta novamente
        // Em sistemas embarcados, pode adicionar um pequeno delay aqui
    }
}

bool emblib_circ_buffer_lockfree_retrieve(emblib_circ_buffer_lockfree_t *circ_buffer, void *data) {
    if (!circ_buffer || !data) return false;

    size_t capacity = circ_buffer->capacity / circ_buffer->elem_size;
    
    while (true) {
        size_t current_head = atomic_load(&circ_buffer->head);
        size_t current_count = atomic_load(&circ_buffer->count);
        
        // Verifica se há dados
        if (current_count == 0) {
            return false; // Buffer vazio
        }
        
        size_t next_head = (current_head + 1) % capacity;
        
        // Tenta reservar a posição
        if (atomic_compare_exchange_weak(&circ_buffer->head, &current_head, next_head)) {
            // Posição reservada, copia os dados
            circ_buffer->copy_fn(data, (char *) circ_buffer->array + 
                               (current_head * circ_buffer->elem_size));
            
            // Decrementa o contador atomicamente
            atomic_fetch_sub(&circ_buffer->count, 1);
            return true;
        }
        
        // CAS falhou, tenta novamente
    }
}

#endif

// =============================================================================
// WRAPPER PARA ESTRUTURAS EXISTENTES
// =============================================================================

/**
 * @brief Wrapper thread-safe para queue existente
 */
typedef struct {
    emblib_queue_t queue;
    EMBLIB_MUTEX_T mutex;
    bool thread_safe_enabled;
} emblib_queue_threadsafe_t;

bool emblib_queue_threadsafe_enqueue(emblib_queue_threadsafe_t *queue, void *data) {
    if (!queue || !data) return false;

    bool result;
    
    if (queue->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&queue->mutex);
    }
    
    result = emblib_queue_enqueue(&queue->queue, data);
    
    if (queue->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&queue->mutex);
    }
    
    return result;
}

bool emblib_queue_threadsafe_dequeue(emblib_queue_threadsafe_t *queue, void *data) {
    if (!queue || !data) return false;

    bool result;
    
    if (queue->thread_safe_enabled) {
        EMBLIB_MUTEX_LOCK(&queue->mutex);
    }
    
    result = emblib_queue_dequeue(&queue->queue, data);
    
    if (queue->thread_safe_enabled) {
        EMBLIB_MUTEX_UNLOCK(&queue->mutex);
    }
    
    return result;
}
