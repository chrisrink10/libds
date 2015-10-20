/*****************************************************************************
 * libds :: dictpriv.c
 *
 * Private header for dictionary data type.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_DICTPRIV_H
#define LIBDS_DICTPRIV_H

#include "libds/dict.h"

struct bucket{
    unsigned int hash;
    void *key;
    void *data;
    struct bucket *next;
};

bool giter_gdict_next(GIter *iter, bool advance);

#endif //LIBDS_DICTPRIV_H
