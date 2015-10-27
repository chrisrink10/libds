/*****************************************************************************
 * libds :: array_test.c
 *
 * Test functions for DSArray.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libds/libds.h"
#include "CUnit/CUnit.h"
#include "array_test.h"

static DSArray *array_test = NULL;

static int array_test_comparator(const void *left, void const *right);

void array_test_setup(void) {
    array_test = dsarray_new(array_test_comparator, free);
    CU_ASSERT_FATAL(array_test != NULL);
}

void array_test_teardown(void) {
    dsarray_destroy(array_test);
    array_test = NULL;
}

void array_test_append(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against null objects? */
    CU_ASSERT(dsarray_append(NULL, dest) == false);
    CU_ASSERT(dsarray_append(array_test, NULL) == false);
    CU_ASSERT(dsarray_len(array_test) == 0);

    /* Can we insert this string? */
    CU_ASSERT(dsarray_append(array_test, dest) == true);
    CU_ASSERT(dsarray_len(array_test) == 1);

    /* Do we get the same string back? */
    test = dsarray_get(array_test, 0);
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
        CU_ASSERT(dsarray_append(array_test, next) == true);
        CU_ASSERT(dsarray_len(array_test) == i+1);
        CU_ASSERT(dsarray_get(array_test, i) == next);

        /* Do they get *appended* rather than inserted at the beginning? */
        CU_ASSERT(strcmp(dsarray_get(array_test, i), next) == 0);
    }
}

void array_test_insert(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against null objects? */
    CU_ASSERT(dsarray_insert(array_test, NULL, 1) == false);
    CU_ASSERT(dsarray_insert(NULL, dest, 1) == false);
    CU_ASSERT(dsarray_len(array_test) == 0);

    /* Do we guard against incorrect indices? */
    CU_ASSERT(dsarray_insert(array_test, dest, -1) == false);
    CU_ASSERT(dsarray_insert(array_test, dest, (int)dsarray_len(array_test)+1) == false);
    CU_ASSERT(dsarray_insert(array_test, dest, 100000) == false);
    CU_ASSERT(dsarray_len(array_test) == 0);

    /* Can we insert this string? */
    CU_ASSERT(dsarray_insert(array_test, dest, 0) == true);
    CU_ASSERT(dsarray_len(array_test) == 1);

    /* Do we get the same string back? */
    test = dsarray_get(array_test, 0);
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
        CU_ASSERT(dsarray_insert(array_test, next, 0) == true);
        CU_ASSERT(dsarray_len(array_test) == i+1);
        CU_ASSERT(dsarray_get(array_test, 0) == next);

        /* Do they get inserted at the beginning rather than inserted at the beginning? */
        CU_ASSERT(strcmp(dsarray_get(array_test, 0), next) == 0);
    }
}

void array_test_extend(void) {
    DSArray *other = dsarray_new(array_test_comparator, free);
    CU_ASSERT_FATAL(other != NULL);

    /* Create the initial array */
    for (int i = 0; i < 10; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    /* Create the array to use with extend */
    for (int i = 0; i < 10; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i+10);
        CU_ASSERT(dsarray_append(other, next) == true);
    }

    /* Perform the extend */
    CU_ASSERT(dsarray_extend(array_test, other) == true);

    /* Test the extend succeeded */
    int val;
    for (int i = 0; i < 20; i++) {
        char *test = dsarray_get(array_test, i);
        CU_ASSERT_FATAL(test != NULL);
        sscanf(test, "%d", &val);
        CU_ASSERT(val == i);
    }

    dsarray_destroy(other);
}

void array_test_get(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    char *test = NULL;
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    CU_ASSERT(dsarray_insert(array_test, dest, 0) == true);
    CU_ASSERT(dsarray_len(array_test) == 1);

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dsarray_get(NULL, 0) == NULL);
    CU_ASSERT(dsarray_get(array_test, -1) == NULL);
    CU_ASSERT(dsarray_get(array_test, ((int)dsarray_len(array_test))+10) == NULL);

    /* Can we get a reference to this object? */
    test = dsarray_get(array_test, 0);
    CU_ASSERT(test == dest);
    CU_ASSERT(strcmp(test, dest) == 0);
}

void array_test_remove(void) {
    char *src = "This is a test string";
    char *dest = malloc(strlen(src) + 1);
    CU_ASSERT_FATAL(dest != NULL);

    strcpy(dest, src);
    CU_ASSERT(strcmp(dest, src) == 0);

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dsarray_remove(NULL, dest) == NULL);
    CU_ASSERT(dsarray_remove(array_test, NULL) == NULL);

    /* Do we return something reasonable if the item isn't found? */
    CU_ASSERT(dsarray_remove(array_test, dest) == NULL);

    /* Does the remove actually work? */
    CU_ASSERT(dsarray_append(array_test, dest) == true);
    CU_ASSERT(dsarray_len(array_test) == 1);
    CU_ASSERT(dsarray_remove(array_test, dest) != NULL);
    CU_ASSERT(dsarray_len(array_test) == 0);

    free(dest);
}

