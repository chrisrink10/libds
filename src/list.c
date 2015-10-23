/*****************************************************************************
 * libds :: list.c
 *
 * Dynamic list/stack data structure.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libds/list.h"
#include "iterpriv.h"

struct DSList {
    void **data;
    size_t len;
    size_t cap;
    dslist_compare_fn cmp;
    dslist_free_fn free;
};

static bool dslist_resize(DSList *list, size_t cap);
static void dslist_free(DSList *list);

/*
 * LIST PUBLIC FUNCTIONS
 */

DSList* dslist_new(dslist_compare_fn cmpfn, dslist_free_fn freefn) {
    return dslist_new_cap(DSLIST_DEFAULT_CAPACITY, cmpfn, freefn);
}

DSList* dslist_new_cap(size_t cap, dslist_compare_fn cmpfn, dslist_free_fn freefn) {
    assert(cap > 0);
    DSList *list = malloc(sizeof(DSList));
    if (!list) {
        return NULL;
    }

    list->data = calloc(cap, sizeof(void*) * cap);
    if (!list->data) {
        free(list);
        return NULL;
    }

    list->len = 0;
    list->cap = cap;
    list->cmp = cmpfn;
    list->free = freefn;
    return list;
}

void dslist_destroy(DSList *list) {
    if (!list) { return; }
    dslist_free(list);
    free(list->data);
    free(list);
}

size_t dslist_len(DSList *list) {
    assert(list);
    return list->len;
}

size_t dslist_cap(DSList *list) {
    assert(list);
    return list->cap;
}

void* dslist_get(DSList *list, int index) {
    if (!list) { return NULL; }
    if (index < 0 || index > list->len) { return NULL; }
    return list->data[index];
}

void dslist_foreach(DSList *list, void (*func)(void*)) {
    if ((!list) || (!func)) { return; }

    for (int i = 0; i < list->len; i++) {
        func(list->data[i]);
    }
}

bool dslist_append(DSList *list, void *elem) {
    return (!list) ? (false) : dslist_insert(list, elem, (int)list->len);
}

bool dslist_extend(DSList *list, DSList *other) {
    if ((!list) || (!other)) { return false; }
    if (other->len == 0) { return true; }

    int len = (int)other->len;
    int i = 0;
    for (i = 0; i < len; i++) {
        if (!dslist_append(list, dslist_get(other, i))) {
            other->len = other->len - i;
            return false;
        }
        other->data[i] = NULL;
    }

    other->len = other->len - i;
    return true;
}

bool dslist_insert(DSList *list, void *elem, int index) {
    if ((!list) || (!elem)) { return false; }

    if ((index < 0) || (index > (list->len))) {
        return false;
    }

    if ((list->len + 1) > list->cap) {
        if (!dslist_resize(list, list->cap * DSLIST_CAPACITY_FACTOR)) {
            return false;
        }
    }

    for (int i = (int)list->len; i > index; i--) {
        list->data[i] = list->data[i-1];
    }

    list->data[index] = elem;
    list->len++;
    return true;
}

void* dslist_remove(DSList *list, void *elem) {
    if ((!list) || (!elem)) { return NULL; }

    int i = dslist_index(list, elem);
    return dslist_remove_index(list, i);
}

void* dslist_remove_index(DSList *list, int index) {
    if (!list) { return NULL; }

    if ((index < 0) || (index >= list->len)) {
        return NULL;
    }

    void* cache = list->data[index];

    for (int i = index; i < list->len; i++) {
        list->data[i] = list->data[i+1];
    }

    list->data[list->len] = NULL;
    list->len--;
    return cache;
}

void* dslist_pop(DSList *list) {
    if (!list) { return NULL; }
    return dslist_remove_index(list, (int)list->len-1);
}

void dslist_clear(DSList *list) {
    if (!list) { return; }
    dslist_free(list);
    list->len = 0;
}

int dslist_index(DSList *list, void *elem) {
    if ((!list) || (!elem)) {
        return DSLIST_NULL_POINTER;
    }
    if (!list->cmp) {
        return DSLIST_NO_CMP_FUNC;
    }

    for(int i = 0; i < list->len; i++) {
        if (list->cmp(&list->data[i], &elem) == 0) {
            return i;
        }
    }

    return DSLIST_NOT_FOUND;
}

void dslist_sort(DSList *list) {
    if ((!list) || (list->len == 0) || (!list->cmp)) {
        return;
    }
    qsort(list->data, list->len, sizeof(void *), list->cmp);
}

void dslist_reverse(DSList *list) {
    if (!list) { return; }

    // Number of operations required, assuming integer truncation
    int lim = ((int)list->len / 2);

    // Upper index, which will count down
    int up = (int)list->len - 1;

    // Swap i index with up index as they both tick towards the middle
    for (int i = 0; i < lim; i++) {
        void *cache = list->data[i];
        list->data[i] = list->data[up];
        list->data[up] = cache;
        up--;
    }
}

DSIter* dslist_iter(DSList *list) {
    if (!list) { return NULL; }

    DSIter *iter = dsiter_priv_new(ITER_LIST, list);
    if (!iter) {
        return NULL;
    }

    return iter;
}

/*
 * PRIVATE FUNCTIONS
 */

// Resize a glist upwards
static bool dslist_resize(DSList *list, size_t cap) {
    assert(list);

    if ((cap < 1) || (list->cap >= cap)) {
        return false;
    }

    void **cache = list->data;
    list->data = malloc(sizeof(void*) * cap);
    if (!list->data) {
        list->data = cache;
        return false;
    }

    for (int i = 0; i < list->len; i++) {
        list->data[i] = cache[i];
    }

    free(cache);
    list->cap = cap;
    return true;
}

// Free all of the value pointers in a DSList if a free function was given.
static void dslist_free(DSList *list) {
    assert(list);
    if (!list->free) { return; }

    for (int i = 0; i < list->len; i++) {
        list->free(list->data[i]);
        list->data[i] = NULL;
    }
}

// Iterate on the next list entry.
bool dsiter_dslist_next(DSIter *iter) {
    assert(iter);
    assert(iter->type == ITER_LIST);
    void *data = NULL;

    if (iter->cur == GITER_NO_MORE_ELEMENTS) {
        return false;
    }

    iter->cur++;
    iter->cnt++;
    data = dslist_get(iter->target.list, iter->cur);
    if (data) {
        return true;
    }

    iter->cur = GITER_NO_MORE_ELEMENTS;
    iter->cnt = GITER_NO_MORE_ELEMENTS;
    return false;
}