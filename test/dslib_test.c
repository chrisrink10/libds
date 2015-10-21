/*****************************************************************************
 * libds :: dslib_test.c
 *
 * Testing suite for libds.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "libds/libds.h"
#include "cunit/CUnit.h"
#include "dslib_test.h"

// Test objects
static DSBuffer *buf_test = NULL;
static DSList *list_test = NULL;
static DSDict *dict_test = NULL;
static int dsdict_collision_cap = 0;
static int dsdict_collision_place = 0;

// Forward declarations for private functions
static int list_test_comparator(const void *left, void const *right);
//static void test_print(void *obj);
static unsigned int dict_test_hash(void *obj);

/*
 * PUBLIC FUNCTIONS
 */

void buf_test_setup(void) {
    buf_test = dsbuf_new_buffer(10);
    CU_ASSERT_FATAL(buf_test != NULL);
}

void buf_test_teardown(void) {
    dsbuf_destroy(buf_test);
    buf_test = NULL;
}

void buf_test_append(void) {
    char *base = "second string";
    DSBuffer *adder = dsbuf_new(base);
    CU_ASSERT_FATAL(adder != NULL);

    /* Verify we don't accept invalid inputs */
    CU_ASSERT(dsbuf_append(NULL, adder) == false);
    CU_ASSERT(dsbuf_append(buf_test, NULL) == false);

    /* Perform the append operation */
    CU_ASSERT(dsbuf_len(buf_test) == 0);
    CU_ASSERT(dsbuf_append(buf_test, adder) == true);
    CU_ASSERT(dsbuf_len(buf_test) == strlen(base));

    dsbuf_destroy(adder);
}

void buf_test_append_char(void) {
    /* Verify we don't accept invalid inputs */
    CU_ASSERT(dsbuf_append_char(buf_test, -1) == false);
    CU_ASSERT(dsbuf_append_char(NULL, 38) == false);

    /* Perform the append operation */
    CU_ASSERT(dsbuf_len(buf_test) == 0);
    CU_ASSERT(dsbuf_append_char(buf_test, '6') == true);
    CU_ASSERT(dsbuf_len(buf_test) == 1);
    CU_ASSERT(dsbuf_char_at(buf_test, 0) == '6');
}

void buf_test_append_str(void) {
    /* Verify we don't accept invalid inputs */
    CU_ASSERT(dsbuf_append_str(buf_test, NULL) == false);
    CU_ASSERT(dsbuf_append_str(NULL, "Test String") == false);

    /* Perform the append operation */
    char *test = "Test String";
    CU_ASSERT(dsbuf_len(buf_test) == 0);
    CU_ASSERT(dsbuf_append_str(buf_test, test) == true);
    CU_ASSERT(dsbuf_len(buf_test) == 11);

    size_t len = strlen(test);
    for (int i = 0; i < len; i++) {
        CU_ASSERT(dsbuf_char_at(buf_test, i) == test[i]);
    }
}

void buf_test_char_at(void) {
    /* Verify we don't accept invalid inputs */
    CU_ASSERT(dsbuf_char_at(buf_test, -1) == DSBUFFER_CHAR_NOT_FOUND);
    CU_ASSERT(dsbuf_char_at(buf_test, 10) == DSBUFFER_CHAR_NOT_FOUND);
    CU_ASSERT(dsbuf_char_at(NULL, 3) == DSBUFFER_CHAR_NOT_FOUND);

    /* Perform the append operation and verify we can access characters */
    CU_ASSERT(dsbuf_len(buf_test) == 0);
    CU_ASSERT(dsbuf_append_char(buf_test, '6') == true);
    CU_ASSERT(dsbuf_append_char(buf_test, '7') == true);
    CU_ASSERT(dsbuf_append_char(buf_test, '8') == true);
    CU_ASSERT(dsbuf_len(buf_test) == 3);
    CU_ASSERT(dsbuf_char_at(buf_test, 0) == '6');
    CU_ASSERT(dsbuf_char_at(buf_test, 1) == '7');
    CU_ASSERT(dsbuf_char_at(buf_test, 2) == '8');
    CU_ASSERT(dsbuf_char_at(buf_test, 6) == DSBUFFER_CHAR_NOT_FOUND);
}

