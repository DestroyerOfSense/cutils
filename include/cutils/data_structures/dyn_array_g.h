#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_G_H_12212019

/** @file
 * @brief Contains a generic version of `ctls_DynArray`.
 *
 * This file contains macros that facilitate the creation of specializations of `ctls_DynArray` for different types
 * in a manner similar to templates in C++. In addition to often being more convenient than the use of void pointers,
 * this allows compilers to optimize more effectively.
 *
 * Each macro takes a `type` and optional `suffix` argument. `type` is simply the type to be specialized for, and
 * `suffix` is an optional suffix to be appended to every name associated with the specialization. If multiple
 * specializations are created in a single program, or if the non-generic `ctls_DynArray` is used, each specialization
 * should have a different suffix, to avoid name conflicts. Here's an example:
 *
 * @code
 * // The corresponding definitions are located in another file.
 * CTLS_DYN_ARRAY_DECL(int, _int)
 *
 * void f(void)
 * {
 *     ctls_DynArray_int arr;
 *     ctls_dyn_init_int(&arr);
 *     // ...
 * }
 * @endcode
 *
 * The functions associated with a given specialization of `ctls_DynArray` are nearly identical to those declared in
 * cutils/data_structures/dyn_array.h. Here are their differences:
 *
 * - The generic function's first parameter is a pointer to the specialization, rather than the non-generic
 *   `ctls_DynArray`.
 * - The generic function has no `elemSize` parameter.
 * - For a specialization for type *T*, `ctls_dyn_append`'s parameter `elem` is of type *T*, rather than
 *   `const void* restrict`.
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
 * @param suffix a string appended to each declared identifier. Optional.
 *
 * Using this macro is equivalent to including a header file for a given specialization. The corresponding
 * implementation is created via `CTLS_DYN_ARRAY_DEF`.
 *
 * If `suffix` is not provided, the corresponding identifiers are the same as those declared in
 * cutils/data_structures/dyn_array.h.
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY_DECL(int, _int)
 *
 * void f(void)
 * {
 *     ctls_DynArray_int arr;
 *     ctls_dyn_init_int(&arr);
 *     // ...
 * }
 * @endcode
 */
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

/**
 * @brief Creates definitions for a specialization of `ctls_DynArray`.
 * @param type the name of the type to be specialized for
 * @param suffix a string appended to each declared identifier. Optional.
 *
 * The corresponding declarations can, and should, be included via `CTLS_DYN_ARRAY_DECL`.
 *
 * If `suffix` is not provided, the corresponding identifiers are the same as those declared in
 * cutils/data_structures/dyn_array.h. In this case, if the implementation file for cutils/data_structures/dyn_array.h
 * is present in the program, a name conflict will result.
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY_DECL(int, _int)
 * CTLS_DYN_ARRAY_DEF(int, _int)
 * @endcode
 */
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

/**
 * @brief a convenience function that calls both `CTLS_DYN_ARRAY_DECL` and `CTLS_DYN_ARRAY_DEF`.
 * @param type the name of the type to be specialized for
 * @param suffix a string appended to each declared identifier. Optional.
 *
 * **Usage**
 * @code
 * CTLS_DYN_ARRAY(int, _int)
 * @endcode
 */
#define CTLS_DYN_ARRAY(type, ...) \
CTLS_DYN_ARRAY_DECL(type, __VA_ARGS__) \
CTLS_DYN_ARRAY_DEF(type, __VA_ARGS__)

#endif
