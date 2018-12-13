#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stddef.h>

struct DynArray;

struct DynArray* dyn_init(struct DynArray* arr, size_t len, size_t elemSize);
void dyn_reset(struct DynArray* arr);
struct DynArray* dyn_setElemCount(struct DynArray* arr, size_t elemCount, size_t elemSize);
struct DynArray* dyn_incrElemCount(struct DynArray* arr, size_t elemSize);
void dyn_shrinkToFit(struct DynArray* arr, size_t elemSize);
void dyn_append(struct DynArray* dst, const void* restrict src, size_t srcLen, size_t elemSize);
void dyn_insert(struct DynArray* dst, const void* src, size_t pos, size_t srcLen, size_t elemSize);

#define dyn_push(arr, elem)                                               \
{                                                                         \
	dyn_incrElemCount(arr, sizeof elem)->data[arr->elemCount - 1] = elem; \
}

#endif