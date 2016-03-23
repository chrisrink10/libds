/*****************************************************************************
 * libds :: array_test.h
 *
 * Test functions for DSArray.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef GRYPHON_ARRAY_TEST_H
#define GRYPHON_ARRAY_TEST_H

void array_test_setup(void);
void array_test_teardown(void);
void array_test_literal(void);
void array_test_append(void);
void array_test_insert(void);
void array_test_extend(void);
void array_test_get(void);
void array_test_top(void);
void array_test_remove(void);
void array_test_remove_index(void);
void array_test_index(void);
void array_test_pop(void);
void array_test_resize(void);
void array_test_sort(void);
void array_test_reverse(void);
void array_test_clear(void);
void array_test_iter(void);

#endif
