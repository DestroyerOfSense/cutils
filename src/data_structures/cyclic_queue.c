#include <stdlib.h>
#include <string.h>

#include "data_structures/cyclic_queue.h"

#define GROWTH_FACTOR 2

extern inline size_t cycq_frontIndex(const struct CyclicQueue* queue, size_t typeSize);
extern inline size_t cycq_backIndex(const struct CyclicQueue* queue, size_t typeSize);
extern inline void cycq_init(struct CyclicQueue* queue, size_t capacity, size_t typeSize);
extern inline void cycq_pop(struct CyclicQueue* queue, size_t typeSize);

void cycq_expand(struct CyclicQueue* queue, size_t typeSize)
{
    size_t newCapacity = queue->capacity * GROWTH_FACTOR;
    char* newData = malloc(newCapacity * typeSize);
    if (newData)
    {
        size_t frontIndex = cycq_frontIndex(queue, typeSize), backIndex = cycq_backIndex(queue, typeSize);
        if (backIndex < frontIndex)
        {
            size_t beforeWrap = queue->capacity - frontIndex;
            memcpy(newData, queue->front, typeSize * beforeWrap);
            memcpy(newData + typeSize * beforeWrap, queue->data, (queue->size - beforeWrap) * typeSize);
        }
        else
            memcpy(newData, queue->front, typeSize * queue->size);
        free(queue->data);
    }
    queue->data = newData, queue->front = newData;
    queue->back = newData + (queue->size - 1) * typeSize;
    queue->capacity = newCapacity;
}

void cycq_push(struct CyclicQueue* restrict queue, const void* restrict elem, size_t typeSize)
{
    if (queue->size == queue->capacity)
        cycq_expand(queue, typeSize);
    if (queue->data)
    {
        size_t newBackIndex;
        if (!queue->size)
            queue->front = queue->data, queue->back = queue->data;
        else
        {
            size_t newBackIndex = (cycq_backIndex(queue, typeSize) + 1) % queue->capacity;
            queue->back = (char*)queue->data + newBackIndex * typeSize;
        }
        memcpy(queue->back, elem, typeSize);
        ++queue->size;
    }
}
