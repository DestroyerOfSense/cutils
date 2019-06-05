/* A dynamically-sized queue, implemented as a ring buffer. */

#ifndef CYCLIC_QUEUE_H
#define CYCLIC_QUEUE_H

#include <string.h>
#include <stdlib.h>

/**
 * @var data
 *     this queue's internal array. Must be dynamically allocated.
 * @var capacity
 *     maximum number of elements that can fit in queue before it must be expanded
 * @var size
 *     current number of elements in queue
 * @var front
 *     index of element at front of queue in `data`, assuming `data` has been cast to element type
 */
struct CyclicQueue
{
    void* data;
    size_t capacity, size, front;
};

/*
 * To access the element at the front of the queue (assuming the queue contains elements of type `temp_t`),
 * do something like
 *
 *     temp_t x = ((temp_t*)queue.data)[queue.front];
 * or
 *
 *     temp_t* data = queue.data;
 *     temp_t x = data[queue.front];
 */

 // When you're done with the queue, simply free `queue.data`.

void cycq_expand(struct CyclicQueue* queue, size_t typeSize);

/**
 * Convenience function that initializes a `CyclicQueue` with suitable default values. `queue->data` will be `NULL` if
 * allocation fails.
 *
 * @param queue
 *     pointer to queue
 * @param capacity
 *     initial queue capacity
 * @param typeSize
 *     `sizeof(elemType)`, where `elemType` is the type of element in the queue
 */
inline void cycq_init(struct CyclicQueue* queue, size_t capacity, size_t typeSize)
{
    queue->data = malloc(capacity * typeSize);
    queue->capacity = capacity, queue->size = 0, queue->front = 0;
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
inline void cycq_push(struct CyclicQueue* restrict queue, const void* restrict elem, size_t typeSize)
{
    if (queue->size == queue->capacity)
        cycq_expand(queue, typeSize);
    if (queue->data)
    {
        size_t i = ((queue->front + queue->size) % queue->capacity) * typeSize;
        memcpy((char*)queue->data + i, elem, typeSize);
        ++queue->size;
    }
}

/**
 * Pops the element at the front of the queue. `queue->size` must be nonzero or undefined behavior will result.
 *
 * @param queue
 *     pointer to queue
 */
inline void cycq_pop(struct CyclicQueue* queue)
{
    queue->front = (queue->front + 1) % queue->capacity;
    --queue->size;
}

#endif
