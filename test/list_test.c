/*****************************************************************************
 * libds :: list_test.c
 *
 * Test functions for DSList.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libds/buffer.h"
#include "libds/list.h"
#include "CUnit/CUnit.h"
#include "list_test.h"

static DSList *list_test = NULL;

static int list_test_comparator(const void *left, const void *right);

void list_test_setup(void) {
    list_test = dslist_new((dslist_compare_fn) list_test_comparator, free);
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
    DSList *other = dslist_new((dslist_compare_fn) list_test_comparator, free);
    CU_ASSERT_FATAL(other != NULL);

    /* Create the initial array */
    for (int i = 0; i < 10; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dslist_append(list_test, next) == true);
    }

    /* Create the array to use with extend */
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

void list_test_clear(void) {
    /* Insert multiple strings */
    for (int i = 0; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        CU_ASSERT(dslist_insert(list_test, next, 0) == true);
    }

    /* Clear out the array */
    dslist_clear(list_test);
    CU_ASSERT(dslist_len(list_test) == 0);

    /* Verify that the elements are no longer in the array */
    for (int i = 0; i < 6; i++) {
        CU_ASSERT(dslist_get(list_test, i) == NULL);
    }
}

void list_test_queue(void) {
    /* Reject invalid inputs */
    CU_ASSERT(dslist_enqueue(NULL, "Non-null value") == false);
    CU_ASSERT(dslist_enqueue(list_test, NULL) == false);
    CU_ASSERT(dslist_dequeue(NULL) == NULL);

    /* Queue up a bunch of strings */
    for (int i = 0; i < 10; i++) {
        char *fmt = "Str %d";
        char *val = malloc(strlen(fmt) + 1);
        CU_ASSERT_FATAL(val != NULL);
        sprintf(val, fmt, i);
        CU_ASSERT(dslist_enqueue(list_test, val) == true);
        CU_ASSERT(dslist_len(list_test) == i+1);
    }

    /* Dequeue the previous strings */
    for (int i = 0; i < 10; i++) {
        char *fmt = "Str %d";
        char *val = malloc(strlen(fmt) + 1);
        CU_ASSERT_FATAL(val != NULL);
        sprintf(val, fmt, i);

        char *tmp = dslist_dequeue(list_test);
        CU_ASSERT(tmp != NULL);
        CU_ASSERT(dslist_len(list_test) == (9-i));
        CU_ASSERT(strcmp(tmp, val) == 0);
        free(tmp);
        free(val);
    }
}

void list_test_iter(void) {
    int num_iters = 0;
    DSList *list = dslist_new((dslist_compare_fn) dsbuf_compare,
                               (dslist_free_fn) dsbuf_destroy);
    CU_ASSERT_FATAL(list != NULL);

    for (int i = 0; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        DSBuffer *buf = dsbuf_new(next);
        CU_ASSERT_FATAL(buf != NULL);
        free(next);
        CU_ASSERT(dslist_append(list, buf) == true);
        num_iters++;
    }

    DSIter *iter = dslist_iter(list);
    CU_ASSERT_FATAL(iter != NULL);
    CU_ASSERT(dsiter_has_next(iter) == true);

    int count_iters = 0;
    while(dsiter_next(iter)) {
        CU_ASSERT(dsiter_key(iter) == NULL);    // Always NULL for lists
        CU_ASSERT(dsiter_value(iter) != NULL);
        count_iters++;
    }

    CU_ASSERT(count_iters == num_iters);
    CU_ASSERT(dsiter_has_next(iter) == false);
    dsiter_destroy(iter);
    dslist_destroy(list);
}

// Comparator used by the list test suite members
static int list_test_comparator(const void *left, const void *right) {
    const char *l = *(const void**)left;
    const char *r = *(const void**)right;
    return strcmp(l, r);
}