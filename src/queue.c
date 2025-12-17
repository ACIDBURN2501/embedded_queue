#include "queue.h"
#include <stdlib.h>

/* Node for linked list representation */
struct queue_node {
        int data;
        struct queue_node *next;
};

/* Queue implementation: head+tail pointers and size counter */
struct queue {
        size_t size;
        struct queue_node *front;
        struct queue_node *rear;
        size_t capacity; /* maximum number of elements */
};

queue_t *
queue_create(size_t capacity)
{
        if (capacity == 0) {
                return NULL;
        }

        queue_t *q = malloc(sizeof(*q));
        if (!q) {
                return NULL;
        }

        q->size = 0;
        q->front = NULL;
        q->rear = NULL;
        q->capacity = capacity;
        return q;
}

void
queue_destroy(queue_t *q)
{
        if (!q) {
                return;
        }
        while (q->front) {
                struct queue_node *tmp = q->front;
                q->front = q->front->next;
                free(tmp);
        }
        free(q);
}

int
queue_enqueue(queue_t *q, int item)
{
        if (!q) {
                return -1;
        }
        if (queue_is_full(q)) {
                return -1;
        }

        struct queue_node *node = malloc(sizeof(*node));
        if (!node) {
                return -1;
        }

        node->data = item;
        node->next = NULL;

        if (q->size == 0) {
                q->front = node;
                q->rear = node;
        } else {
                q->rear->next = node;
                q->rear = node;
        }

        q->size++;
        return 0;
}

bool
queue_dequeue(queue_t *q, int *out_item)
{
        if (!q || !out_item) {
                return false;
        }
        if (queue_is_empty(q)) {
                return false;
        }

        struct queue_node *node = q->front;
        *out_item = node->data;
        q->front = node->next;
        if (!q->front) {
                q->rear = NULL;
        }

        free(node);
        q->size--;
        return true;
}

size_t
queue_size(const queue_t *q)
{
        return q ? q->size : 0;
}

bool
queue_is_empty(const queue_t *q)
{
        return !q || q->size == 0;
}

bool
queue_is_full(const queue_t *q)
{
        if (!q) {
                return false;
        }
        return q->size >= q->capacity;
}
