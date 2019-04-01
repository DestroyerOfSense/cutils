#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <string.h>
#include <stdlib.h>

struct DynArray
{
	void* data;
	size_t len;
	size_t capacity;
};

void* dyn_setLen(struct DynArray* arr, size_t len, size_t elemSize);
void* dyn_shrinkToFit(struct DynArray* arr, size_t elemSize);
void dyn_insert(struct DynArray* dst, const void* src, size_t pos, size_t srcLen, size_t elemSize);

inline struct DynArray* dyn_init(struct DynArray* arr, size_t elemSize)
{
	arr->data = malloc(elemSize << 3);
	arr->len = 0;
	arr->capacity = 8;
	return arr;
}

inline void dyn_reset(struct DynArray* arr)
{
	free(arr->data);
	arr->len = 0;
	arr->capacity = 0;
}

inline void* dyn_incrLen(struct DynArray* arr, size_t elemSize)
{
	return dyn_setLen(arr, arr->len + 1, elemSize);
}

inline void dyn_append(struct DynArray* restrict dst, const void* restrict src, size_t srcLen, size_t elemSize)
{
	size_t oldLen = dst->len;
	char* elems = dyn_setLen(dst, dst->len + srcLen, elemSize);
	if (elems)
		memcpy(elems + elemSize*oldLen, src, elemSize * srcLen);
}

inline void dyn_remove(struct DynArray* arr, size_t from, size_t to, size_t elemSize)
{
	memmove((char*)arr->data + from*elemSize, (char*)arr->data + to*elemSize, (arr->len - to) * elemSize);
	arr->len -= (to - from);
}

#endif