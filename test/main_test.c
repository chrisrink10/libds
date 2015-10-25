/*****************************************************************************
 * libds :: main_test.c
 *
 * Test runner routine.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <stdbool.h>
#include "CUnit/Basic.h"
#include "array_test.h"
#include "buffer_test.h"
#include "dict_test.h"
#include "list_test.h"

bool setup_buffer_tests(void) {
    /* add a suite to the registry */
    CU_pSuite pSuite = CU_add_suite_with_setup_and_teardown("Character Buffer Suite", NULL, NULL, buf_test_setup, buf_test_teardown);
    if (pSuite == NULL) {
        return false;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "Buffer Append", buf_test_append) == NULL) ||
        (CU_add_test(pSuite, "Buffer Append Char", buf_test_append_char) == NULL) ||
        (CU_add_test(pSuite, "Buffer Append Str", buf_test_append_str) == NULL) ||
        (CU_add_test(pSuite, "Buffer Char At", buf_test_char_at) == NULL) ||
        (CU_add_test(pSuite, "Buffer Substring", buf_test_substr) == NULL) ||
        (CU_add_test(pSuite, "Buffer Equals", buf_test_equals) == NULL) ||
        (CU_add_test(pSuite, "Buffer Equals Char*", buf_test_equals_char) == NULL) ||
        (CU_add_test(pSuite, "Buffer To Char*", buf_test_to_char_array) == NULL) ||
        (CU_add_test(pSuite, "Buffer Compare", buf_test_compare) == NULL) ||
        (CU_add_test(pSuite, "Buffer Compare as UTF-8", buf_test_compare_utf8) == NULL)) {
        return false;
    }

    return true;
}

bool setup_dict_tests(void)  {
    /* add a suite to the registry */
    CU_pSuite pSuite = CU_add_suite_with_setup_and_teardown("Dictionary Suite", NULL, NULL, dict_test_setup, dict_test_teardown);
    if (pSuite == NULL) {
        return false;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "Dict Put", dict_test_put) == NULL) ||
        (CU_add_test(pSuite, "Dict Key Collision", dict_test_collision) == NULL) ||
        (CU_add_test(pSuite, "Dict Get", dict_test_get) == NULL) ||
        (CU_add_test(pSuite, "Dict Del", dict_test_del) == NULL) ||
        (CU_add_test(pSuite, "Dict Resize", dict_test_resize) == NULL) ||
        (CU_add_test(pSuite, "Dict Iterator", dict_test_iter) == NULL)) {
        return false;
    }

    return true;
}

bool setup_array_tests(void)  {
    /* add a suite to the registry */
    CU_pSuite pSuite = CU_add_suite_with_setup_and_teardown("Array Suite", NULL, NULL, array_test_setup, array_test_teardown);
    if (pSuite == NULL) {
        return false;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "Array Append", array_test_append) == NULL) ||
        (CU_add_test(pSuite, "Array Insert", array_test_insert) == NULL) ||
        (CU_add_test(pSuite, "Array Extend", array_test_extend) == NULL) ||
        (CU_add_test(pSuite, "Array Get", array_test_get) == NULL) ||
        (CU_add_test(pSuite, "Array Remove", array_test_remove) == NULL) ||
        (CU_add_test(pSuite, "Array Remove by Index", array_test_remove_index) == NULL) ||
        (CU_add_test(pSuite, "Array Get Index", array_test_index) == NULL) ||
        (CU_add_test(pSuite, "Array Pop", array_test_pop) == NULL) ||
        (CU_add_test(pSuite, "Array Resize", array_test_resize) == NULL) ||
        (CU_add_test(pSuite, "Array Reverse", array_test_reverse) == NULL) ||
        (CU_add_test(pSuite, "Array Clear", array_test_clear) == NULL) ||
        (CU_add_test(pSuite, "Array Sort", array_test_sort) == NULL) ||
        (CU_add_test(pSuite, "Array Iterator", array_test_iter) == NULL)) {
        return false;
    }

    return true;
}

bool setup_list_test(void) {
    /* add a suite to the registry */
    CU_pSuite pSuite = CU_add_suite_with_setup_and_teardown("List Suite", NULL, NULL, list_test_setup, list_test_teardown);
    if (pSuite == NULL) {
        return false;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "List Append", list_test_append) == NULL) ||
        (CU_add_test(pSuite, "List Insert", list_test_insert) == NULL) ||
        (CU_add_test(pSuite, "List Extend", list_test_extend) == NULL) ||
        (CU_add_test(pSuite, "List Get", list_test_get) == NULL) ||
        (CU_add_test(pSuite, "List Remove", list_test_remove) == NULL) ||
        (CU_add_test(pSuite, "List Remove by Index", list_test_remove_index) == NULL) ||
        (CU_add_test(pSuite, "List Get Index", list_test_index) == NULL) ||
        (CU_add_test(pSuite, "List Pop", list_test_pop) == NULL) ||
        (CU_add_test(pSuite, "List Reverse", list_test_reverse) == NULL) ||
        (CU_add_test(pSuite, "List Clear", list_test_clear) == NULL) ||
        (CU_add_test(pSuite, "List Enqueue/Dequeue", list_test_queue) == NULL) ||
        (CU_add_test(pSuite, "List Iterator", list_test_iter) == NULL)) {
        return false;
    }

    return true;
}

int main(int argc, const char* argv[]) {
    /* Initialize the CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Add test suites to the registry */
    if ((!setup_array_tests()) ||
        (!setup_buffer_tests()) ||
        (!setup_dict_tests()) ||
        (!setup_list_test()))
    {
        goto cleanup_main;
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Clean-up the registry and return the error */
cleanup_main:
    CU_cleanup_registry();
    return CU_get_error();
}