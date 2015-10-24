/*****************************************************************************
 * libds :: dict_test.c
 *
 * Test functions for DSDict.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "libds/buffer.h"
#include "libds/dict.h"
#include "dict_test.h"

static DSDict *dict_test = NULL;
static int dsdict_collision_cap = 0;
static int dsdict_collision_place = 0;

static unsigned int dict_test_hash(void *obj);

void dict_test_setup(void) {
    dict_test = dsdict_new(dsbuf_hash, dsbuf_compare,
                           (dsdict_free_fn) dsbuf_destroy,
                           (dsdict_free_fn) dsbuf_destroy);
    CU_ASSERT_FATAL(dict_test != NULL);
}

void dict_test_teardown(void) {
    dsdict_destroy(dict_test);
    dict_test = NULL;
}

void dict_test_put(void) {
    /* Produce some mock keys and values */
    DSBuffer *key1 = dsbuf_new("Key1");
    CU_ASSERT_FATAL(key1 != NULL);
    DSBuffer *val1 = dsbuf_new("Val1");
    CU_ASSERT_FATAL(val1 != NULL);
    DSBuffer *val2 = dsbuf_new("Val2");
    CU_ASSERT_FATAL(val2 != NULL);
    DSBuffer *test1 = NULL;
    DSBuffer *test2 = NULL;

    /* Test for invalid inputs */
    dsdict_put(NULL, key1, "String");
    CU_ASSERT(dsdict_count(dict_test) == 0);
    dsdict_put(dict_test, NULL, "String");
    CU_ASSERT(dsdict_count(dict_test) == 0);

    /* Perform the first put */
    dsdict_put(dict_test, key1, val1);
    CU_ASSERT(dsdict_count(dict_test) == 1);
    test1 = dsdict_get(dict_test, key1);
    CU_ASSERT(test1 != NULL);
    CU_ASSERT(test1 == val1);
    CU_ASSERT(dsbuf_equals(test1, val1) == true);

    /* Verify that a put with the same key overwrites properly */
    dsdict_put(dict_test, key1, val2);          /* this also frees val1 */
    CU_ASSERT(dsdict_count(dict_test) == 1);
    test2 = dsdict_get(dict_test, key1);
    CU_ASSERT(test2 != NULL);
    CU_ASSERT(test2 == val2);
    CU_ASSERT(dsbuf_equals(test2, val2) == true);

    /* No cleanup is required since key and value will be freed by teardown */
}

void dict_test_collision(void) {
    DSDict *dict = dsdict_new(dict_test_hash, dsbuf_compare,
                              (dsdict_free_fn) dsbuf_destroy,
                              (dsdict_free_fn) dsbuf_destroy);

    DSBuffer *key1 = dsbuf_new("Key1");
    CU_ASSERT_FATAL(key1 != NULL);
    DSBuffer *key2 = dsbuf_new("Key2--");
    CU_ASSERT_FATAL(key2 != NULL);
    DSBuffer *key3 = dsbuf_new("Key3----");
    CU_ASSERT_FATAL(key3 != NULL);
    DSBuffer *val1 = dsbuf_new("Val1");
    CU_ASSERT_FATAL(val1 != NULL);
    DSBuffer *val2 = dsbuf_new("Val2");
    CU_ASSERT_FATAL(val2 != NULL);
    DSBuffer *test1 = NULL;
    DSBuffer *test2 = NULL;
    DSBuffer *test3 = NULL;

    /* Mock our hash, so we can simulate key collision
     * We are forcing this both puts into the bucket 10 */
    dsdict_collision_cap = (int)dsdict_cap(dict);
    dsdict_collision_place = 10;

    /* Perform the first put */
    dsdict_put(dict, key1, val1);
    CU_ASSERT(dsdict_count(dict) == 1);
    test1 = dsdict_get(dict, key1);
    CU_ASSERT(test1 == val1);
    CU_ASSERT(dsbuf_equals(test1, val1) == true);

    /* Perform the second put, which needs to go into the same bucket */
    dsdict_put(dict, key2, val2);
    CU_ASSERT(dsdict_count(dict) == 2);
    test2 = dsdict_get(dict, key2);
    CU_ASSERT(test2 != NULL);
    CU_ASSERT(test2 == val2);
    CU_ASSERT(dsbuf_equals(test2, val2) == true);
    CU_ASSERT(test1 != test2);

    /* Make sure even in the case of collision that we do not get an
     * incorrect value back from a key which just collided (i.e. another
     * value on the same linked list) */
    test3 = dsdict_get(dict, key3);
    CU_ASSERT(test3 == NULL);

    /* Clean up dict and any keys/values (because free fns were specified) */
    dsdict_destroy(dict);
}

