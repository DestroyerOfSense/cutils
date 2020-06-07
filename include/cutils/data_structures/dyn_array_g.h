#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019

/** @file
 * @brief Contains a generic version of `ctls_DynArray`.
 *
 * This file contains macros that facilitate the creation of specializations of `ctls_DynArray` for different types
 * in a manner similar to templates in C++. In addition to often being more convenient than the use of void pointers,
 * this allows compilers to optimize more effectively.
 *
 * Each macro takes a `type` and `suffix` argument. `type` is simply the type to be specialized for, and `suffix` is a
 * suffix to be appended to every identifier associated with the specialization. Every such identifier contains a
 * trailing underscore. If multiple specializations are created in a single program, each should have a different
 * suffix, to avoid name conflicts. Here's an example:
 *
 * @code
 * // The corresponding definitions are located in another file.
 * CTLS_DYN_ARRAY_DECL(int, int)
 *
 * void f(void)
 * {
 *     ctls_DynArray_int arr;
 *     ctls_dyn_defaultInit_int(&arr);
 *     // ...
 * }
 * @endcode
 *
 * The functions associated with a given specialization of `ctls_DynArray` are nearly identical to those declared in
 * cutils/data_structures/dyn_array.h. Here are their differences:
 *
 * - In a generic function parameter, all occurrences of the identifier `ctls_DynArray` are replaced with the
 *     specialization.
 * - A generic function has no `elemSize` parameter.
 * - For a specialization for type *T*, `ctls_dyn_append`'s parameter `elem` is of type *T*, rather than
 *     `const void* restrict`.
 *
 * It should be noted that the functions and objects associated with a given specialization of `ctls_DynArray` have
 * external linkage if the corresponding functions and objects are declared in cutils/data_structures/dyn_array.h.
 */

/// @cond
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
/// @endcond

#include "cutils/math/constants.h"

#define CTLS_DYN_GROWTH_FACTOR (float)CTLS_PHI
#define CTLS_DYN_DEFAULT_INITIAL_CAPACITY 8

/**
 * @brief Creates declarations for a specialization of `ctls_DynArray`.
 * @param type the name of the type to be specialized for
 * @param suffix a string appended to each declared identifier
 *
 * Using this macro is equivalent to including a header file for a given specialization. The corresponding
 * implementation is created via `CTLS_DYN_ARRAY_DEF`.
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY_DECL(int, int)
 *
 * void f(void)
 * {
 *     ctls_DynArray_int arr;
 *     ctls_dyn_defaultInit_int(&arr);
 *     // ...
 * }
 * @endcode
 */
#define CTLS_DYN_ARRAY_DECL(type, suffix) \
\
struct ctls_DynArray_##suffix \
{ \
    type* data; \
    size_t size, capacity; \
}; \
\
struct ctls_DynArray_##suffix* ctls_dyn_init_##suffix(struct ctls_DynArray_##suffix* dynArr, \
    size_t initialCapacity); \
struct ctls_DynArray_##suffix* ctls_dyn_defaultInit_##suffix(struct ctls_DynArray_##suffix* dynArr); \
void ctls_dyn_reset_##suffix(struct ctls_DynArray_##suffix* dynArr); \
bool ctls_dyn_shrinkToFit_##suffix(struct ctls_DynArray_##suffix* dynArr); \
struct ctls_DynArray_##suffix* ctls_dyn_copy_##suffix(struct ctls_DynArray_##suffix* restrict dest, \
    const struct ctls_DynArray_##suffix* restrict src); \
bool ctls_dyn_append_##suffix(struct ctls_DynArray_##suffix* dynArr, type elem); \
bool ctls_dyn_insert_##suffix(struct ctls_DynArray_##suffix* dynArr, type const* src, size_t pos, \
    size_t srcLen); \
bool ctls_dyn_extend_##suffix(struct ctls_DynArray_##suffix* dynArr, type const* src, size_t srcLen); \
void ctls_dyn_remove_##suffix(struct ctls_DynArray_##suffix* dynArr, size_t from, size_t to);

/**
 * @brief Creates definitions for a specialization of `ctls_DynArray`.
 * @param type the name of the type to be specialized for
 * @param suffix a string appended to each declared identifier
 *
 * The corresponding declarations can, and should, be included via `CTLS_DYN_ARRAY_DECL`.
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY_DECL(int, int)
 * CTLS_DYN_ARRAY_DEF(int, int)
 * @endcode
 */
