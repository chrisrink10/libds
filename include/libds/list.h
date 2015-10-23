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
static const int DSLIST_NOT_FOUND = -1;
static const int DSLIST_NULL_POINTER = -2;
static const int DSLIST_NO_CMP_FUNC = -3;

/**
* @brief Create a new @c DSList List object with @c DSLIST_DEFAULT_CAPACITY
* slots and the given comparator and free function.
*
* Neither function pointer is required to create a new list. If the caller
* does not specify a @c dslist_compare_fn, then @c dslist_sort will become
* a no-op. Likewise, if no @c dslist_free_fn is specified, then the list
* will not free list elements when it is destroyed.
*
* @param cmpfn a function which can compare two list elements
* @param freefn a function which can free a list element
* @returns a new @c DSList object or @c NULL if memory could not be
*          allocated
*/
DSList* dslist_new(dslist_compare_fn cmpfn, dslist_free_fn freefn);

/**
* @brief Create a new @c DSList List object with @c cap slots and the given
* comparator and free function.
*
* Neither function pointer is required to create a new list. If the caller
* does not specify a @c dslist_compare_fn, then @c dslist_sort will become
* a no-op. Likewise, if no @c dslist_free_fn is specified, then the list
* will not free list elements when it is destroyed.
*
* @param cap the starting capacity of the @c DSList
* @param cmpfn a function which can compare two list elements
* @param freefn a function which can free a list element
* @returns a new @c DSList object or @c NULL if memory could not be
*          allocated
*/
DSList* dslist_new_cap(size_t cap, dslist_compare_fn cmpfn, dslist_free_fn freefn);

/**
* @brief Destroy a @c DSList object.
*
* If a @c dslist_free_fn was specified when the list was created, it will
* be called on each element in the list. Otherwise, only the list object
* itself and any references it owned will be destroyed.
*/
void dslist_destroy(DSList *list);

/**
* @brief Return the length of a @c DSList.
*
* @param list a @c DSList object
* @returns the number of elements in @c list
*/
size_t dslist_len(DSList *list);

/**
* @brief Return the capacity of a @c DSList .
*
* @param list a @c DSList object
* @returns the number of elements that @c list can hold
*/
size_t dslist_cap(DSList *list);

/**
* @brief Return the object stored at the given index.
*
* @param list a @c DSList object
* @param index numeric index of the element to get
* @returns @c NULL if the index has no element or is invalid; the
*          object otherwise
*/
void* dslist_get(DSList *list, int index);

/**
* @brief Perform the given function on each object in the list.
*
* The given function is called for every list element including @c NULL
* elements. The given function should be able to operate on @c NULL
* pointers.
*
* @param list a @c DSList object
* @param func a function taking a void pointer which returns nothing
*/
void dslist_foreach(DSList *list, void (*func)(void*));

/**
* @brief Append an element to the end of the list.
*
* @param list a @c DSList object
* @param elem the element to add to the list
* @returns @c false if the element is @c NULL or the list cannot be resized;
*          @c true otherwise
*/
bool dslist_append(DSList *list, void *elem);

/**
* @brief Extend the first @c DSList with the elements of the second.
*
* This function sets each element pointer in @c other to @c NULL
* and sets the length to 0, to avoid any attempts to double-free
* memory, which would result in undefined behavior.
*
* Callers will still be required to destroy the @c other list object,
* though it will no longer contain any references.
*
* @param list the destination @c DSList object
* @param other the source @c DSList object
* @returns @c false if @c list or @c other were @c NULL or if @c list could
*          not be resized; @c true otherwise
*/
bool dslist_extend(DSList *list, DSList *other);

/**
* @brief Insert the given element at the specified index.
*
* Elements may only be inserted between index 0 and index (length - 1).
*
* @param list a @c DSList object
* @param elem the element to insert into @c list
* @param index the index to insert @c elem into @c list
* @returns @c false if @c elem is @c NULL or @c index is invalid or the
*          list could not be resized; @c true otherwise
*/
bool dslist_insert(DSList *list, void *elem, int index);

/**
* @brief Remove the first element in the list matching the given element
* and free that element.
*
* To remove an element with this function, the caller would have had to
* specify a comparator function when this list was created. The element
* will not be freed when removed from the list by this function.
*
* @param list a @c DSList object
* @param elem the element to remove from the list
* @returns a pointer to the element or @c NULL if the element cannot be
*          found or no comparator function was specified
*/
void* dslist_remove(DSList *list, void *elem);

/**
* @brief Remove the element at the given index and return it.
*
* The element will not be freed when removed from the list by this function.
*
* @param list a @c DSList object
* @param index the index of the element to remove
* @returns a pointer to the element or @c NULL if the element cannot be
*          found or no comparator function was specified or the index was
*          invalid
*/
void* dslist_remove_index(DSList *list, int index);

/**
* @brief Pop the top element from the list.
*
* The element will not be freed when removed from the list by this function.
*
* @param list a @c DSList object
* @returns a pointer to the top element or @c NULL if the list is empty
*/
void* dslist_pop(DSList *list);

/**
* @brief Clear the entire list, freeing elements as they are removed.
*
* @param list a @c DSList object
*/
void dslist_clear(DSList *list);

/**
* @brief Return the first index of the given element.
*
* To find an element with this function, the caller would have had to
* specify a comparator function when this list was created.
*
* @param list a @c DSList object
* @param elem the element to find in the list
* @returns the index of the element or @c DSLIST_NOT_FOUND if the element
*          is not found in the list
*/
int dslist_index(DSList *list, void *elem);

/**
* @brief Sort the list in ascending order using the given comparator function.
*
* To sort the list, the caller would have had to specify a comparator
* function when this list was created. The sort will be performed using the
* comparator and the C standard library function @c qsort.
*
* @param list a @c DSList object
*/
void dslist_sort(DSList *list);

/**
* @brief Reverse the list in place.
*
* @param list a @c DSList object
*/
void dslist_reverse(DSList *list);

/**
* @brief Create a new @c DSIter on this list.
*
* @param list a @c DSList object
*/
DSIter* dslist_iter(DSList *list);

#endif //LIBDS_LIST_H
