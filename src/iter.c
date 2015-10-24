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

bool dsiter_next(DSIter *iter) {
    if (!iter) { return false; }

    switch (iter->type) {
        case ITER_DICT:
            return dsiter_dsdict_next(iter, true);
        case ITER_ARRAY: {
            return dsiter_dsarray_next(iter, true);
        }
    }

    return false;
}

bool dsiter_has_next(DSIter *iter) {
    if (!iter) { return false; }

    switch (iter->type) {
        case ITER_DICT:
            return dsiter_dsdict_next(iter, false);
        case ITER_ARRAY:
            return dsiter_dsarray_next(iter, false);
    }

    return false;
}

void* dsiter_key(DSIter *iter) {
    if (!iter) { return NULL; }

    switch(iter->type) {
        case ITER_DICT:
            return (iter->node) ? (iter->node->key) : NULL;
        case ITER_ARRAY:
            return NULL;
    }

    return NULL;
}

void* dsiter_value(DSIter *iter) {
    if (!iter) { return NULL; }

    switch(iter->type) {
        case ITER_DICT:
            return (iter->node) ? (iter->node->data) : NULL;
        case ITER_ARRAY:
            return dsarray_get(iter->target.array, iter->cur);
    }

    return NULL;
}

int dsiter_index(DSIter *iter) {
    if (!iter) { return EOF; }
    return iter->cnt;
}

void dsiter_reset(DSIter *iter) {
    if (!iter) { return; }

    iter->cnt = DSITER_NEW_ITERATOR;
    iter->cur = DSITER_NEW_ITERATOR;
    iter->node = NULL;
}

void dsiter_destroy(DSIter *iter) {
    if (!iter) { return; }

    switch (iter->type) {
        case ITER_DICT:
            iter->target.dict = NULL;
            break;
        case ITER_ARRAY:
            iter->target.array = NULL;
            break;
    }

    iter->node = NULL;
    free(iter);
}

/*
 * PRIVATE FUNCTIONS
 */

// Create a new DSIter of the given type on the given target.
DSIter* dsiter_priv_new(enum IterType type, void *target) {
    DSIter *iter = malloc(sizeof(DSIter));
    if (!iter) {
        return NULL;
    }

    iter->type = type;
    iter->cnt = DSITER_NEW_ITERATOR;
    iter->cur = DSITER_NEW_ITERATOR;
    iter->node = NULL;
    switch(type) {
        case ITER_DICT:
            iter->target.dict = target;
            break;
        case ITER_ARRAY:
            iter->target.array = target;
            break;
        default:
            dsiter_destroy(iter);
            return NULL;
    }

    return iter;
}
