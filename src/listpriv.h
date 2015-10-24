/*****************************************************************************
 * libds :: listpriv.c
 *
 * Private header for the array data structure.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_LISTPRIV_H
#define LIBDS_LISTPRIV_H

#include "libds/list.h"

bool dsiter_dsarray_next(DSIter *iter, bool advance);

#endif //LIBDS_LISTPRIV_H
