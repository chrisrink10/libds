/*****************************************************************************
 * libds :: dict.h
 *
 * Dictionary/hash table data structure.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_DICT_H
#define LIBDS_DICT_H

#include "libds/iter.h"

/**
* @brief Hash table/dictionary generic data structure.
*/
typedef struct DSDict DSDict;

/**
* @brief Hash function used in a @c DSDict to sort and search.
*/
typedef unsigned int (*dsdict_hash_fn)(void*);

/**
* @brief Free function used in a @c DSDict free remaining elements when
* the dictionary is destroyed.
*/
typedef void (*dsdict_free_fn)(void*);

/**
* @brief Create a new @c DSDict object.
*
* The @c dsdict_free_fn is optional, but the @c dsdict_hash_fn is not.
*/
DSDict* dsdict_new(dsdict_hash_fn hash, dsdict_free_fn freefn);

/**
* @brief Destroy a @c DSDict object.
*/
void dsdict_destroy(DSDict *dict);

/**
* @brief Return the number of elements in the collection.
*/
size_t dsdict_count(DSDict *dict);

/**
* @brief Return the capacity of this collection.
*/
size_t dsdict_cap(DSDict *dict);

/**
* @brief Perform the given function on each object in the dictionary.
*/
void dsdict_foreach(DSDict *dict, void (*func)(void*));

/**
 * @brief Create a new @c GIter object for this dictionary.
 */
GIter* dsdict_iter(DSDict *dict);

/**
* @brief Put the given element in the dictionary by key.
*/
void dsdict_put(DSDict *dict, void *key, void *val);

/**
* @brief Get the element given by the key.
*/
void* dsdict_get(DSDict *dict, void *key);

/**
 * @brief Remove the element from the dictionary and return it to the caller.
 *
 * The caller is responsible for freeing this memory.
 */
void* dsdict_del(DSDict *dict, void *key);

#endif //LIBDS_DICT_H
