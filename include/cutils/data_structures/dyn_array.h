#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019

/** @file
 * @brief Contains a dynamic array implementation.
 *
 * Any given `DynArray` `dynArr` should be treated as if there was a constant associated with it of type `size_t` and
 * value *S*, the size of one of its elements. Many functions declared in this file have a final parameter called
 * `elemSize`, of type `size_t`. For every call to one of these functions that acts on `dynArr`, `elemSize` should be
 * set to *S*. In short, the size of an element of a given `DynArray` should be treated as if it was constant.
 *
 * The following invariants should be adhered to:
 * - [data] must be either `NULL` or dynamically allocated.
 * - [size] must never be greater than [capacity].
 * - The block of memory that [data] points should be exactly `DynArray::capacity * S` bytes wide.
 *
 * As long as these invariants are adhered to, the user should feel free to modify a `DynArray`'s fields directly if he
 * has good reason to. For example, this file does not contain a `pop` function, but the same effect can be achieved by
 * simply decrementing [size].
 *
 * `FE_INEXACT` may be raised during a call to `dyn_append`, `dyn_insert`, or `dyn_extend` when the resulting [capacity]
 * would be extremely large, or on systems where the difference in precision between `float` and `double` is abnormally
 * small.
 *
 * [data]: @ref DynArray::data
 * [size]: @ref DynArray::size
 * [capacity]: @ref DynArray::capacity
 */

#include <stddef.h>
#include <stdbool.h>

/** @brief A dynamic array. */
struct DynArray
{
    void* data;         ///< the block of memory containing this `DynArray`'s elements
    size_t size;        ///< number of elements contained in this `DynArray`
    size_t capacity;    ///< maximum number of elements that can be contained in this `DynArray` before it must expand
};

struct DynArray* dyn_init(struct DynArray* dynArr, size_t elemSize);
void dyn_reset(struct DynArray* dynArr, size_t elemSize);
bool dyn_shrinkToFit(struct DynArray* dynArr, size_t elemSize);

bool dyn_append(struct DynArray* restrict dynArr, const void* restrict elem, size_t elemSize);
bool dyn_insert(struct DynArray* dynArr, const void* src, size_t pos, size_t srcLen, size_t elemSize);
bool dyn_extend(struct DynArray* dynArr, const void* src, size_t srcLen, size_t elemSize);
void dyn_remove(struct DynArray* dynArr, size_t from, size_t to, size_t elemSize);

#endif
