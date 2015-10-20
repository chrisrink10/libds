/*****************************************************************************
 * libds :: buffer.h
 *
 * String buffer data type.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#ifndef LIBDS_BUFFER_H
#define LIBDS_BUFFER_H

/**
* @brief Auto-resizing character buffer object.
*/
typedef struct GBuf GBuf;

/**
* @brief Error codes for @c GBuf functions.
*/
static const int GBUF_CHAR_NOT_FOUND = -1;

/**
* @brief Create a new @c GBuf from the given character array.
*
* It is safe to use @c GBuf objects with string literals, since
* the contents are copied into the buffer's internal memory and
* thus a call to @c GBuf will not attempt to free static
* or stack memory.
*/
GBuf *gbuf_new(const char *value);

/**
* @brief Create a new @c GBuf from the given character array with the
* given length. This will permit non-null terminated strings.
*
* It is safe to use @c GBuf objects with string literals, since
* the contents are copied into the buffer's internal memory and
* thus a call to @c gbuf_destroy will not attempt to free static
* or stack memory.
*/
GBuf *gbuf_new_l(const char *value, size_t len);

/**
* @brief Create a new @c GBuf with the given capacity which can be used
* as a character buffer.
*/
GBuf *gbuf_new_buffer(size_t cap);

/**
* @brief Dispose of the @c GBuf object.
*/
void gbuf_destroy(GBuf *str);

/**
* @brief Return the length in number of bytes in the @c GBuf or 0 if
* @c str is null.
*
* Note that this is not the UTF-8 length.
*/
size_t gbuf_len(GBuf *str);

/**
* @brief Return the capacity in number of bytes in the @c GBuf or 0 if
* @c str is null.
*/
size_t gbuf_cap(GBuf *str);

/**
* @brief Append @c newc to @c str, resizing @c str if necessary.
*
* Note that you still have to destroy both @c str and @c newc .
*/
bool gbuf_append(GBuf *str, GBuf *newc);

/**
* @brief Append the new @c char to @c str, resizing if necessary.
*/
bool gbuf_append_char(GBuf *str, int newc);

/**
* @brief Append an existing C-string to a @c GBuf object.
*
* It is safe to use @c GBuf objects with string literals, since
* the contents are copied into the buffer's internal memory and
* thus a call to @c gbuf_destroy will not attempt to free static
* or stack memory.
*/
bool gbuf_append_str(GBuf *str, char *newstr);

/**
* @brief Return the character at the given position.
*
* Returns  -1 if the position is greater than or equal to the
* length of the string or is 0.
*/
int gbuf_char_at(GBuf *str, int pos);

/**
* @brief Return a substring of the given buffer.
*
* Buffer starts at the given position and returning up to @c len
* characters.
*
* Returns a null string if:
* - start is less than 0
* - start is greater than the length of the string
* - len would exceed the length of the string
* - memory cannot be allocated for the new substring
*/
GBuf* gbuf_substr(GBuf *str, int start, size_t len);

/**
* @brief Return true if two @c gbuf objects are equal (length, capacity,
* and string value).
*/
bool gbuf_equals(GBuf *str, GBuf *other);

/**
* @brief Return true if the underlying buffer is value equal to the
* input character array.
*/
bool gbuf_equals_char(GBuf *str, const char *other);

/**
* @brief Return the pointer to the internal character array.
*
* Callers should not try to free this value, since it is the same
* value being used by the @c GBuf .
*/
char* gbuf_char_ptr(GBuf *str);

/**
* @brief Return a @c char array corresponding to the underlying string.
*
* Note that since C strings are terminated with the NUL byte, @c \0 ,
* and a gbuf is not bound by the same restrictions, that you may not be
* able to access the entire string.
*
* Callers are required to call @c free on the return value from this
* function.
*/
char* gbuf_to_char_array(GBuf *str);

/**
* @brief Return a Larson hash of the underlying string.
*/
unsigned int gbuf_hash(void *str);

/**
* @brief Return the difference between left and right.
*
* Returns INT32_MIN if left is NULL;
* Returns INT32_MAX if right is NULL.
*/
int gbuf_compare(void const* left, void const* right);

/**
* @brief Return the Lexicographical difference between left and right.
*/
int gbuf_compare_utf8(void const* left, void const* right);

#endif //LIBDS_BUFFER_H
