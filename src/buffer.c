/**
 * @file buffer.c
 *
 * @brief Automatically resizing string/binary buffer.
 *
 * @author Chris Rink <chrisrink10@gmail.com>
 *
 * @copyright 2015 Chris Rink. MIT Licensed.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "libds/buffer.h"
#include "libds/hash.h"

struct DSBuffer {
    char* str;
    size_t len;
    size_t cap;
};

static bool dsbuf_resize(DSBuffer *str, size_t size);

/*
 * BUFFER PUBLIC FUNCTIONS
 */

DSBuffer * dsbuf_new(const char *value) {
    size_t len = strlen(value);
    return dsbuf_new_l(value, len);
}

DSBuffer * dsbuf_new_l(const char *value, size_t len) {
    if (len < 1) {
        return NULL;
    }

    DSBuffer *s = malloc(sizeof(DSBuffer));
    if (!s) {
        goto cleanup_gbuf;
    }

    s->len = len;
    s->cap = len * DSBUFFER_CAPACITY_FACTOR;
    s->str = malloc(s->cap);
    if (!s->str) {
        goto cleanup_dsbuf_str;
    }

    memcpy(s->str, value, len);
    memset(&s->str[len], '\0', (s->cap - len));
    return s;

cleanup_dsbuf_str:
    free(s->str);
cleanup_gbuf:
    free(s);
    return NULL;
}

DSBuffer * dsbuf_new_buffer(size_t cap) {
    cap = (cap < DSBUFFER_MINIMUM_CAPACITY) ? DSBUFFER_MINIMUM_CAPACITY : cap;

    DSBuffer *s = malloc(sizeof(DSBuffer));
    if (!s) {
        goto cleanup_dsbuf_buffer;
    }

    s->len = 0;
    s->cap = cap;
    s->str = calloc(s->cap, sizeof(char*));
    if (!s->str) {
        goto cleanup_dsbuf_buffer_str;
    }
    return s;

cleanup_dsbuf_buffer_str:
    free(s->str);
cleanup_dsbuf_buffer:
    free(s);
    return NULL;
}

void dsbuf_destroy(DSBuffer *str) {
    if (!str) { return; }
    free(str->str);
    str->str = NULL;
    free(str);
}

size_t dsbuf_len(DSBuffer *str) {
    assert(str);
    return str->len;
}

size_t dsbuf_cap(DSBuffer *str) {
    assert(str);
    return str->cap;
}

bool dsbuf_append(DSBuffer *str, DSBuffer *newc) {
    if ((!str) || (!newc)) {
        return false;
    }

    size_t size = str->len + newc->len;
    size_t threshold = size * DSBUFFER_CAPACITY_FACTOR;
    if (str->cap < threshold) {
        if (!dsbuf_resize(str, threshold)) {
            return false;
        }
    }

    memcpy(&str->str[str->len], newc->str, newc->len);
    str->len += newc->len;
    return true;
}

bool dsbuf_append_char(DSBuffer *str, int newc) {
    if ((!str) || (newc < 0)) {
        return false;
    }

    size_t size = str->len + 1;
    if (str->cap < size) {
        if (!dsbuf_resize(str, str->cap * DSBUFFER_CAPACITY_FACTOR)) {
            return false;
        }
    }

    str->str[str->len] = (char)newc;
    str->len++;
    return true;
}

bool dsbuf_append_str(DSBuffer *str, const char *newstr) {
    if ((!str) || (!newstr)) {
        return false;
    }

    size_t addlen = strlen(newstr);
    size_t size = str->len + addlen;
    if (str->cap < size) {
        if (!dsbuf_resize(str, str->cap * DSBUFFER_CAPACITY_FACTOR)) {
            return false;
        }
    }

    memcpy(&str->str[str->len], newstr, addlen);
    str->len = size;
    return true;
}

int dsbuf_char_at(DSBuffer *str, int pos) {
    if ((!str) || (pos >= str->len) || (pos < 0)) {
        return DSBUFFER_CHAR_NOT_FOUND;
    }

    return str->str[pos];
}

DSBuffer * dsbuf_substr(DSBuffer *str, int start, size_t len) {
    if ((!str) || (start < 0) || (start > str->len) || (len > (str->len - start))) {
        return NULL;
    }

    DSBuffer * sub = dsbuf_new_buffer(len * DSBUFFER_CAPACITY_FACTOR);
    if (!sub) {
        return NULL;
    }

    strncpy(sub->str, &str->str[start], len);
    sub->len = len;
    return sub;
}

bool dsbuf_equals(DSBuffer *str, DSBuffer *other) {
    if ((!str) || (!other)) {
        return false;
    }

    if (str->len != other->len) {
        return false;
    }

    if (str->cap != other->cap) {
        return false;
    }

    int res = strcmp(str->str, other->str);
    return (res == 0);
}

bool dsbuf_equals_char(DSBuffer *str, const char *other) {
    if (!str) { return false; }
    int res = strcmp(str->str, other);
    return (res == 0);
}

const char* dsbuf_char_ptr(DSBuffer *str) {
    if (!str) {
        return NULL;
    }

    return str->str;
}

char* dsbuf_to_char_array(DSBuffer *str) {
    if (!str) {
        return NULL;
    }

    size_t m = (str->len) + 1;
    char* cpy = calloc(m, m);
    if (!cpy) {
        return NULL;
    }

    /* Use strncpy here since we can only return a C-string (NUL terminated),
     * rather than memcpy like the rest of the gbuf functions */
    strncpy(cpy, str->str, str->len);
    return cpy;
}

unsigned int dsbuf_hash(DSBuffer *str) {
    if (!str) { return 0; }
    return (unsigned int) hash_fnv1(str->str);
}

int dsbuf_compare(DSBuffer *left, DSBuffer *right) {
    if (!left) { return INT32_MIN; }
    if (!right) { return INT32_MAX; }
    if (left->len < right->len) { return -1; }
    if (left->len > right->len) { return 1; }
    return memcmp(left->str, right->str, left->len);
}

/*
 * PRIVATE FUNCTIONS
 */

// Resizes a gbuf upwards
static bool dsbuf_resize(DSBuffer *str, size_t size) {
    assert(str);

    if ((size < 1) || (str->cap >= size)) {
        return false;
    }

    char* cache = str->str; // Cache a pointer to the old string
    str->str = malloc(size);
    if (!str->str) {
        str->str = cache;   // Point the old pointer back to the cached value
        return false;
    }

    str->cap = size;
    memcpy(str->str, cache, str->len);
    free(cache);
    return true;
}
