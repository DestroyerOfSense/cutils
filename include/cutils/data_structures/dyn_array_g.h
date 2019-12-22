#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fenv.h>

#include "cutils/math/constants.h"

#define DYN_GROWTH_FACTOR MATH_PHI
#define DYN_DEFAULT_INITIAL_CAPACITY 8

// TODO: Handle pointers to const.
#define DYN_ARRAY_G(type, ...)                                                                                    \
                                                                                                                  \
struct DynArray##__VA_ARGS__                                                                                      \
{                                                                                                                 \
    type* data;                                                                                                   \
    size_t size, capacity;                                                                                        \
};                                                                                                                \
                                                                                                                  \
static bool dyn_reallocData##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr, size_t newCapacity)                \
{                                                                                                                 \
    type* newData = realloc(dynArr->data, newCapacity * sizeof(type));                                            \
    if (newData)                                                                                                  \
        dynArr->data = newData, dynArr->capacity = newCapacity;                                                   \
    return newData;                                                                                               \
}                                                                                                                 \
                                                                                                                  \
static bool dyn_expand##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr)                                         \
{                                                                                                                 \
    fenv_t fenv;                                                                                                  \
    bool fenvSaved = !feholdexcept(&fenv);                                                                        \
    bool succeeded = dyn_reallocData##__VA_ARGS__(dynArr, DYN_GROWTH_FACTOR * dynArr->capacity + 0.5);            \
    if (fenvSaved)                                                                                                \
        feclearexcept(FE_INEXACT), feupdateenv(&fenv);                                                            \
    return succeeded;                                                                                             \
}                                                                                                                 \
                                                                                                                  \
struct DynArray##__VA_ARGS__* dyn_init##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr)                         \
{                                                                                                                 \
    if (!dynArr)                                                                                                  \
        dynArr = malloc(sizeof(struct DynArray##__VA_ARGS__));                                                    \
    if (dynArr)                                                                                                   \
    {                                                                                                             \
        dynArr->data = malloc(DYN_DEFAULT_INITIAL_CAPACITY * sizeof(type));                                       \
        dynArr->size = 0, dynArr->capacity = DYN_DEFAULT_INITIAL_CAPACITY;                                        \
    }                                                                                                             \
    return dynArr;                                                                                                \
}                                                                                                                 \
                                                                                                                  \
void dyn_reset##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr)                                                 \
{                                                                                                                 \
    free(dynArr->data);                                                                                           \
    memset(dynArr, 0, sizeof(struct DynArray##__VA_ARGS__));                                                      \
}                                                                                                                 \
                                                                                                                  \
bool dyn_shrinkToFit##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr)                                           \
{                                                                                                                 \
    return !dynArr->size || dyn_reallocData##__VA_ARGS__(dynArr, dynArr->size);                                   \
}                                                                                                                 \
                                                                                                                  \
bool dyn_append##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr, type elem)                                     \
{                                                                                                                 \
    if (dynArr->size < dynArr->capacity || dyn_expand##__VA_ARGS__(dynArr))                                       \
    {                                                                                                             \
        dynArr->data[dynArr->size++] = elem;                                                                      \
        return true;                                                                                              \
    }                                                                                                             \
    else                                                                                                          \
        return false;                                                                                             \
}                                                                                                                 \
                                                                                                                  \
bool dyn_insert##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr, type const* src, size_t pos, size_t srcLen)    \
{                                                                                                                 \
    while (dynArr->capacity < dynArr->size + srcLen)                                                              \
    {                                                                                                             \
        if (!dyn_expand##__VA_ARGS__(dynArr))                                                                     \
            return false;                                                                                         \
    }                                                                                                             \
    memmove(dynArr->data + pos + srcLen, dynArr->data + pos, (dynArr->size - pos) * sizeof(type));                \
    memmove(dynArr->data + pos, src, srcLen * sizeof(type));                                                      \
    dynArr->size += srcLen;                                                                                       \
    return true;                                                                                                  \
}                                                                                                                 \
                                                                                                                  \
bool dyn_extend##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr, type const* src, size_t srcLen)                \
{                                                                                                                 \
    return dyn_insert##__VA_ARGS__(dynArr, src, dynArr->size, srcLen);                                            \
}                                                                                                                 \
                                                                                                                  \
void dyn_remove##__VA_ARGS__(struct DynArray##__VA_ARGS__* dynArr, size_t from, size_t to)                        \
{                                                                                                                 \
    memmove(dynArr->data + from, dynArr->data + to, (dynArr->size - to) * sizeof(type));                          \
    dynArr->size -= (to - from);                                                                                  \
}

#endif
