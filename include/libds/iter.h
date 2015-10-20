/*****************************************************************************
 * libds :: iter.h
 *
 * Generic iterator for sequence types.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_ITER_H
#define LIBDS_ITER_H

/**
* @brief Generic iterator object.
*/
typedef struct GIter GIter;

/**
 * @brief Constants used as sentinel values for @c GIter objects.
 */
static const int GITER_NEW_ITERATOR = -1;
static const int GITER_NO_MORE_ELEMENTS = -2;

/**
 * @brief Advance the next element in the collection. Returns true if there
 * is another element. False otherwise.
 */
bool giter_next(GIter *iter);

/**
 * @brief Indicates whether there is a next element in the collection without
 * advancing the pointer to that element.
 */
bool giter_has_next(GIter *iter);

/**
 * @brief Return the key associated with the current element.
 *
 * For @c GList objects, the return value is always NULL.
 */
void* giter_key(GIter *iter);

/**
 * @brief Return the value at the current index.
 */
void* giter_value(GIter *iter);

/**
 * @brief Return the current enumerated index for the current element.
 *
 * This is functionally equivalent to keeping your own counter as you iterate.
 */
int giter_index(GIter *iter);

/**
 * @brief Indicates what numerical index of iteration the caller is on.
 * For @c GDicts , this does not correspond to an placement in the table.
 * For @c GLists , this will be the same as the index.
 */
int giter_index(GIter *iter);

/**
 * @brief Destroy an iterator.
 */
void giter_destroy(GIter *iter);

#endif //LIBDS_ITER_H