void buf_test_substr(void) {
    DSBuffer *full = dsbuf_new("Full String with Substring");
    CU_ASSERT_FATAL(full != NULL);

    /* Verify we don't accept invalid inputs */
    CU_ASSERT(dsbuf_substr(NULL, 5, 2) == NULL);
    CU_ASSERT(dsbuf_substr(full, -1, 2) == NULL);
    CU_ASSERT(dsbuf_substr(full, (int)dsbuf_len(full) + 2, 2) == NULL);
    CU_ASSERT(dsbuf_substr(full, 5, dsbuf_len(full)) == NULL);

    /* Perform the substring op */
    DSBuffer *sub = dsbuf_substr(full, 5, 6);
    CU_ASSERT_FATAL(sub != NULL);
    CU_ASSERT(dsbuf_equals_char(sub, "String") == true);
    CU_ASSERT(dsbuf_len(sub) == 6);

    dsbuf_destroy(full);
    dsbuf_destroy(sub);
}

void buf_test_equals(void) {
    char *ts1 = "Test String 1";
    char *ts2 = "Test String 2";
    DSBuffer *buf1 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf1 != NULL);
    DSBuffer *buf2 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf2 != NULL);

    CU_ASSERT(dsbuf_append_str(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_equals_char(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_equals(buf1, buf1) == true);
    CU_ASSERT(dsbuf_equals(buf1, buf2) == true);
    CU_ASSERT(dsbuf_equals(buf2, buf2) == true);
    CU_ASSERT(dsbuf_equals(buf_test, buf1) == false);
    CU_ASSERT(dsbuf_equals(buf_test, buf2) == false);

    dsbuf_destroy(buf1);
    dsbuf_destroy(buf2);
}

void buf_test_equals_char(void) {
    char *ts1 = "Test String 1";
    char *ts2 = "Test String 2";
    DSBuffer *buf1 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf1 != NULL);
    DSBuffer *buf2 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf2 != NULL);

    CU_ASSERT(dsbuf_append_str(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_equals_char(buf1, ts1) == true);
    CU_ASSERT(dsbuf_equals_char(buf1, ts2) == false);
    CU_ASSERT(dsbuf_equals_char(buf2, ts1) == true);
    CU_ASSERT(dsbuf_equals_char(buf2, ts2) == false);
    CU_ASSERT(dsbuf_equals_char(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_equals_char(buf_test, ts1) == false);

    dsbuf_destroy(buf1);
    dsbuf_destroy(buf2);
}

void buf_test_to_char_array(void) {
    char *ts1 = "Test String 1";

    CU_ASSERT(dsbuf_to_char_array(NULL) == NULL);
    CU_ASSERT(dsbuf_append_str(buf_test, ts1) == true);
    char *ts2 = dsbuf_to_char_array(buf_test);
    CU_ASSERT(strcmp(ts1, ts2) == 0);
    free(ts2);
}

void buf_test_compare(void) {
    char *ts1 = "Test String 1";
    char *ts2 = "Test String 2";
    DSBuffer *buf1 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf1 != NULL);
    DSBuffer *buf2 = dsbuf_new(ts1);
    CU_ASSERT_FATAL(buf2 != NULL);

    CU_ASSERT(dsbuf_compare(buf1, NULL) == INT32_MAX);
    CU_ASSERT(dsbuf_compare(NULL, buf1) == INT32_MIN);

    CU_ASSERT(dsbuf_append_str(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_equals_char(buf_test, ts2) == true);
    CU_ASSERT(dsbuf_compare(buf1, buf1) == 0);
    CU_ASSERT(dsbuf_compare(buf1, buf2) == 0);
    CU_ASSERT(dsbuf_compare(buf2, buf2) == 0);
    CU_ASSERT(dsbuf_compare(buf_test, buf1) != 0);
    CU_ASSERT(dsbuf_compare(buf_test, buf2) != 0);

    dsbuf_destroy(buf1);
    dsbuf_destroy(buf2);
}

void buf_test_compare_utf8(void) {
    // TODO: finish writing this once a library is decided on
}

void list_test_setup(void) {
    list_test = dslist_new(list_test_comparator, free);
    CU_ASSERT_FATAL(list_test != NULL);
}

void list_test_teardown(void) {
    dslist_destroy(list_test);
    list_test = NULL;
}

void list_test_append(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against null objects? */
    CU_ASSERT(dslist_append(NULL, dest) == false);
    CU_ASSERT(dslist_append(list_test, NULL) == false);
    CU_ASSERT(dslist_len(list_test) == 0);

    /* Can we insert this string? */
    CU_ASSERT(dslist_append(list_test, dest) == true);
    CU_ASSERT(dslist_len(list_test) == 1);

    /* Do we get the same string back? */
    test = dslist_get(list_test, 0);
    CU_ASSERT(test == dest);
    CU_ASSERT(test != NULL);
    CU_ASSERT(strcmp(test, src) == 0);

    /* Can we add multiple? */
    for (int i = 1; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        /* Do multiple get inserted correctly? */
        CU_ASSERT(dslist_append(list_test, next) == true);
        CU_ASSERT(dslist_len(list_test) == i+1);
        CU_ASSERT(dslist_get(list_test, i) == next);

        /* Do they get *appended* rather than inserted at the beginning? */
        CU_ASSERT(strcmp(dslist_get(list_test, i), next) == 0);
    }
}

void list_test_insert(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against null objects? */
    CU_ASSERT(dslist_insert(list_test, NULL, 1) == false);
    CU_ASSERT(dslist_insert(NULL, dest, 1) == false);
    CU_ASSERT(dslist_len(list_test) == 0);

    /* Do we guard against incorrect indices? */
    CU_ASSERT(dslist_insert(list_test, dest, -1) == false);
    CU_ASSERT(dslist_insert(list_test, dest, (int)dslist_len(list_test)+1) == false);
    CU_ASSERT(dslist_insert(list_test, dest, 100000) == false);
    CU_ASSERT(dslist_len(list_test) == 0);

    /* Can we insert this string? */
    CU_ASSERT(dslist_insert(list_test, dest, 0) == true);
    CU_ASSERT(dslist_len(list_test) == 1);

    /* Do we get the same string back? */
    test = dslist_get(list_test, 0);
    CU_ASSERT(test == dest);
    CU_ASSERT(test != NULL);
    CU_ASSERT(strcmp(test, src) == 0);

    /* Can we add multiple? */
    for (int i = 1; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        /* Do multiple get inserted correctly at the beginning? */
        CU_ASSERT(dslist_insert(list_test, next, 0) == true);
        CU_ASSERT(dslist_len(list_test) == i+1);
        CU_ASSERT(dslist_get(list_test, 0) == next);

        /* Do they get inserted at the beginning rather than inserted at the beginning? */
        CU_ASSERT(strcmp(dslist_get(list_test, 0), next) == 0);
    }
}

void list_test_extend(void) {
    DSList *other = dslist_new(list_test_comparator, free);
    CU_ASSERT_FATAL(other != NULL);

    /* Create the initial list */
    for (int i = 0; i < 10; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    /* Create the list to use with extend */
    for (int i = 0; i < 10; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i+10);
        CU_ASSERT(dslist_append(other, next) == true);
    }

    /* Perform the extend */
    CU_ASSERT(dslist_extend(list_test, other) == true);

    /* Test the extend succeeded */
    int val;
    for (int i = 0; i < 20; i++) {
        char *test = dslist_get(list_test, i);
        CU_ASSERT_FATAL(test != NULL);
        sscanf(test, "%d", &val);
        CU_ASSERT(val == i);
    }

    dslist_destroy(other);
}

void list_test_get(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    CU_ASSERT(dslist_insert(list_test, dest, 0) == true);
    CU_ASSERT(dslist_len(list_test) == 1);

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dslist_get(NULL, 0) == NULL);
    CU_ASSERT(dslist_get(list_test, -1) == NULL);
    CU_ASSERT(dslist_get(list_test, ((int)dslist_len(list_test))+10) == NULL);

    /* Can we get a reference to this object? */
    test = dslist_get(list_test, 0);
    CU_ASSERT(test == dest);
    CU_ASSERT(strcmp(test, dest) == 0);
}

