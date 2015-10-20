/*****************************************************************************
 * libds :: buffer.c
 *
 * String buffer data type.
 *
 * Author:  Chris Rink <chrisrink10@gmail.com>
 *
 * License: MIT (see LICENSE document at source tree root)
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "libds/buffer.h"

static const int GBUF_CAPACITY_FACTOR = 2;
static const int GBUF_MINIMUM_CAPACITY = 20;
static const int GBUF_HASH_SEED = 23;

struct GBuf {
    char* str;
    size_t len;
    size_t cap;
};

static int gbuf_priv_compare(const char *const str, const char *const right);
static unsigned int gbuf_priv_hash(const char* str, unsigned int seed);
static bool gbuf_resize(GBuf *str, size_t size);

/*
 * BUFFER PUBLIC FUNCTIONS
 */

GBuf* gbuf_new(const char *value) {
    size_t len = strlen(value);
    return gbuf_new_l(value, len);
}

GBuf* gbuf_new_l(const char *value, size_t len) {
    if (len < 1) {
        return NULL;
    }

    GBuf *s = malloc(sizeof(GBuf));
    if (!s) {
        goto cleanup_gbuf;
    }

    s->len = len;
    s->cap = len * GBUF_CAPACITY_FACTOR;
    s->str = malloc(s->cap);
    if (!s->str) {
        goto cleanup_gbuf_str;
    }

    memcpy(s->str, value, len);
    memset(&s->str[len], '\0', (s->cap - len));
    return s;

    cleanup_gbuf_str:
    free(s->str);
    cleanup_gbuf:
    free(s);
    return NULL;
}

GBuf* gbuf_new_buffer(size_t cap) {
    cap = (cap < GBUF_MINIMUM_CAPACITY) ? GBUF_MINIMUM_CAPACITY : cap;

    GBuf *s = malloc(sizeof(GBuf));
    if (!s) {
        goto cleanup_gbuf_buffer;
    }

    s->len = 0;
    s->cap = cap;
    s->str = calloc(s->cap, sizeof(char*));
    if (!s->str) {
        goto cleanup_gbuf_buffer_str;
    }
    return s;

    cleanup_gbuf_buffer_str:
    free(s->str);
    cleanup_gbuf_buffer:
    free(s);
    return NULL;
}

void gbuf_destroy(GBuf *str) {
    if (!str) { return; }
    free(str->str);
    str->str = NULL;
    free(str);
}

size_t gbuf_len(GBuf *str) {
    return (str) ? (str->len) : 0;
}

size_t gbuf_cap(GBuf *str) {
    return (str) ? (str->cap) : 0;
}

bool gbuf_append(GBuf *str, GBuf *newc) {
    if ((!str) || (!newc)) {
        return false;
    }

    size_t size = str->len + newc->len;
    size_t threshold = size * GBUF_CAPACITY_FACTOR;
    if (str->cap < threshold) {
        if (!gbuf_resize(str, threshold)) {
            return false;
        }
    }

    memcpy(&str->str[str->len], newc->str, newc->len);
    str->len += newc->len;
    return true;
}

bool gbuf_append_char(GBuf *str, int newc) {
    if ((!str) || (newc < 0)) {
        return false;
    }

    size_t size = str->len + 1;
    if (str->cap < size) {
        if (!gbuf_resize(str, str->cap * GBUF_CAPACITY_FACTOR)) {
            return false;
        }
    }

    str->str[str->len] = (char)newc;
    str->len++;
    return true;
}

bool gbuf_append_str(GBuf *str, char *newstr) {
    if ((!str) || (!newstr)) {
        return false;
    }

    size_t addlen = strlen(newstr);
    size_t size = str->len + addlen;
    if (str->cap < size) {
        if (!gbuf_resize(str, str->cap * GBUF_CAPACITY_FACTOR)) {
            return false;
        }
    }

    memcpy(&str->str[str->len], newstr, addlen);
    str->len = size;
    return true;
}

int gbuf_char_at(GBuf *str, int pos) {
    if ((!str) || (pos >= str->len) || (pos < 0)) {
        return GBUF_CHAR_NOT_FOUND;
    }

    return str->str[pos];
}

GBuf * gbuf_substr(GBuf *str, int start, size_t len) {
    if ((!str) || (start < 0) || (start > str->len) || (len > (str->len - start))) {
        return NULL;
    }

    GBuf * sub = gbuf_new_buffer(len * GBUF_CAPACITY_FACTOR);
    if (!sub) {
        return NULL;
    }

    strncpy(sub->str, &str->str[start], len);
    sub->len = len;
    return sub;
}

bool gbuf_equals(GBuf *str, GBuf *other) {
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

bool gbuf_equals_char(GBuf *str, const char *other) {
    if (!str) { return false; }
    int res = strcmp(str->str, other);
    return (res == 0);
}

char* gbuf_char_ptr(GBuf *str) {
    if (!str) {
        return NULL;
    }

    return str->str;
}

char* gbuf_to_char_array(GBuf *str) {
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

unsigned int gbuf_hash(void *str) {
    if (!str) { return 0; }
    return gbuf_priv_hash(((GBuf *)str)->str, GBUF_HASH_SEED);
}

int gbuf_compare(void const *left, void const *right) {
    if (!left) { return INT32_MIN; }
    if (!right) { return INT32_MAX; }
    return gbuf_priv_compare(((GBuf *)left)->str, ((GBuf *)right)->str);
}

int gbuf_compare_utf8(void const *left, void const *right) {
    int cmp = 0;
    //TODO: Figure out a library to use to effectively do this
/*
    UErrorCode err;
    UConverter *conv = ucnv_open("utf-8", &err);

    gbuf *bufl = (gbuf*)left;
    gbuf *bufr = (gbuf*)right;
    if ((!bufl) || (!bufr)) {
        cmp = (!bufl) ? INT32_MIN : INT32_MAX;
        goto gbuf_done;
    }

    size_t llen = 2 * bufl->len;
    size_t rlen = 2 * bufr->len;
    UChar *l = malloc(llen);
    UChar *r = malloc(rlen);
    if ((!l) || (!r)) {
        cmp = (!l) ? INT32_MIN : INT32_MAX;
        goto cleanup_gbuf_compare;
    }

    ucnv_toUChars(conv, l, (int)llen, bufl->str, (int)bufl->len, &err);
    if (err) { goto cleanup_gbuf_compare; }
    ucnv_toUChars(conv, r, (int)rlen, bufr->str, (int)bufr->len, &err);
    if (err) { goto cleanup_gbuf_compare; }

    cmp = u_strCompare(l, (int)llen, r, (int)rlen, TRUE);

cleanup_gbuf_compare:
    free(l);
    free(r);
gbuf_done:
    ucnv_close(conv);
    */
    return cmp;
}

/*
 * PRIVATE FUNCTIONS
 */

// Resizes a gbuf upwards
static bool gbuf_resize(GBuf *str, size_t size) {
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

// Return the string comparison for these two strings.
static int gbuf_priv_compare(const char *const str, const char *const right) {
    return strcmp(str, right);
}

// Produce a simple hash from the given string value.
static unsigned int gbuf_priv_hash(const char *str, unsigned int seed) {
    unsigned int hash = seed;

    while ((*str)) {
        hash = (hash * 101) + (unsigned) *str++;
    }

    return hash;
}
