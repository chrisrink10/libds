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

static const int DEFAULT_GLIST_CAP = 10;
static const int DEFAULT_GLIST_CAPACITY_FACTOR = 2;

struct GList {
    void **data;
    size_t len;
    size_t cap;
    glist_compare_fn cmp;
    glist_free_fn free;
};

static bool glist_resize(GList *list, size_t cap);
static void glist_free(GList *list);

/*
 * LIST PUBLIC FUNCTIONS
 */

GList* glist_new(glist_compare_fn cmpfn, glist_free_fn freefn) {
    GList *list = malloc(sizeof(GList));
    if (!list) {
        return NULL;
    }

    size_t cap = sizeof(void*) * DEFAULT_GLIST_CAP;
    list->data = calloc(cap, sizeof(void*));
    if (!list->data) {
        free(list);
        return NULL;
    }

    list->len = 0;
    list->cap = DEFAULT_GLIST_CAP;
    list->cmp = cmpfn;
    list->free = freefn;
    return list;
}

void glist_destroy(GList *list) {
    if (!list) { return; }
    glist_free(list);
    free(list->data);
    free(list);
}

size_t glist_len(GList *list) {
    return (list) ? (list->len) : 0;
}

size_t glist_cap(GList *list) {
    return (list) ? (list->cap) : 0;
}

void* glist_get(GList *list, int index) {
    if (!list) { return NULL; }
    if (index < 0 || index > list->len) { return NULL; }
    return list->data[index];
}

void glist_foreach(GList *list, void (*func)(void*)) {
    if ((!list) || (!func)) { return; }

    for (int i = 0; i < list->len; i++) {
        func(list->data[i]);
    }
}

bool glist_append(GList *list, void *elem) {
    if ((!list) || (!elem)) { return false; }
    return glist_insert(list, elem, (int)list->len);
}

bool glist_extend(GList *list, GList *other) {
    if ((!list) || (!other)) { return false; }
    if (other->len == 0) { return false; }

    int len = (int)other->len;
    int i = 0;
    for (i = 0; i < len; i++) {
        if (!glist_append(list, glist_get(other, i))) {
            other->len = other->len - i;
            return false;
        }
        other->data[i] = NULL;
    }

    other->len = other->len - i;
    return true;
}

bool glist_insert(GList *list, void *elem, int index) {
    if ((!list) || (!elem)) { return false; }

    if ((index < 0) || (index > (list->len))) {
        return false;
    }

    if ((list->len + 1) > list->cap) {
        if (!glist_resize(list, list->cap * DEFAULT_GLIST_CAPACITY_FACTOR)) {
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

void* glist_remove(GList *list, void *elem) {
    if ((!list) || (!elem)) { return NULL; }

    int i = glist_index(list, elem);
    return glist_remove_index(list, i);
}

void* glist_remove_index(GList *list, int index) {
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

void* glist_pop(GList *list) {
    if (!list) { return NULL; }
    return glist_remove_index(list, (int)list->len-1);
}

void glist_clear(GList *list) {
    if (!list) { return; }
    glist_free(list);
    list->len = 0;
}

int glist_index(GList *list, void *elem) {
    if ((!list) || (!elem) || (!list->cmp)) {
        return GLIST_NULL_POINTER;
    }

    for(int i = 0; i < list->len; i++) {
        if (list->cmp(&list->data[i], &elem) == 0) {
            return i;
        }
    }

    return GLIST_NOT_FOUND;
}

void glist_sort(GList *list) {
    if ((!list) || (list->len == 0) || (!list->cmp)) {
        return;
    }
    qsort(list->data, list->len, sizeof(void *), list->cmp);
}

void glist_reverse(GList *list) {
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

GIter* glist_iter(GList *list) {
    if (!list) { return NULL; }

    GIter *iter = giter_priv_new(ITER_LIST, list);
    if (!iter) {
        return NULL;
    }

    return iter;
}

/*
 * PRIVATE FUNCTIONS
 */

// Resize a glist upwards
static bool glist_resize(GList *list, size_t cap) {
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

// Free all of the value pointers in a GList if a free function was given.
static void glist_free(GList *list) {
    assert(list);
    if (!list->free) { return; }

    for (int i = 0; i < list->len; i++) {
        list->free(list->data[i]);
        list->data[i] = NULL;
    }
}

// Iterate on the next list entry.
bool giter_glist_next(GIter *iter) {
    assert(iter);
    assert(iter->type == ITER_LIST);
    void *data = NULL;

    if (iter->cur == GITER_NO_MORE_ELEMENTS) {
        return false;
    }

    iter->cur++;
    iter->cnt++;
    data = glist_get(iter->target.list, iter->cur);
    if (data) {
        return true;
    }

    iter->cur = GITER_NO_MORE_ELEMENTS;
    iter->cnt = GITER_NO_MORE_ELEMENTS;
    return false;
}