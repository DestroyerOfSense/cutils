#include <stdlib.h>
#include <string.h>

#include "cyclic_queue.h"

#define GROWTH_FACTOR 2

void cycq_expand(struct CyclicQueue* queue, size_t typeSize)
{
    size_t newCapacity = queue->capacity * GROWTH_FACTOR;
    char* newData = malloc(newCapacity * typeSize);
    if (newData)
    {
        if (queue->front + queue->size > queue->capacity)
        {
            int beforeWrap = queue->capacity - queue->front;
            memcpy(newData, queue->data + typeSize * queue->front, typeSize * beforeWrap);
            memcpy(newData + typeSize * beforeWrap, queue->data, (queue->size - beforeWrap) * typeSize);
        }
        else
            memcpy(newData, queue->data + typeSize * queue->front, typeSize * queue->size);
        free(queue->data);
    }
    queue->data = newData, queue->capacity = newCapacity, queue->front = 0;
}

extern inline void cycq_init(struct CyclicQueue* queue, size_t queueSize, size_t typeSize);
extern inline void cycq_push(struct CyclicQueue* restrict queue, const void* restrict elem, size_t typeSize);
extern inline void cycq_pop(struct CyclicQueue* queue);
