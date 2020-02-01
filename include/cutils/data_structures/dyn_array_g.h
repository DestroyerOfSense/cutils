#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "cutils/math/constants.h"

#define CTLS_DYN_GROWTH_FACTOR (float)CTLS_PHI
#define CTLS_DYN_DEFAULT_INITIAL_CAPACITY 8

#define CTLS_DYN_ARRAY_DECL(type, ...) \
\
struct ctls_DynArray##__VA_ARGS__ \
{ \
    type* data; \
    size_t size, capacity; \
}; \
\
struct ctls_DynArray##__VA_ARGS__* ctls_dyn_init##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr); \
void ctls_dyn_reset##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr); \
bool ctls_dyn_shrinkToFit##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr); \
bool ctls_dyn_append##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type elem); \
bool ctls_dyn_insert##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type const* src, size_t pos, \
    size_t srcLen); \
bool ctls_dyn_extend##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type const* src, size_t srcLen); \
void ctls_dyn_remove##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, size_t from, size_t to);

#define CTLS_DYN_ARRAY_DEF(type, ...) \
\
static bool ctls_dyn_reallocData##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, size_t newCapacity) \
{ \
    type* newData = realloc(dynArr->data, newCapacity * sizeof(type)); \
    if (newData) \
        dynArr->data = newData, dynArr->capacity = newCapacity; \
    return newData; \
} \
\
static bool ctls_dyn_expand##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr) \
{ \
    return ctls_dyn_reallocData##__VA_ARGS__(dynArr, nearbyint((double)CTLS_DYN_GROWTH_FACTOR * (dynArr)->capacity)); \
} \
\
struct ctls_DynArray##__VA_ARGS__* ctls_dyn_init##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr) \
{ \
    if (!dynArr) \
        dynArr = malloc(sizeof(struct ctls_DynArray##__VA_ARGS__)); \
    if (dynArr) \
    { \
        dynArr->data = malloc(CTLS_DYN_DEFAULT_INITIAL_CAPACITY * sizeof(type)); \
        if (dynArr->data) \
            dynArr->capacity = CTLS_DYN_DEFAULT_INITIAL_CAPACITY; \
        dynArr->size = 0; \
    } \
    return dynArr; \
} \
\
void ctls_dyn_reset##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr) \
{ \
    free(dynArr->data); \
    memset(dynArr, 0, sizeof(struct ctls_DynArray##__VA_ARGS__)); \
} \
\
bool ctls_dyn_shrinkToFit##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr) \
{ \
    return !dynArr->size || ctls_dyn_reallocData##__VA_ARGS__(dynArr, dynArr->size); \
} \
\
bool ctls_dyn_append##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type elem) \
{ \
    if (dynArr->size < dynArr->capacity || ctls_dyn_expand##__VA_ARGS__(dynArr)) \
    { \
        dynArr->data[dynArr->size++] = elem; \
        return true; \
    } \
    else \
        return false; \
} \
\
bool ctls_dyn_insert##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type const* src, size_t pos, \
    size_t srcLen) \
{ \
    while (dynArr->capacity < dynArr->size + srcLen) \
    { \
        if (!ctls_dyn_expand##__VA_ARGS__(dynArr)) \
            return false; \
    } \
    memmove(dynArr->data + pos + srcLen, dynArr->data + pos, (dynArr->size - pos) * sizeof(type)); \
    memmove(dynArr->data + pos, src, srcLen * sizeof(type)); \
    dynArr->size += srcLen; \
    return true; \
} \
\
bool ctls_dyn_extend##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, type const* src, size_t srcLen) \
{ \
    return ctls_dyn_insert##__VA_ARGS__(dynArr, src, dynArr->size, srcLen); \
} \
\
void ctls_dyn_remove##__VA_ARGS__(struct ctls_DynArray##__VA_ARGS__* dynArr, size_t from, size_t to) \
{ \
    memmove(dynArr->data + from, dynArr->data + to, (dynArr->size - to) * sizeof(type)); \
    dynArr->size -= (to - from); \
}

#define CTLS_DYN_ARRAY(type, ...) \
CTLS_DYN_ARRAY_DECL(type, __VA_ARGS__) \
CTLS_DYN_ARRAY_DEF(type, __VA_ARGS__)

#endif
