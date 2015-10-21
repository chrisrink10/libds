/**
 * @file dict.h
 *
 * @brief Dictionary/hash table data structure.
 *
 * @author Chris Rink <chrisrink10@gmail.com>
 *
 * @copyright 2015 Chris Rink. MIT Licensed.
 */

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
* @brief Comparator function used in a @c DSDict to compare two keys by value.
*/
typedef int (*dsdict_compare_fn)(const void*, const void*);

/**
* @brief Create a new @c DSDict object with the given hash and free function.
*
* The caller is required to specify a @c dsdict_hash_fn and a
* @c dsdict_compare_fn. The @c dsdict_free_fn is optional. If the caller
* does not specify the free function, then element values will not be
* freed when the @c DSDict object is destroyed.
*
* @param hash a hashing function used to hash dictionary keys
* @param cmpfn a function which can compare two dictionary keys by value
* @param freefn a function which can free hash table values
* @returns a new @c DSDict object or @c NULL if no hash function is
*          specified or memory could not be allocated
*/
DSDict* dsdict_new(dsdict_hash_fn hash, dsdict_compare_fn cmpfn, dsdict_free_fn freefn);

/**
* @brief Destroy a @c DSDict object.
*
* If a @c dsdict_free_fn was specified when the dictionary was created, it
* will be called on each element. Otherwise, only the dictionary object
* itself and any references it owned will be destroyed.
*
* @param dict a @c DSDict object
*/
void dsdict_destroy(DSDict *dict);

/**
* @brief Return the number of elements in the collection.
*
* @param dict a @c DSDict object
* @returns the number of elements in @c dict
*/
size_t dsdict_count(DSDict *dict);

/**
* @brief Return the capacity of this collection.
*
* This value is not particularly useful since the @c DSDict will typically
* resize itself well before reaching full capacity to reduce the likelihood
* of hash collisions. However, it is made available for applications that
* might be interested in the internal capacity.
*
* @param dict a @c DSDict object
* @returns the number of elements that @c dict can hold
*/
size_t dsdict_cap(DSDict *dict);

/**
* @brief Perform the given function on each object in the dictionary.
*
* The given function is called for every dictionary element including
* @c NULL elements. The given function should be able to operate on @c NULL
* pointers.
*
* @param dict a @c DSDict object
* @param func a function taking a void pointer which returns nothing
*/
void dsdict_foreach(DSDict *dict, void (*func)(void*));

/**
 * @brief Create a new @c GIter object for this dictionary.
 */
GIter* dsdict_iter(DSDict *dict);

/**
* @brief Put the given element in the dictionary by key.
*
* Put operations will overwrite elements already in the dictionary with
* keys that compare equal. If the caller specified an element free function
* when this @c dict was created, the previous value will be freed before
* it is overwritten.
*
* A put operation may trigger a resize if the dictionary exceeds its
* internal load factor. Puts are not guaranteed to be O(1) because
* hashing collisions in the table are handled with a linked list.
*
* @param dict a @c DSDict object
* @param key the key
* @param val the value
*/
void dsdict_put(DSDict *dict, void *key, void *val);

/**
* @brief Get the element given by the key.
*
* @param dict a @c DSDict object
* @param key the keyed element to find
* @returns @c NULL if the element does not exist in the dictionary;
*          the element otherwise
*/
void* dsdict_get(DSDict *dict, void *key);

/**
* @brief Remove the element from the dictionary and return it to the caller.
*
* The caller is responsible for freeing this memory.
*
* @param dict a @c DSDict object
* @param key the keyed element to find
* @returns @c NULL if the element does not exist in the dictionary;
*          the element otherwise
*/
void* dsdict_del(DSDict *dict, void *key);

#endif //LIBDS_DICT_H
