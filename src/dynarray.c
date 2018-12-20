#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "math_constants.h"
#include "dynarray.h"

#define GROWTH_FACTOR PHI

typedef struct DynArray DynArray;

extern inline DynArray* dyn_init(DynArray* arr, size_t elemSize);
extern inline void dyn_reset(DynArray* arr);
extern inline void* dyn_incrElemCount(DynArray* arr, size_t elemSize);
extern inline void dyn_append(DynArray* dst, const void* src, size_t srcLen, size_t elemSize);
extern inline void dyn_remove(DynArray* arr, size_t from, size_t to, size_t elemSize);

void* dyn_setElemCount(DynArray* arr, size_t to, size_t elemSize)
{
	if (to > arr->len)
	{
		size_t newLen = fmax(to, round(GROWTH_FACTOR * arr->len));
		void* newData = realloc(arr->data, newLen * elemSize);
		if (newData)
			arr->data = newData, arr->len = newLen;
		else
		{
			free(arr->data);
			arr->data = NULL;
		}
	}
	arr->elemCount = to;
	return arr->data;
}

void* dyn_shrinkToFit(DynArray* arr, size_t elemSize)
{
	if (arr->len > arr->elemCount)
	{
		void* newData = realloc(arr->data, arr->elemCount * elemSize);
		if (newData)
			arr->data = newData, arr->len = arr->elemCount;
		else
		{
			free(arr->data);
			arr->data = NULL;
		}
	}
	return arr->data;
}

void dyn_insert(DynArray* dst, const void* src, size_t pos, size_t srcLen, size_t elemSize)
{
	size_t oldElemCount = dst->elemCount;
	if (dyn_setElemCount(dst, dst->elemCount + srcLen, elemSize))
	{
		char* start = (char*)dst->data + elemSize*pos;
		memmove(start + elemSize*srcLen, start, (oldElemCount - pos) * elemSize);
		memmove(start, src, srcLen * elemSize);
	}
}