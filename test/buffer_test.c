/*****************************************************************************
 * libds :: buffer_test.c
 *
 * Test functions for DSBuffer.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "libds/buffer.h"
#include "buffer_test.h"

static DSBuffer *buf_test = NULL;

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
