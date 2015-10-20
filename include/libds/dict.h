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
typedef struct GDict GDict;

/**
* @brief Hash function used in a @c GDict to sort and search.
*/
typedef unsigned int (*gdict_hash_fn)(void*);

/**
* @brief Free function used in a @c GDict free remaining elements when
* the dictionary is destroyed.
*/
typedef void (*gdict_free_fn)(void*);

/**
* @brief Create a new @c GDict object.
*
* The @c gdict_free_fn is optional, but the @c gdict_hash_fn is not.
*/
GDict* gdict_new(gdict_hash_fn hash, gdict_free_fn freefn);

/**
* @brief Destroy a @c GDict object.
*/
void gdict_destroy(GDict *dict);

/**
* @brief Return the number of elements in the collection.
*/
size_t gdict_count(GDict *dict);

/**
* @brief Return the capacity of this collection.
*/
size_t gdict_cap(GDict *dict);

/**
* @brief Perform the given function on each object in the dictionary.
*/
void gdict_foreach(GDict *dict, void (*func)(void*));

/**
 * @brief Create a new @c GIter object for this dictionary.
 */
GIter* gdict_iter(GDict *dict);

/**
* @brief Put the given element in the dictionary by key.
*/
void gdict_put(GDict *dict, void *key, void *val);

/**
* @brief Get the element given by the key.
*/
void* gdict_get(GDict *dict, void *key);

/**
 * @brief Remove the element from the dictionary and return it to the caller.
 *
 * The caller is responsible for freeing this memory.
 */
void* gdict_del(GDict *dict, void *key);

#endif //LIBDS_DICT_H
