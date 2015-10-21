/*****************************************************************************
 * libds :: iter.c
 *
 * Generic iterator for sequence types.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "iterpriv.h"

/*
 * ITERATOR PUBLIC FUNCTIONS
 */

bool giter_next(GIter *iter) {
    if (!iter) { return false; }

    switch (iter->type) {
        case ITER_DICT:
            return giter_gdict_next(iter, true);
        case ITER_LIST: {
            return giter_dslist_next(iter);
        }
    }

    return false;
}

bool giter_has_next(GIter *iter) {
    if (!iter) { return false; }

    switch (iter->type) {
        case ITER_DICT:
            return giter_gdict_next(iter, false);
        case ITER_LIST:
            return (dslist_get(iter->target.list, iter->cur+1) != NULL);
    }

    return false;
}

void* giter_key(GIter *iter) {
    if (!iter) { return NULL; }

    switch(iter->type) {
        case ITER_DICT:
            return iter->node->key;
        case ITER_LIST:
            return NULL;
    }

    return NULL;
}

void* giter_value(GIter *iter) {
    if (!iter) { return NULL; }

    switch(iter->type) {
        case ITER_DICT:
            return iter->node->data;
        case ITER_LIST:
            return dslist_get(iter->target.list, iter->cur);
    }

    return NULL;
}

int giter_index(GIter *iter) {
    if (!iter) { return EOF; }
    return iter->cnt;
}

void giter_destroy(GIter *iter) {
    if (!iter) { return; }

    switch (iter->type) {
        case ITER_DICT:
            free(iter->target.dict);
            break;
        case ITER_LIST:
            free(iter->target.list);
            break;
    }

    free(iter->node);
    free(iter);
}

/*
 * PRIVATE FUNCTIONS
 */

// Create a new GIter of the given type on the given target.
GIter* giter_priv_new(enum IterType type, void *target) {
    GIter *iter = malloc(sizeof(GIter));
    if (!iter) {
        return NULL;
    }

    iter->type = type;
    iter->cnt = GITER_NEW_ITERATOR;
    iter->cur = GITER_NEW_ITERATOR;
    iter->node = NULL;
    switch(type) {
        case ITER_DICT:
            iter->target.dict = target;
            break;
        case ITER_LIST:
            iter->target.list = target;
            break;
        default:
            giter_destroy(iter);
            return NULL;
    }

    return iter;
}
