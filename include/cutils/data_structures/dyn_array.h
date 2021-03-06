#ifndef CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019
#define CUTILS_DATA_STRUCTURES_DYN_ARRAY_H_12142019

/** @file
 * @brief Contains a dynamic array implementation.
 *
 * Any given dynamic array `dynArr` should be treated as if there was a constant associated with it of type `size_t` and
 * value *S*, the size of one of its elements. Many functions declared in this file have a final parameter called
 * `elemSize`, of type `size_t`. For every call to one of these functions that acts on `dynArr`, `elemSize` should be
 * set to *S*. In short, the size of an element of a given dynamic array should be treated as if it was constant.
 *
 * The following invariants should be adhered to:
 * - `ctls_DynArray::data` must be either `NULL` or dynamically allocated.
 * - If `ctls_DynArray::data != NULL`:
 *     - `ctls_DynArray::capacity` must not be zero.
 *     - `ctls_DynArray::size` must not be greater than `ctls_DynArray::capacity`.
 *     - The block of memory that `ctls_DynArray::data` points to must be exactly `ctls_DynArray::capacity * S` bytes
 *         wide.
 *     - `ctls_DynArray::data` must point to the first element of `dynArr`, and `dynArr`'s elements must be contiguous.
 *
 * As long as these invariants are adhered to, the user should feel free to modify a dynamic array's fields directly if
 * he has good reason to. For example, this file does not contain a `pop` function, but the same effect can be achieved
 * by simply decrementing `ctls_DynArray::size`.
 *
 * A dynamic array's elements can be accessed via `ctls_DynArray::data`.
 *
 * Though a given dynamic array's elements are often all of the same type, there is no reason why they cannot be of
 * different types, as long as each type has the same size.
 *
 * `FE_INEXACT` may be raised during a call to `ctls_dyn_append()`, `ctls_dyn_insert()`, or `ctls_dyn_extend()`.
 */

#include <stddef.h>
#include <stdbool.h>

