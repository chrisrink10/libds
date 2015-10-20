/*****************************************************************************
 * libds :: dslib_test.h
 *
 * Testing suite for libds.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef GRYPHON_DSLIB_TEST_H
#define GRYPHON_DSLIB_TEST_H

/*
 * Buffer testing functions
 */
void buf_test_setup(void);
void buf_test_teardown(void);
void buf_test_append(void);
void buf_test_append_char(void);
void buf_test_append_str(void);
void buf_test_char_at(void);
void buf_test_substr(void);
void buf_test_equals(void);
void buf_test_equals_char(void);
void buf_test_to_char_array(void);
void buf_test_compare(void);
void buf_test_compare_utf8(void);

/*
 * List testing functions
 */
void list_test_setup(void);
void list_test_teardown(void);
void list_test_append(void);
void list_test_insert(void);
void list_test_extend(void);
void list_test_get(void);
void list_test_remove(void);
void list_test_remove_index(void);
void list_test_index(void);
void list_test_pop(void);
void list_test_resize(void);
void list_test_sort(void);
void list_test_reverse(void);
void list_test_iter(void);

/*
 * Dict testing functions
 */
void dict_test_setup(void);
void dict_test_teardown(void);
void dict_test_put(void);
void dict_test_collision(void);
void dict_test_get(void);
void dict_test_del(void);
void dict_test_resize(void);
void dict_test_iter(void);

#endif
