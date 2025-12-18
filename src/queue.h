/*
 * queue.h
 *
 * MISRA-oriented, single-producer/single-consumer (SPSC) typed ring-buffer
 * queue.
 *
 * - No dynamic allocation: storage is embedded in the queue instance.
 * - Designed for embedded MCUs.
 * - Default behavior on full: overwrite oldest.
 *
 * Concurrency notes:
 * - This is a Single-Producer Single-Consumer (SPSC) queue.
 * - In "Fail-on-full" mode (QUEUE_OVERWRITE_ON_FULL=0):
 *   - Lock-free if the platform has atomic size_t writes and no memory
 * reordering.
 *   - Internal memory barriers are included to help prevent reordering.
 * - In "Overwrite-on-full" mode (QUEUE_OVERWRITE_ON_FULL=1):
 *   - NOT lock-free! The producer modifies 'tail', which is also modified by
 * the consumer.
 *   - Critical sections (QUEUE_ENTER_CRITICAL / QUEUE_EXIT_CRITICAL) ARE
 * REQUIRED if the producer and consumer can preempt each other (e.g., ISR and
 * Main).
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "queue_version.h"
#include <stdbool.h>
#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
#define QUEUE__UNUSED __attribute__((unused))
#else
#define QUEUE__UNUSED
#endif

#ifndef QUEUE_OVERWRITE_ON_FULL
#define QUEUE_OVERWRITE_ON_FULL 1
#endif

#ifndef QUEUE_BARRIER
#if defined(__GNUC__) || defined(__clang__)
#define QUEUE_BARRIER() __asm__ volatile("" : : : "memory")
#else
#define QUEUE_BARRIER()
#endif
#endif

#ifndef QUEUE_ENTER_CRITICAL
#define QUEUE_ENTER_CRITICAL()                                                 \
        do {                                                                   \
        } while (0)
#endif

#ifndef QUEUE_EXIT_CRITICAL
#define QUEUE_EXIT_CRITICAL()                                                  \
        do {                                                                   \
        } while (0)
#endif

typedef enum {
        QUEUE_STATUS_OK = 0,
        QUEUE_STATUS_EMPTY,
        QUEUE_STATUS_FULL,
        QUEUE_STATUS_BAD_ARG,
        QUEUE_STATUS_OVERWROTE
} queue_status_t;

static inline size_t
queue__next_index(size_t index, size_t ring_size)
{
        index++;
        if (index >= ring_size) {
                index = 0;
        }
        return index;
}

#if QUEUE_OVERWRITE_ON_FULL
#define QUEUE__HANDLE_FULL(q, ring_size, status_var)                           \
        do {                                                                   \
                (q)->tail = queue__next_index((q)->tail, (ring_size));         \
                (status_var) = QUEUE_STATUS_OVERWROTE;                         \
        } while (0)
#else
#define QUEUE__HANDLE_FULL(q, ring_size, status_var)                           \
        do {                                                                   \
                (void)(q);                                                     \
                (void)(ring_size);                                             \
                (status_var) = QUEUE_STATUS_FULL;                              \
        } while (0)
#endif

/*
 * QUEUE_DEFINE(name, type, capacity)
 *
 * Defines:
 * - type: `name##_t`
 * - functions: `name##_init`, `name##_enqueue`, `name##_dequeue`, ...
 *
 * `capacity` is the usable element capacity.
 */
#define QUEUE_DEFINE(name, type, capacity)                                     \
        typedef struct {                                                       \
                type buffer[(capacity) + 1U];                                  \
                size_t head;                                                   \
                size_t tail;                                                   \
        } name##_t;                                                            \
                                                                               \
        static inline QUEUE__UNUSED void name##_init(name##_t *q)              \
        {                                                                      \
                if (!q) {                                                      \
                        return;                                                \
                }                                                              \
                q->head = 0U;                                                  \
                q->tail = 0U;                                                  \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED bool name##_is_empty(const name##_t *q)    \
        {                                                                      \
                return !q || (q->head == q->tail);                             \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED bool name##_is_full(const name##_t *q)     \
        {                                                                      \
                if (!q) {                                                      \
                        return false;                                          \
                }                                                              \
                const size_t ring_size = (capacity) + 1U;                      \
                return queue__next_index(q->head, ring_size) == q->tail;       \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED size_t name##_capacity(void)               \
        {                                                                      \
                return (capacity);                                             \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED size_t name##_count(const name##_t *q)     \
        {                                                                      \
                if (!q) {                                                      \
                        return 0;                                              \
                }                                                              \
                const size_t ring_size = (capacity) + 1U;                      \
                if (q->head >= q->tail) {                                      \
                        return q->head - q->tail;                              \
                }                                                              \
                return ring_size - (q->tail - q->head);                        \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED queue_status_t name##_enqueue(             \
            name##_t *q, const type *item)                                     \
        {                                                                      \
                if (!q || !item) {                                             \
                        return QUEUE_STATUS_BAD_ARG;                           \
                }                                                              \
                                                                               \
                const size_t ring_size = (capacity) + 1U;                      \
                queue_status_t status = QUEUE_STATUS_OK;                       \
                                                                               \
                QUEUE_ENTER_CRITICAL();                                        \
                size_t next_head = queue__next_index(q->head, ring_size);      \
                if (next_head == q->tail) {                                    \
                        QUEUE__HANDLE_FULL(q, ring_size, status);              \
                }                                                              \
                                                                               \
                if (status != QUEUE_STATUS_FULL) {                             \
                        q->buffer[q->head] = *item;                            \
                        QUEUE_BARRIER();                                       \
                        q->head = next_head;                                   \
                }                                                              \
                                                                               \
                QUEUE_EXIT_CRITICAL();                                         \
                return status;                                                 \
        }                                                                      \
                                                                               \
        static inline QUEUE__UNUSED queue_status_t name##_dequeue(name##_t *q, \
                                                                  type *out)   \
        {                                                                      \
                if (!q || !out) {                                              \
                        return QUEUE_STATUS_BAD_ARG;                           \
                }                                                              \
                                                                               \
                const size_t ring_size = (capacity) + 1U;                      \
                QUEUE_ENTER_CRITICAL();                                        \
                if (q->head == q->tail) {                                      \
                        QUEUE_EXIT_CRITICAL();                                 \
                        return QUEUE_STATUS_EMPTY;                             \
                }                                                              \
                                                                               \
                *out = q->buffer[q->tail];                                     \
                QUEUE_BARRIER();                                               \
                q->tail = queue__next_index(q->tail, ring_size);               \
                QUEUE_EXIT_CRITICAL();                                         \
                return QUEUE_STATUS_OK;                                        \
        }

#endif /* QUEUE_H */