void list_test_remove(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dslist_remove(NULL, dest) == NULL);
    CU_ASSERT(dslist_remove(list_test, NULL) == NULL);

    /* Do we return something reasonable if the item isn't found? */
    CU_ASSERT(dslist_remove(list_test, dest) == NULL);

    /* Does the remove actually work? */
    CU_ASSERT(dslist_append(list_test, dest) == true);
    CU_ASSERT(dslist_len(list_test) == 1);
    CU_ASSERT(dslist_remove(list_test, dest) != NULL);
    CU_ASSERT(dslist_len(list_test) == 0);

    free(dest);
}

void list_test_remove_index(void) {
    char *test = NULL;

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dslist_remove_index(NULL, 1) == NULL);
    CU_ASSERT(dslist_remove_index(list_test, -1) == NULL);
    CU_ASSERT(dslist_remove_index(list_test, (int)dslist_len(list_test)+1) == NULL);

    /* Generate some testing data */
    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    /* Do we properly remove from the beginning? ("Test 0") */
    CU_ASSERT(dslist_len(list_test) == 8);
    test = dslist_remove_index(list_test, 0);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dslist_get(list_test, 0) != test);
    CU_ASSERT(strcmp(dslist_get(list_test, 0), test) != 0);
    free(test);

    /* Do we properly remove from the middle? ("Test 4") */
    CU_ASSERT(dslist_len(list_test) == 7);
    test = dslist_remove_index(list_test, 3);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dslist_get(list_test, 3) != test);
    CU_ASSERT(strcmp(dslist_get(list_test, 3), test) != 0);
    free(test);

    /* Do we properly remove from the middle? ("Test 7") */
    CU_ASSERT(dslist_len(list_test) == 6);
    test = dslist_remove_index(list_test, 5);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dslist_get(list_test, 5) == NULL);
    CU_ASSERT(dslist_len(list_test) == 5);
    free(test);
}

