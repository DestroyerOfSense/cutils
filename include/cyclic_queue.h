/* A dynamically-sized queue, implemented as a ring buffer. */

#ifndef CYCLIC_QUEUE_H
#define CYCLIC_QUEUE_H

#include <string.h>

struct CyclicQueue
{
    void* data;
    size_t capacity, size, front;
};

/*
 * To access the element at the front of the queue (assuming the queue contains elements of type `temp_t`),
 * do something like:
 *
 * temp_t x = ((temp_t*)queue.data)[queue.front];
 */

 // When you're done with the queue, simply free `queue.data`.

/*
 * @params
 * queue: pointer to queue
 * typeSize: `sizeof(elemType)`, where `elemType` is the type of element in the queue
 *
 * `queue->data` will be `NULL` if queue expansion fails.
 */
void cycq_expand(struct CyclicQueue* queue, size_t typeSize);

/*
 * @params
 * queue: pointer to queue
 * capacity: initial queue capacity
 * typeSize: `sizeof(elemType)`, where `elemType` is the type of element in the queue
 *
 * `queue->data` will be `NULL` if allocation fails.
 */
inline void cycq_init(struct CyclicQueue* queue, size_t capacity, size_t typeSize)
{
    queue->data = malloc(capacity * typeSize);
    queue->capacity = capacity, queue->size = 0, queue->front = 0;
}

/*
 * @params
 * queue: pointer to queue
 * elem: pointer to object to be pushed into the queue
 * typeSize: `sizeof(elemType)`, where `elemType` is the type of element in the queue
 *
 * `queue->data` will be `NULL` if queue expansion fails.
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

/*
 * @params
 * queue: pointer to queue
 *
 * `queue->size` must be nonzero or undefined behavior will result.
 */
inline void cycq_pop(struct CyclicQueue* queue)
{
    queue->front = (queue->front + 1) % queue->capacity;
    --queue->size;
}

#endif
