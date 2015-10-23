/*****************************************************************************
 * libds :: dict_test.h
 *
 * Test functions for DSDict.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_DICT_TEST_H
#define LIBDS_DICT_TEST_H

void dict_test_setup(void);
void dict_test_teardown(void);
void dict_test_put(void);
void dict_test_collision(void);
void dict_test_get(void);
void dict_test_del(void);
void dict_test_resize(void);
void dict_test_iter(void);

#endif //LIBDS_DICT_TEST_H
