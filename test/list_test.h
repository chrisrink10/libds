/*****************************************************************************
 * libds :: list_test.h
 *
 * Test functions for DSList.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef GRYPHON_LIST_TEST_H
#define GRYPHON_LIST_TEST_H

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

#endif
