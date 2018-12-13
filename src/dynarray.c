#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "math_constants.h"

#define GROWTH_FACTOR PHI

struct DynArray
{
	void* data;
	size_t elemCount;
	size_t len;
};
typedef struct DynArray DynArray;

DynArray* dyn_init(DynArray* arr, size_t len, size_t elemSize)
{
	arr->data = malloc(len * elemSize);
	arr->elemCount = 0;
	arr->len = len;
	return arr;
}

void dyn_reset(DynArray* arr)
{
	free(arr->data);
	arr->elemCount = 0;
	arr->len = 0;
}

DynArray* dyn_setElemCount(DynArray* arr, size_t elemCount, size_t elemSize)
{
	if (elemCount > arr->len)
	{
		size_t len = fmax(elemCount, round(GROWTH_FACTOR * arr->len));
		void* data = realloc(arr->data, len * elemSize);
		if (data)
			arr->data = data, arr->len = len;
		else
		{
			free(arr->data);
			arr->data = NULL;
		}
	}
	arr->elemCount = elemCount;
	return arr;
}

DynArray* dyn_incrElemCount(DynArray* arr, size_t elemSize)
{
	return dyn_setElemCount(arr, arr->elemCount + 1, elemSize);
}

void dyn_shrinkToFit(DynArray* arr, size_t elemSize)
{
	if (arr->len > arr->elemCount)
	{
		void* data = realloc(arr->data, arr->elemCount * elemSize);
		if (data)
			arr->data = data, arr->len = arr->elemCount;
		else
		{
			free(arr->data);
			arr->data = NULL;
		}
	}
}

void dyn_insert(DynArray* dst, const void* src, size_t pos, size_t srcLen, size_t elemSize)
{
	if (!dyn_setElemCount(dst, dst->elemCount + srcLen, elemSize)->data)
		return;
	void* start = dst->data + pos;
	memmove(start + srcLen, start, (dst->elemCount - pos) * elemSize);
	memmove(start, src, srcLen * elemSize);
}

void dyn_append(DynArray* dst, const void* restrict src, size_t srcLen, size_t elemSize)
{
	void* end = dst->data + dst->elemCount;
	if (dyn_setElemCount(dst, dst->elemCount + srcLen, elemSize)->data)
		memcpy(end, src, srcLen * elemSize);
}