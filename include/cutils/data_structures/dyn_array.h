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
 * - `DynArray::data` must be either `NULL` or dynamically allocated.
 * - `DynArray::size` must never be greater than `DynArray::capacity`.
 * - The block of memory that `DynArray::data` points should be exactly `DynArray::capacity * S` bytes wide.
 *
 * As long as these invariants are adhered to, the user should feel free to modify a `DynArray`'s fields directly if he
 * has good reason to. For example, this file does not contain a `pop` function, but the same effect can be achieved by
 * simply decrementing `DynArray::size`.
 *
 * Though a given `DynArray`'s elements are often all of the same type, there is no reason why they cannot be of
 * different types, as long as each type has the same size.
 *
 * `FE_INEXACT` may be raised during a call to `dyn_append()`, `dyn_insert()`, or `dyn_extend()` when it would make
 * `DynArray::capacity` extremely large, or on systems where the difference in precision between `float` and `double` is
 * abnormally small.
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization and Cleanup
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes a `DynArray` with suitable defaults.
 * @param dynArr pointer to an uninitialized `DynArray`, or `NULL`
 * @param elemSize size of one of `dynArr`'s elements
 * @return If `dynArr` is not null, returns `dynArr`. Otherwise, a dynamically allocated `DynArray` is returned, or
 *     `NULL` if this cannot be done.
 *
 * First, if `dynArr` is null, it is assigned a dynamically allocated `DynArray`. If this fails, `NULL` is returned.
 * Then, `dynArr->data` is assigned a dynamically allocated memory block of a set default size, or `NULL` if allocation
 * fails. If allocation of the memory block is successful, `dynArr->capacity` is set to the aforementioned default size.
 * Lastly, `dynArr->size` is set to zero, and `dynArr` is returned.
 *
 * This is only a convenience function. A `DynArray` can be initialized manually if desired.
 */
struct DynArray* dyn_init(struct DynArray* dynArr, size_t elemSize);

/**
 * @brief Frees `dynArr->data` and zeroes `dynArr`'s members out.
 * @param dynArr pointer to an initialized `DynArray`
 * @param elemSize size of one of `dynArr`'s elements
 *
 * This is only a convenience function. Cleanup and resetting of a `DynArray` can be performed manually, if desired.
 */
void dyn_reset(struct DynArray* dynArr, size_t elemSize);

/**
 * @brief Frees excess memory held by a `DynArray`.
 * @param dynArr pointer to a `DynArray`
 * @param elemSize the size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * If the operation succeeds, `dynArr->capacity` is equal to `dynArr->size`.
 */
bool dyn_shrinkToFit(struct DynArray* dynArr, size_t elemSize);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Adds an element to the end of a `DynArray`.
 * @param dynArr pointer to an initialized `DynArray`
 * @param elem pointer to the element that is to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * Intended as a substitute for adding an r-value of `dynArr`'s "type" to the end, as this is impossible with void
 * pointers. Note that `dynArr` and `elem` are `restrict`, and therefore `elem` cannot already be contained in `dynArr`.
 */
bool dyn_append(struct DynArray* restrict dynArr, const void* restrict elem, size_t elemSize);

/**
 * @brief Inserts the elements of `src` before `dynArr->data + pos`.
 * @param dynArr pointer to an initialized `DynArray`
 * @param src pointer to the elements that are to be added
 * @param pos index before which elements are to be added. Must not be negative or greater than `dynArr->size`.
 * @param srcLen number of elements that are to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * If the operation succeeds, the resulting array is of the form
 *     \f[ [a_0, a_1, ..., a_{pos - 1}, src_0, src_1, ..., src_{srcLen}, a_{pos}, a_{pos + 1}, ..., a_{size}] \f]
 * , where \f$a\f$ is the previous array and \f$size\f$ is the size of the previous array.
 */
bool dyn_insert(struct DynArray* dynArr, const void* src, size_t pos, size_t srcLen, size_t elemSize);

/**
 * @brief Inserts the elements of `src` at the end of a `DynArray`.
 * @param dynArr pointer to an initialized `DynArray`
 * @param src pointer to the elements that are to be added
 * @param srcLen number of elements that are to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if the operation succeeds, `false` if not
 *
 * Equivalent to `dyn_insert(dynArr, src, dynArr->size, srcLen, elemSize)`.
 */
bool dyn_extend(struct DynArray* dynArr, const void* src, size_t srcLen, size_t elemSize);

/**
 * @brief Removes the elements from a `DynArray` at indices \f$i\f$ such that \f$from <= i < to\f$.
 * @param dynArr pointer to an initialized `DynArray`
 * @param from first index whose corresponding element is removed. Must be between 0 and `dynArr->size - 1`, inclusive.
 * @param to index after the last whose corresponding element is removed. Must be between `0` and `dynArr->size`,
 *     inclusive, and must not be less than `from`.
 *
 * The resulting array is of the form
 *     \f[ [a_0, a_1, ..., a_{from - 1}, a_{to}, a_{to + 1}, a_{size}] \f]
 * , where \f$a\f$ is the previous array and \f$size\f$ is the size of the previous array.
 */
void dyn_remove(struct DynArray* dynArr, size_t from, size_t to, size_t elemSize);

#endif
