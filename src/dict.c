/*****************************************************************************
 * libds :: dict.c
 *
 * Dictionary/hash table data structure.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dictpriv.h"
#include "iterpriv.h"

static const double DSDICT_DEFAULT_LOAD = 0.66;
static const int DSDICT_DEFAULT_CAP = 64;
static const int DSDICT_DEFAULT_CAPACITY_FACTOR = 2;

/*
 * Prime moduli for hash table capacity
 * - Array index is power of 2 (i.e. index 1 is 2^1 = 2)
 * - Value at index is modulus to use for capacity at indexed power of 2
 * - Powers of 2 given at: https://primes.utm.edu/lists/2small/0bit.html
 */
#define POW2(n, k) ((1 << n) - k)
static const int DSDICT_MOD_TABLE[] = {
        1, 2, 3, 7, 13, 31, 61, 127, 251,                           /* Powers 0 through 8 */
        POW2(9, 3), POW2(10, 3), POW2(11, 9), POW2(12, 3),          /* Powers 9 through 12 */
        POW2(13, 1), POW2(14, 3), POW2(15, 19), POW2(16, 15),       /* Powers 13 through 16 */
        POW2(17, 1), POW2(18, 5), POW2(19, 1), POW2(20, 3),         /* Powers 17 through 20 */
        POW2(21, 9), POW2(22, 3), POW2(23, 15), POW2(24, 3),        /* Powers 21 through 24 */
        POW2(25, 39), POW2(26, 5), POW2(27, 39), POW2(28, 57),      /* Powers 25 through 28 */
        POW2(29, 3), POW2(30, 35), POW2(31, 1),                     /* Powers 29 through 31 */
};

struct DSDict {
    struct bucket **vals;
    size_t cnt;
    size_t cap;
    dsdict_hash_fn hash;
    dsdict_free_fn free;
};

static bool dsdict_resize(DSDict *dict, size_t cap);
static void dsdict_free(DSDict *dict);
static inline int dsdict_place(unsigned int hash, size_t cap);

/*
 * DICTIONARY PUBLIC FUNCTIONS
 */

DSDict * dsdict_new(dsdict_hash_fn hash, dsdict_free_fn freefn) {
    if (!hash) { return NULL; }

    DSDict *dict = malloc(sizeof(DSDict));
    if (!dict) {
        return NULL;
    }

    size_t cap = sizeof(struct bucket *) * DSDICT_DEFAULT_CAP;
    dict->vals = calloc(cap, sizeof(struct bucket *));
    if (!dict->vals) {
        free(dict);
        return NULL;
    }

    dict->cnt = 0;
    dict->cap = DSDICT_DEFAULT_CAP;
    dict->hash = hash;
    dict->free = freefn;
    return dict;
}

void dsdict_destroy(DSDict *dict) {
    if (!dict) { return; }
    dsdict_free(dict);
    free(dict->vals);
    free(dict);
}

size_t dsdict_count(DSDict *dict) {
    assert(dict);
    return dict->cnt;
}

size_t dsdict_cap(DSDict *dict) {
    assert(dict);
    return dict->cap;
}

void dsdict_foreach(DSDict *dict, void (*func)(void*)) {
    if ((!dict) || (!func)) { return; }

    for (int i = 0; i < dict->cnt; i++) {
        if (!dict->vals[i]) { continue; }
        func(dict->vals[i]->data);

        struct bucket *next = dict->vals[i]->next;
        while ((next)){
            func(next->data);
            next = next->next;
        }
    }
}

void dsdict_put(DSDict *dict, void *key, void *val) {
    if ((!dict) || (!key)) { return; }

    unsigned int hash = dict->hash(key);
    int place = dsdict_place(hash, dict->cap);

    // Get reference to place and see if there is data there;
    // if not, just set the data
    struct bucket *cur = dict->vals[place];
    if (!cur) {
        dict->vals[place] = malloc(sizeof(struct bucket));
        if (!dict->vals[place]) { return; }
        cur = dict->vals[place];
        goto dsdict_put_op;
    }

    // If there was data, check if it's the same value;
    // if so, we can overwrite it and we're done
    if (cur->hash == hash) {
        cur->data = val;
        goto cleanup_dsdict_put;
    }

    // Otherwise traverse linked list and check if we can
    // overwrite any connected nodes
    struct bucket *prev = cur;
    cur = cur->next;
    while ((cur)) {
        prev = cur;
        if (cur->hash == hash) {
            cur->data = val;
            goto cleanup_dsdict_put;
        }
        cur = cur->next;
    }

    // If we made it this far, we need to add a new node at cur
    prev->next = malloc(sizeof(struct bucket));
    cur = prev->next;

    // Actually perform the titular "put"
dsdict_put_op:
    if (!cur) { return; }

    cur->hash = hash;
    cur->key = key;
    cur->data = val;
    cur->next = NULL;
    dict->cnt++;

    // Clean up and decide if we need to resize no
cleanup_dsdict_put: {
        double load = ((double)dict->cnt / dict->cap);
        if (load >= DSDICT_DEFAULT_LOAD) {
            dsdict_resize(dict, dict->cap * DSDICT_DEFAULT_CAPACITY_FACTOR);
        }
    }
    return;
}