#define CTLS_DYN_ARRAY_DEF(type, suffix) \
\
static bool ctls_dyn_reallocData_##suffix(struct ctls_DynArray_##suffix* dynArr, size_t newCapacity) \
{ \
    type* newData = realloc(dynArr->data, newCapacity * sizeof(type)); \
    if (newData) \
        dynArr->data = newData, dynArr->capacity = newCapacity; \
    return newData; \
} \
\
static bool ctls_dyn_expand_##suffix(struct ctls_DynArray_##suffix* dynArr) \
{ \
    return ctls_dyn_reallocData_##suffix(dynArr, nearbyint((double)CTLS_DYN_GROWTH_FACTOR * (dynArr)->capacity)); \
} \
\
struct ctls_DynArray_##suffix* ctls_dyn_init_##suffix(struct ctls_DynArray_##suffix* dynArr, \
    size_t initialCapacity) \
{ \
    bool dynArrOriginallyNull = !dynArr; \
    if (dynArrOriginallyNull) \
        dynArr = malloc(sizeof *dynArr); \
    if (dynArr) \
    { \
        void* newData = malloc(initialCapacity * sizeof(type)); \
        if (newData) \
            *dynArr = (struct ctls_DynArray_##suffix){newData, 0, initialCapacity}; \
        else \
        { \
            if (dynArrOriginallyNull) \
                free(dynArr); \
            dynArr = NULL; \
        } \
    } \
    return dynArr; \
} \
\
struct ctls_DynArray_##suffix* ctls_dyn_defaultInit_##suffix(struct ctls_DynArray_##suffix* dynArr) \
{ \
    return ctls_dyn_init_##suffix(dynArr, CTLS_DYN_DEFAULT_INITIAL_CAPACITY); \
} \
\
void ctls_dyn_reset_##suffix(struct ctls_DynArray_##suffix* dynArr) \
{ \
    free(dynArr->data); \
    memset(dynArr, 0, sizeof(struct ctls_DynArray_##suffix)); \
} \
\
bool ctls_dyn_shrinkToFit_##suffix(struct ctls_DynArray_##suffix* dynArr) \
{ \
    return !dynArr->size || ctls_dyn_reallocData_##suffix(dynArr, dynArr->size); \
} \
\
struct ctls_DynArray_##suffix* ctls_dyn_copy_##suffix(struct ctls_DynArray_##suffix* restrict dest, \
    const struct ctls_DynArray_##suffix* restrict src) \
{ \
    bool destOriginallyNull = !dest; \
    if (!dest || !dest->data) \
    { \
        dest = ctls_dyn_init_##suffix(dest, src->capacity); \
        if (!dest) \
            return NULL; \
    } \
    else if (!ctls_dyn_reallocData_##suffix(dest, src->capacity)) \
        return NULL; \
    memcpy(dest->data, src->data, src->size * sizeof(type)); \
    dest->capacity = src->capacity, dest->size = src->size; \
    return dest; \
} \
\
bool ctls_dyn_append_##suffix(struct ctls_DynArray_##suffix* dynArr, type elem) \
{ \
    if (dynArr->size < dynArr->capacity || ctls_dyn_expand_##suffix(dynArr)) \
    { \
        dynArr->data[dynArr->size++] = elem; \
        return true; \
    } \
    else \
        return false; \
} \
\
bool ctls_dyn_insert_##suffix(struct ctls_DynArray_##suffix* dynArr, type const* src, size_t pos, \
    size_t srcLen) \
{ \
    while (dynArr->capacity < dynArr->size + srcLen) \
    { \
        if (!ctls_dyn_expand_##suffix(dynArr)) \
            return false; \
    } \
    memmove(dynArr->data + pos + srcLen, dynArr->data + pos, (dynArr->size - pos) * sizeof(type)); \
    memmove(dynArr->data + pos, src, srcLen * sizeof(type)); \
    dynArr->size += srcLen; \
    return true; \
} \
\
bool ctls_dyn_extend_##suffix(struct ctls_DynArray_##suffix* dynArr, type const* src, size_t srcLen) \
{ \
    return ctls_dyn_insert_##suffix(dynArr, src, dynArr->size, srcLen); \
} \
\
void ctls_dyn_remove_##suffix(struct ctls_DynArray_##suffix* dynArr, size_t from, size_t to) \
{ \
    memmove(dynArr->data + from, dynArr->data + to, (dynArr->size - to) * sizeof(type)); \
    dynArr->size -= (to - from); \
}

/**
 * @brief a convenience function that calls both `CTLS_DYN_ARRAY_DECL` and `CTLS_DYN_ARRAY_DEF`.
 * @param type the name of the type to be specialized for
 * @param suffix a string appended to each declared identifier
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY(int, int)
 * @endcode
 */
#define CTLS_DYN_ARRAY(type, suffix) \
CTLS_DYN_ARRAY_DECL(type, suffix) \
CTLS_DYN_ARRAY_DEF(type, suffix)

#endif