void array_test_remove_index(void) {
    char *test = NULL;

    /* Do we guard against invalid inputs? */
    CU_ASSERT(dsarray_remove_index(NULL, 1) == NULL);
    CU_ASSERT(dsarray_remove_index(array_test, -1) == NULL);
    CU_ASSERT(dsarray_remove_index(array_test, (int)dsarray_len(array_test)+1) == NULL);

    /* Generate some testing data */
    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    /* Do we properly remove from the beginning? ("Test 0") */
    CU_ASSERT(dsarray_len(array_test) == 8);
    test = dsarray_remove_index(array_test, 0);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dsarray_get(array_test, 0) != test);
    CU_ASSERT(strcmp(dsarray_get(array_test, 0), test) != 0);
    free(test);

    /* Do we properly remove from the middle? ("Test 4") */
    CU_ASSERT(dsarray_len(array_test) == 7);
    test = dsarray_remove_index(array_test, 3);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dsarray_get(array_test, 3) != test);
    CU_ASSERT(strcmp(dsarray_get(array_test, 3), test) != 0);
    free(test);

    /* Do we properly remove from the middle? ("Test 7") */
    CU_ASSERT(dsarray_len(array_test) == 6);
    test = dsarray_remove_index(array_test, 5);
    CU_ASSERT(test != NULL);
    CU_ASSERT(dsarray_get(array_test, 5) == NULL);
    CU_ASSERT(dsarray_len(array_test) == 5);
    free(test);
}

void array_test_index(void) {
    CU_ASSERT(dsarray_index(NULL, array_test) == DSARRAY_NULL_POINTER);
    CU_ASSERT(dsarray_index(array_test, NULL) == DSARRAY_NULL_POINTER);
    CU_ASSERT(dsarray_index(array_test, array_test) == DSARRAY_NOT_FOUND);

    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);

        CU_ASSERT(dsarray_index(array_test, next) == i);
    }
}

void array_test_pop(void) {
    CU_ASSERT(dsarray_pop(NULL) == NULL);

    for (int i = 0; i < 8; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    for (int i = 7; i >= 0; i--) {
        CU_ASSERT(dsarray_len(array_test) == i+1);
        char *next = dsarray_pop(array_test);
        CU_ASSERT(next != NULL);
        CU_ASSERT(dsarray_len(array_test) == i);
        free(next);
    }
}

void array_test_resize(void) {
    int cap = ((int)dsarray_cap(array_test)) * 3;
    for (int i = 0; i < cap; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    CU_ASSERT(dsarray_len(array_test) == cap);
    CU_ASSERT(dsarray_cap(array_test) >= cap);
}

void array_test_sort(void) {
    for (int i = 0; i < 15; i++) {
        char *some = "%d";
        char *next = malloc(5);
        CU_ASSERT_FATAL(next != NULL);

        /* Produce a 3 digit number, so sorting can be checked
         * against integral values */
        int r = rand();
        int b = (r % 900) + 100;
        sprintf(next, some, b);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    dsarray_sort(array_test);

    int val;
    int prev = -1; // rand() should always return value between 0 and RAND_MAX
    for (int i = 0; i < 15; i++) {
        char *test = dsarray_get(array_test, i);
        sscanf(test, "%d", &val);
        CU_ASSERT(val >= prev);
        prev = val;
    }
}

void array_test_reverse(void) {
    for (int i = 0; i < 11; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);

        sprintf(next, some, i);
        CU_ASSERT(dsarray_append(array_test, next) == true);
    }

    dsarray_reverse(array_test);

    int expected = 10;
    for (int i = 0; i < 11; i++) {
        char *test = dsarray_get(array_test, i);
        int val;
        sscanf(test, "%*s %d", &val);
        CU_ASSERT(val == expected);
        expected--;
    }
}

void array_test_clear(void) {
    /* Insert multiple strings */
    for (int i = 0; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        CU_ASSERT(dsarray_insert(array_test, next, 0) == true);
    }

    /* Clear out the array */
    dsarray_clear(array_test);
    CU_ASSERT(dsarray_len(array_test) == 0);

    /* Verify that the elements are no longer in the array */
    for (int i = 0; i < 6; i++) {
        CU_ASSERT(dsarray_get(array_test, i) == NULL);
    }
}

void array_test_iter(void) {
    int num_iters = 0;
    DSArray *array = dsarray_new((dsarray_compare_fn) dsbuf_compare,
                                 (dsarray_free_fn) dsbuf_destroy);
    CU_ASSERT_FATAL(array != NULL);

    for (int i = 0; i < 6; i++) {
        char *some = "Test %d";
        char *next = malloc(strlen(some) + 1);
        CU_ASSERT_FATAL(next != NULL);
        sprintf(next, some, i);

        DSBuffer *buf = dsbuf_new(next);
        CU_ASSERT_FATAL(buf != NULL);
        free(next);
        CU_ASSERT(dsarray_append(array, buf) == true);
        num_iters++;
    }

    DSIter *iter = dsarray_iter(array);
    CU_ASSERT_FATAL(iter != NULL);
    CU_ASSERT(dsiter_has_next(iter) == true);

    int count_iters = 0;
    while(dsiter_next(iter)) {
        CU_ASSERT(dsiter_key(iter) == NULL);    /* Always NULL for arrays */
        CU_ASSERT(dsiter_value(iter) != NULL);
        count_iters++;
    }

    CU_ASSERT(count_iters == num_iters);
    CU_ASSERT(dsiter_has_next(iter) == false);
    dsiter_destroy(iter);
    dsarray_destroy(array);
}

// Comparator used by the array test suite members
static int array_test_comparator(const void *left, const void *right) {
    const char *l = *(const void**)left;
    const char *r = *(const void**)right;
    return strcmp(l, r);
}
