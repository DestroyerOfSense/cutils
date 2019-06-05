/* A dynamically-sized queue, implemented as a ring buffer. */

#ifndef CYCLIC_QUEUE_H
#define CYCLIC_QUEUE_H

#include <string.h>
#include <stdlib.h>

/**
 * @var data
 *     this queue's internal array. Must be dynamically allocated.
 * @var front
 *     pointer to element in front of queue (i.e. the next to be popped) or to `NULL` if queue is empty
 * @var back
 *     pointer to element in back of queue (i.e. the last to be pushed) or to `NULL` if queue is empty
 * @var capacity
 *     maximum number of elements that can fit in queue before it must be expanded
 * @var size
 *     current number of elements in queue
 */
struct CyclicQueue
{
    void *data, *front, *back;
    size_t capacity, size;
};

// not part of external interface
size_t cycq_frontIndex(const struct CyclicQueue* queue, size_t typeSize);
size_t cycq_backIndex(const struct CyclicQueue* queue, size_t typeSize);
void cycq_expand(struct CyclicQueue* queue, size_t typeSize);

/**
 * Initializes a `CyclicQueue` with suitable default values. `queue->data` will be `NULL` if allocation fails.
 *
 * @param queue
 *     pointer to queue
 * @param capacity
 *     initial queue capacity. Should always be nonzero.
 * @param typeSize
 *     `sizeof(elemType)`, where `elemType` is the type of element in the queue
 */
inline void cycq_init(struct CyclicQueue* queue, size_t capacity, size_t typeSize)
{
    queue->data = malloc(capacity * typeSize), queue->front = NULL, queue->back = NULL;
    queue->capacity = capacity, queue->size = 0;
}

/**
 * Pushes an element to the back of the queue. `queue->data` will be `NULL` if queue expansion fails.
 *
 * @param
 *     queue: pointer to queue
 * @param elem
 *     pointer to object to be pushed into the queue
 * @param typeSize
 *     `sizeof(elemType)`, where `elemType` is the type of element in the queue
 */
void cycq_push(struct CyclicQueue* restrict queue, const void* restrict elem, size_t typeSize);

/**
 * Pops the element at the front of the queue.
 *
 * @param queue
 *     pointer to nonempty queue
 * @param typeSize
 *     `sizeof(elemType)`, where `elemType` is the type of element in the queue
 */
inline void cycq_pop(struct CyclicQueue* queue, size_t typeSize)
{
    if (queue->size == 1)
        queue->front = NULL, queue->back = NULL;
    else
    {
        size_t newFrontIndex = (cycq_frontIndex(queue, typeSize) + 1) % queue->capacity;
        queue->front = (char*)queue->data + newFrontIndex * typeSize;
    }
    --queue->size;
}

#endif