void* dsdict_get(DSDict *dict, void *key) {
    if ((!dict) || (!key)) { return NULL; }

    unsigned int hash = dict->hash(key);
    int place = dsdict_place(hash, dict->cap);

    struct bucket *cur = dict->vals[place];
    if (!cur) { return NULL; }
    if (cur->hash == hash) {
        return cur->data;
    }

    cur = cur->next;
    while ((cur)) {
        if (cur->hash == hash) {
            return cur->data;
        }
        cur = cur->next;
    }

    return NULL;
}

void* dsdict_del(DSDict *dict, void *key) {
    if ((!dict) || (!key)) { return NULL; }

    unsigned int hash = dict->hash(key);
    int place = dsdict_place(hash, dict->cap);

    struct bucket *cur = dict->vals[place];
    if (!cur) { return NULL; }
    if (cur->hash == hash) {
        void *cache = cur->data;
        dict->vals[place] = cur->next;
        free(cur);
        dict->cnt--;
        return cache;
    }

    cur = cur->next;
    while ((cur)) {
        if (cur->hash == hash) {
            void *cache = cur->data;
            dict->vals[place] = cur->next;
            free(cur);
            dict->cnt--;
            return cache;
        }
        cur = cur->next;
    }

    return NULL;
}

GIter* dsdict_iter(DSDict *dict) {
    if (!dict) { return NULL; }

    GIter *iter = giter_priv_new(ITER_DICT, dict);
    if (!iter) {
        return NULL;
    }

    return iter;
}

/*
 * PRIVATE FUNCTIONS
 */

// Resize a DSDict upwards
static bool dsdict_resize(DSDict *dict, size_t cap) {
    assert(dict);

    if ((cap < 1) || (dict->cap >= cap)) {
        return false;
    }

    struct bucket **cache = dict->vals;
    dict->vals = calloc(cap, sizeof(struct bucket));
    if (!dict->vals) {
        dict->vals = cache;
        return false;
    }

    // TODO: Properly resize a DSDict object

    return true;
}

// Free all of the value pointers in a DSDict if a free function was given.
static void dsdict_free(DSDict *dict) {
    assert(dict);
    bool can_free = (dict->free) ? true : false;

    for (int i = 0; i < dict->cap; i++) {
        if (!dict->vals[i]) { continue; }
        if (can_free) {
            dict->free(dict->vals[i]->data);
        }

        struct bucket *cur = NULL;
        struct bucket *next = dict->vals[i]->next;
        while ((next)){
            if (can_free) {
                dict->free(next->data);
            }
            cur = next;
            next = next->next;
            free(cur);
        }

        free(dict->vals[i]);
        dict->vals[i] = NULL;
    }
}

// Given a hash value and a capacity, compute the place of the element in the array.
static inline int dsdict_place(unsigned int hash, size_t cap) {
    int power = (int)log2((int)cap);
    int mod = ((power >= 0) && (power <= 31)) ? DSDICT_MOD_TABLE[power] : (int)cap;
    return (hash % mod);
}

// Iterate on the next dictionary entry.
bool giter_dsdict_next(GIter *iter, bool advance) {
    assert(iter);
    assert(iter->type == ITER_DICT);

    // If we already know there are no more elements, quit
    if (iter->cur == GITER_NO_MORE_ELEMENTS) {
        return false;
    }

    // Get the initial node pointer
    if (iter->cur == GITER_NEW_ITERATOR) {
        DSDict *dict = iter->target.dict;

        // We do not need to traverse any linked lists
        // since this is explicitly the first node
        for (int i = 0; i < dict->cap; i++) {
            if (dict->vals[i]) {
                if (advance) {
                    iter->node = dict->vals[i];
                    iter->cur = i;
                }
                return true;
            }
        }

        // No elements found in this dictionary
        if (advance) {
            iter->cur = GITER_NO_MORE_ELEMENTS;
            iter->cnt = GITER_NO_MORE_ELEMENTS;
        }
        return false;
    }

    // If there is a next node, set our next pointer to that
    if (iter->node->next) {
        if (advance) {
            iter->node = iter->node->next;
            iter->cnt++;
        }
        return true;
    }

    // Otherwise, traverse through the array to find the next pointer
    DSDict *dict = iter->target.dict;
    for (int i = (iter->cur + 1); i < dict->cap; i++) {
        if (dict->vals[i]) {
            if (advance) {
                iter->cur = i;
                iter->cnt++;
                iter->node = dict->vals[i];
            }
            return true;
        }
    }

    // If we get here, there is no more data in the dict
    if (advance) {
        iter->cur = GITER_NO_MORE_ELEMENTS;
        iter->cnt = GITER_NO_MORE_ELEMENTS;
        iter->node = NULL;
    }
    return false;
}