void list_test_index(void) {
    CU_ASSERT(dslist_index(NULL, list_test) == DSLIST_NULL_POINTER);
    CU_ASSERT(dslist_index(list_test, NULL) == DSLIST_NULL_POINTER);
    CU_ASSERT(dslist_index(list_test, list_test) == DSLIST_NOT_FOUND);

    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);

        CU_ASSERT(dslist_index(list_test, next) == i);
    }
}

void list_test_pop(void) {
    CU_ASSERT(dslist_pop(NULL) == NULL);

    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    for (int i = 7; i >= 0; i--) {
        CU_ASSERT(dslist_len(list_test) == i+1);
        char *next = dslist_pop(list_test);
        CU_ASSERT(next != NULL);
        CU_ASSERT(dslist_len(list_test) == i);
        free(next);
    }
}

void list_test_resize(void) {
    int cap = ((int)dslist_cap(list_test)) * 3;
    for (int i = 0; i < cap; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    CU_ASSERT(dslist_len(list_test) == cap);
    CU_ASSERT(dslist_cap(list_test) >= cap);
}

void list_test_sort(void) {
    for (int i = 0; i < 15; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        /* Produce a 3 digit number, so sorting can be checked
         * against integral values */
        int r = rand();
        int b = (r % 900) + 100;
        sprintf(next, some, b);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    dslist_sort(list_test);

    int val;
    int prev = -1; // rand() should always return value between 0 and RAND_MAX
    for (int i = 0; i < 15; i++) {
        char *test = dslist_get(list_test, i);
        sscanf(test, "%d", &val);
        CU_ASSERT(val >= prev);
        prev = val;
    }
}

void list_test_reverse(void) {
    for (int i = 0; i < 11; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    dslist_reverse(list_test);

    int expected = 10;
    for (int i = 0; i < 11; i++) {
        char *test = dslist_get(list_test, i);
        int val;
        sscanf(test, "%*s %d", &val);
        CU_ASSERT(val == expected);
        expected--;
    }
}

void list_test_iter(void) {

}

void dict_test_setup(void) {
    dict_test = dsdict_new(dsbuf_hash, dsbuf_compare, (dslist_free_fn) dsbuf_destroy);
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

    /* Clean up objects no longer in the dict */
    dsbuf_destroy(key1);
}

void dict_test_collision(void) {
    DSDict *dict = dsdict_new(dict_test_hash, dsbuf_compare, (dsdict_free_fn)dsbuf_destroy);

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

    dsbuf_destroy(key1);
    dsbuf_destroy(key2);
    dsbuf_destroy(key3);
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

    dsbuf_destroy(key1);
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
    // TODO: test how to resize after implementing resizing
}

void dict_test_iter(void) {

}


/*
 * PRIVATE FUNCTIONS
 */

// Comparator used by the list test suite members
static int list_test_comparator(const void *left, const void *right) {
    const char *l = *(const void**)left;
    const char *r = *(const void**)right;
    return strcmp(l, r);
}

// Print function suitable for a foreach call call
/*
static void test_print(void *obj) {
    printf("%s %p\n", (char*)obj, obj);
}
*/

// Mock hash function for testing hashing collisions. Produces the same
// hash for strings of different sizes. This is important in the case
// that you need to have a semi-deterministic way to mock the hash (i.e.
// you need the same hash again for a get).
static unsigned int dict_test_hash(void *obj) {
    char *str = *(char**)obj;
    return (unsigned int)((strlen(str) * dsdict_collision_cap) + dsdict_collision_place);
}
