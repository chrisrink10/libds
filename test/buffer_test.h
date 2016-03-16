/*****************************************************************************
 * libds :: buffer_test.h
 *
 * Test functions for DSBuffer.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_BUFFER_TEST_H
#define LIBDS_BUFFER_TEST_H

void buf_test_setup(void);
void buf_test_teardown(void);
void buf_test_append(void);
void buf_test_append_char(void);
void buf_test_append_str(void);
void buf_test_char_at(void);
void buf_test_dup(void);
void buf_test_substr(void);
void buf_test_equals(void);
void buf_test_equals_char(void);
void buf_test_to_char_array(void);
void buf_test_compare(void);
void buf_test_compare_utf8(void);

#endif //LIBDS_BUFFER_TEST_H
