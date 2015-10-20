/*****************************************************************************
 * libds :: list.h
 *
 * Dynamic list/stack data structure.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_LIST_H
#define LIBDS_LIST_H

#include "libds/iter.h"

/**
* @brief List/stack generic data structure.
*/
typedef struct GList GList;

/**
* @brief Comparator function used in a @c GList to sort and search.
*/
typedef int (*glist_compare_fn)(const void*, const void*);

/**
* @brief Free function used in a @c GList free remaining elements when
* the list is destroyed.
*/
typedef void (*glist_free_fn)(void*);

/**
* @brief Errors returned from @c GList functions returning indices.
*/
static const int GLIST_NOT_FOUND = -1;
static const int GLIST_NULL_POINTER = -2;

/**
* @brief Create a new @c GList List object with the given
* comparator and free function.
*/
GList* glist_new(glist_compare_fn cmpfn, glist_free_fn freefn);

/**
* @brief Destroy a @c GList object.
*/
void glist_destroy(GList *list);

/**
* @brief Return the length of a @c GList .
*/
size_t glist_len(GList *list);

/**
* @brief Return the capacity of a @c GList .
*/
size_t glist_cap(GList *list);

/**
* @brief Return the object stored at the given index.
*/
void* glist_get(GList *list, int index);

/**
* @brief Perform the given function on each object in the list.
*/
void glist_foreach(GList *list, void (*func)(void*));

/**
* @brief Append an element to the end of the list.
*
* This is functionally equivalent to calling
* @c glist_insert(list, elem, glist_len(list));
*/
bool glist_append(GList *list, void *elem);

/**
* @brief Extend the first @c GList with the elements of the second.
*
* This function sets each element pointer in @c other to @c NULL
* and sets the length to 0, to avoid any attempts to double-free
* memory, which would result in undefined behavior.
*/
bool glist_extend(GList *list, GList *other);

/**
* @brief Insert the given element at the specified index.
*/
bool glist_insert(GList *list, void *elem, int index);

/**
* @brief Remove the first element in the list matching the given element
* and free that element.
*/
void* glist_remove(GList *list, void *elem);

/**
* @brief Remove the element at the given index and return it.
*/
void* glist_remove_index(GList *list, int index);

/**
* @brief Pop the top element from the list.
*/

void* glist_pop(GList *list);

/**
* @brief Clear the list, freeing all elements.
*/
void glist_clear(GList *list);

/**
* @brief Return the first index of the given element or -1 if it is not found.
*/
int glist_index(GList *list, void *elem);

/**
* @brief Sort the list in ascending order using the given comparator function.
*/
void glist_sort(GList *list);

/**
* @brief Reverse the list in place.
*/
void glist_reverse(GList *list);

/**
* @brief Create a new @c GIter on this list.
*/
GIter* glist_iter(GList *list);

#endif //LIBDS_LIST_H
