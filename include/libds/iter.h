/**
 * @file buffer.h
 *
 * @brief Generic iterator for sequence types.
 *
 * @author Chris Rink <chrisrink10@gmail.com>
 *
 * @copyright 2015 Chris Rink. MIT Licensed.
 */

#ifndef LIBDS_ITER_H
#define LIBDS_ITER_H

/**
* @brief Generic iterator object.
*/
typedef struct DSIter DSIter;

/**
 * @brief Constants used as sentinel values for @c DSIter objects.
 */
static const int GITER_NEW_ITERATOR = -1;
static const int GITER_NO_MORE_ELEMENTS = -2;

/**
 * @brief Advance the next element in the collection. Returns true if there
 * is another element. False otherwise.
 */
bool dsiter_next(DSIter *iter);

/**
 * @brief Indicates whether there is a next element in the collection without
 * advancing the pointer to that element.
 */
bool dsiter_has_next(DSIter *iter);

/**
 * @brief Return the key associated with the current element.
 *
 * For @c GList objects, the return value is always NULL.
 */
void* dsiter_key(DSIter *iter);

/**
 * @brief Return the value at the current index.
 */
void* dsiter_value(DSIter *iter);

/**
 * @brief Return the current enumerated index for the current element.
 *
 * This is functionally equivalent to keeping your own counter as you iterate.
 */
int dsiter_index(DSIter *iter);

/**
 * @brief Indicates what numerical index of iteration the caller is on.
 * For @c GDicts , this does not correspond to an placement in the table.
 * For @c GLists , this will be the same as the index.
 */
int dsiter_index(DSIter *iter);

/**
 * @brief Destroy an iterator.
 */
void dsiter_destroy(DSIter *iter);

#endif //LIBDS_ITER_H
