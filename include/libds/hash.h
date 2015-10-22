/*****************************************************************************
 * libds :: hash.h
 *
 * Hashing algorithms.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_HASH_H
#define LIBDS_HASH_H

#include <stdint.h>

uint32_t hash_larson(const char *str);
uint32_t hash_fnv1(const char *str);
uint32_t hash_djb2(const char *str);
uint32_t hash_sdbm(const char *str);

#endif //LIBDS_HASH_H