/** @brief A dynamic array. */
struct ctls_DynArray
{
    /** @brief the block of memory that contains this dynamic array's elements */
    void* data;
    /** @brief number of elements contained in this dynamic array */
    size_t size;
    /** @brief maximum number of elements that can be contained in this dynamic array until it must be expanded */
    size_t capacity;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization and Cleanup
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes a dynamic array with a given initial capacity.
 * @param dynArr pointer to an uninitialized dynamic array, or `NULL`
 * @param initialCapacity `dynArr`'s chosen initial capacity, must be nonzero
 * @param elemSize size of one of `dynArr`'s elements
 * @return On success, returns a dynamically allocated dynamic array if `dynArr` was originally `NULL`, `dynArr`
 *     otherwise. On failure, returns `NULL`.
 *
 * First, if `dynArr` is null, it is assigned a dynamically allocated dynamic array. Then, `dynArr->data` is assigned a
 * dynamically allocated memory block just large enough for `initialCapacity` elements. If all allocations were
 * successful, `dynArr->capacity` is set to `initialCapacity`, `dynArr->size` is set to zero, and `dynArr` is returned.
 * Otherwise, `dynArr` is freed if it was dynamically allocated within the function, and `NULL` is returned.
 *
 * This is only a convenience function. A dynamic array can be initialized manually if desired.
 */
struct ctls_DynArray* ctls_dyn_init(struct ctls_DynArray* dynArr, size_t initialCapacity, size_t elemSize);

/**
 * @brief Initializes a dynamic array with the default initial capacity.
 * @param dynArr pointer to an uninitialized dynamic array, or `NULL`
 * @param elemSize size of one of `dynArr`'s elements
 * @return On success, returns a dynamically allocated dynamic array if `dynArr` was originally `NULL`, `dynArr`
 *     otherwise. On failure, returns `NULL`.
 *
 * If `DEFAULT_INITIAL_CAPACITY` is the default initial capacity, this function is equivalent to
 * `ctls_dyn_init(dynArr, DEFAULT_INITIAL_CAPACITY, elemSize)`.
 *
 * This is only a convenience function. A dynamic array can be initialized manually if desired.
 */
struct ctls_DynArray* ctls_dyn_defaultInit(struct ctls_DynArray* dynArr, size_t elemSize);

/**
 * @brief Frees `dynArr->data` and zeroes `dynArr`'s members out.
 * @param dynArr pointer to an initialized dynamic array
 * @param elemSize size of one of `dynArr`'s elements
 *
 * This is only a convenience function. Cleanup and resetting of a dynamic array can be performed manually, if desired.
 */
void ctls_dyn_reset(struct ctls_DynArray* dynArr, size_t elemSize);

/**
 * @brief Frees excess memory held by a dynamic array.
 * @param dynArr pointer to a dynamic array
 * @param elemSize the size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * If the operation succeeds, `dynArr->capacity` is equal to `dynArr->size`.
 */
bool ctls_dyn_shrinkToFit(struct ctls_DynArray* dynArr, size_t elemSize);

/**
 * @brief Copies the contents of one dynamic array into another.
 * @param dest pointer to the destination dynamic array; must be either initialized, zeroed out, or `NULL`
 * @param src pointer to the source dynamic array
 * @param elemSize the size of one of `src`'s elements
 * @return On success, returns a pointer to a dynamic array that contains copies of `src`'s elements. This pointer
 *     equals `dest` if `dest` was not `NULL`. Otherwise, it points to a dynamically allocated dynamic array. On
 *     failure, returns `NULL`.
 *
 * If the operation succeeds, the dynamic array accessible via the returned pointer has the same size, capacity, and
 * element size as `src`.
 *
 * This is only a convenience function. Making a copy of a dynamic array can be performed manually, if desired. This
 * should be done if, for instance, `src`'s elements are aggregates and must be deep copied.
 */
struct ctls_DynArray* ctls_dyn_copy(struct ctls_DynArray* restrict dest, const struct ctls_DynArray* restrict src,
    size_t elemSize);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Adds an element to the end of a dynamic array.
 * @param dynArr pointer to an initialized dynamic array
 * @param elem pointer to the element that is to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * Intended as a substitute for adding an r-value of `dynArr`'s "type" to the end, as this is impossible with void
 * pointers. Note that `dynArr` and `elem` are `restrict`, and therefore the object `elem` points to cannot already be
 * contained in `dynArr`.
 */
bool ctls_dyn_append(struct ctls_DynArray* restrict dynArr, const void* restrict elem, size_t elemSize);

/**
 * @brief Inserts the elements of `src` before `dynArr->data + pos`.
 * @param dynArr pointer to an initialized dynamic array
 * @param src pointer to the elements that are to be added
 * @param pos index before which elements are to be added. Must not be negative or greater than `dynArr->size`.
 * @param srcLen number of elements that are to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if operation succeeds, `false` if not
 *
 * If the operation succeeds, the resulting dynamic array is of the form
 *     \f[[a_0, a_1, ..., a_{pos - 1}, src_0, src_1, ..., src_{srcLen - 1}, a_{pos}, a_{pos + 1}, ..., a_{size - 1}]\f]
 * , where \f$a\f$ is the previous dynamic array and \f$size\f$ is the size of the previous dynamic array.
 */
bool ctls_dyn_insert(struct ctls_DynArray* dynArr, const void* src, size_t pos, size_t srcLen, size_t elemSize);

/**
 * @brief Inserts the elements of `src` at the end of a dynamic array.
 * @param dynArr pointer to an initialized dynamic array
 * @param src pointer to the elements that are to be added
 * @param srcLen number of elements that are to be added
 * @param elemSize size of one of `dynArr`'s elements
 * @return `true` if the operation succeeds, `false` if not
 *
 * Equivalent to `ctls_dyn_insert(dynArr, src, dynArr->size, srcLen, elemSize)`.
 */
bool ctls_dyn_extend(struct ctls_DynArray* dynArr, const void* src, size_t srcLen, size_t elemSize);

/**
 * @brief Removes the elements from a dynamic array at indices \f$i\f$ such that \f$from <= i < to\f$.
 * @param dynArr pointer to an initialized dynamic array
 * @param from first index whose corresponding element is removed. Must be between 0 and `dynArr->size - 1`, inclusive.
 * @param to index after the last whose corresponding element is removed. Must be between `0` and `dynArr->size`,
 *     inclusive, and must not be less than `from`.
 * @param elemSize size of one of `dynArr`'s elements
 *
 * The resulting dynamic array is of the form
 *     \f[[a_0, a_1, ..., a_{from - 1}, a_{to}, a_{to + 1},..., a_{size - 1}]\f]
 * , where \f$a\f$ is the previous dynamic array and \f$size\f$ is the size of the previous dynamic array.
 */
void ctls_dyn_remove(struct ctls_DynArray* dynArr, size_t from, size_t to, size_t elemSize);

#endif
