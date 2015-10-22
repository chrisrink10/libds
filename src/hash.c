/*****************************************************************************
 * libds :: hash.h
 *
 * Hashing algorithms.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdint.h>

static const uint32_t HASH_LARSON_SEED = 23;
static const uint32_t HASH_LARSON_FACTOR = 101;
static const uint32_t HASH_FNV1_OFFSET_BASIS = 2166136261;
static const uint32_t HASH_FNV1_PRIME = 16777619;
static const uint32_t HASH_DJB2A_SEED = 5381;
static const uint32_t HASH_DJB2A_FACTOR = 33;
static const uint32_t HASH_SDBM_SHIFT1 = 6;
static const uint32_t HASH_SDBM_SHIFT2 = 16;

// The simple hash algorithm developed by Paul Larson at Microsoft Research
// From StackOverflow: http://stackoverflow.com/a/629127/1582301
uint32_t hash_larson(const char *str) {
    uint32_t hash = HASH_LARSON_SEED;

    while ((*str)) {
        hash = (hash * HASH_LARSON_FACTOR) + (unsigned) *str++;
    }

    return hash;
}

// The FNV1 hash algorithm
// From: http://www.isthe.com/chongo/tech/comp/fnv/index.html
uint32_t hash_fnv1(const char *str) {
    uint32_t hash = HASH_FNV1_OFFSET_BASIS;

    while ((*str)) {
        hash = (hash * HASH_FNV1_PRIME);
        hash = hash ^ (unsigned) *str++;
    }

    return hash;
}

// The DJB2 hash algorithm
// From: http://www.cse.yorku.ca/~oz/hash.html
uint32_t hash_djb2(const char *str) {
    uint32_t hash = HASH_DJB2A_SEED;

    while ((*str)) {
        hash = (hash * HASH_DJB2A_FACTOR) + (unsigned) *str++;
    }

    return hash;
}

// The SDBM hash algorithm:
// From: http://www.cse.yorku.ca/~oz/hash.html
uint32_t hash_sdbm(const char *str) {
    uint32_t hash = 0;

    while ((*str)) {
        hash = (unsigned) *str++
               + (hash << HASH_SDBM_SHIFT1)
               + (hash << HASH_SDBM_SHIFT2)
               - hash;
    }

    return hash;
}