void dict_test_get(void) {
    /* Produce some mock keys and values */
    DSBuffer *key1 = dsbuf_new("Key1");
    CU_ASSERT_FATAL(key1 != NULL);
    DSBuffer *val1 = dsbuf_new("Val1");
    CU_ASSERT_FATAL(val1 != NULL);
    DSBuffer *test1 = NULL;

    /* Test for invalid inputs */
    CU_ASSERT(dsdict_get(NULL, key1) == NULL);
    CU_ASSERT(dsdict_get(dict_test, NULL) == NULL);
    CU_ASSERT(dsdict_get(dict_test, key1) == NULL);

    /* Make sure we get a value back */
    dsdict_put(dict_test, key1, val1);
    CU_ASSERT(dsdict_count(dict_test) == 1);
    test1 = dsdict_get(dict_test, key1);
    CU_ASSERT(test1 == val1);
    CU_ASSERT(dsbuf_equals(test1, val1) == true);
    CU_ASSERT(dsdict_count(dict_test) == 1);

    /* No cleanup is required since key and value will be freed by teardown */
}

void dict_test_del(void) {
    /* Produce some mock keys and values */
    DSBuffer *key1 = dsbuf_new("Key1");
    CU_ASSERT_FATAL(key1 != NULL);
    DSBuffer *val1 = dsbuf_new("Val1");
    CU_ASSERT_FATAL(val1 != NULL);
    DSBuffer *test1 = NULL;
    DSBuffer *test2 = NULL;

    /* Test for invalid inputs */
    CU_ASSERT(dsdict_del(NULL, key1) == NULL);
    CU_ASSERT(dsdict_del(dict_test, NULL) == NULL);
    CU_ASSERT(dsdict_del(dict_test, key1) == NULL);

    /* Make sure we get a value back */
    dsdict_put(dict_test, key1, val1);
    CU_ASSERT(dsdict_count(dict_test) == 1);
    test1 = dsdict_get(dict_test, key1);
    CU_ASSERT(test1 == val1);
    CU_ASSERT(dsbuf_equals(test1, val1) == true);
    CU_ASSERT(dsdict_count(dict_test) == 1);

    /* Attempt to delete the value */
    test2 = dsdict_del(dict_test, key1);
    CU_ASSERT(test2 == val1);
    CU_ASSERT(dsbuf_equals(test2, val1) == true);
    CU_ASSERT(dsdict_count(dict_test) == 0);

    dsbuf_destroy(key1);
    dsbuf_destroy(val1);
}

