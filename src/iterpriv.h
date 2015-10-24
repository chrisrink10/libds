/*****************************************************************************
 * libds :: iterpriv.h
 *
 * Private header for the iterator object.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_ITERPRIV_H
#define LIBDS_ITERPRIV_H

#include "dictpriv.h"
#include "arraypriv.h"

enum IterType {
    ITER_ARRAY,
    ITER_DICT,
};

union IterTarget {
    DSArray *array;
    DSDict *dict;
};

struct DSIter {
    enum IterType type;
    union IterTarget target;
    int cur;
    int cnt;
    struct bucket *node;
};

DSIter* dsiter_priv_new(enum IterType type, void *target);

#endif //LIBDS_ITERPRIV_H
