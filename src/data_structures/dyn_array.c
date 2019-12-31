#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cutils/data_structures/dyn_array.h"
#include "cutils/math/constants.h"

#define GROWTH_FACTOR MATH_PHI
#define DEFAULT_INITIAL_CAPACITY 8

static bool reallocData(struct DynArray* dynArr, size_t newCapacity, size_t elemSize)
{
    void* newData = realloc(dynArr->data, newCapacity * elemSize);
    if (newData)
        dynArr->data = newData, dynArr->capacity = newCapacity;
    return newData;
}

// TODO: Document possible raising of `FE_INEXACT`, and possibly provide convenient solution.
#define EXPAND(dynArr, elemSize) (reallocData((dynArr), GROWTH_FACTOR * (dynArr)->capacity + 0.5, (elemSize)))

struct DynArray* dyn_init(struct DynArray* dynArr, size_t elemSize)
{
    if (!dynArr)
        dynArr = malloc(sizeof(struct DynArray));
    if (dynArr)
    {
        dynArr->data = malloc(DEFAULT_INITIAL_CAPACITY * elemSize);
        if (dynArr->data)
            dynArr->capacity = DEFAULT_INITIAL_CAPACITY;
        dynArr->size = 0;
    }
    return dynArr;
}

void dyn_reset(struct DynArray* dynArr, size_t elemSize)
{
    free(dynArr->data);
    memset(dynArr, 0, sizeof(struct DynArray));
}

bool dyn_shrinkToFit(struct DynArray* dynArr, size_t elemSize)
{
    return !dynArr->size || reallocData(dynArr, dynArr->size, elemSize);
}

bool dyn_append(struct DynArray* restrict dynArr, const void* restrict elem, size_t elemSize)
{
    if (dynArr->size < dynArr->capacity || EXPAND(dynArr, elemSize))
    {
        memcpy((char*)dynArr->data + elemSize * dynArr->size, elem, elemSize);
        ++dynArr->size;
        return true;
    }
    else
        return false;
}

bool dyn_insert(struct DynArray* dynArr, const void* src, size_t pos, size_t srcLen, size_t elemSize)
{
    while (dynArr->capacity < dynArr->size + srcLen)
    {
        if (!EXPAND(dynArr, elemSize))
            return false;
    }
    char* data = dynArr->data;
    size_t scaledPos = pos * elemSize, scaledSrcLen = srcLen * elemSize;
    memmove(data + scaledPos + scaledSrcLen, data + scaledPos, dynArr->size * elemSize - scaledPos);
    memmove(data + scaledPos, src, scaledSrcLen);
    dynArr->size += srcLen;
    return true;
}

bool dyn_extend(struct DynArray* dynArr, const void* src, size_t srcLen, size_t elemSize)
{
    return dyn_insert(dynArr, src, dynArr->size, srcLen, elemSize);
}

void dyn_remove(struct DynArray* dynArr, size_t from, size_t to, size_t elemSize)
{
    size_t scaledFrom = from * elemSize, scaledTo = to * elemSize;
    memmove((char*)dynArr->data + scaledFrom, (char*)dynArr->data + scaledTo, dynArr->size * elemSize - scaledTo);
    dynArr->size -= (to - from);
}
