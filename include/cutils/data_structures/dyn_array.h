#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019

#include <stddef.h>
#include <stdbool.h>

struct DynArray
{
    void* data;
    size_t size, capacity;
};

struct DynArray* dyn_init(struct DynArray* dynArr, size_t elemSize);
void dyn_reset(struct DynArray* dynArr, size_t elemSize);
bool dyn_shrinkToFit(struct DynArray* dynArr, size_t elemSize);

bool dyn_append(struct DynArray* restrict dynArr, const void* restrict elem, size_t elemSize);
bool dyn_insert(struct DynArray* dynArr, const void* src, size_t pos, size_t srcLen, size_t elemSize);
bool dyn_extend(struct DynArray* dynArr, const void* src, size_t srcLen, size_t elemSize);
void dyn_remove(struct DynArray* dynArr, size_t from, size_t to, size_t elemSize);

#endif
