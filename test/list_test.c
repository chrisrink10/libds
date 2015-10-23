/*****************************************************************************
 * libds :: list_test.c
 *
 * Test functions for DSLib.
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
#include "list_test.h"

static DSList *list_test = NULL;

static int list_test_comparator(const void *left, void const *right);

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

// Comparator used by the list test suite members
static int list_test_comparator(const void *left, const void *right) {
    const char *l = *(const void**)left;
    const char *r = *(const void**)right;
    return strcmp(l, r);
}
