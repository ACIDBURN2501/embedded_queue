/*
 * queue.h
 * Fixed-capacity FIFO queue of int values.
 *
 * Notes:
 * - This implementation is not thread-safe.
 * - Capacity must be > 0.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

/* Opaque queue type */
struct queue;
typedef struct queue queue_t;

/* Create a new empty queue with the given capacity. Returns NULL on error. */
queue_t *queue_create(size_t capacity);

/* Destroy the queue and free resources. Safe to call with NULL. */
void queue_destroy(queue_t *q);

/* Enqueue an integer item. Return 0 on success, -1 on failure/full. */
int queue_enqueue(queue_t *q, int item);

/* Dequeue into *out_item. Returns true on success, false if empty/failure. */
bool queue_dequeue(queue_t *q, int *out_item);

/* Return the current number of items in the queue (0 if q is NULL). */
size_t queue_size(const queue_t *q);

/* Return true if the queue is empty (true if q is NULL). */
bool queue_is_empty(const queue_t *q);

/* Return true if the queue is full (false if q is NULL). */
bool queue_is_full(const queue_t *q);

#endif /* QUEUE_H */
