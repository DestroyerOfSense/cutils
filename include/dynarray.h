#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <string.h>
#include <stdlib.h>

struct DynArray
{
	void* data;
	size_t elemCount;
	size_t len;
};

void* dyn_setElemCount(struct DynArray* arr, size_t elemCount, size_t elemSize);
void* dyn_shrinkToFit(struct DynArray* arr, size_t elemSize);
void dyn_insert(struct DynArray* dst, const void* src, size_t pos, size_t srcLen, size_t elemSize);

inline struct DynArray* dyn_init(struct DynArray* arr, size_t elemSize)
{
	arr->data = malloc(elemSize << 3);
	arr->elemCount = 0;
	arr->len = 8;
	return arr;
}

inline void dyn_reset(struct DynArray* arr)
{
	free(arr->data);
	arr->elemCount = 0;
	arr->len = 0;
}

inline void* dyn_incrElemCount(struct DynArray* arr, size_t elemSize)
{
	return dyn_setElemCount(arr, arr->elemCount + 1, elemSize);
}

inline void dyn_append(struct DynArray* dst, const void* src, size_t srcLen, size_t elemSize)
{
	size_t oldElemCount = dst->elemCount;
	char* elems = dyn_setElemCount(dst, dst->elemCount + srcLen, elemSize);
	if (elems)
		memmove(elems + elemSize*oldElemCount, src, elemSize * srcLen);
}

inline void dyn_remove(struct DynArray* arr, size_t from, size_t to, size_t elemSize)
{
	memmove((char*)arr->data + from*elemSize, (char*)arr->data + to*elemSize, (arr->elemCount - to) * elemSize);
	arr->elemCount -= (to - from);
}

#endif