void dict_test_resize(void) {
    static char *keyfmt = "Key %d";
    static char *valfmt = "Value %d";

    // Put lots of elements into the dictionary (to force multiple resizes)
    int cap = ((int)dsdict_cap(dict_test)) * 4;
    int xtra = (int)log10(cap) + 2;
    for (int i = 0; i < cap; i++) {
        char *key = malloc(strlen(keyfmt) + xtra);
        CU_ASSERT_FATAL(key != NULL);
        char *val = malloc(strlen(valfmt) + xtra);
        CU_ASSERT_FATAL(val != NULL);

        sprintf(key, keyfmt, i);
        sprintf(val, valfmt, i);
        DSBuffer *keybuf = dsbuf_new(key);
        CU_ASSERT_FATAL(keybuf != NULL);
        free(key);
        DSBuffer *valbuf = dsbuf_new(val);
        CU_ASSERT_FATAL(valbuf != NULL);
        free(val);

        dsdict_put(dict_test, keybuf, valbuf);
        CU_ASSERT(dsdict_count(dict_test) == i+1);
        CU_ASSERT(dsdict_get(dict_test, keybuf) == valbuf);
    }

    // Verify all of those elements still exist
    for (int i = 0; i < cap; i++) {
        char *key = malloc(strlen(keyfmt) + xtra);
        CU_ASSERT_FATAL(key != NULL);
        char *val = malloc(strlen(valfmt) + xtra);
        CU_ASSERT_FATAL(val != NULL);

        sprintf(key, keyfmt, i);
        sprintf(val, valfmt, i);
        DSBuffer *keybuf = dsbuf_new(key);
        CU_ASSERT_FATAL(keybuf != NULL);
        free(key);
        DSBuffer *valbuf = dsbuf_new(val);
        CU_ASSERT_FATAL(valbuf != NULL);
        free(val);

        DSBuffer *tmpval = dsdict_get(dict_test, keybuf);
        CU_ASSERT(tmpval != NULL);
        CU_ASSERT(dsbuf_compare(valbuf, tmpval) == 0);

        // Clean these up since they were never inserted into the DSDict
        dsbuf_destroy(keybuf);
        dsbuf_destroy(valbuf);
    }

    CU_ASSERT(dsdict_count(dict_test) == cap);
}

void dict_test_iter(void) {
    int num_iters = 0;
    DSDict *dict = dsdict_new(dsbuf_hash, dsbuf_compare,
                              (dsdict_free_fn) dsbuf_destroy,
                              (dsdict_free_fn) dsbuf_destroy);
    CU_ASSERT_FATAL(dict != NULL);

    for (int i = 0; i < 6; i++) {
        char *keyfmt = "Key %d";
        char *valfmt = "Value %d";
        char *key = malloc(strlen(keyfmt) + 1);
        CU_ASSERT_FATAL(key != NULL);
        sprintf(key, keyfmt, i);
        char *val = malloc(strlen(valfmt) + 1);
        CU_ASSERT_FATAL(val != NULL);
        sprintf(val, valfmt, i);

        DSBuffer *keybuf = dsbuf_new(key);
        CU_ASSERT_FATAL(keybuf != NULL);
        free(key);
        DSBuffer *valbuf = dsbuf_new(val);
        CU_ASSERT_FATAL(valbuf != NULL);
        free(val);
        dsdict_put(dict, keybuf, valbuf);
        num_iters++;
    }

    DSIter *iter = dsdict_iter(dict);
    CU_ASSERT_FATAL(iter != NULL);
    CU_ASSERT(dsiter_has_next(iter) == true);

    int count_iters = 0;
    while(dsiter_next(iter)) {
        CU_ASSERT(dsiter_key(iter) != NULL);
        CU_ASSERT(dsiter_value(iter) != NULL);
        count_iters++;
    }

    CU_ASSERT(count_iters == num_iters);
    CU_ASSERT(dsiter_has_next(iter) == false);
    dsiter_destroy(iter);
    dsdict_destroy(dict);
}

// Mock hash function for testing hashing collisions. Produces the same
// hash for strings of different sizes. This is important in the case
// that you need to have a semi-deterministic way to mock the hash (i.e.
// you need the same hash again for a get).
static unsigned int dict_test_hash(void *obj) {
    char *str = *(char**)obj;
    return (unsigned int)((strlen(str) * dsdict_collision_cap) + dsdict_collision_place);
}
