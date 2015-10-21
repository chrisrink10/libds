/**
 * @file list.h
 *
 * @brief Automatically resizing list/stack implementation.
 *
 * @author Chris Rink <chrisrink10@gmail.com>
 *
 * @copyright 2015 Chris Rink. MIT Licensed.
 */

#ifndef LIBDS_LIST_H
#define LIBDS_LIST_H

#include "libds/iter.h"

/**
* @brief Auto-resizing generic list object.
*
* DSList objects are typically resized by @c DSLIST_CAPACITY_FACTOR
* whenever a resize is necessary using the API.
*/
typedef struct DSList DSList;

/**
* @brief The default capacity of a @c DSList.
*/
static const int DSLIST_DEFAULT_CAPACITY = 10;

/**
* @brief The factor by which a @c DSList is resized when needed
*/
static const int DSLIST_CAPACITY_FACTOR = 2;

/**
* @brief Comparator function used in a @c DSList to sort and search.
*/
typedef int (*dslist_compare_fn)(const void*, const void*);

/**
* @brief Free function used in a @c DSList free remaining elements when
* the list is destroyed.
*/
typedef void (*dslist_free_fn)(void*);

/**
* @brief Errors returned from @c DSList functions returning indices.
*/
static const int GLIST_NOT_FOUND = -1;
static const int GLIST_NULL_POINTER = -2;

/**
* @brief Create a new @c DSList List object with the given
* comparator and free function.
*/
DSList* dslist_new(dslist_compare_fn cmpfn, dslist_free_fn freefn);

/**
* @brief Destroy a @c DSList object.
*/
void dslist_destroy(DSList *list);

/**
* @brief Return the length of a @c DSList .
*/
size_t dslist_len(DSList *list);

/**
* @brief Return the capacity of a @c DSList .
*/
size_t dslist_cap(DSList *list);

/**
* @brief Return the object stored at the given index.
*/
void* dslist_get(DSList *list, int index);

/**
* @brief Perform the given function on each object in the list.
*/
void dslist_foreach(DSList *list, void (*func)(void*));

/**
* @brief Append an element to the end of the list.
*
* This is functionally equivalent to calling
* @c dslist_insert(list, elem, dslist_len(list));
*/
bool dslist_append(DSList *list, void *elem);

/**
* @brief Extend the first @c DSList with the elements of the second.
*
* This function sets each element pointer in @c other to @c NULL
* and sets the length to 0, to avoid any attempts to double-free
* memory, which would result in undefined behavior.
*/
bool dslist_extend(DSList *list, DSList *other);

/**
* @brief Insert the given element at the specified index.
*/
bool dslist_insert(DSList *list, void *elem, int index);

/**
* @brief Remove the first element in the list matching the given element
* and free that element.
*/
void* dslist_remove(DSList *list, void *elem);

/**
* @brief Remove the element at the given index and return it.
*/
void* dslist_remove_index(DSList *list, int index);

/**
* @brief Pop the top element from the list.
*/

void* dslist_pop(DSList *list);

/**
* @brief Clear the list, freeing all elements.
*/
void dslist_clear(DSList *list);

/**
* @brief Return the first index of the given element or -1 if it is not found.
*/
int dslist_index(DSList *list, void *elem);

/**
* @brief Sort the list in ascending order using the given comparator function.
*/
void dslist_sort(DSList *list);

/**
* @brief Reverse the list in place.
*/
void dslist_reverse(DSList *list);

/**
* @brief Create a new @c GIter on this list.
*/
GIter* dslist_iter(DSList *list);

#endif //LIBDS_LIST_H
